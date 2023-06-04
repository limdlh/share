#ifndef _I_HTTP_HANDLER_
#define _I_HTTP_HANDLER_
#include "variant.h"
#include "ihttp_handler_creater.h"

class  IHttpHandler{
protected:
	IHttpHandlerCreater *m_handler_creater;
	string m_req_ip;		//请求IP
	string m_url_path;		//请求url path
	Variant m_in_header;	//请求消息头
	Variant m_out_header;	//返回消息头
public:
	IHttpHandler();
	virtual ~IHttpHandler();

	/*设置m_handler_creater*/
	void set_handler_creater(IHttpHandlerCreater *creater);

	/*获取m_handler_creater*/
	IHttpHandlerCreater * get_handler_creater();
	
	//设置req IP
	virtual void set_req_ip(string ip);
	
	//获取req IP
	virtual string get_req_ip();

	//设置url path
	virtual void set_url_path(string path);
	
	//获取url path
	virtual string get_url_path();

	//设置消息头
	virtual void  set_in_header(Variant &header);

	//获取消息头
	virtual Variant & get_in_header();

	//获取返回消息头
	virtual Variant &get_out_header();

	//设置返回消息头
	virtual void set_out_header(Variant & header);

	//设置返回消息头
	virtual void set_out_header(string key,string value);
	
	virtual void handle(string &input,string &output)=0;
};

#endif

