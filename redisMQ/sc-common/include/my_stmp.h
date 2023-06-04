#ifndef __MY_STMP_H
#define __MY_STMP_H
#include<iostream>
#include <list>
using namespace std;
class MyStmp{
private:
	string m_userName;
	string m_password;
	string m_url;
	string m_topic;
	string m_from;
	list<string> m_tos;
	int m_reads;
	char *m_send_contex;
	int m_send_contex_size;
	string m_all_send_contex;
	bool m_log_flag;
	
	
public:
	MyStmp();
	~MyStmp();
	void enable_log(bool flag);
	void set_user_name(string name);
	void set_password(string password);
	void set_url(string url);
	void set_topic(string topic);
	void set_from(string from);
	void clear_to();
	void add_to(string to);
	void set_send_contex(char *contex,int size);
	int send_mail();
private:
	int make_send_contex();
public:
	static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
};

#endif
