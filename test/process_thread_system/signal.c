#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h> 
#include <signal.h>
#include <pthread.h>
 
typedef void (*signal_handler)(int);
 
void signal_routine(int sig)
{
  switch (sig)
  {
    case 1:
    {
      printf(">>: SIGHUP \r\n");// 终端线路挂断
      break;
    }
    case 2:
    {
      printf(">>: SIGINT \r\n");//"ctrl+c"
      break;      
    }
    case 3:
    {
      printf(">>: SIGQUIT \r\n");//"ctrl+\""
      break;
    }
	case 6:
    {
      printf(">>: SIGABRT \r\n");//堆异常操作
      break;
    }
	case 9:
    {
      printf(">>: SIGKILL \r\n");//杀死进程
      break;
    }
	case 11:
    {
      printf(">>: SIGSEGV \r\n");//段非法错误
      break;
    }
    case 15:
    {
      printf(">>: SIGTERM \r\n");//软件终止信号
      break;
    }
	case 24:
    {
      printf(">>: SIGSTOP \r\n");//ctrl+z,暂停进程
      break;
    }
  }
  printf("killing on exit! sig: %d\n", sig);
  exit(0);
	return;
}

void *consumer(void *arg)
{
	printf("consumer()\n");
	sleep(1);
	int n = 0;
	int *p = &n;
	printf("*p: %d\n", *p);
	//free(p);
}

void test()
{
	printf("test()\n");
	sleep(1);
	int *pt = 123;
	printf("*pt: %d\n", *pt);
	free(pt);
}

int main(void)
{
	signal_handler sig_handler = signal_routine;
	signal(SIGHUP, sig_handler);
	signal(SIGINT, sig_handler);
	signal(SIGKILL, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGTERM, sig_handler);
	signal(SIGSEGV, sig_handler);
	signal(SIGILL, sig_handler); //4
	signal(SIGTRAP, sig_handler);//5
	signal(SIGABRT, sig_handler);//6
	signal(SIGURG, sig_handler);//29
	signal(SIGFPE, sig_handler);//8
	signal(SIGBUS, sig_handler);//10
	signal(SIGPIPE, sig_handler);//13
	signal(SIGALRM, sig_handler);//14
	signal(SIGIOT, sig_handler);//6
	signal(SIGSTOP, sig_handler);//24
	
	pthread_t pid;
	pthread_create(&pid, NULL, consumer, NULL);
    //test();
	
	while(true)
	{
		int a = 1 + 1;
		//sleep(1);
	}
	return 0;
}
 