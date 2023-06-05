//TTSQueue.cpp
#include "TTSQueue.h"


void InitCQueue(TTSQueue pQueue)
{
    if (NULL == pQueue)
    {
        return;
    }
    pQueue->front = NULL;
    pQueue->rear = NULL;
}

LNode* BuyNode(char* val)
{
    LNode *pTmp = (LNode*)malloc(sizeof(LNode));
	strcpy(pTmp->data, val);
    pTmp->next= NULL;
    return pTmp;
}

void Push(TTSQueue pQueue, char* val)
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

bool Pop(TTSQueue pQueue, char *rtval)
{
    if (!IsEmpty(pQueue))
    {
        LNode *pTmp = pQueue->front;
		strcpy(rtval, pTmp->data);
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

int GetCQueueLen(TTSQueue pQueue)
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

void ShowQueue(TTSQueue pQueue)
{
    LNode *pCur = pQueue->front;
    while (NULL != pCur)
    {
        printf("%s ", pCur->data);
        pCur= pCur->next;
    }
    printf("\n");
}

bool IsEmpty(TTSQueue pQueue)
{
    return pQueue->front == NULL;
}

bool GetFront(TTSQueue pQueue, char *rtval)
{
    if (IsEmpty(pQueue))
    {
        return false;
    }

	strcpy(rtval, pQueue->front->data);
    return true;
}

void Destroy(TTSQueue pQueue)
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
    TTS_Queue head;
    InitCQueue(&head);
    Push(&head,"中国");
    Push(&head,"中华人民共和国");
    Push(&head,"长城");
    Push(&head,"浙江杭州");
    Push(&head,"在编编编程时加入错误检查就可及时时发现错误并且对出现的异常进行处理在编写如果目的缓冲函数时首先尽量使目的缓冲区长度足够长另外要检测目的缓冲区和源缓冲区如果目的缓冲区或源缓冲区是空就要在异常处理中结束程序");
	Push(&head,"在编编编程时加入错误检查就可及时时发现错误并且对出现的异常进行处理在编写如果目的缓冲函数时首先尽量使目的缓冲区长度足够长另外要检测目的缓冲区和源缓冲区如果目的缓冲区或源缓冲区是空就要在异常处理中结束程序结束程序");
    ShowQueue(&head);
    while (!IsEmpty(&head))
    {
        char tmp[MAX_TTS_WIDTH];
        if (Pop(&head, tmp))
        {
            printf("出队元素：%s, length: %d\n", tmp, strlen(tmp));
        }
		ShowQueue(&head);
    }
    Destroy(&head);
    return 0;
}

