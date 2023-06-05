#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


static char http_buffer[4096] = {0};
static int pos = 0;
static size_t aliyunWakeupCB(char *ptr, size_t size, size_t nmemb, void *userdata){
	size_t len = size * nmemb;  
	memcpy(http_buffer + pos, ptr, len);
	pos += len;
	printf("------------------>>http_buffer: %s, len: %ld\n", http_buffer, len);

	return len;  
}

void wakeupByAliyun(){
	CURL *curl;
	curl = curl_easy_init();
	if (!curl)
	{
		fprintf(stderr,"curl init failed\n");
		return;
	}
 
    char* aliyunWakeupURL = "https://iotapi.dev.hongyanyun.cn/controlscreen/device/setThing";  //用于测试阶段
    //char* aliyunWakeupURL = "https://iotapi.hongyanyun.cn";  //用于正式阶段
    char* aliyunWakeupFIELDS = "{\"args\": \"\",\"productKey\":\"a164msJWX5c\",\"deviceName\":\"H000059200400029\",\"identifier\":\"WakeUp\"}";
    printf("aliyunWakeupURL:%s\n", aliyunWakeupURL);
	printf("aliyunWakeupFIELDS:%s\n", aliyunWakeupFIELDS);
	
	curl_easy_setopt(curl, CURLOPT_URL, aliyunWakeupURL); //url地址
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); //不检查ssl，可访问https
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);  //不检查ssl，可访问https
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, aliyunWakeupFIELDS); //post参数
	curl_easy_setopt(curl, CURLOPT_POST, 1); //设置为非0表示本次操作为post
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); //打印调试信息

    struct curl_slist *http_header = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
	curl_slist_append(http_header, "debug:1");  //用于测试阶段
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); //设置为非0,响应头信息location
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//接收数据时超时设置，如果10秒内数据未
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, aliyunWakeupCB);
    CURLcode res;
	int cnt = 0;

loop:
	pos = 0;
	res = curl_easy_perform(curl);
	if (res != CURLE_OK){
		fprintf(stderr,"-----------> 返回值:%d\n",res);
		if(cnt < 5)
		{
			cnt ++;
			//sleep(1);
			goto loop;
		}
	}else{
		printf("Success -----------> 返回值:%d\n",res);
	}

	curl_easy_cleanup(curl);
}

int main(int argc,char *argv[])
{
	wakeupByAliyun();
    //printf("end  limd -----------> \n");
    return 0;
}
