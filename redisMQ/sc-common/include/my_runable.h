#ifndef _MY_RUNABLE_H
#define _MY_RUNABLE_H

class MyRunable{
private:
	bool m_need_delete_flag;
public:
	MyRunable(){m_need_delete_flag=true;};
	virtual ~MyRunable(){};
	/*�Ƿ���Ҫ�̳߳ؽ����ͷ�,true�����Ƿ�,false ���ͷ�*/
	void set_needed_delete(bool flag){m_need_delete_flag=flag;};
	/*����ȷ���Ƿ���Ҫ�̳߳ؽ����ͷ�,true�����Ƿ�,false ���ͷ�*/
	virtual bool NeededDelete(){return m_need_delete_flag;};
	virtual void Run()=0;
};
#endif
