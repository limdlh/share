// producer_consumer.c
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>

#define NP 2                             // 生产者的数量
#define NC 3                             // 消费者的数量

int producer(void *);                    // 线程函数
int consumer(void *);

struct Arg { int id; Buffer *bufPtr; };          // 线程函数的参数
_Noreturn void errorExit(const char* msg)
{
    fprintf(stderr, "%s\n", msg); exit(0xff);
}

int main(void)
{
    printf("Producer-Consumer Demo\n\n");
    Buffer buf;                                  // 为5个产品创建一个缓冲区
    bufInit( &buf, 5 );

    thrd_t prod[NP], cons[NC];           // 线程
    struct Arg prodArg[NP], consArg[NC]; // 线程的参数
    int i = 0, res = 0;

    for ( i = 0; i < NP; ++i )                // 启动生产者
    {
        prodArg[i].id = i+1, prodArg[i].bufPtr = &buf;
        if (thrd_create( &prod[i], producer, &prodArg[i] ) != thrd_success)
           errorExit("Thread error.");
    }

    for ( i = 0; i < NC; ++i )                // 启动消费者
    {
       consArg[i].id = i+1, consArg[i].bufPtr = &buf;
       if ( thrd_create( &cons[i], consumer, &consArg[i] ) != thrd_success)
          errorExit("Thread error.");
    }

    for ( i = 0; i < NP; ++i )                // 等待线程结束
      thrd_join(prod[i], &res),
      printf("\nProducer %d ended with result %d.\n", prodArg[i].id, res);

      for ( i = 0; i < NC; ++i )
         thrd_join(cons[i], &res),
         printf("Consumer %d ended with result %d.\n", consArg[i].id, res);
      bufDestroy( &buf );
      return 0;
}

int producer(void *arg)                         // 生产者线程函数
{
    struct Arg *argPtr = (struct Arg *)arg;
    int id = argPtr->id;
    Buffer *bufPtr = argPtr->bufPtr;
    int count = 0;
    for (int i = 0; i < 10; ++i)
    {
        int data = 10*id + i;
        if (bufPut( bufPtr, data ))
            printf("Producer %d produced %d\n", id, data), ++count;
        else
        { fprintf( stderr,
                 "Producer %d: error storing %d\n", id, data);
          return -id;
        }
    }
    return count;
}

int consumer(void *arg)                         // 消费者线程函数
{
    struct Arg *argPtr = (struct Arg *)arg;
    int id = argPtr->id;
    Buffer *bufPtr = argPtr->bufPtr;
  
    int count = 0;
    int data = 0;
    while (bufGet( bufPtr, &data, 2 ))
    {
        ++count;
        printf("Consumer %d consumed %d\n", id, data);
    }
    return count;
}