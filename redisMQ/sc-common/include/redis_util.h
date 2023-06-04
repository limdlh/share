#ifndef REDIS_UTIL_H
#define REDIS_UTIL_H
#include "rediswrapper.h"

class RedisUtil
{
private:
	static RedisWrapper redis;
public:
	/*
	  Redis ��ʼ�������֧��255���ն�
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
	*return 0 �ɹ�������ʧ��,res Ϊ�������,ʧ��ʱΪ����ԭ��
	*/
	static int runCommand(Variant& res,const char* format, ...);

	/*
	*return 0 �ɹ�������ʧ��,res Ϊ�������,ʧ��ʱΪ����ԭ��
	*/
	static int vRunCommand(Variant& res,const char* format, va_list ap);

	/*
	*return 0 �ɹ�������ʧ��
	*/
	static int runCommand(const char* format, ...);

	/*
	*lockname �����ƣ�acquireTimeout ��ȡ����ʱʱ��(s),timeout ���ĳ�ʱʱ��(s)
	*/
	static bool lockWithTimeout(string lockname,int acquireTimeout, int timeout);

	/*
	*�ͷ���
	*/
	static bool unlock(string lockname);
	
};
#endif
