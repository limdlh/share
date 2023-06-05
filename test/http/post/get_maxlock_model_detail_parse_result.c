#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include "./max_lock/cJSON.h"


#define JSON_VALUE_LENGTH 32

bool parseJsonStr(cJSON* pSubJson, char* subItem, char jsonVal[]){
    if(NULL == pSubJson || NULL == subItem) {
        printf("NULL == pSubJson or NULL == subItem !!!ERROR!!\n");
        return false;
    }

    cJSON* identifier = cJSON_GetObjectItem(pSubJson, "identifier");
    if(identifier) {
        char* ident_str = identifier->valuestring;
        if(!strcmp(ident_str, subItem))
        {
            cJSON* value = cJSON_GetObjectItem(pSubJson, "value");
            if(value) {
                char* value_str = value->valuestring;
                printf("identifier: %s, value: %s\n", ident_str, value_str);
                int len = strlen(value_str);
                if(len >= JSON_VALUE_LENGTH)
                {
                    printf("Value: %s, Length: %d, too large than %d!!! ERROR!!!\n", value_str, len, JSON_VALUE_LENGTH);
                    return false;
                }
                memset(jsonVal, 0, JSON_VALUE_LENGTH);
                strcpy(jsonVal, value_str);
                return true;
            }
            else
            {
                printf("ERROR!!! %s found, but no value!!!!\n", subItem);
                return false;
            }
        }
    }
    return false;
}

static char http_buffer[4096] = {0};
static int pos = 0;
static size_t aliyunWakeupCB(char *ptr, size_t size, size_t nmemb, void *userdata){
	size_t len = size * nmemb;  
	memcpy(http_buffer + pos, ptr, len);
	pos += len;
	printf("-->http_buffer: %s, len: %ld\n", http_buffer, len);
	
	cJSON* root = cJSON_Parse(http_buffer);
	cJSON* format = cJSON_GetObjectItem(root, "code");
	if(format == NULL || format->type == cJSON_NULL){
	    printf("No item [code] in json\n");
	} else {
		int valueInt = format->valueint;
		printf("JSON code: %d\n", valueInt);
		if(200 == valueInt){
			printf("http request response success -> 200\n");
			format = cJSON_GetObjectItem(root, "data");
			if(format == NULL || format->type == cJSON_NULL){
				printf("No item [data] in json\n");
			} else {
				printf("data: %s\n", cJSON_Print(format));
				format = cJSON_GetObjectItem(format, "property");
				if(format == NULL || format->type == cJSON_NULL){
					printf("No item [property] in json\n");
				}else{
					printf("property: %s\n", cJSON_Print(format));
					format = cJSON_GetObjectItem(format, "list");
					if(format == NULL || format->type == cJSON_NULL){
						printf("No item [list] in json\n");
					}else{
						printf("list: %s\n", cJSON_Print(format));
						int cntlist = cJSON_GetArraySize(format);
						printf("cntlist: %d\n", cntlist);
						cJSON* pSub;
						char jsonValue[JSON_VALUE_LENGTH];
						for(int i = 0; i < cntlist; i++){
							pSub = cJSON_GetArrayItem(format, i);
							if(NULL == pSub || cJSON_NULL == pSub->type){
								printf("pSub[%d] is NULL, or cJSON_NULL == pSub->type\n", i);
								continue;
							}
							if(parseJsonStr(pSub, "LowPowerState", jsonValue)){
								printf("LowPowerState: %d\n", atoi(jsonValue));
							}
						}
					}
				}
			}
		}
	}
	cJSON_Delete(root);

	return len;  
}

int main(int argc,char *argv[])
{
	CURL *curl;
	curl = curl_easy_init();
	if (!curl){
		fprintf(stderr,"curl init failed\n");
		return -1;
	}
 
    char* YunWakeupURL = "https://iotapi.dev.hongyanyun.cn/controlscreen/device/detail";  //用于测试阶段
    //char* YunWakeupURL = "https://iotapi.hongyanyun.cn";  //用于正式阶段
    char* YunWakeupFIELDS = "{\"homeId\":\"1574646939153145857\",\"productKey\":\"a164msJWX5c\",\"deviceName\":\"H000059200400029\",\"checkStatus\":0}";
    printf("YunWakeupURL:%s\n", YunWakeupURL);
	printf("YunWakeupFIELDS:%s\n", YunWakeupFIELDS);
	
	curl_easy_setopt(curl, CURLOPT_URL, YunWakeupURL); //url地址
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); //不检查ssl，可访问https
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);  //不检查ssl，可访问https
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, YunWakeupFIELDS); //post参数
	curl_easy_setopt(curl, CURLOPT_POST, 1); //设置问非0表示本次操作为post
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); //打印调试信息

	struct curl_slist *http_header = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
	curl_slist_append(http_header, "debug:1");  //用于测试阶段
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); //设置为非0,响应头信息location
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//接收数据时超时设置，如果10秒内数据未
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, aliyunWakeupCB);
	int cnt = 0;
	CURLcode res;
loop:
	pos = 0;
	res = curl_easy_perform(curl);
	if (res != CURLE_OK){
		fprintf(stderr,"-----------> 返回值:%d\n",res);
		if(cnt < 5){
			cnt ++;
			sleep(1);
			goto loop;
		}
	}else{
		printf("Success -----------> 返回值:%d\n",res);
	}

	curl_easy_cleanup(curl);
    //printf("end  limd -----------> \n");
    return 0;
}
