#ifndef _MY_REGEX_H
#define _MY_REGEX_H
#include "mycommon.h"
#include <pcre.h>
class MyRegex{
private:
	vector<string> m_results;
	pcre *m_pcre_prepared;
	string m_errmsg;
public:
	MyRegex();
	~MyRegex();
	/*��������ʽ���б���,expression ������ʽ,caseless �Ƿ����ִ�Сд,true������,false����*/
	int compile(string& expression,bool caseless=false);
	/*��ȡ������Ϣ*/
	string get_errmsg();
	/*�Ƿ�ƥ��*/
	bool match(string& dest);
	/*��ȡƥ����,��0 ��ʼ*/
	string get_result(int index);
	/*��ȡ�������*/
	int get_result_counts();
	
};
#endif //_MY_REGEX_H
