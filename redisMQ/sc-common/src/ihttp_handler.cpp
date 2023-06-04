#include "ihttp_handler.h"




IHttpHandler::IHttpHandler(){
	m_handler_creater=NULL;
}
IHttpHandler::~IHttpHandler(){
}

/*设置m_handler_creater*/
void IHttpHandler::set_handler_creater(IHttpHandlerCreater *creater){
	m_handler_creater = creater;
}

/*获取m_handler_creater*/
IHttpHandlerCreater * IHttpHandler::get_handler_creater(){
	return m_handler_creater;
}

//设置req IP
void IHttpHandler::set_req_ip(string ip){
	m_req_ip=ip;
}
	
//获取req IP
string IHttpHandler::get_req_ip(){
	return m_req_ip;
}

//设置url path
void IHttpHandler::set_url_path(string path){
	m_url_path=path;
}
	
//获取url path
string IHttpHandler::get_url_path(){
	return m_url_path;
}

	//设置消息头
void  IHttpHandler::set_in_header(Variant &header){
	m_in_header=header;
}
//获取消息头
Variant & IHttpHandler::get_in_header(){
	return m_in_header;
}

//获取返回消息头
Variant &IHttpHandler::get_out_header(){
	return m_out_header;
}

//设置返回消息头
void IHttpHandler::set_out_header(Variant & header){
	m_out_header=header;
	
}

//设置返回消息头
void IHttpHandler::set_out_header(string key,string value){
	m_out_header[key]=value;
}