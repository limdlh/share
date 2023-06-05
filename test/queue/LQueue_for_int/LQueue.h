//LQueue.h
#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct _LNode
{
    int data;
    struct _LNode *next;
}LNode;

typedef struct _LQueue
{
    LNode *front;
    LNode *rear;
}Queue, *LQueue;

void InitCQueue(LQueue pQueue);             //初始化队列
LNode* BuyNode(int val);                //从堆中申请一个节点的内存空间
void Push(LQueue pQueue, int val);       //入队，从队尾(rear)入
bool Pop(LQueue pQueue, int *rtval);     //出队，从队首(front)出
int GetCQueueLen(LQueue pQueue);         //获取队列长度
void ShowQueue(LQueue pQueue);           //输出队列所有元素
bool IsEmpty(LQueue pQueue);             //队列为空则返回true
bool GetFront(LQueue pQueue, int *rtval); //获取队首元素
void Destroy(LQueue pQueue);             //销毁队列(释放所有节点的内存空间)
