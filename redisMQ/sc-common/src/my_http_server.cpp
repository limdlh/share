#include<event2/thread.h>
#include<event2/bufferevent.h>
#include "linuxplatform.h"
#include "logging.h"
#include "my_thread_pool.h"
#include "ihttp_factory.h"
#include "my_http_server.h"
#include "format.h"



MyHttpServer::MyHttpServer(){
	
}

MyHttpServer::~MyHttpServer(){
	
}

MyHttpServer* MyHttpServer::create(){
	return new MyHttpServer;
}

void MyHttpServer::release(MyHttpServer *server){
	delete server;
}

static void event_log(int severity, const char *msg){
	switch (severity) {
	case EVENT_LOG_DEBUG:
		DEBUG("%s",msg);
		break;
	case EVENT_LOG_MSG:
		INFO("%s",msg);
		break;
	case EVENT_LOG_WARN:
		WARN("%s",msg);
		break;
	case EVENT_LOG_ERR:
		ERROR("%s",msg);
		break;
	default:
		INFO("unkonw level:%s",msg);
		break;
	}
}
bool MyHttpServer::init_server(string ip,short port){
	m_ip=ip;
	m_port=port;
	event_set_log_callback(event_log);
	return true;
}


bool MyHttpServer::start_server(){
	pthread_t pid;
	if(pthread_create(&pid,NULL,http_server_callback,this)==0){			//这个地方不使用线程池
		INFO("start http server [%s:%d]...",STR(m_ip),m_port);
		return true;
	}
	else{
		ERROR("start http server error");
		return false;
	}
}

static struct bufferevent* genbev(struct event_base *base , void *args){
	return bufferevent_socket_new(base, -1, BEV_OPT_THREADSAFE);
}

void * MyHttpServer::http_server_callback(void *args){
	pthread_detach(pthread_self());
	MyHttpServer *server=(MyHttpServer*)args;
	struct event_base *base;
	struct evhttp *http;
	struct evhttp_bound_socket *handle;

	unsigned short port = 0;
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){
		ERROR("SIGPIPE SIG_IGN error");
		return NULL;
	}
	evthread_use_pthreads();
	base = event_base_new();
	if (!base) {
		ERROR( "Couldn't create an event_base");
		return NULL;
	}

	http=evhttp_new(base);
	if(!http){
		ERROR( "couldn't create evhttp. Exiting");
		return NULL;
	}
	evhttp_set_gencb(http,handle_req_callback,server);
	evhttp_set_bevcb(http,genbev,base);
	handle = evhttp_bind_socket_with_handle(http, STR(server->m_ip), server->m_port);
	if (!handle) {
		ERROR("couldn't bind to port %d. Exiting",
		    server->m_port);
		return NULL;
	}
	event_base_dispatch(base);
}



//处理消息线程
class ReqProcess:public MyRunable{
	MyHttpServer * m_service;
	struct evhttp_request *m_http_req;
public:
	ReqProcess(MyHttpServer* service,struct evhttp_request *req){
		m_service=service;
		m_http_req=req;
	}
	
	virtual ~ReqProcess(){
		
	}

	
	void Run(){
		struct evhttp_request *req=m_http_req;
		evhttp_uri *decoded=NULL;
		struct evbuffer *buf=NULL;
		struct evkeyvalq *headers;
		struct evkeyval *header;
		const char *path=NULL;
		char *body=NULL;
		int body_len=0,header_code=200;
		const char *uri=evhttp_request_get_uri(req);
		string input,output,header_errmsg="OK";
		Variant in_header,out_header;
		do{
			decoded=evhttp_uri_parse(uri);				//解析uri
			if(decoded ==NULL){
				break;
			}
			path=evhttp_uri_get_path(decoded);			//获取路径,/xxx/xx
			INFO("path=[%s]",path);
			buf = evhttp_request_get_input_buffer(req);		//获取body
			body_len=evbuffer_get_length(buf);
			body=new char[body_len+1];
			evbuffer_remove(buf,body,body_len);
			body[body_len]='\0';
			input=body;

			headers = evhttp_request_get_input_headers(req);		//获取req  header
			for (header = headers->tqh_first; header; header = header->next.tqe_next) {
				in_header[(const char*)header->key]=header->value;
			}
			

			IHttpHandler *handler=IHttpFactory::create(path);		//创建http handler

			if(handler!=NULL){
				//DEBUG("\ninput->%s\n[%s]\n",path,input.c_str());
				handler->set_in_header(in_header);
				handler->set_req_ip(req->remote_host?req->remote_host:"");
				handler->set_url_path(path);
				handler->handle(input,output);
				out_header=handler->get_out_header();
				//DEBUG("\noutput->%s\n[%s]\n",path,output.c_str());
				IHttpFactory::release(handler);
			}
			else{
				//DEBUG("\ninput->%s\n[%s]\n",path,input.c_str());
				output=format("not found,path=[%s]",path);
			//	DEBUG("\noutput->%s\n[%s]\n",path,output.c_str());
			}
			
			
			headers=evhttp_request_get_output_headers(req);
			//发送完关闭连接
			evhttp_add_header(headers,"Connection","close");
			if(out_header==V_MAP){
				FOR_MAP(out_header,string,Variant,h){
					string key=h->first;
					string value=h->second;
					evhttp_add_header(headers,key.c_str(),value.c_str());
				}
			}
			if(output=="")
				evhttp_send_reply(req,header_code,STR(header_errmsg),NULL);
			else{
				evbuffer *evb=evbuffer_new();
				evbuffer_add(evb,STR(output),output.length());
				evhttp_send_reply(req, header_code,STR(header_errmsg), evb);
				evbuffer_free(evb);
			}
		}while(0);
		if(body!=NULL)
			delete []body;
		if(decoded!=NULL){
			evhttp_uri_free(decoded);
		}
	};
};

//把消息放入到线程中去执行
void MyHttpServer::handle_req_callback(struct evhttp_request *req, void *arg){
	
	ReqProcess *reqProcess=new ReqProcess((MyHttpServer*)arg,req);
	reqProcess->set_needed_delete(true);
	if(MyThreadPoolManager::start(reqProcess)){
		evhttp_send_reply(req,501,"Innr Error",NULL);
	}
}





