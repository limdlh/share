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
	/*对正则表达式进行编译,expression 正则表达式,caseless 是否区分大小写,true不区分,false区分*/
	int compile(string& expression,bool caseless=false);
	/*获取错误信息*/
	string get_errmsg();
	/*是否匹配*/
	bool match(string& dest);
	/*获取匹配结果,从0 开始*/
	string get_result(int index);
	/*获取结果数量*/
	int get_result_counts();
	
};
#endif //_MY_REGEX_H
