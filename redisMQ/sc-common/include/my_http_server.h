#ifndef _MY_HTTP_SERVER_H
#define _MY_HTTP_SERVER_H
#include<event2/event.h>
#include<event2/http.h>
#include<event2/http_struct.h>
#include<event2/buffer.h>
#include <event2/keyvalq_struct.h>
#include "ihttp_handler.h"


class MyHttpServer{
private:
	string m_ip;
	int m_port;
	MyHttpServer();
	~MyHttpServer();
public:
	static MyHttpServer* create();
	static void release(MyHttpServer *server);
	bool init_server(string ip,short port);
	bool start_server();
	static void *http_server_callback(void *args);
	static void handle_req_callback(struct evhttp_request *req, void *arg);
};


#endif //_MY_HTTP_SERVER_H

