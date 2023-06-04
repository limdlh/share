#include "ihttp_handler.h"




IHttpHandler::IHttpHandler(){
	m_handler_creater=NULL;
}
IHttpHandler::~IHttpHandler(){
}

/*����m_handler_creater*/
void IHttpHandler::set_handler_creater(IHttpHandlerCreater *creater){
	m_handler_creater = creater;
}

/*��ȡm_handler_creater*/
IHttpHandlerCreater * IHttpHandler::get_handler_creater(){
	return m_handler_creater;
}

//����req IP
void IHttpHandler::set_req_ip(string ip){
	m_req_ip=ip;
}
	
//��ȡreq IP
string IHttpHandler::get_req_ip(){
	return m_req_ip;
}

//����url path
void IHttpHandler::set_url_path(string path){
	m_url_path=path;
}
	
//��ȡurl path
string IHttpHandler::get_url_path(){
	return m_url_path;
}

	//������Ϣͷ
void  IHttpHandler::set_in_header(Variant &header){
	m_in_header=header;
}
//��ȡ��Ϣͷ
Variant & IHttpHandler::get_in_header(){
	return m_in_header;
}

//��ȡ������Ϣͷ
Variant &IHttpHandler::get_out_header(){
	return m_out_header;
}

//���÷�����Ϣͷ
void IHttpHandler::set_out_header(Variant & header){
	m_out_header=header;
	
}

//���÷�����Ϣͷ
void IHttpHandler::set_out_header(string key,string value){
	m_out_header[key]=value;
}