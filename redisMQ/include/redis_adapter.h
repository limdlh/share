#ifndef _REDIS_ADAPTER_H_
#define _REDIS_ADAPTER_H_
 
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <hiredis/hiredis.h>
#include "mycommon.h"


using namespace std;

typedef struct _CALL_NOTIFY{
 int    type;            //消息类型
 int    pid;             //项目ID
 int    subpid;          //子项目ID
 int    agentid;         //坐席Id
 int    staffid;         //员工Id
 char   uuid[64];        //通话ID
 char   callnum[16];     //用户号码
 char   accessnum[16];   //被叫号码
 time_t calltime;        //拨打时间
}CALL_NOTIFY_, *PCALL_NOTIFY;


class RedisAdapter
{
public:
    enum Redis_Data_Type {PULL_PUSH_DATA = 0, PUB_SUB_DATA, Defalt};
	bool m_online;
	
    RedisAdapter(string& host, int port, string& passwd);
    ~RedisAdapter();
    bool connect();
	bool authenticate();
    bool requirePassWord();
    string get(string key);
    void set(string key, string value);
    int pop(string& value, int data_type);
    int push(string& value, int data_type);

private:
    redisContext* m_connect;
    redisReply* m_reply;
	string m_host;
	string m_passwd;
	int m_port;
};
 
#endif  //_REDIS_ADAPTER_H_