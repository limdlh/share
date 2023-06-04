#ifndef _MY_HTTP_REQ_H
#define _MY_HTTP_REQ_H

#include<map>
#include<string>
using namespace std;
class MyHttpReq{
private:
	static bool m_init_flags;
	static pthread_mutex_t m_init_flag_lock;
	const char *m_req_body;
	int m_req_body_size;
	
	string m_res_body;

	char m_url[256];
	map<string,string> m_req_header;
	map<string,string> m_res_header;

	long m_err_code;
	int m_timeout;  //超时时间
	int m_connect_timeout;	//连接超时时间

	char m_cooki_file[256];
	
private:
	void reset_body();
	
public:
	MyHttpReq();
	~MyHttpReq();
	static MyHttpReq *CreateHttpReq();
	static void ReleaseHttpReq(MyHttpReq * myReq);
	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp); 
	static size_t header_callback(char *buffer, size_t size,size_t nitems, void *userdata);


	//设置url
	void SetUrl(const char *url);

	//设置http头
	void SetHeader(const char *key,const char *value);
	//设置超时时间
	void SetTimeout(int timeout);
	//设置connect 超时时间
	void SetConnectTimeout(int connect_timeout);

	//设置缓存文件
	void SetCookiFile(char *path);
	//设置请求的body
	void SetReqBody(const char *body,int len);
	//获取http返回头
	char * GetHeader(char *key);
	//获取http返回 body
	char *GetBody(int *len);

	string GetBody();

	//获取错误码
	int GetErrCode();
	int Post();
	int Get();
	
	
};




#endif
