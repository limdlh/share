//
//  Task ventilator in C++
//  Binds PUSH socket to tcp://XXX.XXX.XXX.XXX:5557
//  Sends batch of tasks to workers via that socket
//  This code only used for test
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include "zmq.hpp"
#include "mycommon.h"
#include "itwconfig.h"
#include "zhelpers.hpp"


struct CTX_ITW{
    void *context;
    ITWCONFIG* configer;
};

typedef struct _CALL_NOTIFY{
 int    type;            //消息类型
 int    pid;             //项目ID
 int    subpid;          //子项目ID
 int    agentid;         //坐席Id
 int    staffid;         //员工Id
 char   uuid[64];        //通话ID
 char   callnum[16];     //用户号码
 char   accessnum[16];   //被叫号码
 time_t calltime;        //拨打时间
}CALL_NOTIFY_, *PCALL_NOTIFY;


#define within(num) (int) ((float) num * random () / (RAND_MAX + 1.0))
int gCount = 0;

void* pub_Message(void* args)
{
    DEBUG("pub_Message() ");

	CTX_ITW* ctx_itw_p = (CTX_ITW*)args;
	ITWCONFIG* configer = (ITWCONFIG*)(ctx_itw_p->configer);
	string pub_zmq_url = configer->getValueByKey("vent.zmq_url");
	int NUM = atoi(configer->getValueByKey("test.pub_NUMBER").c_str());
	const int NBR_WORKERS = atoi(configer->getValueByKey("vent.pub_thread").c_str());
	NUM = NUM / NBR_WORKERS;
	INFO("pub_Message() pub_zmq_url: %s, NUM:%d", pub_zmq_url.c_str(), NUM);
	
	void *context = (void *)(ctx_itw_p->context);
	void *sender = zmq_socket (context, ZMQ_PUSH);
	assert (sender);
    int rc = zmq_connect (sender, pub_zmq_url.c_str());
	if(rc == -1)
	{
		ERROR ("E: zmq_connect failed: %s", strerror (errno));
	}
	
    srandom ((unsigned) time (NULL)); // Initialize random number generator

    char buffer [50] = {0};
	int len = 0;
	int workload, zipcode = 10001, n = 0;
	int task_nbr;
    for (task_nbr = 0; task_nbr < NUM; task_nbr++)
	{
        //  Random workload from 1 to 1000
        workload = within (1000) + 1;
        len = sprintf (buffer, "%05d %d --> %d test string for pub/sub", zipcode, n++, workload);
		DEBUG("pub_Message: %s, length: %d", buffer, len);
		s_send(sender, buffer);
    }

	gCount += task_nbr;
	zmq_close (sender);
	
    return NULL;
}

void* push_Message(void* args)
{
    DEBUG("push_Message() ");

    CTX_ITW* ctx_itw_p = (CTX_ITW*)args;
	ITWCONFIG* configer = (ITWCONFIG*)(ctx_itw_p->configer);
	string vent_zmq_url = configer->getValueByKey("vent.zmq_url");
	int NUM = atoi(configer->getValueByKey("test.push_NUMBER").c_str());
	const int NBR_WORKERS = atoi(configer->getValueByKey("vent.push_thread").c_str());
	NUM = NUM / NBR_WORKERS;
	INFO("push_Message() vent_zmq_url: %s, NUM:%d", vent_zmq_url.c_str(), NUM);
	
	void *context = (void *)(ctx_itw_p->context);
	void *sender = zmq_socket (context, ZMQ_PUSH);
	assert (sender);
    int rc = zmq_connect (sender, vent_zmq_url.c_str());
	if(rc == -1)
	{
		ERROR ("E: zmq_connect failed: %s", strerror (errno));
	}
	
    //char buffer [150] = {0};
	int task_nbr;
	//int n = 0, workload, zipcode = 0, callNum, accessNum;
    for (task_nbr = 0; task_nbr < NUM; task_nbr++) {
        //  Random workload from 0 to 99999
        /*workload = within (100000);
		callNum = within (100000);
	    accessNum = within (100000);
	    n = sprintf (buffer, "{\"projectId\":\"%05d\",\"subprojectId\":\"%d\",\"uuid\":\"12345\",\"callNum\":\"%05d\",\"accessNum\":\"%05d\",\"callTime\":\"01:12\"}", zipcode++, workload, callNum, accessNum);
		DEBUG("push_Message: %s, length: %d", buffer, n);*/
		//s_send(sender, buffer);
  
		CALL_NOTIFY_ call_message;
		call_message.pid = task_nbr;
	    call_message.subpid = within (1000);
		memcpy(call_message.uuid, "123456789", 9);
		call_message.agentid = within (100000);
		call_message.staffid = 1000000083;
		memcpy(call_message.callnum, "18751991697", 11);
		memcpy(call_message.accessnum, "12345", 5);
		call_message.calltime = 1644823164 - within (100000);
	
		const void* d(reinterpret_cast<const void*>(&call_message));
		int len = sizeof(call_message);
		DEBUG("push_Message: ength: %d", len);
		zmq_send (sender, d, len, 0);
    }

    gCount += task_nbr;
	zmq_close (sender);

    return NULL;
}

int main (int argc, char *argv[])
{
    init_logging("./logs/", "vent_zmq.log", "VENT", 100, 7, 1024 * 1024 * 100);
	ITWCONFIG configer("redis_mq.conf");
	std::string vent_start_url = configer.getValueByKey("vent.start_url");
	int total_msec = 0;     //  Total expected cost in msecs
	
	void *context = zmq_ctx_new ();
	void *sink = zmq_socket (context, ZMQ_PUSH);
	zmq_connect (sink, vent_start_url.c_str());

	struct CTX_ITW ctx_itw;
	ctx_itw.configer = &configer;
	ctx_itw.context = context;
	
	NOTICE("Press Enter when the workers are ready: ");
    getchar ();
	NOTICE("Sending tasks to workers... ");

	//  send start signal
	char str[6] = "start";
	s_send (sink, str);

    //  Start our clock now
    struct timeval tstart;
    gettimeofday (&tstart, NULL);
	
	const int NBR_PUSH_THREAD = atoi(configer.getValueByKey("vent.push_thread").c_str());
    pthread_t push_pthread[NBR_PUSH_THREAD];
    for (int worker_nbr = 0; worker_nbr < NBR_PUSH_THREAD; ++worker_nbr) 
	{
		pthread_create(push_pthread + worker_nbr, NULL, push_Message, &ctx_itw);
		pthread_join(*(push_pthread + worker_nbr), NULL);
    }

	const int NBR_PUB_THREAD = atoi(configer.getValueByKey("vent.pub_thread").c_str());
    pthread_t pub_pthread[NBR_PUB_THREAD];
    for (int worker_nbr = 0; worker_nbr < NBR_PUB_THREAD; ++worker_nbr) 
	{
		pthread_create(pub_pthread + worker_nbr, NULL, pub_Message, &ctx_itw);
		pthread_join(*(pub_pthread + worker_nbr), NULL);
    }
	
	//  Calculate and report duration of batch
    struct timeval tend, tdiff;
    gettimeofday (&tend, NULL);

    if (tend.tv_usec < tstart.tv_usec) {
        tdiff.tv_sec = tend.tv_sec - tstart.tv_sec - 1;
        tdiff.tv_usec = 1000000 + tend.tv_usec - tstart.tv_usec;
    }
    else {
        tdiff.tv_sec = tend.tv_sec - tstart.tv_sec;
        tdiff.tv_usec = tend.tv_usec - tstart.tv_usec;
    }
    total_msec = tdiff.tv_sec * 1000 + tdiff.tv_usec / 1000;
	
	NOTICE("Total elapsed time: %d msec, send items: %d", total_msec, gCount);
	
    zmq_close (sink);
    zmq_ctx_term (context);

    return 0;
}
