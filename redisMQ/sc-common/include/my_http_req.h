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
	int m_timeout;  //��ʱʱ��
	int m_connect_timeout;	//���ӳ�ʱʱ��

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


	//����url
	void SetUrl(const char *url);

	//����httpͷ
	void SetHeader(const char *key,const char *value);
	//���ó�ʱʱ��
	void SetTimeout(int timeout);
	//����connect ��ʱʱ��
	void SetConnectTimeout(int connect_timeout);

	//���û����ļ�
	void SetCookiFile(char *path);
	//���������body
	void SetReqBody(const char *body,int len);
	//��ȡhttp����ͷ
	char * GetHeader(char *key);
	//��ȡhttp���� body
	char *GetBody(int *len);

	string GetBody();

	//��ȡ������
	int GetErrCode();
	int Post();
	int Get();
	
	
};




#endif
