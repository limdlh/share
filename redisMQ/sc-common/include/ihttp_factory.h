#ifndef _I_HTTP_FACTORY_H
#define _I_HTTP_FACTORY_H
#include "ihttp_handler.h"
#include "ihttp_handler_creater.h"


class IHttpFactory{
private:
	//不是太明确,通过正则来匹配的
	static list<IHttpHandlerCreater *> m_list_creates;
	//create 读写锁
	static pthread_rwlock_t m_creates_lock;	
public:
	/*注册creater*/
	static int register_creater(IHttpHandlerCreater *creater);
	/*根据路径创建Handler*/
	static IHttpHandler *create(string path);
	/*释放Handler*/
	static int release(IHttpHandler *handler);
};
#endif 
