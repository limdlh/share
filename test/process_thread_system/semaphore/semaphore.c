/*
多个线程可以多次post，只有一个线程wait；一次wait即把信号清零
*/

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM 1
int queue[NUM];
sem_t product_number;

void *producer(void *arg)
{   
    int p = 0, n = 0;
    while (1) {
        queue[p] = rand() % 1000 + 1;
        printf("Produce %d >>>>>>>>>>>>>>>>>>> \n", queue[p]); 
		if(!sem_getvalue(&product_number, &n)){
			printf("producer()product_number: %d\n", n);
		}
		sem_post(&product_number);//可以释放上述操作所占用的资源并唤醒等待执行的下个线程，并且使semaphore的>    值+1 
		if(!sem_getvalue(&product_number, &n)){
			printf("producer()product_number: %d\n", n);
		}
		
        p = (p+1)%NUM;//循环队列+1
        sleep(rand()%5);
    }
}

void *producer_1(void *arg)
{   
    int p = 0, n = 0;
    while (1) {
        queue[p] = rand() % 1000 + 1;
        printf("producer_1 %d >>>>>>>>>>>>>>>>>>> \n", queue[p]); 
		if(!sem_getvalue(&product_number, &n)){
			printf("producer_1()product_number: %d\n", n);
		}
		sem_post(&product_number);//可以释放上述操作所占用的资源并唤醒等待执行的下个线程，并且使semaphore的>    值+1 
		if(!sem_getvalue(&product_number, &n)){
			printf("producer_1()product_number: %d\n", n);
		}
		
        p = (p+1)%NUM;//循环队列+1
        sleep(rand()%5);
    }
}

void *consumer(void *arg)
{
    int n = 0, c = 0;
    while (1) {
		if(!sem_getvalue(&product_number, &n)){
			printf("consumer()product_number: %d\n", n);
		}
        sem_wait(&product_number);//此时product线程的semophore值已经从init中的0变1,故此处有1个资源可供wait使用，不会阻塞！取用后在此变回0，但可以执行后续操作了。
		printf("Consume %d <<<<<<<<<<<<<<<<<<<<<<< \n", queue[c]);
		if(!sem_getvalue(&product_number, &n)){
			printf("<<<<<<<<<< consumer()product_number: %d\n", n);
			while(n > 0){
				sem_wait(&product_number);
				n--;
				printf("------ n: %d\n", n);
			};
		}
		sleep(5);
        //sleep(rand()%5);
    }
}

int main(int argc, char *argv[])
{
    pthread_t pid, pid_1, cid;

    sem_init(&product_number, 0, 0);
    pthread_create(&pid, NULL, producer, NULL);
	pthread_create(&pid_1, NULL, producer_1, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
    pthread_join(pid, NULL);
	pthread_join(pid_1, NULL);
    pthread_join(cid, NULL);
    sem_destroy(&product_number);
    return 0;
}
