#ifndef _IHTTP_HANDLER_CREATER_H
#define _IHTTP_HANDLER_CREATER_H

class IHttpHandler;

class IHttpHandlerCreater{
private:

public:
	IHttpHandlerCreater(){};
	virtual ~IHttpHandlerCreater(){};
	/*
	*����IhttpHandler�ɹ�����IHttpHandler,ʧ�ܷ���NULL
	*/
	virtual IHttpHandler * create(string path)=0;
	/*
	*�ͷ�IHttpHandler,�ɹ�����0,ʧ�ܷ���-1
	*/
	virtual int release(IHttpHandler * handler)=0;

};
#endif