#include "mycommon.h"
#include "redis_util.h"


RedisWrapper RedisUtil::redis;

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

int RedisUtil::init(string endpoint, int socktNum, int contimeout, int rwtimeout, int conretry){
	return redis.init(endpoint,socktNum,contimeout,rwtimeout,conretry);
}

/*
*return 0 成功，其他失败,res 为结果内容,失败时为错误原因
*/
int RedisUtil::vRunCommand(Variant& res,const char* format, va_list ap){
	int ret=0;
	redisReply *reply=NULL;
	REDIS_SOCKET*  socket=NULL;
	do{
		socket=redis.GetSocket();
		if(socket==NULL){
			res="get redis socket error";
			ret=-1;
			break;
		}
	    reply = redis.vRunCmd(socket,format, ap);
		if(reply==NULL){
			res="exec command error";
			ret=-1;
			break;
		}
		
		switch(reply->type){
			case REDIS_REPLY_STRING:
				res=reply->str;
				break;
			case REDIS_REPLY_ARRAY:
				res.Reset();
				for(int j = 0; j < reply->elements; j++){
					res.PushToArray(reply->element[j]->str);
				}
				break;
			case REDIS_REPLY_INTEGER:
				res=(int64_t)reply->integer;
				break;
			case REDIS_REPLY_NIL:
				res.Reset(false);
				break;
			case REDIS_REPLY_STATUS:
				res=reply->str;
				if(strncmp(reply->str,"OK",2))
					ret=-1;
				break;
			case REDIS_REPLY_ERROR:
				res=reply->str;
				ret=-1;
				break;
		}
		
	}while(0);
	
	if(reply!=NULL){
		freeReplyObject(reply);
	}
	
	if(socket!=NULL){	
		redis.ReleaseSocket(socket);
	}
	return ret;
}


/*
*return 0 success,other failed
*/
int RedisUtil::runCommand(Variant& res,const char* format, ...){
	int ret=0;
	va_list ap;
	va_start(ap, format);
	ret=vRunCommand(res,format,ap);
	va_end(ap);
	/*va_start(ap, format);
	vprintf(format,ap);
	va_end(ap);
	printf("\n");
	*/
	return ret;
}

int RedisUtil::runCommand(const char* format, ...){
	int ret=0;
	Variant tmp;
	va_list ap;
	va_start(ap, format);
	ret=vRunCommand(tmp,format,ap);
	va_end(ap);
	/*va_start(ap, format);
	vprintf(format,ap);
	va_end(ap);
	printf("\n");
	*/
	return ret;
}

/*
*lockname 锁名称，acquireTimeout 获取锁超时时间(s),timeout 锁的超时时间(s)
*/
bool RedisUtil::lockWithTimeout(string lockname,int acquireTimeout, int timeout){
	bool ret=false;
	do{
		time_t endtime=time(NULL)+acquireTimeout;
		Variant res;
		int tmp=0;
		do{
			if(timeout<=0){
				tmp=runCommand(res,"SET %s %d NX",STR(lockname),time(NULL));
			}else{
				tmp=runCommand(res,"SET %s %d EX %d NX",STR(lockname),time(NULL),timeout);
			}
			if(tmp!=0){
				break;
			}
			if(res!=V_NULL ){	// 返回非null ，获取到锁，跳出
				 ret=true;
				 break;
			}
			if(acquireTimeout>0){	//有获取锁超时时间，进循环等待获取
				usleep(40);
			}else{
				break;
			}
		}while(time(NULL)<endtime);
	}while(0);
	return ret;
}

/*
*释放锁
*/
bool RedisUtil::unlock(string lockname){
	if(runCommand("DEL %s",STR(lockname))!=0){
		return false;
	}
	return true;
	
}



