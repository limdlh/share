#include "mycommon.h"
#include "rediswrapper.h"
#include <vector>

using namespace std;

RedisWrapper::RedisWrapper(){}
RedisWrapper::~RedisWrapper(){
	redis_pool_destroy(m_inst);
}
/*
  Redis 初始化，最多支持255个终端
	@param endpoint  : host,port,db,passwd;host,port,db,passwd
	@param socktNum  : 20
	@param contimeout: 10000
	@param rwtimeout : 5000
	@param conretry  : 1
	
redis_endpoints=192.168.173.111,6379,1,0 //host,port,db,password
redis_conn_timeout=10000
redis_readwrite_timeout=5000
redis_socket_num=20
redis_conn_retry_delay=1
*/
int RedisWrapper::init(string &endpoint, int socktNum, int contimeout, int rwtimeout, int conretry){
    int iEndpointNum=1;
	REDIS_ENDPOINT endpoints[255]={0};
	INFO("RedisWrapper endpoint.[%s]", STR(endpoint));
	if (endpoint.length() < 1){
		endpoint = "127.0.0.1,6379,0,0";
	}
	vector<string> vcHost;
	split(endpoint, ";", vcHost);
	iEndpointNum=vcHost.size();

	for(int j=0; j<iEndpointNum && j<254; j++){
		vector<string> tmpHost;
        REDIS_ENDPOINT tmpEnd={0};  
		//split(vcHost[j].substr(1,vcHost[j].length()-2), ",", tmpHost);
		split(vcHost[j], ",", tmpHost);
  		if(tmpHost.size() > 0) memcpy(tmpEnd.host, tmpHost[0].c_str(), tmpHost[0].length());
		if(tmpHost.size() > 1) tmpEnd.port = atoi(tmpHost[1].c_str());
		if(tmpHost.size() > 2) tmpEnd.db = atoi(tmpHost[2].c_str());
		if(tmpHost.size() > 3) memcpy(tmpEnd.password, tmpHost[3].c_str(), tmpHost[3].length());
        endpoints[j]=tmpEnd;
		DEBUG("RedisWrapper endpoint for[%s] [%s][%d][%d][%s]", vcHost[j].c_str(), tmpEnd.host, tmpEnd.port, tmpEnd.db, tmpEnd.password);
	}
	//REDIS_ENDPOINT endpoints[1] = {
	//{ "192.168.173.111", 6379 }
	//};

	REDIS_CONFIG conf = {
		(REDIS_ENDPOINT*)&endpoints,    //host,port
		iEndpointNum,                              //num_endpoints
		(contimeout?contimeout:10000),                          //connect_timeout
		(rwtimeout?rwtimeout:5000),                           //net_readwrite_timeout
		(socktNum? socktNum:20),                             //num_redis_socks
		(conretry?conretry:1),                              //connect_failure_retry_delay
	};

	if (redis_pool_create(&conf, &m_inst) < 0){
		ERROR("redis pool create fail.");
		return 1;
	}
	DEBUG("RedisWrapper end.");
	return 0;
}

REDIS_SOCKET*  RedisWrapper::GetSocket(){
	REDIS_SOCKET* sock;
	if ((sock = redis_get_socket(m_inst)) == NULL) {
		ERROR("get socket null[%s][%d].", m_inst->config->endpoints->host, m_inst->config->endpoints->port);
		return NULL;
	}
	return sock;
}

void RedisWrapper::ReleaseSocket(REDIS_SOCKET* sock){
	redis_release_socket(m_inst, sock);
}

redisReply* RedisWrapper::RunCmd(REDIS_SOCKET* sock, const char* format, ...){
	va_list ap;
    void *reply;
    va_start(ap, format);
    reply = vRunCmd(sock,format, ap);
    va_end(ap);
    return (redisReply*)reply;
}

redisReply* RedisWrapper::vRunCmd(REDIS_SOCKET* sock, const char* format, va_list ap){
	return (redisReply*)redis_vcommand(sock,m_inst,format,ap);
}





