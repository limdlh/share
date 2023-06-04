#ifndef _MY_THREAD_POOL_H
#define _MY_THREAD_POOL_H
#include "linuxplatform.h"
#include "my_runable.h"
struct ST_THREAD_PARAM{
	pthread_t pid;
	bool is_run;		//�Ƿ�����
	bool is_free;		//�Ƿ����
	bool is_close;
};
class MyThreadPoolManager{
private:
	//�����̵߳�����
	static int m_free_count;
	//��æ�̵߳�����
	static int m_busy_count;
	//�����̵߳�����
	static int m_all_count;

	//��С�߳���
	static int m_min_count;
	//����߳���
	static int m_max_count;

	//��ʼ����ʶ
	static bool m_init_flags;

	/*�ȴ�ִ�е��߳�*/
	static list<MyRunable *> m_runables;
	
	//�̲߳���
	static list<ST_THREAD_PARAM *> m_threads;

	static pthread_mutex_t m_threads_lock;

	//�����߳���
	static pthread_mutex_t m_count_lock;

	static pthread_cond_t m_wait_cond;

	static pthread_mutex_t m_wait_lock;

	//�̳߳ؽ�����ʶ
	static bool m_globle_stop_flags;

	
private:
	/*thread �ص�*/
	static void *common_callback (void *param);
	static void delete_param(void *param);

public:
	/*�̳߳س�ʼ��*/
	static int init(int min,int max);
	/*�̳߳�ע��*/
	static int unint();
	/*�����߳�*/
	static int start(MyRunable *runable);


	
};
/*�����߳�*/

#endif //_MY_THREAD_POOL_H
