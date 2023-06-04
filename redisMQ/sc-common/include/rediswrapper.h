#ifndef REDISWRAPPER_H
#define REDISWRAPPER_H

#include <hiredis/hiredis.h>
#include "hiredispool.h"

class RedisWrapper
{
public:
    RedisWrapper();
    ~RedisWrapper();

    /*
    endpoint  : host,port,db;host,port,db
	socktNum  : 20
	contimeout: 10000
	rwtimeout : 5000
	conretry  : 1
	*/
    int init(string &endpoint, int socktNum=20, int contimeout=10000, int rwtimeout=5000, int conretry=1);

    REDIS_SOCKET*  GetSocket();

    void ReleaseSocket(REDIS_SOCKET* sock);

	/**
	#define REDIS_REPLY_STRING 1  
	#define REDIS_REPLY_ARRAY 2 
	#define REDIS_REPLY_INTEGER 3
	#define REDIS_REPLY_NIL 4  
	#define REDIS_REPLY_STATUS 5
	#define REDIS_REPLY_ERROR 6 
	*/
    redisReply* RunCmd(REDIS_SOCKET* sock, const char* format, ...);

	
	/**
	#define REDIS_REPLY_STRING 1  
	#define REDIS_REPLY_ARRAY 2 
	#define REDIS_REPLY_INTEGER 3
	#define REDIS_REPLY_NIL 4  
	#define REDIS_REPLY_STATUS 5
	#define REDIS_REPLY_ERROR 6 
	*/
	redisReply* vRunCmd(REDIS_SOCKET* sock, const char* format, va_list ap);
	

private:
    REDIS_INSTANCE* m_inst;
};
#endif


