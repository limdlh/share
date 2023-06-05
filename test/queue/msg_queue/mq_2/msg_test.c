/*
2022/11/11
author:chenchen4396
*/
//API参考手册：https://www.bookstack.cn/read/linuxapi/SUMMARY.md
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <fcntl.h>

pthread_t s1, s2, r;
sem_t mutex;
sem_t full, empty;
sem_t over;
sem_t s_display, r_display;

struct msgbuff
{
	long msg_type; //消息类型，可以自定义，在使用msgrcv函数的时候会使用
	char msg[100]; //消息数组大小
};

void receive(void *arg)
{
	struct msgbuff buf;
	int flag1 = 1;
	int flag2 = 1;
	int ret = -1;
	while ((flag1 + flag2) != 0)
	{
		//初始化消息队列
		memset(&buf, 0, sizeof(buf));
		//互斥信号量mutex，资源信号量full
		// mutex和full的wait顺序不能调换，否则会发生死锁，参考生存者与消费者模型
		sem_wait(&r_display);
		sem_wait(&full);
		sem_wait(&mutex);
		// msgrcv接受消息
		// arg是传入的msgid
		// 1代表msgtyp，指定一个接受的消息类型
		// IPC_NOWAIT 如果需要等待，则不发送消息并且调用进程立即返回,避免阻塞
		ret = msgrcv(*(int *)arg, &buf, sizeof(buf.msg), 1, IPC_NOWAIT);
		if (strlen(buf.msg) != 0)
		{
			printf("receive: %s \n", buf.msg); //打印接收到的内容
			printf("-----------------------------------------\n");
		}
		//判断sender1和sender2是否发送end
		if (flag1 && strncmp(buf.msg, "end1", 4) == 0)
		{
			memset(&buf, 0, sizeof(buf));
			strcpy(buf.msg, "over1");
			//使over消息的msgtyp为2,方便分开接受
			buf.msg_type = 2;
			ret = msgsnd(*(int *)arg, &buf, sizeof(buf.msg), 0);
			if (ret == -1)
			{
				printf("sender1 msgsnd error\n");
				sem_post(&mutex);
				exit(1);
			}
			sem_post(&over);
			flag1 = 0;
		}
		else if (flag2 && strncmp(buf.msg, "end2", 4) == 0)
		{
			memset(&buf, 0, sizeof(buf));
			strcpy(buf.msg, "over2");
			buf.msg_type = 2;
			ret = msgsnd(*(int *)arg, &buf, sizeof(buf.msg), 0);
			if (ret == -1)
			{
				printf("sender2 msgsnd error\n");
				sem_post(&mutex);
				exit(1);
			}
			sem_post(&over);
			flag2 = 0;
		}
		sem_post(&mutex);
		sem_post(&empty);
		sem_post(&s_display);
		sleep(1);
	}
	printf("sender1 and sender2 all over!\n");
}
 
void sender1(void *arg)
{
	struct msgbuff buf;
	int flag = 1;
	int ret = -1;
	while (flag)
	{
		memset(&buf, 0, sizeof(buf));
		sem_wait(&s_display);
		sem_wait(&empty);
		sem_wait(&mutex);
		printf("sender1> ");
		scanf("%s", buf.msg);
		buf.msg_type = 1;
		if (!strncmp(buf.msg, "exit", 4))
		{
			strcpy(buf.msg, "end1");
			flag = 0;
		}
		else
		{
			strcat(buf.msg, "   ——s1");
		}
		ret = msgsnd(*(int *)arg, &buf, sizeof(buf.msg), IPC_NOWAIT);
		if (ret == -1)
		{
			printf("sender1 msgsnd error\n");
			sem_post(&mutex);
			exit(1);
		}
		sem_post(&r_display);
		sem_post(&full);
		sem_post(&mutex);
		sleep(1);
	}
	sem_wait(&over);
	sem_wait(&empty);
	sem_wait(&mutex);
	ret = msgrcv(*(int *)arg, &buf, sizeof(buf.msg), 2, IPC_NOWAIT);
	if (strncmp(buf.msg, "over1", 5) == 0)
	{
		sem_post(&full);
		sem_post(&mutex);
		pthread_exit(NULL);
	}
}
 
void sender2(void *arg)
{
	struct msgbuff buf;
	int flag = 1;
	int ret = -1;
	while (flag)
	{
		//初始化buf
		memset(&buf, 0, sizeof(buf));
		//等待信号量，申请顺序有些不能反!参考生产者消费者死锁问题
		sem_wait(&s_display);
		sem_wait(&empty);
		sem_wait(&mutex);
		printf("sender2> ");
		scanf("%s", buf.msg);
		//设置msg_type为1，与msgrcv第四个参数有关
		buf.msg_type = 1;
		if (!strncmp(buf.msg, "exit", 4)){
			//当sender1输入exit，则对receive发送end1
			strcpy(buf.msg, "end2");
			flag = 0;
		}
		else{
			strcat(buf.msg, "   ——s2");
		}
		// msgsnd添加消息到队列
		ret = msgsnd(*(int *)arg, &buf, sizeof(buf.msg), IPC_NOWAIT);
		if (ret == -1){
			printf("sender2 msgsnd error\n");
			sem_post(&mutex);
			exit(1);
		}
		//释放信号量
		sem_post(&r_display);
		sem_post(&full);
		sem_post(&mutex);
		sleep(1);
	}
	//接受over信号
	sem_wait(&over);
	sem_wait(&empty);
	sem_wait(&mutex);
	ret = msgrcv(*(int *)arg, &buf, sizeof(buf.msg), 2, IPC_NOWAIT);
	if (strncmp(buf.msg, "over2", 5) == 0)
	{
		sem_post(&full);
		sem_post(&mutex);
		pthread_exit(NULL);
	}
}
/*
	初始化信号量
	sem为初始化的信号量名,pshare为0表示只在该进程的所有线程间共享,非0则可以在进程之间共享，value为信号量的大小
	kernel源码中有static inline void sema_init(struct semaphore *sem, int val)
	sema_init是Linux内核的计数信号量实现初始化函数。
	sem_init是Posix线程库的初始化程序（因此被用户空间代码使用）
*/
int main()
{
	//互斥信号量mutex，互斥使用消息队列
	sem_init(&mutex, 0, 1);
	//同步信号量full,empty
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, 5);
	//用over信号判断某个sender线程的结束
	sem_init(&over, 0, 0);
	//display信号量实际作用只是为了能让sender线程和receive线程实现一发一收的格式
	//不使用这两个信号量可能会无法保证线程的执行顺序
	sem_init(&s_display, 0, 1);
	sem_init(&r_display, 0, 0);
 
	// 可以用判断一下信号量是否初始化成功，sem_getvalue(&mutex,&x);
	int ret = 1; //返回值
	key_t key = 100;//消息队列的key值
	//IPC_CREAT表示key值不存在就创建，0666是设置权限
	int msgid = msgget(key, IPC_CREAT | 0666);
	if (msgid == -1)
	{
		printf("msgget error\n");
		exit(1);
	}
	// pthread_create创建三个线程
	//第二个参数用来 设置线程的属性，设为NULL
	//第四个参数为传入执行函数的实参
	
	//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
	ret = pthread_create(&s1, NULL, sender1, (void *)&msgid);
	if (ret != 0)
	{
		printf("create  sender1  error!\n");
		exit(1);
	}
	ret = pthread_create(&r, NULL, receive, (void *)&msgid);
	if (ret != 0)
	{
		printf("create  receiver  error!\n");
		exit(1);
	}
	ret = pthread_create(&s2, NULL, sender2, (void *)&msgid);
	if (ret != 0)
	{
		printf("create  sender2  error!\n");
		exit(1);
	}
 
	// pthread_join使三个进程阻塞
	//为了回收资源，主线程会等待子线程结束。该函数就是用来等待线程终止的
	pthread_join(s1, NULL);
	pthread_join(s2, NULL);
	pthread_join(r, NULL);
 
	//删除消息队列
	msgctl(msgid, IPC_RMID, 0);
	return 0;
}