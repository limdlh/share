#include "linuxplatform.h"
#include "logging.h"
#include "timer_manager.h"
Timer::Timer(int time_out){
	m_time_out=time_out;
	m_timer_fd=timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
	itimerspec tmp;
	itimerspec dummy;
	memset(&tmp, 0, sizeof (tmp));
	tmp.it_interval.tv_nsec = 0;
	tmp.it_interval.tv_sec = time_out;
	tmp.it_value.tv_nsec = 0;
	tmp.it_value.tv_sec = time_out;
	if (timerfd_settime(m_timer_fd, 0, &tmp, &dummy) != 0) {
		int err = errno;
		ERROR("timerfd_settime failed with error (%d) %s", err, strerror(err));
	}
	m_on_time_out=NULL;
	m_ptr=NULL;
	m_add_flags=false;
	m_ref=0;
	pthread_mutex_init(&m_lock,NULL);
}

Timer::~Timer(){
	if(m_add_flags)
		TimerManager::delete_timer(this);
	if(m_timer_fd>0)
		close(m_timer_fd);
	pthread_mutex_destroy(&m_lock);
}

Timer *Timer::create_timer(int time_out){
	Timer * ret=new Timer(time_out);
	if(ret!=NULL)
		ret->inc_ref();
	return ret;
}
void Timer::release_timer(Timer *timer){
	timer->dec_ref();
}

void Timer::inc_ref(){
	pthread_mutex_lock(&m_lock);
	m_ref++;
	pthread_mutex_unlock(&m_lock);
}
void Timer::dec_ref(){
	int ret;
	pthread_mutex_lock(&m_lock);
	m_ref--;
	ret=m_ref;
	pthread_mutex_unlock(&m_lock);
	if(ret==0)
		delete this;
	
}

int Timer::set_process(void (*on_time_out)(void *ptr),void *ptr){
	if(on_time_out==NULL)
		return -1;
	m_on_time_out=on_time_out;
	m_ptr=ptr;
	return 0;
}
int Timer::get_timer_fd(){
	return m_timer_fd;
}

int Timer::get_time_out(){
	return m_time_out;
}

void Timer::set_add_flags(bool flags){
	pthread_mutex_lock(&m_lock);
	m_add_flags=flags;
	pthread_mutex_unlock(&m_lock);
}
bool Timer::get_add_flags(){
	return m_add_flags;
}
void Timer::on_run(){
	char _count[8];
	read(m_timer_fd, &_count, 8);
	if(m_on_time_out!=NULL){
		pthread_mutex_lock(&m_lock);
		if(m_add_flags==true)		//已经添加才会执行
			m_on_time_out(m_ptr);
		pthread_mutex_unlock(&m_lock);
	}
}