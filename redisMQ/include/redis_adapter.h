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
 int    type;            //��Ϣ����
 int    pid;             //��ĿID
 int    subpid;          //����ĿID
 int    agentid;         //��ϯId
 int    staffid;         //Ա��Id
 char   uuid[64];        //ͨ��ID
 char   callnum[16];     //�û�����
 char   accessnum[16];   //���к���
 time_t calltime;        //����ʱ��
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