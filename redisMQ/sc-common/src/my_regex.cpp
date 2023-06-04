#include "my_regex.h"
#include "mycommon.h"


MyRegex::MyRegex(){
	m_pcre_prepared=NULL;


}
MyRegex::~MyRegex(){
	if (m_pcre_prepared!=NULL){
		pcre_free(m_pcre_prepared);
		m_pcre_prepared = NULL;
	}
	
}

/*��������ʽ���б���,expression ������ʽ,case �Ƿ����ִ�Сд,true������,false����*/
int MyRegex::compile(string& expression,bool caseless){
	const char *error = NULL;	/* Used to hold any errors                                           */
	int error_offset = 0;		/* Holds the offset of an error                                      */
	if (m_pcre_prepared!=NULL){
		pcre_free(m_pcre_prepared);
		m_pcre_prepared = NULL;
	}
	m_pcre_prepared=pcre_compile(STR(expression),caseless?PCRE_CASELESS:0,&error,&error_offset,NULL);
	if(error!=NULL){
		if (m_pcre_prepared!=NULL){
			pcre_free(m_pcre_prepared);
			m_pcre_prepared = NULL;
		}
		m_errmsg=format("Regular Expression Error expression[%s] error[%s] location[%d]\n", STR(expression), error, error_offset);
		return -1;
	}
	return 0;
}


/*��ȡ������Ϣ*/
string MyRegex::get_errmsg(){
	return m_errmsg;
}


/*�Ƿ�ƥ��*/
bool MyRegex::match(string& dest){
	int match_count = 0;		/* Number of times the regex was matched                             */
	int offset_vectors[255];	/* not used, but has to exist or pcre won't even try to find a match */
	int pcre_flags = 0;
	if(m_pcre_prepared==NULL){
		m_errmsg="pcre prepared is null";
		ERROR("%s",STR(m_errmsg));
		return false;
	}
	/* So far so good, run the regex */
	match_count =
		pcre_exec(m_pcre_prepared, NULL, STR(dest), (int) dest.length(), 0, pcre_flags, offset_vectors, sizeof(offset_vectors) / sizeof(offset_vectors[0]));
	if(match_count<=0){
		return false;
	}
	m_results.clear();
	m_results.push_back(dest);
	for(int i=1;i<match_count;i++){
		m_results.push_back(dest.substr(offset_vectors[2 * i],offset_vectors[2 * i + 1] - offset_vectors[2 * i]));
	}
	return true;
}

/*��ȡƥ����,��0 ��ʼ*/
string MyRegex::get_result(int index){
	if(m_results.size()>index){
		return m_results[index];
	}
	return "";
}
/*��ȡ�������*/
int MyRegex::get_result_counts(){
	return m_results.size();
}

