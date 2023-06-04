#ifndef _IHTTP_HANDLER_CREATER_H
#define _IHTTP_HANDLER_CREATER_H

class IHttpHandler;

class IHttpHandlerCreater{
private:

public:
	IHttpHandlerCreater(){};
	virtual ~IHttpHandlerCreater(){};
	/*
	*创建IhttpHandler成功返回IHttpHandler,失败返回NULL
	*/
	virtual IHttpHandler * create(string path)=0;
	/*
	*释放IHttpHandler,成功返回0,失败返回-1
	*/
	virtual int release(IHttpHandler * handler)=0;

};
#endif