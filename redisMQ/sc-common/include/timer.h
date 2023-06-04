#ifndef _TIMER_H_
#define _TIMER_H_

class Timer{
private:
	int m_timer_fd;
	int m_time_out;
	void (*m_on_time_out)(void *ptr);
	void *m_ptr;
	int m_ref;
	bool m_add_flags;
	pthread_mutex_t m_lock;
	Timer(int time_out);
public:
	static Timer *create_timer(int time_out);
	static void release_timer(Timer *timer);
	~Timer();
	int get_timer_fd();
	int get_time_out();
	void set_add_flags(bool flags);
	bool get_add_flags();
	void inc_ref();
	void dec_ref();
	int set_process(void (*on_time_out)(void *ptr),void *ptr);
	void on_run();
};
#endif
