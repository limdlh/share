#ifndef _DATA_SWITCH_H_
#define _DATA_SWITCH_H_
 
#include "redis_adapter.h"
#include "itwconfig.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <zmq.h>


struct CTX_ITW{
    void *context;
    ITWCONFIG* configer;
};

enum { CAP = 256 };

class DataSwitch
{
public:
	DataSwitch();
	~DataSwitch();
	bool init(CTX_ITW* p_ctx_itw);
    static void* fetch_thread(void* args);
	static void* push_thread(void* args);
	bool fetch_Data();
	bool push_Data();
	int structToJSON(Variant &req, char *buffer, string &reqString);
	int httpPost(string& url, string &reqString, string *res);
	bool createFectchProxy();
	void create_proxy();

private:
	void *m_context;
	ITWCONFIG* m_configer;
    void *m_fetch_clients;
	void *m_fetch_server;

	string m_fetch_zmq_inproc;
	string m_redis_ip;
	int m_redis_port;
	string m_redis_passwd;
	string m_fetch_zmq_url;

	string m_http_post_url;
};
 
#endif  //_DATA_SWITCH_H_


