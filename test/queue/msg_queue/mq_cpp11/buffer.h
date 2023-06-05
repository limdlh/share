/* buffer.h
* 用于线程安全缓冲区的所有声明
*/
#include <stdbool.h>
#include <threads.h>

typedef struct Buffer
{
    int *data;                          // 指向数据数组的指针
    size_t size, count;                 // 元素数量的最大值和当前值
    size_t tip, tail;                   // tip = 下一个空点的索引
    mtx_t mtx;                          // 一个互斥
    cnd_t cndPut, cndGet;               // 两个条件变量
} Buffer;

bool bufInit( Buffer *bufPtr, size_t size );
void bufDestroy(Buffer *bufPtr);

bool bufPut(Buffer *bufPtr, int data);
bool bufGet(Buffer *bufPtr, int *dataPtr, int sec);
