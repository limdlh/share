#include "redis_adapter.h"


using namespace std;

RedisAdapter::RedisAdapter(string& host, int port, string& passwd)
{
    m_host = host;
	m_port = port;
	m_passwd = passwd;
	m_online = false;
}

RedisAdapter::~RedisAdapter()
{
	this->m_connect = NULL;
	this->m_reply = NULL;
}

bool RedisAdapter::connect()
{
    //DEBUG("RedisAdapter::connect()");
	this->m_connect = redisConnect(m_host.c_str(), m_port);
	
	if(this->m_connect == NULL || this->m_connect->err)
	{
	    int i = 0;
	    do{
			sleep(2); //Wait two seconds to try again
			this->m_connect = redisConnect(m_host.c_str(), m_port);
			ERROR("connect error!!!! host: %s, port:%d, %s", m_host.c_str(), m_port, this->m_connect->errstr);
		}while((this->m_connect == NULL || this->m_connect->err)&&(i++ < 65535));

		if(i >= 65535)
		{
		    FATAL("Can not connect to host: %s, port:%d, %s", m_host.c_str(), m_port, this->m_connect->errstr);
			return false;
		}
	}

	if(this->requirePassWord() == true)
	{
		if(this->authenticate() == true)
		{
			INFO("authenticate success");
		}
		else
		{
			ERROR("authenticate fail!!! Input password: %s", m_passwd.c_str());
		}
	}

	this->m_online = true;
	
	return true;
}

bool RedisAdapter::requirePassWord()
{
    //DEBUG("RedisAdapter::requirePassWord()");
	bool retBool = false;
	int replyType = -1;
	size_t found = 0;
	
    this->m_reply = (redisReply*)redisCommand(this->m_connect, "CONFIG get requirepass");
	if(this->m_reply == NULL)
	{
		ERROR("this->m_reply == NULL, [CONFIG get requirepass] fail, please check network!!!!!!!!!!");
		return false;
	}
    
	size_t repArraySize = 0;
	string strReply;
	
	replyType = this->m_reply->type;
	switch (replyType) 
	{
		case REDIS_REPLY_ERROR:
		    strReply = this->m_reply->str;
			WARN("requirepass() type: %d, REDIS_REPLY_ERROR, error reply->str: %s", replyType, this->m_reply->str);
			found = strReply.find("NOAUTH Authentication required");
			if(found!=string::npos)//Password setted
			{
				WARN("requirepass()  Password setted, please input right password to login");
				retBool = true;
			}
			break;
		case REDIS_REPLY_ARRAY:
			repArraySize = this->m_reply->elements;
			if(repArraySize == 2)
			{
				strReply = this->m_reply->element[1]->str;
				//DEBUG("requirepass() type: %d, REDIS_REPLY_ARRAY; strReply: %s", replyType, strReply.c_str());
				if(strReply.length() <= 0)//No password
				{
					WARN("requirepass() No password, no need to login with password");
					retBool = false;
				}
			}
			else
			{
				WARN("requirepass() REPLY_ARRAY size abnormal, please check!!!!! reply->elements: %zu", repArraySize);
				for (size_t j = 0; j < this->m_reply->elements; j++) 
				{
					WARN("%zu) %s\n", j, this->m_reply->element[j]->str);
				}
				WARN("requirepass() elements list display above ^^^^^^^^^^^^^");
			}
			break;
		default:
			WARN("switch to default, requirepass() replay unhandle type:[%d]", replyType);
	}

    freeReplyObject(this->m_reply);
    return retBool;
}

bool RedisAdapter::authenticate()
{
    //DEBUG("RedisAdapter::authenticate()");
	string strCommand = "AUTH " + m_passwd;
    this->m_reply = (redisReply*)redisCommand(this->m_connect, strCommand.c_str());
	DEBUG("type: %d, reply->str: %s, strCommand: %s", this->m_reply->type, this->m_reply->str, strCommand.c_str());
	
	int replyType = -1;
	size_t found;
	bool retBool = false;
	string strReply;
	
	replyType = this->m_reply->type;
	switch (replyType) 
	{
		case REDIS_REPLY_ERROR:
		    strReply = this->m_reply->str;
			WARN("type: %d, REDIS_REPLY_ERROR, error reply->str: %s\n", replyType, this->m_reply->str);
			found = strReply.find("ERR AUTH <password> called without any password configured for the default user");
			/* Whole replay content: ERR AUTH <password> called without any password configured for the default user. Are you sure your configuration is correct? */
			if(found!=string::npos)//No password setted, just return true
			{
				WARN("No password setted, no need to login with password, just return true");
				retBool = true;
			}
			break;
		case REDIS_REPLY_STATUS:
			strReply = this->m_reply->str;
			//DEBUG("type: %d, REDIS_REPLY_STATUS; reply str: %s", replyType, strTemp.c_str());
			if(strReply.compare("OK") == 0)
			{
				INFO("authenticate() success\n");
				retBool = true;
			}
			else
			{
				found = strReply.find("WRONGPASS");
				 /* Whole replay content: (error) WRONGPASS invalid username-password pair or user is disabled. */
				if(found!=string::npos)
				{
					ERROR("authenticate() fail, password is wrong, please check!!!!");
					retBool = false;
				}
			}
			break;
		default:
			WARN("switch to default, authenticate() replay unhandle type:[%d]", replyType);
	}
	
    freeReplyObject(this->m_reply);
	return retBool;
}

string RedisAdapter::get(string key)
{
	this->m_reply = (redisReply*)redisCommand(this->m_connect, "GET %s", key.c_str());
	string str = this->m_reply->str;
	freeReplyObject(this->m_reply);
	return str;
}

void RedisAdapter::set(string key, string value)
{
	redisCommand(this->m_connect, "SET %s %s", key.c_str(), value.c_str());
}

int RedisAdapter::pop(string& value, int data_type)
{
    //DEBUG("RedisAdapter::pop()");
	if(data_type == PULL_PUSH_DATA)
	{
		this->m_reply = (redisReply*)redisCommand(this->m_connect, "BLPOP REDISMQ_PULL_PUSH 5");//Block POP for 3 seconds
	}
	else// if(data_type == PUB_SUB_DATA)
	{
		this->m_reply = (redisReply*)redisCommand(this->m_connect, "BLPOP REDISMQ_PUB_SUB 5");//Block POP for 3 seconds
	}
	
	if(this->m_reply == NULL)
	{
		ERROR("error, this->m_reply == NULL, can not connect to redis server, please check network!!!!!!!!");
		m_online = false;
		return REDIS_REPLY_ERROR;
	}

	int replyType = -1;
	size_t repArraySize = 0;
	replyType = this->m_reply->type;
	switch (replyType) 
	{
		case REDIS_REPLY_NIL:
			//INFO("BLPOP type: %d, REDIS_REPLY_NIL, no data", replyType);
			break;
		case REDIS_REPLY_ERROR:
			ERROR("BLPOP type: %d, REDIS_REPLY_ERROR, reply->str: %s", replyType, this->m_reply->str);
			m_online = false;
			break;
		case REDIS_REPLY_STRING:
			value = this->m_reply->str;
			//DEBUG("BLPOP type: %d, REDIS_REPLY_STRING; pop str: %s ", replyType, value.c_str());
			break;
		case REDIS_REPLY_ARRAY:
			repArraySize = this->m_reply->elements;
			if(repArraySize == 2)
			{
				value = this->m_reply->element[1]->str;
				//DEBUG("BLPOP type: %d, REDIS_REPLY_ARRAY; pop str: %s", replyType, value.c_str());
			}
			else
			{
				WARN("REPLY_ARRAY size abnormal, please check!!!!! reply->elements: %zu", repArraySize);
				for (size_t j = 0; j < this->m_reply->elements; j++) 
				{
					WARN("%zu) %s", j, this->m_reply->element[j]->str);
				}
				WARN("elements list display above ^^^^^^^^^^^^^");
			}
			break;
		default:
			WARN("switch to default, BLPOP unhandle type:[%d]", replyType);
	}

	freeReplyObject(this->m_reply);
	
	return replyType;
}

int RedisAdapter::push(string& value, int data_type)
{
    //DEBUG("RedisAdapter::push() value: %s", value.c_str());
	if(data_type == PULL_PUSH_DATA)
	{
		this->m_reply = (redisReply*)redisCommand(this->m_connect, "RPUSH REDISMQ_PULL_PUSH %s", value.c_str());
	}
	else// if(data_type == PUB_SUB_DATA)
	{
		this->m_reply = (redisReply*)redisCommand(this->m_connect, "RPUSH REDISMQ_PUB_SUB %s", value.c_str());
	}

	if(this->m_reply == NULL)
	{
	    ERROR("RedisAdapter disconnect now, this->m_reply == NULL, push fail, please check network!!!!!!!!!!");
		m_online = false;
		return REDIS_REPLY_ERROR;
	}

    int replyType = -1;
	replyType = this->m_reply->type;
	switch (replyType) 
	{
		case REDIS_REPLY_ERROR:
			ERROR("RPUSH type: %d, REDIS_REPLY_ERROR, push error str: %s", replyType, this->m_reply->str);
			m_online = false;
			break;
		case REDIS_REPLY_INTEGER:
			//DEBUG("RPUSH type: %d, REDIS_REPLY_INTEGER, OK, success", replyType);
			break;
		default:
			WARN("RPUSH unhandle type:[%d], m_reply->integer: %lld", replyType, this->m_reply->integer);
	}

	freeReplyObject(this->m_reply);

	return replyType;
}

