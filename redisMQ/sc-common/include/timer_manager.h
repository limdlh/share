#ifndef _TIMER_MANAGER_H
#define _TIMER_MANAGER_H
#include "timer.h"
#include "pthread.h"
class TimerManager{
private:
	static int m_epoll;
	static pthread_t m_pid;
	static bool m_finish;
	static list<Timer *> m_timer_drop_lists;
	static pthread_mutex_t m_timer_drop_lists_lock;
public:
	static void *on_run(void * args);
	static int init_timer_manager();
	static int uninit_timer_manager();
	static int add_timer(Timer *timer);
	
	static int delete_timer(Timer *timer);

	static int delete_timer();
};
#endif
