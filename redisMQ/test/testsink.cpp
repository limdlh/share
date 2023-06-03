//
//  Task sink in C++
//  Binds PULL socket to tcp://XXX.XXX.XXX.XXX:5558
//  Collects results from workers via that socket
//  This code only used for test
//

#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include "zhelpers.hpp"
#include "zmq.hpp"
#include "mycommon.h"
#include "itwconfig.h"

//#define SUB_PUB_ON

struct CTX_ITW{
    void *context;
    ITWCONFIG* configer;
};
int gCount = 0;
struct timeval tStart;

void calculateDuration()
{   // Calculate and report duration of batch
    struct timeval tEnd, tDiff;
    gettimeofday (&tEnd, NULL);

    if (tEnd.tv_usec < tStart.tv_usec) {
        tDiff.tv_sec = tEnd.tv_sec - tStart.tv_sec - 1;
        tDiff.tv_usec = 1000000 + tEnd.tv_usec - tStart.tv_usec;
    }
    else {
        tDiff.tv_sec = tEnd.tv_sec - tStart.tv_sec;
        tDiff.tv_usec = tEnd.tv_usec - tStart.tv_usec;
    }

	int totalMSec = 0;     //  Total expected cost in msecs
    totalMSec = tDiff.tv_sec * 1000 + tDiff.tv_usec / 1000;

	NOTICE("Total elapsed time: %d msec, received items: %d", totalMSec, gCount);
}

void* pull_Data(void* args){
    DEBUG("pull_Data() ");

	CTX_ITW* ctx_itw_p = (CTX_ITW*)args;
	ITWCONFIG* configer = (ITWCONFIG*)(ctx_itw_p->configer);
	//string sink_pull_url = configer->getValueByKey("sink.pull_url");
	string sink_pull_url = "tcp://*:5551";
	
	/* These code used to create proxy for publisher, while user want to create multi-thread */
	string sink_pull_inproc = configer->getValueByKey("sink.pull_inproc");
	
	int NUM = atoi(configer->getValueByKey("test.push_NUMBER").c_str());
	INFO("pull_Data() sink_pull_inproc: %s, NUM:%d", sink_pull_inproc.c_str(), NUM);

	void *context = (void *)(ctx_itw_p->context);
    void *receiver = zmq_socket(context, ZMQ_PULL);
	assert (receiver);

	/* These code used to create proxy for publisher, while user want to create multi-thread */
    //int rc = zmq_connect(receiver, sink_pull_inproc.c_str());

	int rc = zmq_bind(receiver, sink_pull_url.c_str());
    if (rc == -1) {
		ERROR ("E: zmq_connect failed: %s, sink_pull_url: %s", strerror (errno), sink_pull_url.c_str());
	}
	
    int task_nbr;
	std::string strReply;
    for (task_nbr = 0; task_nbr < NUM; task_nbr++)
	{
        strReply = s_recv (receiver);
	    DEBUG("pull_Data(): NUM.%d, content: %s", task_nbr, strReply.c_str());
    }

	gCount += task_nbr;
	zmq_close (receiver);
	
	return NULL;
}

void* sub_Data(void* args){
    DEBUG("sub_Data() ");

	CTX_ITW* ctx_itw_p = (CTX_ITW*)args;
	ITWCONFIG* configer = (ITWCONFIG*)(ctx_itw_p->configer);
	string sink_sub_url = configer->getValueByKey("sink.sub_url");
	int NUM = atoi(configer->getValueByKey("test.pub_NUMBER").c_str());
	INFO("sub_Data() sink_sub_url: %s, NUM:%d", sink_sub_url.c_str(), NUM);

	void *context = (void *)(ctx_itw_p->context);
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    assert (subscriber);
	DEBUG("sub_Data() ");
    int rc = zmq_connect(subscriber, sink_sub_url.c_str());
	if (rc == -1) {
		ERROR ("E: zmq_connect failed: %s", strerror (errno));
	}
    DEBUG("sub_Data() ");
    char *filter = "10001";//  set subscribe information, now let it to be 10001
    //zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filter, strlen (filter));
	rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
	if (rc == -1) {
		ERROR ("E: zmq_setsockopt failed: %s", strerror (errno));
	}
    DEBUG("sub_Data() ");
    int update_nbr;
	std::string strReply;
    for (update_nbr = 0; update_nbr < NUM; update_nbr++) 
	{
	    printf("sub_Data() line: 114\n");
	    DEBUG("sub_Data() ");
        strReply = s_recv (subscriber);
        //sscanf (string, "%d %d %d", &zipcode, &temperature, &relhumidity);
        DEBUG("NUM.%d: Pub/Sub Data: %s", update_nbr, strReply.c_str());
    }

	gCount += update_nbr;
	zmq_close (subscriber);
	
	return NULL;
}

int main (int argc, char *argv[])
{
    init_logging("./logs/", "sink_zmq.log", "SINK", 100, 7, 1024 * 1024 * 100);
	ITWCONFIG configer("redis_mq.conf");
	std::string sink_start_url = configer.getValueByKey("sink.start_url");

	void *context = zmq_ctx_new ();
    void *starter = zmq_socket (context, ZMQ_PULL);
    zmq_bind (starter, sink_start_url.c_str());

	struct CTX_ITW ctx_itw;
	ctx_itw.configer = &configer;
	ctx_itw.context = context;

#ifdef SUB_PUB_ON
    /* These code used to create proxy for publisher, while user want to create multi-thread */
    string sink_pull_url = configer.getValueByKey("sink.pull_url");
	string sink_pull_inproc = configer.getValueByKey("sink.pull_inproc");
    void *clients = zmq_socket (context, ZMQ_PULL);
    zmq_bind (clients, sink_pull_url.c_str());
    void *workers = zmq_socket (context, ZMQ_PUSH);
    zmq_bind (workers, sink_pull_inproc.c_str());
#endif

    // Wait start message
	NOTICE("before receive message");
	std::string str = s_recv (starter);
	NOTICE("Received start message, str: %s \nNow start to receive data", str.c_str());
	
    //  Start our clock now
    gettimeofday (&tStart, NULL);

    /* If create multi-thread by zmq_proxy(), this process will not end, then the during will not come out, so always only one thread */
    int NBR_PUSH_THREAD = atoi(configer.getValueByKey("sink.push_thread").c_str());
	NBR_PUSH_THREAD = (NBR_PUSH_THREAD == 0)? 0:1;
    pthread_t pull_pthread[NBR_PUSH_THREAD];
    for (int worker_nbr = 0; worker_nbr < NBR_PUSH_THREAD; ++worker_nbr) 
	{
		pthread_create(pull_pthread + worker_nbr, NULL, pull_Data, &ctx_itw);

		/* zmq_proxy will make this process running endless */
		NOTICE("test ->>>>>>>>> ");
		pthread_join(*(pull_pthread + worker_nbr), NULL);
		NOTICE("test ->>>>>>>>> ");
    }

	int NBR_PUB_THREAD = atoi(configer.getValueByKey("sink.pub_thread").c_str());
	NBR_PUB_THREAD = (NBR_PUB_THREAD == 0)? 0:1;
	NOTICE("NBR_PUB_THREAD: %d", NBR_PUB_THREAD);
    pthread_t sub_pthread[NBR_PUB_THREAD];
    for (int worker_nbr = 0; worker_nbr < NBR_PUB_THREAD; ++worker_nbr) 
	{
		pthread_create(sub_pthread + worker_nbr, NULL, sub_Data, &ctx_itw);

		/* zmq_proxy will make this process running endless */
		pthread_join(*(sub_pthread + worker_nbr), NULL);
    }

#ifdef SUB_PUB_ON
	/* These code used to create proxy for publisher, while user want to create multi-thread */
	zmq_proxy(clients, workers, NULL);
	zmq_close (clients);
	zmq_close (workers);
#endif

	calculateDuration();
	
	zmq_close (starter);
    zmq_ctx_term (context);

    return 0;
}


