#include "linuxplatform.h"
#include "my_regex.h"
#include "ihttp_factory.h"


list<IHttpHandlerCreater *> IHttpFactory::m_list_creates;

//create 读写锁
pthread_rwlock_t IHttpFactory::m_creates_lock=PTHREAD_RWLOCK_INITIALIZER;

	
	
/*注册creater*/
int IHttpFactory::register_creater(IHttpHandlerCreater *creater){
	int ret = 0;
	if (creater == NULL){
		return -1;
	}
	pthread_rwlock_wrlock(&m_creates_lock);
	m_list_creates.push_back(creater);
	pthread_rwlock_unlock(&m_creates_lock);

	return ret;
}


/*根据路径创建Handler*/
IHttpHandler *IHttpFactory::create(string path){
	IHttpHandler * ret=NULL;
	list<IHttpHandlerCreater*>::iterator iter;
	pthread_rwlock_rdlock(&m_creates_lock);
	do{
		for (iter = m_list_creates.begin(); iter != m_list_creates.end(); iter++){
			IHttpHandlerCreater *creater = *iter;
			if ((ret = creater->create(path))!=NULL){
				ret->set_handler_creater(creater);
				break;
			}
		}	
	}while(0);
	pthread_rwlock_unlock(&m_creates_lock);
	return  ret;
}

/*释放Handler*/
int IHttpFactory::release(IHttpHandler *handler){
	IHttpHandlerCreater* creater=NULL;
	creater=handler->get_handler_creater();
	if(creater!=NULL){
		return creater->release(handler);
	}
	ERROR("handler do not containt http handler creater");
	return -1;
}


