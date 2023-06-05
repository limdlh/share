
#include "msg.h"


Queue * q = NULL;

void* th_test(void *arg) {
	int times = 30000;
	while(times > 0){
        EnQueue(q, "hello, here is limd, what do you want to do??");
		--times;
	}
	printf("---------------------->>>GetSize: %d\n", GetSize(q));
	return arg;
}

void* th_insert(void *arg) {
	int times = 10000;
	while(times > 0){
        EnQueue(q, "hello, here is limd!");
		--times;
	}
	printf("---------------------->>>GetSize: %d\n", GetSize(q));
	return arg;
}


int main(){
    q = InitQueue();

	printf("GetSize: %d\n", GetSize(q));
	EnQueue(q, "hello, here is limd, what do you want to do??");
	EnQueue(q, "limd");
	EnQueue(q, "txt");
    EnQueue(q, "hello");
	EnQueue(q, "limd");
	EnQueue(q, "txt");
	printf("GetSize: %d\n", GetSize(q));
	QueueTraverse(q);

	//printf("DeQueue: %s\n", DeQueue(q)->frame);
    pthread_t th;
    int ret = pthread_create(&th, NULL, th_test, NULL);
    if(ret != 0){
        printf("Create thread:[th] error!");
        return -1;
    }

    pthread_t th_1;
    ret = pthread_create(&th_1, NULL, th_insert, NULL);
    if(ret != 0){
        printf("Create thread:[th_1] error!");
        return -1;
    }

    pthread_join(th, NULL);
    pthread_join(th_1, NULL);

    /*char tmp[100];
    DeQueue(q, tmp);
    printf("DeQueue: %s\n", tmp);

	printf("GetSize: %d\n", GetSize(q));
	QueueTraverse(q);

	DeQueue(q, tmp);
    printf("DeQueue: %s\n", tmp);*/
	printf("GetSize: %d\n", GetSize(q));
	QueueTraverse(q);
}


