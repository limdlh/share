
#include "msg.h"


int main(){
    Queue * q = InitQueue();

	printf("GetSize: %d\n", GetSize(q));
	EnQueue(q, "hello");
	EnQueue(q, "limd");
	EnQueue(q, "txt");

	printf("GetSize: %d\n", GetSize(q));

	QueueTraverse(q);

	//printf("DeQueue: %s\n", DeQueue(q)->frame);

    PNode tmp = DeQueue(q);
    printf("DeQueue: %s\n", tmp->frame);
    //free tmp;

    //char str[100];
	//GetFront(q, str);
	//printf("str: %s\n", str);
	
	printf("GetSize: %d\n", GetSize(q));

	QueueTraverse(q);
}


