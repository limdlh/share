#ifndef REDIS_UTIL_H
#define REDIS_UTIL_H
#include "rediswrapper.h"

class RedisUtil
{
private:
	static RedisWrapper redis;
public:
	/*
	  Redis 初始化，最多支持255个终端
	    @param endpoint  : host,port,db;host,port,db
		@param socktNum  : 20
		@param contimeout: 10000
		@param rwtimeout : 5000
		@param conretry  : 1
		
	redis_endpoints=192.168.173.111,6379,1 //host,port,db
	redis_conn_timeout=10000
	redis_readwrite_timeout=5000
	redis_socket_num=20
	redis_conn_retry_delay=1
	*/
	static int init(string endpoint, int socktNum, int contimeout, int rwtimeout, int conretry);

	/*
	*return 0 成功，其他失败,res 为结果内容,失败时为错误原因
	*/
	static int runCommand(Variant& res,const char* format, ...);

	/*
	*return 0 成功，其他失败,res 为结果内容,失败时为错误原因
	*/
	static int vRunCommand(Variant& res,const char* format, va_list ap);

	/*
	*return 0 成功，其他失败
	*/
	static int runCommand(const char* format, ...);

	/*
	*lockname 锁名称，acquireTimeout 获取锁超时时间(s),timeout 锁的超时时间(s)
	*/
	static bool lockWithTimeout(string lockname,int acquireTimeout, int timeout);

	/*
	*释放锁
	*/
	static bool unlock(string lockname);
	
};
#endif
