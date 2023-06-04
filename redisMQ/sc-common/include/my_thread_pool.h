#ifndef _MY_THREAD_POOL_H
#define _MY_THREAD_POOL_H
#include "linuxplatform.h"
#include "my_runable.h"
struct ST_THREAD_PARAM{
	pthread_t pid;
	bool is_run;		//是否运行
	bool is_free;		//是否空闲
	bool is_close;
};
class MyThreadPoolManager{
private:
	//空闲线程的数量
	static int m_free_count;
	//正忙线程的数量
	static int m_busy_count;
	//所有线程的数量
	static int m_all_count;

	//最小线程数
	static int m_min_count;
	//最大线程数
	static int m_max_count;

	//初始化标识
	static bool m_init_flags;

	/*等待执行的线程*/
	static list<MyRunable *> m_runables;
	
	//线程参数
	static list<ST_THREAD_PARAM *> m_threads;

	static pthread_mutex_t m_threads_lock;

	//数量线程锁
	static pthread_mutex_t m_count_lock;

	static pthread_cond_t m_wait_cond;

	static pthread_mutex_t m_wait_lock;

	//线程池结束标识
	static bool m_globle_stop_flags;

	
private:
	/*thread 回调*/
	static void *common_callback (void *param);
	static void delete_param(void *param);

public:
	/*线程池初始化*/
	static int init(int min,int max);
	/*线程池注销*/
	static int unint();
	/*启动线程*/
	static int start(MyRunable *runable);


	
};
/*开启线程*/

#endif //_MY_THREAD_POOL_H
