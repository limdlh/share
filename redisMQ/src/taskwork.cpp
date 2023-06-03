#include "taskwork.h"


#include <sys/syscall.h>
#define gettidv1() syscall(__NR_gettid)


int daemonize() {
	pid_t pid;
	umask(0);
	if ((pid = fork()) < 0)
		return -1;
	else if (pid != 0)
		exit(0);

	setsid();
	//chdir("/");

	/* redirect std* to null */
	int fd;
	fd = open("/dev/null", O_RDONLY);
	if (fd != 0) {
		dup2(fd, 0);
		close(fd);
	}

	fd = open("/dev/null", O_WRONLY);
	if (fd != 1) {
		dup2(fd, 1);
		close(fd);
	}

	fd = open("/dev/null", O_WRONLY);
	if (fd != 2) {
		dup2(fd, 2);
		close(fd);
	}
	
	INFO("daemonize() success");
	
	return 0;
}

bool createPubProxy(CTX_ITW* ctx_itw, void *pub_clients, void *pub_server) 
{
	assert (pub_clients);
	assert (pub_server);

	ITWCONFIG* configer = (ITWCONFIG*)(ctx_itw->configer);
	
	string pub_zmq_inproc = configer->getValueByKey("pub.zmq_inproc");
	int rc = 0;
    rc = zmq_bind (pub_clients, pub_zmq_inproc.c_str());
	if (rc == -1) 
	{
		ERROR ("E: bind failed: %s", strerror (errno));
		return false;
	}

	string pub_zmq_url = configer->getValueByKey("pub.zmq_url");
    rc = zmq_bind (pub_server, pub_zmq_url.c_str());
	if (rc == -1) 
	{
		ERROR ("E: bind failed: %s", strerror (errno));
		return false;
	}

	return true;
}

void* pub_Data(void* args)
{
	DEBUG("pub_Data()");
	CTX_ITW* ctx_itw_p = (CTX_ITW*)args;
	ITWCONFIG* configer = (ITWCONFIG*)(ctx_itw_p->configer);
	void *context = (void *)(ctx_itw_p->context);
	string pub_zmq_inproc = configer->getValueByKey("pub.zmq_inproc");
	string pub_redis_ip = configer->getValueByKey("redis.ip");
	int pub_redis_port = atoi(configer->getValueByKey("redis.port").c_str());
	string pub_redis_passwd = configer->getValueByKey("redis.passwd");
	INFO("pub_Data() pub_zmq_inproc: %s, ip:%s, port:%d, passwd:%s", pub_zmq_inproc.c_str(), pub_redis_ip.c_str(), pub_redis_port, pub_redis_passwd.c_str());

	/* These code used to create proxy for publisher, while user want to create multi-thread */
	void *publisher = zmq_socket (context, ZMQ_PUSH);
	assert (publisher);
    int rc = zmq_connect (publisher, pub_zmq_inproc.c_str());
	if (rc == -1)
	{
		ERROR("E: zmq_connect failed: %s, pub_zmq_inproc: %s", strerror (errno), pub_zmq_inproc.c_str());
		return NULL;
	}

	RedisAdapter *r = new RedisAdapter(pub_redis_ip, pub_redis_port, pub_redis_passwd);
	if(!r->connect()) 
	{
		ERROR("pub_Data --> connect redis server error! ip: %s, port: %d", pub_redis_ip.c_str(), pub_redis_port);
	}

	int retType = -1;
	string strPop;
	size_t len = 0;

	while (1) 
	{
		if(r->m_online == false) 
		{
			ERROR("pub_Data --> connect redis server error! ip: %s, port: %d", pub_redis_ip.c_str(), pub_redis_port);
			r->connect();
		}
        else 
		{
			retType = r->pop(strPop, RedisAdapter::PUB_SUB_DATA);
			switch (retType) 
			{
				case REDIS_REPLY_STRING:
				case REDIS_REPLY_ARRAY:
					len = strPop.length();
				    DEBUG("[%ld] pub_Data --> pop result: %s, length: %d ", (long int)gettidv1(), strPop.c_str(), len);
					s_send (publisher, strPop.c_str());
					break;
				case REDIS_REPLY_ERROR:
					r->m_online = false;
					break;
				case REDIS_REPLY_NIL:
					INFO("pub_Data() replyType: %d, REDIS_REPLY_NIL, no data", retType);
					break;
				default:
					INFO("switch to default");
					break;
			}
		}
    }
	
	return NULL;
}

void create_publish_process(ITWCONFIG* configer)
{
    DEBUG("create_publish_process() ");
	
    void *context = zmq_ctx_new ();
    struct CTX_ITW ctx_itw;
	ctx_itw.configer = configer;
	ctx_itw.context = context;

	void *pub_clients = zmq_socket (context, ZMQ_PULL);
	void *pub_server = zmq_socket (context, ZMQ_PUB);
	if(createPubProxy(&ctx_itw, pub_clients, pub_server) == false)
	{
	    ERROR("E: createPubProxy() error");
	    return;
	}

	const int NBR_WORKERS = atoi(configer->getValueByKey("thread.pub_count").c_str());
    pthread_t pub_pthread[NBR_WORKERS];
    for (int worker_nbr = 0; worker_nbr < NBR_WORKERS; ++worker_nbr) 
	{
		pthread_create(pub_pthread + worker_nbr, NULL, pub_Data, &ctx_itw);
    }

	// These code used to create proxy for publisher, while user want to create multi-thread
	zmq_proxy (pub_clients, pub_server, NULL);
}


