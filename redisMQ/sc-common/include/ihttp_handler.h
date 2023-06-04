#ifndef _I_HTTP_HANDLER_
#define _I_HTTP_HANDLER_
#include "variant.h"
#include "ihttp_handler_creater.h"

class  IHttpHandler{
protected:
	IHttpHandlerCreater *m_handler_creater;
	string m_req_ip;		//����IP
	string m_url_path;		//����url path
	Variant m_in_header;	//������Ϣͷ
	Variant m_out_header;	//������Ϣͷ
public:
	IHttpHandler();
	virtual ~IHttpHandler();

	/*����m_handler_creater*/
	void set_handler_creater(IHttpHandlerCreater *creater);

	/*��ȡm_handler_creater*/
	IHttpHandlerCreater * get_handler_creater();
	
	//����req IP
	virtual void set_req_ip(string ip);
	
	//��ȡreq IP
	virtual string get_req_ip();

	//����url path
	virtual void set_url_path(string path);
	
	//��ȡurl path
	virtual string get_url_path();

	//������Ϣͷ
	virtual void  set_in_header(Variant &header);

	//��ȡ��Ϣͷ
	virtual Variant & get_in_header();

	//��ȡ������Ϣͷ
	virtual Variant &get_out_header();

	//���÷�����Ϣͷ
	virtual void set_out_header(Variant & header);

	//���÷�����Ϣͷ
	virtual void set_out_header(string key,string value);
	
	virtual void handle(string &input,string &output)=0;
};

#endif

