#include "linuxplatform.h"
#include "logging.h"
#include "timer.h"
#include "timer_manager.h"


#define EPOLL_QUERY_SIZE 1024
int TimerManager::m_epoll=-1;
pthread_t TimerManager::m_pid=-1;
bool TimerManager::m_finish=true;
pthread_mutex_t TimerManager::m_timer_drop_lists_lock=PTHREAD_MUTEX_INITIALIZER;
list<Timer *> TimerManager::m_timer_drop_lists;
struct epoll_event _query[EPOLL_QUERY_SIZE];
void *TimerManager::on_run(void * args){
	int32_t eventsCount = 0;
	while(!m_finish){
		if ((eventsCount = epoll_wait(m_epoll, _query, EPOLL_QUERY_SIZE, 1000)) < 0) {
			if (errno!= EINTR){
				ERROR("Unable to execute epoll_wait: (%d) %s", errno, strerror(errno));
				break;
			}
		}
		for (int32_t i = 0; i < eventsCount; i++) {
			Timer *timer=(Timer*)_query[i].data.ptr;
			if((_query[i].events&EPOLLERR)!=0){
				delete_timer(timer);
				continue;
			}
			timer->on_run();
		}
		delete_timer();
	}
	return NULL;
	
		
	
	
}
int TimerManager::init_timer_manager(){
	m_epoll=epoll_create(1000);
	if(m_epoll<0){
		ERROR("epoll_create error,TimerManager::init_timer_manager failed");
		return -1;
	}
	if(pthread_create(&m_pid,NULL,on_run,NULL)!=0){
		ERROR("pthread_create error,TimerManager::init_timer_manager failed");
		close(m_epoll);
		m_epoll=-1;
		return -1;
	}
	m_finish=false;
	return 0;
}

int TimerManager::uninit_timer_manager(){
	m_finish=true;
	if(m_epoll!=-1)
		close(m_epoll);
	return 0;
	
}
int TimerManager::add_timer(Timer *timer){
	if(timer->get_add_flags()){
		WARN("timer already add to timer manager");
		return -1;
	}
	struct epoll_event evt = {0,
		{0}};
	evt.events = EPOLLIN;
	evt.data.ptr=timer;
	timer->inc_ref();
	if (epoll_ctl(m_epoll, EPOLL_CTL_ADD,timer->get_timer_fd(), &evt) != 0) {
		int err = errno;
		FATAL("Unable to disable read data: (%d) %s", err, strerror(err));
		timer->dec_ref();
		return -1;
	}
	timer->set_add_flags(true);			//ÉèÖÃtimerÎªÌí¼Ó×´Ì¬
	return 0;
	
}
int TimerManager::delete_timer(Timer *timer){
	timer->set_add_flags(false);		//ÉèÖÃtimerÎªÉ¾³ý×´Ì¬
	pthread_mutex_lock(&m_timer_drop_lists_lock);
	m_timer_drop_lists.push_back(timer);
	pthread_mutex_unlock(&m_timer_drop_lists_lock);
	return 0;
}

int TimerManager::delete_timer(){
	struct epoll_event evt = {0,
		{0}};
	pthread_mutex_lock(&m_timer_drop_lists_lock);
	list<Timer*>::iterator iter=m_timer_drop_lists.begin();
	Timer *timer=NULL;
	for(;iter!=m_timer_drop_lists.end();iter=m_timer_drop_lists.begin()){
		evt.events = EPOLLIN;
		timer=*iter;
		evt.data.ptr=timer;
		m_timer_drop_lists.pop_front();
		if(epoll_ctl(m_epoll, EPOLL_CTL_DEL,timer->get_timer_fd(), &evt)!=0){
			FATAL("Unable to disable read data: (%d) %s", errno, strerror(errno));
		}
		timer->dec_ref();
	}
	pthread_mutex_unlock(&m_timer_drop_lists_lock);
	return 0;
}