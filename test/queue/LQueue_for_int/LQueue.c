//LQueue.cpp
#include"LQueue.h"

//初始化队列
void InitCQueue(LQueue pQueue)
{
    if (NULL == pQueue)         //实际上就是初始化头结点
    {
        return;
    }
    pQueue->front = NULL;
    pQueue->rear = NULL;
}

LNode* BuyNode(int val)
{
    LNode *pTmp = (LNode*)malloc(sizeof(LNode));
    pTmp->data= val;
    pTmp->next= NULL;
    return pTmp;
}

//入队，从队尾(rear)入
void Push(LQueue pQueue, int val)
{
    LNode *pCur = BuyNode(val);
    if (NULL == pQueue->rear)
    {
        pQueue->front = pCur;
        pQueue->rear = pQueue->front;
    }
    else
    {
        pQueue->rear->next = pCur;
        pQueue->rear = pCur;
    }
}

//出队，从队首(front)出
bool Pop(LQueue pQueue, int *rtval)
{
    if (!IsEmpty(pQueue))
    {
        LNode *pTmp = pQueue->front;
        *rtval = pTmp->data;
        pQueue->front = pTmp->next;
        free(pTmp);
        if (NULL == pQueue->front)
        {
            pQueue->rear = pQueue->front;
        }
        return true;
    }
    return false;
}

//获取队列长度
int GetCQueueLen(LQueue pQueue)
{
    int iCount = 0;
    LNode *pCur = pQueue->front;
    while (NULL != pCur)
    {
        ++iCount;
        pCur= pCur->next;
    }
    return iCount;
}

//输出队列所有元素
void ShowQueue(LQueue pQueue)
{
    LNode *pCur = pQueue->front;
    while (NULL != pCur)
    {
        printf("%5d", pCur->data);
        pCur= pCur->next;
    }
    printf("\n");
}

//队列为空则返回true
bool IsEmpty(LQueue pQueue)
{
    return pQueue->front == NULL;
}

//获取队首元素
bool GetFront(LQueue pQueue, int *rtval)
{
    if (IsEmpty(pQueue))
    {
        return false;
    }

    *rtval = pQueue->front->data;
    return true;
}

//销毁队列(释放所有节点的内存空间)
void Destroy(LQueue pQueue)
{
    LNode *pCur = pQueue->front;
    LNode *pTmp;
    while (NULL != pCur)
    {
        pTmp= pCur->next;
        free(pCur);
        pCur= pTmp;
    }
}

int main()
{
    Queue head;
    InitCQueue(&head);
    Push(&head,1);
    Push(&head,2);
    Push(&head,3);
    Push(&head,4);
    Push(&head,5);
    ShowQueue(&head);
    while (!IsEmpty(&head))
    {
        int tmp;
        if (Pop(&head, &tmp))
        {
            printf("出队元素：%5d\n", tmp);
        }
    }
    Destroy(&head);
    return 0;
}

