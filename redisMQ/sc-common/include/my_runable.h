#ifndef _MY_RUNABLE_H
#define _MY_RUNABLE_H

class MyRunable{
private:
	bool m_need_delete_flag;
public:
	MyRunable(){m_need_delete_flag=true;};
	virtual ~MyRunable(){};
	/*是否需要线程池进行释放,true进行是否,false 不释放*/
	void set_needed_delete(bool flag){m_need_delete_flag=flag;};
	/*用于确认是否需要线程池进行释放,true进行是否,false 不释放*/
	virtual bool NeededDelete(){return m_need_delete_flag;};
	virtual void Run()=0;
};
#endif
