#include <curl/curl.h>
#include "format.h"
#include "linuxplatform.h"
#include "my_stmp.h"




MyStmp::MyStmp(){
	m_reads=0;
	m_send_contex=NULL;
	m_send_contex_size=0;
	m_log_flag=false;
	
}

MyStmp::~MyStmp(){
}

void MyStmp::enable_log(bool flag){
	m_log_flag=flag;
}
void MyStmp::set_user_name(string name){
	m_userName=name;
}
void MyStmp::set_password(string password){
	m_password=password;
}
void MyStmp::set_url(string url){
	m_url=url;
}
void MyStmp::set_topic(string topic){
	m_topic=topic;
}
void MyStmp::set_from(string from){
	m_from=format("<%s>",STR(from));
}
void MyStmp::clear_to(){
	m_tos.clear();
}
void MyStmp::add_to(string to){
	m_tos.push_back(format("%s",STR(to)));
}

void MyStmp::set_send_contex(char *contex,int size){
	m_send_contex=contex;
	m_send_contex_size=size;
}

const char *send_format="To: %s\r\n"
						"From: %s\r\n"
						"Subject: %s\r\n"
						"Content-Type: text/plain;charset=\"UTF8\"\r\n"
						"\r\n";

int MyStmp::make_send_contex(){
	string send_to;
	list<string>::iterator iter= m_tos.begin();
    for(;iter!=m_tos.end();iter++){
		send_to+=format("%s;",(*iter).c_str());
	}
	m_all_send_contex=format(send_format,STR(send_to),STR(m_from),STR(m_topic));
	if(m_send_contex!=NULL && m_send_contex_size>0)
		m_all_send_contex.append(m_send_contex,m_send_contex_size);
	return 0;
	
	
}

int MyStmp::send_mail(){
	CURL *curl;
  	CURLcode res = CURLE_OK;
	struct curl_slist *recipients = NULL;
	m_reads=0;
	curl = curl_easy_init();
	if(curl) {
   		/* This is the URL for your mailserver */
    	curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());

	    /* Note that this option isn't strictly required, omitting it will result in
	     * libcurl sending the MAIL FROM command with empty sender data. All
	     * autoresponses should have an empty reverse-path, and should be directed
	     * to the address in the reverse-path which triggered them. Otherwise, they
	     * could cause an endless loop. See RFC 5321 Section 4.5.5 for more details.
	     */
	     //开启日志
		if(m_log_flag==true)
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	     /*使用ipv4DNS解析*/
		 curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
		
   		 curl_easy_setopt(curl, CURLOPT_MAIL_FROM, m_from.c_str());
		 /* Add two recipients, in this particular case they correspond to the
    	  * To: and Cc: addressees in the header, but they could be any kind of
     	  * recipient. */
     	  list<string>::iterator iter= m_tos.begin();
     	 for(;iter!=m_tos.end();iter++){
			recipients = curl_slist_append(recipients, (*iter).c_str());
			
		 }
		 curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
		 curl_easy_setopt(curl, CURLOPT_USERNAME,m_userName.c_str());
		 curl_easy_setopt(curl, CURLOPT_PASSWORD, m_password.c_str());
		 /* We're using a callback function to specify the payload (the headers and
	     * body of the message). You could just use the CURLOPT_READDATA option to
	     * specify a FILE pointer to read from. */
	     curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
	     curl_easy_setopt(curl, CURLOPT_READDATA, this);
	     curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		 //组装context
		 make_send_contex();
		 /* Send the message */
   		 res = curl_easy_perform(curl);
		 /* Check for errors */
	     if(res != CURLE_OK)
	      fprintf(stderr, "curl_easy_perform() failed: %s\n",
	              curl_easy_strerror(res));

	     /* Free the list of recipients */
	     curl_slist_free_all(recipients);
		 /* curl won't send the QUIT command until you call cleanup, so you should be
	     * able to re-use this connection for additional messages (setting
	     * CURLOPT_MAIL_FROM and CURLOPT_MAIL_RCPT as required, and calling
	     * curl_easy_perform() again. It may not be a good idea to keep the
	     * connection open for a very long time though (more than a few minutes may
	     * result in the server timing out the connection), and you do want to clean
	     * up in the end.
	     */
   		 curl_easy_cleanup(curl);
	}
	m_send_contex=NULL;
	m_send_contex_size=0;
	return (int)res;
}
	


size_t MyStmp::payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  MyStmp *pStmp= (MyStmp *)userp;
  const char *data;
  int need_len=size*nmemb;
  if((size == 0) || (nmemb == 0) || (need_len < 1)) {
    return 0;
  }

  data=pStmp->m_all_send_contex.c_str();
  int data_size=pStmp->m_all_send_contex.length();
  int avilid_len=data_size-pStmp->m_reads;
  int read_len=avilid_len<need_len?avilid_len:need_len;
  if(read_len>0){
		memcpy(ptr,data+pStmp->m_reads,read_len);
		pStmp->m_reads+=read_len;
		return read_len;
  }

  return 0;
}










