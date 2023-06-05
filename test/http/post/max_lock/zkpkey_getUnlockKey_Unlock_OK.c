#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <curl/curl.h>
#include "cJSON.h"


static char http_buffer[4096] = {0};
static int pos = 0;
static size_t cb(char *ptr, size_t size, size_t nmemb, void *userdata){
	size_t len = size * nmemb;  
	memcpy(http_buffer + pos, ptr, len);
	pos += len;
	//printf("------------------>>http_buffer: %s, len: %ld\n", http_buffer, len);

	return len;  
}

void parseJson(char* unlockKey, char* http_buffer){
	//printf("http_buffer: %s\n", http_buffer);
    cJSON* root = cJSON_Parse(http_buffer);
	cJSON* format = cJSON_GetObjectItem(root, "code");
	if(format == NULL || format->type == cJSON_NULL){
	    printf("No item [code] in json\n");
	} else {
		char* valuestring = format->valuestring;
		//printf("JSON code: %s\n", valuestring);
		if(!strcmp(valuestring, "SUCCESS")){
			format = cJSON_GetObjectItem(root, "data");
			if(format == NULL || format->type == cJSON_NULL){
				printf("No item [data] in json\n");
			} else {
				cJSON* tmpJson = cJSON_GetObjectItem(format, "response");
				if(tmpJson == NULL || tmpJson->type != cJSON_Array){
					printf("No item [response] in json or format->type is NOT cJSON_Array\n");
				} else {
					char* valuestring = cJSON_GetArrayItem(tmpJson, 0)->valuestring;
					//printf("JSON response: %s\n", valuestring);
					int length = strlen(valuestring);
					if(length < 256){
					    strcpy(unlockKey, valuestring);
					}else{
						printf("ERROR!!! JSON response length: %d, it's too large!!!!\n", length);
					}
				}
			}
		} else {
			printf("zkpKey, get Max_Lock Unlock key error!!!\n");
		}
	}
}

void getUnlockKey(char* unlockKey){
	static const char *pCertFile = "./8166028_midtest.lock.hongyanyun.cn.pem"; //client certificate
	const char *pKeyName = "./8166028_midtest.lock.hongyanyun.cn.key";        // private.key

	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL *curl;
	curl = curl_easy_init();
	if (!curl){
		fprintf(stderr,"curl init failed\n");
		return;
	}
 
    char* url_get_key = "https://midtest.lock.hongyanyun.cn:8088/v1/lock/zkpKey";   //用于测试阶段
	char* field_get_key = "{\"appUserId\":\"1569502704483377154\",\"deviceName\":\"H000059200400029\",\"productKey\":\"a164msJWX5c\"}";
    printf("url_get_key:%s\n", url_get_key);
	printf("field_get_key:%s\n", field_get_key);
	
	curl_easy_setopt(curl, CURLOPT_URL, url_get_key); //url地址
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); //检查ssl
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);  //检查ssl
	curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");/* cert is stored PEM coded in file...   since PEM is default, we needn't set it for PEM */
    curl_easy_setopt(curl, CURLOPT_SSLCERT, pCertFile);/* set the cert for client authentication */
	curl_easy_setopt(curl, CURLOPT_SSLKEY, pKeyName);/* set the private key (file or ID in engine) */
    curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");/* if we use a key stored in a crypto engine, we must set the key type to "ENG" */
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, field_get_key); //post参数
	curl_easy_setopt(curl, CURLOPT_POST, 1); //设置问非0表示本次操作为post
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0); //打印调试信息

	struct curl_slist *http_header = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
	curl_slist_append(http_header, "debug:1");  //用于测试阶段
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); //设置为非0,响应头信息location
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//接收数据时超时设置，如果10秒内数据未
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
	int cnt = 0;
    CURLcode res;
	
loop:
	pos = 0;
	res = curl_easy_perform(curl);
	if (res != CURLE_OK){
		fprintf(stderr," HTTP POST ERROR!! error -----------> 返回值:%d\n",res);
		if(cnt < 5)
		{
			cnt ++;
			sleep(1);
			goto loop;
		}
	} else {
		//printf("Success -----------> 返回值:%d\n",res);
		parseJson(unlockKey, http_buffer);
	}

	curl_easy_cleanup(curl);
}

bool parseResult(char* http_buffer){
	//printf("http_buffer: %s\n", http_buffer);
    cJSON* root = cJSON_Parse(http_buffer);
	cJSON* format = cJSON_GetObjectItem(root, "code");
	if(format == NULL || format->type == cJSON_NULL){
	    printf("No item [code] in json\n");
	} else {
		int valueInt = format->valueint;
		printf("JSON code: %d\n", valueInt);
		return (200 == valueInt);
	}
}

bool unlock(char* unlockKey){
	CURL *curl = curl_easy_init();
	if (!curl){
		fprintf(stderr,"curl init failed\n");
		return false;
	}
 
    char* POSTURL = "https://iotapi.dev.hongyanyun.cn/controlscreen/device/setThing";  //用于测试阶段
//#define POSTFIELDS "{\"productKey\":\"a164msJWX5c\",\"deviceName\":\"H000059200400029\",\"identifier\":\"RemoteUnlock\",\"args\":\"{\\\"unlockCmd\\\":\\\"BPuEkJP7v5MMuYDwYp/JVRslSUxyfcYCrm1LT3z/iehE+RDl+3iGdT68snPBxvMUxhn7bedZ4iNhGSJE58o3LARrcOkGjsKEAlGYsuraE2MU2ZlVpc+mlb+L0RFPpgfRd02xBIjvDWygsnPIkQ==\\\"}\"}"

    char POSTFIELDS[512] = {0};
	int n = snprintf(POSTFIELDS, 512, "%s%s%s", "{\"productKey\":\"a164msJWX5c\",\"deviceName\":\"H000059200400029\",\"identifier\":\"RemoteUnlock\",\"args\":\"{\\\"unlockCmd\\\":\\\"", unlockKey, "\\\"}\"}");
	
    printf("POSTURL:%s\n", POSTURL);
	printf("POSTFIELDS:%s\n", POSTFIELDS);
	
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL); //url地址
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); //不检查ssl，可访问https
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);  //不检查ssl，可访问https
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POSTFIELDS); //post参数
	curl_easy_setopt(curl, CURLOPT_POST, 1); //设置问非0表示本次操作为post
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0); //打印调试信息

	struct curl_slist *http_header = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
	curl_slist_append(http_header, "debug:1");  //用于测试阶段
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); //设置为非0,响应头信息location
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//接收数据时超时设置，如果10秒内数据未
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    int cnt = 0;
	bool bRet = false;
	CURLcode res;
	
loop:
	pos = 0;
	res = curl_easy_perform(curl);
	if (res != CURLE_OK){
		if(cnt < 5){
			cnt ++;
			sleep(1);
			goto loop;
		}
	}else{
		printf("Success -----------> 返回值:%d\n",res);
		bRet = parseResult(http_buffer);
	}

	curl_easy_cleanup(curl);
	return bRet;
}

int main(int argc,char *argv[])
{
	char unlockKey[256] = {0};
	getUnlockKey(unlockKey);
    printf("limd -----------> unlockKey: %s\n", unlockKey);
	bool bRet = unlock(unlockKey);
	if(bRet){
		printf("Unlock SUCCESS\n");
	}else{
		printf("Unlock Fail!!!\n");
	}
    return 0;
}
