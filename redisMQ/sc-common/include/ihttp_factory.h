#ifndef _I_HTTP_FACTORY_H
#define _I_HTTP_FACTORY_H
#include "ihttp_handler.h"
#include "ihttp_handler_creater.h"


class IHttpFactory{
private:
	//����̫��ȷ,ͨ��������ƥ���
	static list<IHttpHandlerCreater *> m_list_creates;
	//create ��д��
	static pthread_rwlock_t m_creates_lock;	
public:
	/*ע��creater*/
	static int register_creater(IHttpHandlerCreater *creater);
	/*����·������Handler*/
	static IHttpHandler *create(string path);
	/*�ͷ�Handler*/
	static int release(IHttpHandler *handler);
};
#endif 
