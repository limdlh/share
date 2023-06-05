/* -------------------------------------------------------------
* buffer.c
* 定义用于处理Buffer的函数
*/
#include "buffer.h"
#include <stdlib.h>                       // 为了使用malloc()和free()
bool bufInit( Buffer *bufPtr, size_t size)
{
    if ((bufPtr->data = malloc( size * sizeof(int))) == NULL)
       return false;
    bufPtr->size = size;
    bufPtr->count = 0;
    bufPtr->tip = bufPtr->tail = 0;
    return    mtx_init( &bufPtr->mtx, mtx_plain) == thrd_success
           && cnd_init( &bufPtr->cndPut) == thrd_success
           && cnd_init( &bufPtr->cndGet) == thrd_success;
}

void bufDestroy(Buffer *bufPtr)
{
    cnd_destroy( &bufPtr->cndGet );
    cnd_destroy( &bufPtr->cndPut );
    mtx_destroy( &bufPtr->mtx );
    free( bufPtr->data );
}

// 在缓冲区中插入一个新元素
bool bufPut(Buffer *bufPtr, int data)
{
    mtx_lock( &bufPtr->mtx );

    while (bufPtr->count == bufPtr->size)
       if (cnd_wait( &bufPtr->cndPut, &bufPtr->mtx ) != thrd_success)
          return false;

    bufPtr->data[bufPtr->tip] = data;
    bufPtr->tip = (bufPtr->tip + 1) % bufPtr->size;
    ++bufPtr->count;

    mtx_unlock( &bufPtr->mtx );
    cnd_signal( &bufPtr->cndGet );

    return true;
}

// 从缓冲区中移除一个元素
// 如果缓冲区是空的，则等待不超过sec秒
bool bufGet(Buffer *bufPtr, int *dataPtr, int sec)
{
    struct timespec ts;
    timespec_get( &ts, TIME_UTC );             // 当前时间
    ts.tv_sec += sec;                              // + sec秒延时

    mtx_lock( &bufPtr->mtx );
    while ( bufPtr->count == 0 )
       if (cnd_timedwait(&bufPtr->cndGet,
                         &bufPtr->mtx, &ts) != thrd_success)
          return false;

    *dataPtr = bufPtr->data[bufPtr->tail];
    bufPtr->tail = (bufPtr->tail + 1) % bufPtr->size;
    --bufPtr->count;

    mtx_unlock( &bufPtr->mtx );
    cnd_signal( &bufPtr->cndPut );

    return true;
}