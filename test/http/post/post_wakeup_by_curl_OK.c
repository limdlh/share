#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

 
#define POSTURL    "https://iotapi.dev.hongyanyun.cn/controlscreen/device/setThing"  //用于测试阶段
//#define POSTURL    "https://iotapi.hongyanyun.cn"  //用于正式阶段
#define POSTFIELDS "{\"args\": \"\",\"productKey\":\"a164msJWX5c\",\"deviceName\":\"H000059200400029\",\"identifier\":\"WakeUp\"}"
 
int main(int argc,char *argv[])
{
    CURL *curl;
	curl = curl_easy_init();
	if (!curl)
	{
		fprintf(stderr,"curl init failed\n");
		return -1;
	}
	
	printf("POSTURL:%s\n", POSTURL);
	printf("POSTFIELDS:%s\n", POSTFIELDS);
 
	curl_easy_setopt(curl, CURLOPT_URL,POSTURL); //url地址
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); //不检查ssl，可访问https
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);  //不检查ssl，可访问https
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,POSTFIELDS); //post参数
	curl_easy_setopt(curl, CURLOPT_POST,1); //设置问非0表示本次操作为post
	curl_easy_setopt(curl, CURLOPT_VERBOSE,1); //打印调试信息

    struct curl_slist *http_header = NULL;
	http_header = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
	curl_slist_append(http_header, "debug:1");  //用于测试阶段
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1); //设置为非0,响应头信息location
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//接收数据时超时设置，如果10秒内数据未
 
    CURLcode res;
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		switch(res)
		{
			case CURLE_UNSUPPORTED_PROTOCOL:
				fprintf(stderr,"-----------> 不支持的协议,由URL的头部指定\n");
				break;
			case CURLE_COULDNT_CONNECT:
				fprintf(stderr,"-----------> 不能连接到remote主机或者代理\n");
				break;
			case CURLE_HTTP_RETURNED_ERROR:
				fprintf(stderr,"-----------> http返回错误\n");
				break;
			case CURLE_READ_ERROR:
				fprintf(stderr,"-----------> 读本地文件错误\n");
				break;
			default:
				fprintf(stderr,"-----------> 返回值:%d\n",res);
				break;
		}
		return -1;
	}
	else
	{
		printf("Success -----------> 返回值:%d\n",res);
	}
 
	curl_easy_cleanup(curl);
    printf("end  limd -----------> \n");
    return 0;
}
