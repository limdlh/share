#include <curl/curl.h>
#include "mycommon.h"
#include "my_http_req.h"

#define HTTP_INIT_ERROR -1

bool MyHttpReq::m_init_flags=false;
pthread_mutex_t MyHttpReq::m_init_flag_lock=PTHREAD_MUTEX_INITIALIZER;
#define RES_BODY_DEFAULT_LEN 65535
MyHttpReq::MyHttpReq(){
	m_req_body=NULL;
	m_req_body_size = 0;
	m_err_code=0;
	m_timeout=-1;
	m_connect_timeout=-1;
	m_cooki_file[0]='\0';
	if(m_init_flags==false){
		pthread_mutex_lock(&m_init_flag_lock);
		if(m_init_flags==false){
			curl_global_init(CURL_GLOBAL_ALL); 
			m_init_flags=true;
		}
		pthread_mutex_unlock(&m_init_flag_lock);
	}
}

MyHttpReq::~MyHttpReq(){
	
	
}

void MyHttpReq::reset_body(){
	
}

MyHttpReq * MyHttpReq::CreateHttpReq(){
	return new MyHttpReq;
	
	
}

void MyHttpReq::ReleaseHttpReq(MyHttpReq * myReq){
	if(myReq!=NULL){
		delete myReq;
	}
}


size_t MyHttpReq::write_data(void *buffer, size_t size, size_t nmemb, void *userp){
	MyHttpReq *req = (MyHttpReq*)userp;
	int ret = size*nmemb;
	req->m_res_body.append((char*)buffer, ret);
	return ret;
}

size_t MyHttpReq::header_callback(char *buffer, size_t size,size_t nitems, void *userdata){
	MyHttpReq *req=(MyHttpReq*)userdata;
	int ret=size*nitems;
	char key[128]={0},value[512]={0};
	char *tmp1=NULL,*tmp2=NULL;
	int len=0;
	do{
		
		tmp1=strchr(buffer,':');
		if(tmp1==NULL)
			break;
		len=tmp1-buffer;
		len=len>sizeof(key)-1?sizeof(key)-1:len;
		strncpy(key,buffer,len);

		tmp1+=2;// skip [: ]
		tmp2=strstr(tmp1,"\r\n");
		if(tmp2==NULL)
			break;
		len=tmp2-tmp1;
		len=len>sizeof(value)-1?sizeof(value)-1:len;
		strncpy(value,tmp1,len);
		req->m_res_header[lowerCase(key)]=value;
		
	}while(0);
	return ret;
}
void MyHttpReq::SetUrl(const char *url){
    memset(m_url, 0, sizeof(m_url));
	strncpy(m_url,url,sizeof(m_url));
	
}
void MyHttpReq::SetHeader(const char *key,const char *value){
	m_req_header[key]=value;
}

//设置执行超时时间,单位毫秒
void MyHttpReq::SetTimeout(int timeout){
	m_timeout=timeout;
}
//设置connect 超时时间,单位毫秒
void MyHttpReq::SetConnectTimeout(int connect_timeout){
	m_connect_timeout=connect_timeout;
}

void MyHttpReq::SetCookiFile(char * path){
	strncpy(m_cooki_file,path,sizeof(m_cooki_file));
}

void MyHttpReq::SetReqBody(const char *body,int len){
	
	m_req_body=body;
	m_req_body_size=len;
}
char *  MyHttpReq::GetHeader(char *key){
	map<string,string>::iterator iter;
	iter=m_res_header.find(lowerCase(key));
	if(iter==m_res_header.end())
		return NULL;
	return (char*) iter->second.c_str();
}

char *MyHttpReq::GetBody(int *len){
	if(len!=NULL)
		*len=m_res_body.length();
	return (char*)m_res_body.c_str();
}

string MyHttpReq::GetBody(){
	return m_res_body;
}

int MyHttpReq::GetErrCode(){
	return m_err_code;
}

int MyHttpReq::Post(){
	reset_body();
	char tmpBuff[1024];
	map<string,string>::iterator iter;
	curl_slist *http_headers=NULL;
	CURL *easy_handle=NULL;
	easy_handle=curl_easy_init();
	if(easy_handle==NULL){
		m_err_code = HTTP_INIT_ERROR;
		return -1;
	}
	for(iter=m_req_header.begin();iter!=m_req_header.end();iter++){
		snprintf(tmpBuff,sizeof(tmpBuff),"%s: %s",iter->first.c_str(),iter->second.c_str());
		http_headers=curl_slist_append(http_headers,tmpBuff);
	}
	/*使用ipv4DNS解析*/
	curl_easy_setopt(easy_handle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
	curl_easy_setopt(easy_handle, CURLOPT_URL, m_url);
	/*禁止使用信号*/
	curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
	
	/*禁止重用SOCKET 解决CLOSE_WAIT和TIME_WAIT的超时时间*/
	curl_easy_setopt(easy_handle, CURLOPT_FORBID_REUSE, 1);
	/*设置超时时间*/
	//连接超时
	if(m_connect_timeout!=-1){
		curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, m_connect_timeout);
	}
	//等待超时
	if(m_timeout!=-1){
		curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, m_timeout);
	}
	if(m_req_body!=NULL){
		curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, m_req_body_size); 
		curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, (void*)m_req_body);
	}else{
		curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, (void*)NULL);
		curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, 0);
	}
	curl_easy_setopt(easy_handle,CURLOPT_HEADERFUNCTION,header_callback);
	curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA,this);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_data);  
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(easy_handle, CURLOPT_POST, 1);  
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1);
	if(http_headers!=NULL){
		curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, http_headers);
	}
	if(m_cooki_file[0]!=0)
   		curl_easy_setopt(easy_handle, CURLOPT_COOKIEFILE, m_cooki_file);  
	CURLcode ret_code=curl_easy_perform(easy_handle); 
	curl_slist_free_all(http_headers);
	if(ret_code!=CURLE_OK){
		ERROR("put %s error,error code =[%d]",m_url,ret_code);
		m_err_code = ret_code;
		curl_easy_cleanup(easy_handle);
		return -1;
	}
	curl_easy_getinfo(easy_handle,CURLINFO_RESPONSE_CODE,&m_err_code);
	curl_easy_cleanup(easy_handle);
	return 0;
}

int MyHttpReq::Get(){
	reset_body();
	char tmpBuff[1024];
	map<string,string>::iterator iter;
	curl_slist *http_headers=NULL;
	CURL *easy_handle=NULL;
	easy_handle=curl_easy_init();
	if(easy_handle==NULL){
		m_err_code = HTTP_INIT_ERROR;
		return -1;
	}
	for(iter=m_req_header.begin();iter!=m_req_header.end();iter++){
		snprintf(tmpBuff,sizeof(tmpBuff),"%s: %s",iter->first.c_str(),iter->second.c_str());
		http_headers=curl_slist_append(http_headers,tmpBuff);
	}
	curl_easy_setopt(easy_handle, CURLOPT_URL, m_url);

	/*禁止使用信号*/
	curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
	
	/*禁止重用SOCKET 解决CLOSE_WAIT和TIME_WAIT的超时时间*/
	curl_easy_setopt(easy_handle, CURLOPT_FORBID_REUSE, 1);
	/*设置超时时间*/
	//连接超时
	if(m_connect_timeout!=-1){
		curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, m_connect_timeout);
	}
	//等待超时
	if(m_timeout!=-1){
		curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, m_timeout);
	}
	/*使用ipv4DNS解析*/
	curl_easy_setopt(easy_handle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
	/*if(m_req_body!=NULL){
   		curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, (void*)m_req_body);
		curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, m_req_body_size); 
	}else{
		curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, (void*)NULL);
		curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, 0);
	}
	*/
	
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_data);  
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, this);
	//curl_easy_setopt(easy_handle,CURLOPT_HEADERFUNCTION,header_callback);
	//curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA,this);
	//curl_easy_setopt(easy_handle, CURLOPT_POST, 0);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1);
	if(m_cooki_file[0]!=0)
   		curl_easy_setopt(easy_handle, CURLOPT_COOKIEFILE, m_cooki_file);  
	CURLcode ret_code=curl_easy_perform(easy_handle); 
	curl_slist_free_all(http_headers);
	if(ret_code!=CURLE_OK){
		ERROR("Get %s error",m_url);
		m_err_code = ret_code;
		curl_easy_cleanup(easy_handle);
		return -1;
	}
	curl_easy_getinfo(easy_handle,CURLINFO_RESPONSE_CODE,&m_err_code);
	curl_easy_cleanup(easy_handle);
	return 0;
}

