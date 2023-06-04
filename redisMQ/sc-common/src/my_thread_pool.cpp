#include "linuxplatform.h"
#include "logging.h"
#include "my_thread_pool.h"

//空闲线程的数量
int MyThreadPoolManager::m_free_count=0;
	//正忙线程的数量
int MyThreadPoolManager::m_busy_count=0;
	//所有线程的数量
int MyThreadPoolManager::m_all_count=0;

	//最小线程数
int MyThreadPoolManager::m_min_count=0;
	//最大线程数
int MyThreadPoolManager::m_max_count=0;

//初始化标识
bool MyThreadPoolManager::m_init_flags=false;

/*等待执行的线程*/
list<MyRunable *> MyThreadPoolManager::m_runables;

//线程参数
list<ST_THREAD_PARAM *> MyThreadPoolManager::m_threads;

pthread_mutex_t MyThreadPoolManager::m_threads_lock=PTHREAD_MUTEX_INITIALIZER;


//数量线程锁
pthread_mutex_t MyThreadPoolManager::m_count_lock=PTHREAD_MUTEX_INITIALIZER;

//线程池结束标识
bool MyThreadPoolManager::m_globle_stop_flags=false;


//等待信号
pthread_cond_t MyThreadPoolManager::m_wait_cond=PTHREAD_COND_INITIALIZER;
//等待锁
pthread_mutex_t MyThreadPoolManager::m_wait_lock=PTHREAD_MUTEX_INITIALIZER;


void MyThreadPoolManager::delete_param(void *param){
	pthread_mutex_lock(&m_threads_lock);
	list<ST_THREAD_PARAM *>::iterator iter = m_threads.begin();
	for (; iter != m_threads.end();iter++){
		if (param == *iter){
			m_threads.erase(iter);
			break;
		}
	}
	pthread_mutex_unlock(&m_threads_lock);
	
}

/*thread 回调*/
void *MyThreadPoolManager::common_callback (void *param){
	pthread_detach(pthread_self());
	ST_THREAD_PARAM *threadParm=(ST_THREAD_PARAM*)param;
	while(m_globle_stop_flags==false &&threadParm->is_run==true){
		MyRunable * tmpRunable=NULL;
		pthread_mutex_lock(&m_wait_lock);
		list<MyRunable*>::iterator iter=m_runables.begin();
		if(iter==m_runables.end()){		//没有任务,进行等待
			struct timespec timeout;
			timeout.tv_sec = time(NULL) + 300;
			timeout.tv_nsec = 0;
			int retCode = pthread_cond_timedwait(&m_wait_cond, &m_wait_lock, &timeout);
			if (retCode == ETIMEDOUT){// 超时
				if (m_all_count > m_min_count){
					m_free_count--;
					m_all_count--;
					pthread_mutex_unlock(&m_wait_lock);
					break;
				}
				else{
					pthread_mutex_unlock(&m_wait_lock);
					continue;
				}
			}
			else if (retCode == 0){
				iter = m_runables.begin();
				if (iter != m_runables.end()){
					tmpRunable = *iter;
					m_runables.erase(iter);
				}
			}
		}else{
			tmpRunable=*iter;
			m_runables.erase(iter);
		}
		if(tmpRunable!=NULL){
			threadParm->is_free=false;		//线程BUSY
			m_busy_count++;
			m_free_count--;
		}
		pthread_mutex_unlock(&m_wait_lock);
		if(tmpRunable!=NULL){
			
			tmpRunable->Run();
			if(tmpRunable->NeededDelete()==true){
				delete tmpRunable;
			}
			pthread_mutex_lock(&m_wait_lock);
			threadParm->is_free=true;		//线程free
			m_busy_count--;
			m_free_count++;
			pthread_mutex_unlock(&m_wait_lock);
		}
	}
	INFO("exit thread [%d]", threadParm->pid);
	threadParm->is_close=true;
	delete_param(threadParm);
	delete threadParm;
	return NULL;
}


int MyThreadPoolManager::init(int min,int max){
	int ret=0;
	INFO("start init MyThreadPoolManager,min:[%d],max:[%d]",min,max);
	pthread_mutex_lock(&m_count_lock);
	do{
		if(m_init_flags==true){
			ret=-1;
			ERROR("MyThreadPoolManager already init");
			break;
		}
		
		m_min_count=min;
		m_max_count=max;
		if(m_min_count>m_max_count || m_min_count<0){
			ret=-1;
			ERROR("MyThreadPoolManager init param error");
			break;
		}

		for(int i=0;i<m_min_count;i++){
			ST_THREAD_PARAM *param=new ST_THREAD_PARAM;
			param->is_free=true;
			param->is_run=true;
			param->is_close=false;
			if(pthread_create(&param->pid,NULL,common_callback,param)!=0){
				delete param;
				param=NULL;
				continue;
			}
			INFO("start thread [%d]", param->pid);
			pthread_mutex_lock(&m_threads_lock);
			m_threads.push_back(param);
			pthread_mutex_unlock(&m_threads_lock);
			
		}
		m_all_count=m_threads.size();
		m_free_count=m_all_count;
		m_busy_count=0;
		m_init_flags=true;
		
	}while(0);
	pthread_mutex_unlock(&m_count_lock);
	INFO("end init MyThreadPoolManager");
	return ret;
};

int MyThreadPoolManager::unint(){
	if(m_init_flags==false){
		ERROR("MyThreadPoolManager do not init");
		return -1;
	}
	m_globle_stop_flags=true;
	pthread_cond_broadcast(&m_wait_cond);
	sleep(1);
	return 0;
}

int MyThreadPoolManager::start(MyRunable *runable){
	if(m_init_flags==false){
		ERROR("MyThreadPoolManager do not init");
		return -1;
	}
	pthread_mutex_lock(&m_wait_lock);
	if(m_free_count<1){
		if(m_all_count<m_max_count){			//线程不足,增加线程
			ST_THREAD_PARAM *param=new ST_THREAD_PARAM;
			param->is_free=true;
			param->is_run=true;
			param->is_close=false;
			m_free_count++;
			m_all_count++;
			if(pthread_create(&param->pid,NULL,common_callback,param)!=0){
				delete param;
				param=NULL;
			}else{
				INFO("start thread [%d]", param->pid);
				pthread_mutex_lock(&m_threads_lock);
				m_threads.push_back(param);
				pthread_mutex_unlock(&m_threads_lock);
			}
		}
	}
	m_runables.push_back(runable);
	pthread_cond_signal(&m_wait_cond);
	pthread_mutex_unlock(&m_wait_lock);
	return 0;
}

