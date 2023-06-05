//TTSQueue.h
#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TTS_WIDTH 301 //最多输入100个汉字

typedef struct _LNode
{
    char data[MAX_TTS_WIDTH];
    struct _LNode *next;
}LNode;

typedef struct _TTSQueue
{
    LNode *front;
    LNode *rear;
}TTS_Queue, *TTSQueue;

void InitCQueue(TTSQueue pQueue);             //初始化队列
LNode* BuyNode(char* val);                //从堆中申请一个节点的内存空间
void Push(TTSQueue pQueue, char* val);       //入队，从队尾(rear)入
bool Pop(TTSQueue pQueue, char* rtval);     //出队，从队首(front)出
int GetCQueueLen(TTSQueue pQueue);         //获取队列长度
void ShowQueue(TTSQueue pQueue);           //输出队列所有元素
bool IsEmpty(TTSQueue pQueue);             //队列为空则返回true
bool GetFront(TTSQueue pQueue, char *rtval); //获取队首元素
void Destroy(TTSQueue pQueue);             //销毁队列(释放所有节点的内存空间)
