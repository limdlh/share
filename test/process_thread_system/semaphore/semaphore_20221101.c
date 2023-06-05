#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM 1
int queue[NUM];
sem_t blank_number, product_number;

void *producer(void *arg)
{   
    int p = 0;
    while (1) {
        sem_wait(&blank_number);//wait可以获得信号，使init初始化中semaphore值-1，如果减到0则挂起等待，成功取用后继续进行后续操作
        queue[p] = rand() % 1000 + 1;
        printf("Produce %d\n", queue[p]); 
		sem_post(&product_number);//可以释放上述操作所占用的资源并唤醒等待执行的下个线程，并且使semaphore的>    值+1    
        p = (p+1)%NUM;//循环队列+1
        sleep(rand()%5);
    }
}

void *consumer(void *arg)
{
    int c = 0;
    while (1) {
        sem_wait(&product_number);//此时product线程的semophore值已经从init中的0变1,故此处有1个资源可供wait使用，不会阻塞！取用后在此变回0，但可以执行后续操作了。
        //此处因为还有一个信号可供wait取用，成功取用然后继续后续操作
        printf("Consume %d\n", queue[c]);
        queue[c] = 0;//取用过的队列值设为0，便于观察进程间是否同步（即是否consumer取了已经取过的队列值:）
        sem_post(&blank_number);//释放一个信号（此处释放的是wait后续操作所占用的线程信号）给blank_number并唤醒下一个阻塞或等待中的进程,semaphore+1
        c = (c+1)%NUM;
        sleep(rand()%5);
    }
}

int main(int argc, char *argv[])
{
    pthread_t pid, cid;

    sem_init(&blank_number, 0, NUM);
    sem_init(&product_number, 0, 0);
    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
    sem_destroy(&blank_number);
    sem_destroy(&product_number);
    return 0;
}
