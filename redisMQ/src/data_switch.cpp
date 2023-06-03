//#include "taskwork.h"
#include "data_switch.h"


#include <sys/syscall.h>
#define gettidv1() syscall(__NR_gettid)


DataSwitch::DataSwitch()
{
    DEBUG("DataSwitch::DataSwitch()");
}

DataSwitch::~DataSwitch()
{
    DEBUG("DataSwitch::~DataSwitch()");

	this->m_configer = NULL;
	this->m_context = NULL;
	this->m_fetch_clients = NULL;
	this->m_fetch_server = NULL;
}

bool DataSwitch::init(CTX_ITW* p_ctx_itw)
{
    DEBUG("DataSwitch::init()");
	
	m_configer = (ITWCONFIG*)(p_ctx_itw->configer);
	m_context = (void *)(p_ctx_itw->context);

	m_fetch_zmq_inproc = m_configer->getValueByKey("fetch.zmq_inproc");
	m_redis_ip = m_configer->getValueByKey("redis.ip");
	m_redis_port = atoi(m_configer->getValueByKey("redis.port").c_str());
	m_redis_passwd = m_configer->getValueByKey("redis.passwd");
	m_fetch_zmq_url = m_configer->getValueByKey("fetch.zmq_url");
	m_http_post_url = m_configer->getValueByKey("http.post_ip") + m_configer->getValueByKey("http.post_url");

	if(createFectchProxy() == false)
	{
	    ERROR("E: createFectchProxy() error");
	    return false;
	}

	if(fetch_Data() == false)
	{
	    ERROR("E: fetch_Data() error");
	    return false;
	}
	
	if(push_Data() == false)
	{
	    ERROR("E: push_Data() error");
	    return false;
	}

	return true;
}

bool DataSwitch::createFectchProxy() 
{
    m_fetch_clients = zmq_socket (m_context, ZMQ_PULL);
	assert (m_fetch_clients);
	m_fetch_server = zmq_socket (m_context, ZMQ_PUSH);
	assert (m_fetch_server);

	int rc = 0;
    rc = zmq_bind (m_fetch_clients, m_fetch_zmq_url.c_str());
	if (rc == -1) 
	{
		ERROR ("E: bind failed: %s", strerror (errno));
		return false;
	}

    rc = zmq_bind (m_fetch_server, m_fetch_zmq_inproc.c_str());
    if (rc == -1) 
	{
		ERROR ("E: bind failed: %s", strerror (errno));
		return false;
	}

	return true;
}

void DataSwitch::create_proxy()
{
    DEBUG("fetch_thread()");

	/* zmq_proxy() place here of the end, because it will block the code after it */
	zmq_proxy (m_fetch_clients, m_fetch_server, NULL);
}

void* DataSwitch::fetch_thread(void* args)
{
    DEBUG("fetch_thread()");

	DataSwitch* pThis = (DataSwitch*)args;

	void *receiver = zmq_socket (pThis->m_context, ZMQ_PULL);
	assert (receiver);
    int rc = zmq_connect (receiver, pThis->m_fetch_zmq_inproc.c_str());
	if (rc == -1) 
	{
		ERROR("E: zmq_connect failed: %s, fetch_zmq_inproc: %s", strerror(errno), pThis->m_fetch_zmq_inproc.c_str());
		return NULL;
	}
	
	RedisAdapter *r = new RedisAdapter(pThis->m_redis_ip, pThis->m_redis_port, pThis->m_redis_passwd);
	if(!r->connect())
	{
		ERROR("fetch_Data --> connect redis server error! ip: %s, port: %d", pThis->m_redis_ip.c_str(), pThis->m_redis_port);
	}

	int retType = -1, dataType = 0;
	int size = 0;
	char buffer [CAP];
	string reqString;
	Variant req;
	
	while (1) 
	{
		if(r->m_online == false) 
		{
			ERROR("fetch_thread --> connect redis server error! ip: %s, port: %d", pThis->m_redis_ip.c_str(), pThis->m_redis_port);
			r->connect();
		}
        else
		{
		    size = zmq_recv (receiver, buffer, CAP, 0);
		    if (size == -1)
			{
				INFO("fetch_thread --> zmq_recv() return -1 ! ");
		        continue;
			}
		    buffer[size < CAP ? size : CAP - 1] = '\0';

			dataType = pThis->structToJSON(req, buffer, reqString);
			DEBUG("[%ld]fetch_Data()  length: %d, reqString: %s", (long int)gettidv1(), size, reqString.c_str());

			/*if (d->datatype == 10001) //Only for test now. String start with "10001 " is used for pub/sub
            //if(dataType == 0)
			{
				retType = r->push(reqString, RedisAdapter::PUB_SUB_DATA);
			}
			else if(dataType == 1)
			{*/
				retType = r->push(reqString, RedisAdapter::PULL_PUSH_DATA);
			//}

			if(retType == REDIS_REPLY_ERROR)
			{
			    ERROR("fetch_Data --> REDIS_REPLY_ERROR, redis push data fail, retType: %d", retType);
			    r->m_online = false;
			}

			reqString.clear();
		}
    }
	
	return NULL;
}

void* DataSwitch::push_thread(void* args)
{
    DEBUG("push_thread()");

	DataSwitch* pThis = (DataSwitch*)args;

	RedisAdapter *r = new RedisAdapter(pThis->m_redis_ip, pThis->m_redis_port, pThis->m_redis_passwd);
	if(!r->connect())
	{
		ERROR("push_Data --> connect redis server error! ip: %s, port: %d", pThis->m_redis_ip.c_str(), pThis->m_redis_port);
	}
	
	string strPop;
	size_t len = 0;
	int retType = -1;
	string res;
	int ret;
	
	while (1) 
	{
		if(r->m_online == false)
		{
			ERROR("push_Data --> connect redis server error! ip: %s, port: %d", pThis->m_redis_ip.c_str(), pThis->m_redis_port);
			r->connect();
		}
        else
		{
			retType = r->pop(strPop, RedisAdapter::PULL_PUSH_DATA);
			switch (retType) 
			{
				case REDIS_REPLY_STRING:
				case REDIS_REPLY_ARRAY:
					len = strPop.length();
				    DEBUG("[%ld] m_http_post_url: %s, push_Data --> pop result: %s, length: %d ", (long int)gettidv1(), pThis->m_http_post_url.c_str(), strPop.c_str(), len);
					ret = pThis->httpPost(pThis->m_http_post_url, strPop, &res);
					if(ret != 0)
					{
					    ERROR("httpPost() fail! httpPost() return ret: %d,  error! ", ret);
					}
					strPop.clear();
					break;
				case REDIS_REPLY_ERROR:
					r->m_online = false;
					break;
				case REDIS_REPLY_NIL:
					INFO("push_Data() replyType: %d, REDIS_REPLY_NIL, no data", retType);
					break;
				default:
					INFO("switch to default");
					break;
			}
		}
    }
	
	return NULL;
}

int DataSwitch::structToJSON(Variant &req, char *buffer, string &reqString)
{
    PCALL_NOTIFY d = reinterpret_cast<PCALL_NOTIFY>(buffer);

	req["projectId"] = d->pid;
	req["subprojectId"] = d->subpid;
	req["uuid"] = d->uuid;
	req["agentId"] = d->agentid;
	req["staffId"] = d->staffid;
	req["callNum"] = d->callnum;
	req["accessNum"] = d->accessnum;
	req["callTime"] = from_unix_time(d->calltime, "%Y-%m-%d %H:%M:%S");
	req.SerializeToJSON(reqString);
	
	//DEBUG("fetch_Data() reqString: %s", reqString.c_str());
	return d->type;
}

/* Fetch data from source to message queue, and then store to redis */
bool DataSwitch::fetch_Data()
{
    DEBUG("fetch_Data()");

	bool fetch_thread_init = false;
	const int NBR_FETCH = atoi(m_configer->getValueByKey("thread.fetch_count").c_str());
    pthread_t fetch_pthread[NBR_FETCH];
    for (int worker_nbr = 0; worker_nbr < NBR_FETCH; ++worker_nbr) 
	{
		//pthread_create(fetch_pthread + worker_nbr, NULL, fetch_thread, this);
		if(pthread_create(fetch_pthread + worker_nbr, NULL, fetch_thread, this) != 0)
		{
			ERROR("pthread_create error, DataSwitch::fetch_Data() failed");
		}
		else
		{
		    fetch_thread_init = true;
		}
    }

	return fetch_thread_init;
}

/* Get data from redis to message queue, and then push to destination */
bool DataSwitch::push_Data()
{
    DEBUG("push_Data()");

	bool push_thread_init = false;
	const int NBR_PUSH = atoi(m_configer->getValueByKey("thread.push_count").c_str());
    pthread_t push_pthread[NBR_PUSH];
    for (int worker_nbr = 0; worker_nbr < NBR_PUSH; ++worker_nbr) 
	{
		if(pthread_create(push_pthread + worker_nbr, NULL, push_thread, this) != 0)
		{
			ERROR("pthread_create error, DataSwitch::push_Data() failed");
		}
		else
		{
		    push_thread_init = true;
		}
    }

	return push_thread_init;
}

int DataSwitch::httpPost(string& url, string &reqString, string *res)
{
	int ret=0;
	MyHttpReq *http_req=NULL;

	do{
		http_req=MyHttpReq::CreateHttpReq();
		http_req->SetUrl(STR(url));
		http_req->SetConnectTimeout(1000);
		http_req->SetHeader("Content-Type", "application/json");
		http_req->SetTimeout(10000);
		http_req->SetReqBody(STR(reqString),reqString.length());
		//DEBUG("start url=[%s] req=[%s]",STR(url),STR(reqString));
		int errorcode=http_req->Post();
		if(errorcode!=0){
			WARN("call http post error url=[%s],curl code1=%d,[%s]",STR(url),errorcode,STR(reqString));
			//28timeout, 7cant connection, 55send data fail
			sleep(10);
			if(errorcode=http_req->Post() != 0){
				WARN("call http post error url=[%s],curl code2=%d",STR(url),errorcode);
				sleep(50);
				if(errorcode=http_req->Post() != 0){
					ERROR("call http post error url=[%s],curl code3=%d",STR(url),errorcode);
					//ret=ErrCode::ERR_HTTP_OPER_ERR;
					ret=-1;
					break;
				}
			}
		}
		*res=http_req->GetBody();
		//DEBUG("end url=[%s] res=[%s]",STR(url),STR(res));
	}while(false);
	if(http_req!=NULL){
		MyHttpReq::ReleaseHttpReq(http_req);
	}
	return ret;
}



