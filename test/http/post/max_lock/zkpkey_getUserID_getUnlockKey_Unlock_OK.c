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

bool parseUnlockKeyJson(char* unlockKey, char* http_buffer){
	//printf("http_buffer: %s\n", http_buffer);
    cJSON* root = cJSON_Parse(http_buffer);
	cJSON* format = cJSON_GetObjectItem(root, "code");
	if(format == NULL || format->type == cJSON_NULL){
	    printf("No item [code] in json\n");
	} else {
		char* codeString = format->valuestring;
		if(!strcmp(codeString, "SUCCESS")){
			format = cJSON_GetObjectItem(root, "data");
			if(format == NULL || format->type == cJSON_NULL){
				printf("No item [data] in json\n");
			} else {
				cJSON* tmpJson = cJSON_GetObjectItem(format, "response");
				if(tmpJson == NULL || tmpJson->type != cJSON_Array){
					printf("No item [response] in json or format->type is NOT cJSON_Array\n");
				} else {
					char* responseStr = cJSON_GetArrayItem(tmpJson, 0)->valuestring;
					int length = strlen(responseStr);
					if(length < 256){
						//printf("JSON response: %s\n", responseStr);
					    strcpy(unlockKey, responseStr);
						return true;
					}else{
						printf("ERROR!!! JSON response [%s] length: %d, it's too large!!!!\n", responseStr, length);
					}
				}
			}
		} else {
			printf("zkpKey, get Max_Lock Unlock key error!!!JSON code: %s\n", codeString);
		}
	}
	return false;
}

bool getUnlockKey(char* APPUserId, char* unlockKey){
	static const char *pCertFile = "./8166028_midtest.lock.hongyanyun.cn.pem"; //client certificate
	const char *pKeyName = "./8166028_midtest.lock.hongyanyun.cn.key";        // private.key

	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL *curl;
	curl = curl_easy_init();
	if (!curl){
		fprintf(stderr,"curl init failed\n");
		return false;
	}
 
    char* url_get_key = "https://midtest.lock.hongyanyun.cn:8088/v1/lock/zkpKey";   //用于测试阶段
	char body_get_unlockkey[128] = {0};
	int n = snprintf(body_get_unlockkey, 128, "%s%s%s", "{\"appUserId\":\"", APPUserId, "\",\"deviceName\":\"H000059200400029\",\"productKey\":\"a164msJWX5c\"}");
	
    printf("url_get_key:%s\n", url_get_key);
	printf("body_get_unlockkey:%s\n", body_get_unlockkey);
	
	curl_easy_setopt(curl, CURLOPT_URL, url_get_key); //url地址
	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); //检查ssl
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); //检查ssl
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);  //检查ssl
	curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");/* cert is stored PEM coded in file...   since PEM is default, we needn't set it for PEM */
    curl_easy_setopt(curl, CURLOPT_SSLCERT, pCertFile);/* set the cert for client authentication */
	curl_easy_setopt(curl, CURLOPT_SSLKEY, pKeyName);/* set the private key (file or ID in engine) */
    curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");/* if we use a key stored in a crypto engine, we must set the key type to "ENG" */
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_get_unlockkey); //post参数
	curl_easy_setopt(curl, CURLOPT_POST, 1); //设置问非0表示本次操作为post
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); //打印调试信息

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
		fprintf(stderr," HTTP POST ERROR!! error -----------> 返回值:%d\n",res);
		if(cnt < 5)
		{
			cnt ++;
			sleep(1);
			goto loop;
		}
	} else {
		//printf("Success -----------> 返回值:%d\n",res);
		bRet = parseUnlockKeyJson(unlockKey, http_buffer);
	}

	curl_easy_cleanup(curl);
	return bRet;
}

bool parseUnlockJson(char* http_buffer){
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

bool unlock(char* POSTURL, char* POSTFIELDS){
	CURL *curl = curl_easy_init();
	if (!curl){
		fprintf(stderr,"curl init failed\n");
		return false;
	}

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
		//printf("Success -----------> 返回值:%d\n",res);
		bRet = parseUnlockJson(http_buffer);
	}

	curl_easy_cleanup(curl);
	return bRet;
}

bool parseAPPUserIdJson(char* APPUserId, char* http_buffer){
	//printf("http_buffer: %s\n", http_buffer);
    cJSON* root = cJSON_Parse(http_buffer);
	cJSON* format = cJSON_GetObjectItem(root, "code");
	if(format == NULL || format->type == cJSON_NULL){
	    printf("No item [code] in json\n");
	} else {
		int valueInt = format->valueint;
		if (200 == valueInt){
			format = cJSON_GetObjectItem(root, "data");
			if(format == NULL || format->type == cJSON_NULL){
				printf("No item [code] in json\n");
			} else {
			    char* dataString = format->valuestring;
				int len = strlen(dataString);
				if(len > 23 || len < 10){
					printf("Error, JSON data: %s length[%d] is invalid\n", dataString, len);
				}else{
					strcpy(APPUserId, dataString);
					return true;
				}
			}
		}else{
			printf("Fail, JSON code: %d\n", valueInt);
		}
	}
	return false;
}

bool getAppUserId(char* APPUserId){
	CURL *curl = curl_easy_init();
	if (!curl){
		fprintf(stderr,"curl init failed\n");
		return false;
	}

    char* POSTURL = "https://iotapi.dev.hongyanyun.cn/controlscreen/smarthome/gateway/user/userId";//用于测试阶段 //接口已废弃 2022.10.24
    //#define POSTURL    "https://iotapi.hongyanyun.cn"  //用于正式阶段
    char* POSTFIELDS = "{\"gatewayProductKey\":\"a1tNlnarBMz\",\"gatewayDeviceName\":\"H000057509200253\"}";
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
		//printf("Success -----------> 返回值:%d\n",res);
		bRet = parseAPPUserIdJson(APPUserId, http_buffer);
	}

	curl_easy_cleanup(curl);
	return bRet;
}

int main(int argc,char *argv[])
{
	char APPUserId[24] = {0};
	bool bRet = getAppUserId(APPUserId);
	if(bRet){
		printf("getAppUserId SUCCESS, APPUserId: %s\n", APPUserId);
	}else{
		printf("getAppUserId Fail!!!\n");
		return -1;
	}
	
	char unlockKey[256] = {0};
	bRet = getUnlockKey(APPUserId, unlockKey);
	if(bRet){
		printf("getUnlockKey SUCCESS, unlockKey: %s\n", unlockKey);
	}else{
		printf("getUnlockKey Fail!!!\n");
		return -1;
	}
    printf("limd -----------> unlockKey: %s\n", unlockKey);
	
	char* POSTURL = "https://iotapi.dev.hongyanyun.cn/controlscreen/device/setThing";  //用于测试阶段
	char POSTFIELDS[512] = {0};
	int n = snprintf(POSTFIELDS, 512, "%s%s%s", "{\"productKey\":\"a164msJWX5c\",\"deviceName\":\"H000059200400029\",\"identifier\":\"RemoteUnlock\",\"args\":\"{\\\"unlockCmd\\\":\\\"", unlockKey, "\\\"}\"}");
	bRet = unlock(POSTURL, POSTFIELDS);
	if(bRet){
		printf("Unlock SUCCESS\n");
	}else{
		printf("Unlock Fail!!!\n");
		return -1;
	}
    return 0;
}
