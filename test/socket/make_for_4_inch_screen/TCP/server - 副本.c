#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
	   
#include "cJSON.h"

#define PORT 10081
#define BACKLOG 2/*最大监听数*/ 
#define MAX_DATA 512//Socket接收到的数据最大程度 

#define SEND_PORT 10080
#define SEND_DEST_IP "192.168.8.153"

// {"topic":"/LAPI/V1.0/System/LowPowerState","data":{"DeviceName": "H000059200400029","ProductKey": "a164msJWX5c","State": 0}}
// {"topic":"/LAPI/V1.0/System/Alarm","data":{"DeviceName": "H000059200400029","ProductKey": "a164msJWX5c","AlarmDesc": "DoorBell","TimeStamp": "1666720663000","AlarmSeq": 0}}
// {"topic":"/LAPI/V1.0/System/Wakeup","data":{"DeviceName": "H000059200400029","ProductKey": "a164msJWX5c"}}
bool gBoolWakeup = false;
int sockfd, new_fd;/*socket句柄和建立连接后的句柄*/

void stop(int signo)
{
	printf("stop\n");
	close(new_fd);	
	close(sockfd);
	_exit(0);
}

void parseJson(cJSON* rootJson)
{
    if(NULL == rootJson){
        printf("rootJson == NULL");
	    return;
    }
	
	cJSON *json_tmp = cJSON_GetObjectItem(rootJson, "topic");
	if(NULL == json_tmp || cJSON_NULL == json_tmp->type)
	{
	    printf("No [topic] item in json");
		return;
	}
	
	char* topic_str = json_tmp->valuestring;
	printf("topic_str: %s\n", topic_str);
	
	cJSON* data_json = cJSON_GetObjectItem(rootJson, "data");
	
	json_tmp = cJSON_GetObjectItem(data_json, "DeviceName");
	if(NULL == json_tmp || cJSON_NULL == json_tmp->type)
	{
	    printf("No [DeviceName] item in json");
		return;
	}
	char* deviceName_str = json_tmp->valuestring;
	printf("deviceName_str: %s\n", deviceName_str);
	
	json_tmp = cJSON_GetObjectItem(data_json, "ProductKey");
	if(NULL == json_tmp || cJSON_NULL == json_tmp->type)
	{
	    printf("No [ProductKey] item in json");
		return;
	}
	char* productKey_str = json_tmp->valuestring;
	printf("productKey_str: %s\n", productKey_str);

    char topic_item[50] = {0};
	char *ptr = strrchr(topic_str, '/');
	if(NULL == ptr)
	{
		printf("topic content in json is invalid, topic_str: %s", topic_str);
	}
	else
	{
		sprintf(topic_item, "%s", ptr + 1);
	}
		
	if(!strcmp(topic_item, "LowPowerState"))
    {
        printf("topic is [LowPowerState]\n");
		json_tmp = cJSON_GetObjectItem(data_json, "State");
		if(NULL == json_tmp || cJSON_NULL == json_tmp->type){
			printf("No [State] item in json");
			return;
		}
		int state_int = json_tmp->valueint;
		printf("state_int: %d\n", state_int);
    } else if (!strcmp(topic_item, "Alarm")){
        printf("topic is [Alarm]\n");
		json_tmp = cJSON_GetObjectItem(data_json, "AlarmDesc");
		if(NULL == json_tmp || cJSON_NULL == json_tmp->type){
			printf("No [AlarmDesc] item in json");
			return;
		}
		char* alarmDesc_str = json_tmp->valuestring;
		printf("alarmDesc_str: %d\n", alarmDesc_str);
		
		json_tmp = cJSON_GetObjectItem(data_json, "TimeStamp");
		if(NULL == json_tmp || cJSON_NULL == json_tmp->type){
			printf("No [TimeStamp] item in json");
			return;
		}
		char* timeStamp_str = json_tmp->valuestring;
		printf("timeStamp_str: %d\n", timeStamp_str);
		
		json_tmp = cJSON_GetObjectItem(data_json, "AlarmSeq");
		if(NULL == json_tmp || cJSON_NULL == json_tmp->type) {
			printf("No [AlarmSeq] item in json");
			return;
		}
		int alarmSeq_int = json_tmp->valueint;
		printf("alarmSeq_int: %d\n", alarmSeq_int);
    } else if (!strcmp(topic_item, "Wakeup")) {
        printf("topic is [Wakeup]\n");
		json_tmp = cJSON_GetObjectItem(data_json, "State");
		if(NULL == json_tmp || cJSON_NULL == json_tmp->type){
			printf("No [State] item in json");
			return;
		}
		int state_int = json_tmp->valueint;
		printf("state_int: %d\n", state_int);
		if(state_int) {
		    gBoolWakeup = true;
		} else {
			gBoolWakeup = false;
		}
    } else {
		printf("invalid topic_item: %s", topic_item);
	}

	printf("\n");
}

void* thread(void *arg) {
	int sock_fd = *(int*)arg;
    printf( "This is a thread and arg = %d.\n", sock_fd);
	
	
	int sendSockfd;
    struct sockaddr_in dest_addr;/*目标地址信息*/

    sendSockfd=socket(AF_INET,SOCK_STREAM,0);/*建立socket*/
    if(sendSockfd==-1){
        printf("socket failed:%d\n",errno);
    }

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SEND_PORT);
    dest_addr.sin_addr.s_addr = inet_addr(SEND_DEST_IP);
    bzero(&(dest_addr.sin_zero), 8);
    
    if(connect(sendSockfd,(struct sockaddr*)&dest_addr,sizeof(struct sockaddr))==-1){//连接方法，传入句柄，目标地址和大小 
        printf("connect failed:%d\n",errno);//失败时可以打印errno 
    } else{
        printf("connect success\n");
		int time_interval = 200;
		while((time_interval <= 2000) && (!gBoolWakeup)){
			time_interval += 50;
			char* wakeupStr = "{\"topic\":\"/LAPI/V1.0/System/Wakeup\",\"data\":{\"DeviceName\": \"H000059200400029\",\"ProductKey\": \"a164msJWX5c\"}}";
			int len = strlen(wakeupStr)+1;
			send(sendSockfd, wakeupStr, len, 0);//发送内容，参数分别是连接句柄，内容，大小，其他信息（设为0即可） 
			printf("wakeupStr: %s, time_interval: %d, len: %d\n", wakeupStr, time_interval, len);
			usleep(time_interval * 1000);
		}
    }
    close(sendSockfd);//关闭socket 
	
	
	
    /*int time_interval = 200;
	sleep(1);
	
	while((time_interval <= 2000) && (!gBoolWakeup)){
		if(sock_fd == -1){
            printf("receive failed\n");
        } else {
			time_interval += 50;
			char* wakeupStr = "{\"topic\":\"/LAPI/V1.0/System/Wakeup\",\"data\":{\"DeviceName\": \"H000059200400029\",\"ProductKey\": \"a164msJWX5c\"}}";
			int len = strlen(wakeupStr)+1;
			send(sock_fd, wakeupStr, len, 0);//发送内容，参数分别是连接句柄，内容，大小，其他信息（设为0即可） 
			printf("wakeupStr: %s, time_interval: %d, len: %d\n", wakeupStr, time_interval, len);
		}
		usleep(time_interval * 1000);
	}*/
	
    return arg;
}

int main(){
    struct sockaddr_in my_addr;/*本方地址信息结构体，下面有具体的属性赋值*/
    struct sockaddr_in their_addr;/*对方地址信息*/
    int sin_size;
	char buf[MAX_DATA] = {0};
	cJSON* root;
	int state_int = 0;
	int n = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);//建立socket 
    if(sockfd == -1){
        printf("socket failed:%d\n", errno);
        return -1;
    }
    my_addr.sin_family = AF_INET;/*该属性表示接收本机或其他机器传输*/
    my_addr.sin_port = htons(PORT);/*端口号*/
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);/*IP，括号内容表示本机IP*/
    bzero(&(my_addr.sin_zero), 8);/*将其他属性置0*/
    if(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) < 0){//绑定地址结构体和socket
        printf("bind error\n");
        return -1;
    }
	listen(sockfd, BACKLOG);//开启监听 ，第二个参数是最大监听数
	
	pthread_t th;
	//int arg = 200;
	int ret = pthread_create(&th, NULL, thread, &sockfd);
    if( ret != 0 ){
        printf( "Create thread error!\n");
        return -1;
    }
	
	signal(SIGINT, stop); /*注册SIGINT 信号*/
	
	while(1){
		sin_size = sizeof(struct sockaddr_in);
		new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);//在这里阻塞直到接收到消息，参数分别是socket句柄，接收到的地址信息以及大小 
		if(new_fd == -1){
            printf("receive failed\n");
            continue;
        } else {
            printf("receive success from port: %d\n", PORT);
			while(1){
				memset(buf, 0, MAX_DATA);
				recv(new_fd, buf, MAX_DATA, 0);
				if(n < 0){
					printf("recv error, n = [%d]\n", n);
					break;
				}
				printf("new_fd receive buf: %s\n", buf);
				
				root = cJSON_Parse(buf);
				parseJson(root);
			}
        }
    }
    return 0;
}

