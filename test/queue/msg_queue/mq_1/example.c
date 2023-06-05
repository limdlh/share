#include <stdio.h>
#include <unistd.h>
#include "msg_process.h"

tmsg_buffer* test_msg_buff = NULL;

void* th_startVideo(void *arg) {
    printf("Wait for wakeup message to start video ----------------------> ");
    char table[] = "{\"hi\",\"limd\",\"hi\",\"limd\"}";
	test_msg_buff->sendmsg(test_msg_buff,0,0,table,sizeof(table));
	return arg;
}

int main(int argc,char *argv[])
{
	test_msg_buff = msg_buffer_init();
	/*char table[] = "{\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\",\"hello\","
				   "\"world\",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\""
				   ",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\""
				   ",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\""
				   ",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\",\"hello\",\"world\"}";*/
	char table[] = "{\"hello\",\"world\",\"hello\",\"world\"}";
    printf("sizeof Table: %d\n", sizeof(table));
	test_msg_buff->sendmsg(test_msg_buff,0,0,table,sizeof(table)); //发送数据
	int n = getnum(test_msg_buff);
	printf("getnum of Table n: %d\n", n);

	pthread_t th;
	int ret = pthread_create(&th, NULL, th_startVideo, NULL);
    if(ret != 0){
        printf("Create thread:[th_startVideo] error!");
        return 0;
    }
	n = getnum(test_msg_buff);
	printf("getnum of Table n: %d\n", n);
	sleep(1);
 
	tmsg_element* event = NULL;
	event = test_msg_buff->get_timeout(test_msg_buff,1000);  //接收数据
	if(event != NULL)
	{
		if(event->dt != NULL)
		{
			int i = 0;
			printf("Recv:");
			for(i=0; i<event->dt_len; i++)
			{
				printf("%c",event->dt[i]);
			}
			printf("\n");
		}
	}
	free_tmsg_element(event);
	
    return 0;
}