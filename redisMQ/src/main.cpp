#include "taskwork.h"
#include "data_switch.h"


int main (int argc, char *argv[])
{
    INFO("Message queue task start to run >> \n");
	
    if (argc == 2 && strcmp(argv[1], "-d") == 0){
		daemonize();
	}
	
	init_logging("./logs/", "redis_zmq.log", "REDIS_ZMQ", 100, 7, 1024 * 1024 * 100);

    ITWCONFIG configer("redis_mq.conf");
	void *context = zmq_ctx_new ();
	
	struct CTX_ITW ctx_itw;
	ctx_itw.configer = &configer;
	ctx_itw.context = context;

	DataSwitch ds;
	if(ds.init(&ctx_itw) == false)
	{
	    ERROR("DataSwitchy.init() error");
	    return -1;
	}

    /* Create publish process */
	pid_t publish_pid; 
	publish_pid = fork();
	if (publish_pid < 0)
	{
		printf("error in fork! \n");
	}
	else if (publish_pid == 0)
	{
	    //printf("main() -------------------------> 11111 \n");
		create_publish_process(&configer);
	}

    /* zmq_proxy() place here of the end, because it will block the code after it */
	ds.create_proxy();

	//Keep main thread running
	while (1)
	{
		sleep(100);
	}
	
    return 0;
}

