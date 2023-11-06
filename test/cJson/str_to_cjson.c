#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

//(char(*p3Arr))[2][64] = NULL;
char* json1 = "{ \"json\" : { \"id\":1, \"nodeId\":11, \"deviceId\":111, \"deviceName\":\"aaa\", \"ieee\":\"01212\", \"ep\":\"1111\", \"type\":\"bbb\" }}";
    char* json2 = "{\"id\":[1,2,3,4,5], \"nodeId\":11, \"deviceId\":111, \"deviceName\":\"aaa\"}";
	char* json3 = "{\"topic\":\"/LAPI/V1.0/System/LowPowerState\",\"data\":{\"DeviceName\": \"H000059200400029\",\"ProductKey\": \"a164msJWX5c\",\"State\": 0}}";
	char* json4 = "{\"success\":true,\"code\":200,\"message\":\"请求成功\",\"localizedMsg\":\"请求成功\"}";
	char* json5 = "{\"LowPowerState\":{\"time\":1667556963260,\"value\":1}}";
	char* json6 = "{\"data\":{\"response\":[\"BJKsXmTpX2jw==\"]},\"code\":\"SUCCESS\",\"msg\":\"成功\"}";
	char* json7 = "{\"success\":true,\"code\":200,\"message\":\"请求成功\",\"localizedMsg\":\"请求成功\",\"data\":[{\"userId\":\"1555436203659993089\",\"deviceName\":\"000D6F00175D15C0\",\"productKey\":\"a1CKFOuptQq\",\"nickName\":\"10A 20\",\"thingType\":\"DEVICE\",\"parentIotId\":\"yqoE8teeOlfR0PuY9Wf8000000\",\"privateData\":\"\",\"phone\":\"18658827506\",\"roomName\":\"\",\"homeId\":\"1555436203659993089\",\"homeName\":\"18658827506的家\",\"parentDeviceName\":\"H000057509200342\",\"parentProductKey\":\"a1tNlnarBMz\",\"deviceType\":0,\"gatewayIotId\":\"\"},{\"userId\":\"1555436203659993089\",\"deviceName\":\"b0fd2482eff543418a439641ad978d2a\",\"productKey\":\"a1Sim1SWGvA\",\"nickName\":\"组播5\",\"thingType\":\"VIRTUAL\",\"parentIotId\":\"\",\"privateData\":\"\",\"phone\":\"18658827506\",\"roomName\":\"\",\"homeId\":\"1555436203659993089\",\"homeName\":\"18658827506的家\",\"parentDeviceName\":\"\",\"parentProductKey\":\"\",\"deviceType\":2,\"gatewayIotId\":\"yqoE8teeOlfR0PuY9Wf8000000\"},{\"userId\":\"1555436203659993089\",\"deviceName\":\"d35b4875eafa49acbb23b413a5f110c8\",\"productKey\":\"a1Sim1SWGvA\",\"nickName\":\"组播11\",\"thingType\":\"VIRTUAL\",\"parentIotId\":\"\",\"privateData\":\"\",\"phone\":\"18658827506\",\"roomName\":\"\",\"homeId\":\"1555436203659993089\",\"homeName\":\"18658827506的家\",\"parentDeviceName\":\"\",\"parentProductKey\":\"\",\"deviceType\":2,\"gatewayIotId\":\"yqoE8teeOlfR0PuY9Wf8000000\"},{\"userId\":\"1555436203659993089\",\"deviceName\":\"ee9b77373012439c9f949bbfbf5fd614\",\"productKey\":\"a1Sim1SWGvA\",\"nickName\":\"组播8\",\"thingType\":\"VIRTUAL\",\"parentIotId\":\"\",\"privateData\":\"\",\"phone\":\"18658827506\",\"roomName\":\"\",\"homeId\":\"1555436203659993089\",\"homeName\":\"18658827506的家\",\"parentDeviceName\":\"\",\"parentProductKey\":\"\",\"deviceType\":2,\"gatewayIotId\":\"yqoE8teeOlfR0PuY9Wf8000000\"}]}";
	char* json8 = "{\"items\": [         {             \"gunid\": 1,             \"type\": 3,             \"volmax\": \"222\",             \"volmin\": \"222\",             \"curmax\": \"22\",             \"tempmax\": 65535,             \"meterenable\": 255,             \"jueyuanenbale\": 255,             \"plcenable\": 255,             \"gonglvmax\": \"22\",             \"stafenable\": 255,             \"waittime\": 255,             \"retrycnt\": 255,             \"jiayuchongenable\": 0,             \"guntemppolicytype\": 0,             \"gunsafepower\": 0,             \"coolerenable\": \"0\"         },         {             \"gunid\": 2,             \"type\": 255,             \"volmax\": 65535,             \"volmin\": 65535,             \"curmax\": 65535,             \"tempmax\": 65535,             \"meterenable\": 255,             \"jueyuanenbale\": 255,             \"plcenable\": 255,             \"gonglvmax\": 65535,             \"stafenable\": 255,             \"waittime\": 255,             \"retrycnt\": 255         }     ],     \"action\": \"setgun\",     \"token\": \"287477BC059B63038DA83EA980FD5D56A85367F4ADAA929DE12E2AAC6823FDD6546889381126BBC694A3459C208E9857\" }";

void parseJson(char* all[][2][64])
{
    cJSON* root;
    int value_int;

	printf("-------------------->> json7: %s\n", json7);
	root = cJSON_Parse(json7);
	cJSON* data_Json = cJSON_GetObjectItem(root, "data");//获取数组的根节点
	cJSON *array_sub, *node_id_json;
	value_int = cJSON_GetArraySize(data_Json);//获取数组长度
    printf("数组长度为:%d\n", value_int);
	//char(*all)[2][64] = (char*)malloc(value_int * 2 * 64);
	all = (char*)malloc(value_int * 2 * 64);
    for (int i = 0; i < value_int; i++) {//循环打印数组的内容
		array_sub = cJSON_GetArrayItem(data_Json,i);
		if(array_sub == NULL)
		{	
			continue;	
		}
		if((node_id_json= cJSON_GetObjectItem(array_sub,"deviceName")) != NULL)
		{
			strcpy(all[i][0], node_id_json->valuestring);
			printf("deviceName: %s\t", node_id_json->valuestring);
		}
		if((node_id_json= cJSON_GetObjectItem(array_sub,"productKey")) != NULL)
		{
			strcpy(all[i][1], node_id_json->valuestring);
			printf("productKey: %s\n", node_id_json->valuestring);
		}
    }
	/*printf("-----------------------------\n");
	for (int i = 0; i < value_int; i++)
	{
		printf("deviceName: %s\t", all[i][0]);
		printf("productKey: %s\n", all[i][1]);
	}
	free(all);*/
	
    cJSON_Delete(root);

    //return all;
}


int main() {
    
    cJSON* root;
    cJSON* format;
    cJSON* array;
    int value_int;
    char* value_string;

    //解析json1, 通过cJSON_Parse解析接收到的字符串，再通过cJSON_GetObjectItem获取指定键的值，最后释放该JSON结点的内存
    root = cJSON_Parse(json1);
    format = cJSON_GetObjectItem(root, "json");
    value_int = cJSON_GetObjectItem(format, "nodeId")->valueint;
    value_string = cJSON_GetObjectItem(format, "ieee")->valuestring;
    printf("%d\n", value_int);
    printf("%s\n", value_string);
    cJSON_Delete(root);

    //解析json2, 添加了数组解析
    root = cJSON_Parse(json2);
    //数组解析
    array = cJSON_GetObjectItem(root, "id");//获取数组的根节点
    value_int = cJSON_GetArraySize(array);//获取数组长度
    printf("数组长度为:%d\n", value_int);
    //循环打印数组的内容
    for (int i = 0; i < value_int; i++) {
        value_int = cJSON_GetArrayItem(array, i)->valueint;//获取数组下标为i的数据，以int形式解析
        printf("第%d个元素为：%d\n",i+1,value_int);
    }
    value_string = cJSON_GetObjectItem(root, "deviceName")->valuestring;
    printf("%s\n", value_string);
	
	//解析json3，包括嵌套JSON
	printf("json3: %s\n", json3);
	root = cJSON_Parse(json3);
	char* topic_str = cJSON_GetObjectItem(root, "topic")->valuestring;
	printf("topic_str: %s\n", topic_str);
	cJSON* data_json = cJSON_GetObjectItem(root, "data");//获取数组的根节点
	char* value_str = cJSON_GetObjectItem(data_json, "DeviceName")->valuestring;
	char* ProductKey_str = cJSON_GetObjectItem(data_json, "ProductKey")->valuestring;
	int State_int = cJSON_GetObjectItem(data_json, "State")->valueint;
	printf("value_str: %s\n", value_str);
	printf("ProductKey_str: %s\n", ProductKey_str);
	printf("State_int：%d\n", State_int);
	
	//解析json4, 获取bool值
	printf("json4: %s\n", json4);
	root = cJSON_Parse(json4);
	char* success = cJSON_GetObjectItem(root, "success")->valuestring;
	printf("success: %s\n", success);
	
	if(cJSON_IsTrue(cJSON_GetObjectItem(root, "success"))) {
		printf("====================\n");
	} else {
		printf("--------------------\n");
    }
	
	root = cJSON_Parse(json5);
    format = cJSON_GetObjectItem(root, "LowPowerState");
	if(format == NULL || format->type == cJSON_NULL){
	    printf("No item [LowPowerState] in json");
	} else {
		cJSON* tmpJson = cJSON_GetObjectItem(format, "value");
		if(tmpJson == NULL || tmpJson->type != cJSON_Number){
			printf("No item [value] in json or format->type is NOT cJSON_Number");
		} else {
			int value_int = tmpJson->valueint;
			printf("json5: value: %d, ", value_int);
		}
			
		/*value_int = cJSON_GetObjectItem(format, "value")->valueint;
		printf("json5: value: %d, ", value_int);
		value_int = cJSON_GetObjectItem(format, "time")->valueint;
		printf("time: %d\n", value_int);*/
	}
	
	printf(">>>>>>> json6: %s\n", json6);
    root = cJSON_Parse(json6);
	format = cJSON_GetObjectItem(root, "code");
	if(format == NULL || format->type == cJSON_NULL){
	    printf("No item [code] in json\n");
	} else {
		char* valuestring = format->valuestring;
		printf("json6: code: %s\n", valuestring);
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
					printf("json6: response: %s\n", valuestring);
				}
			}
		} else {
			printf("zkpKey, get Max_Lock Unlock key error!!!\n");
		}
	}

    //解析json8，包括嵌套JSON
	printf("-------------------->> json8: %s\n", json8);
	root = cJSON_Parse(json8);
	cJSON* data_Json = cJSON_GetObjectItem(root, "items");//获取数组的根节点
	cJSON *array_sub, *node_id_json;
	value_int = cJSON_GetArraySize(data_Json);//获取数组长度
    printf("数组长度为:%d\n", value_int);
    for (int i = 0; i < value_int; i++) {//循环打印数组的内容
		array_sub = cJSON_GetArrayItem(data_Json, i);
		if(array_sub == NULL)
		{	
			continue;	
		}
		if((node_id_json= cJSON_GetObjectItem(array_sub,"gunid")) != NULL)
		{
			printf("gunid: %d\t", node_id_json->valueint);
		}
		if((node_id_json= cJSON_GetObjectItem(array_sub,"volmin")) != NULL)
		{
			printf("volmin: %s\n", node_id_json->valuestring);
		}
    }
	printf("-----------------------------\n");
	
	/*char(*p)[2][64] = NULL;
	parseJson(p);
	printf("-----------------------------\n");
	if(p == NULL)
	{
		printf("--NULL---NULL----NULL----------NULL----------\n");
	}
	for (int i = 0; i < 4; i++)
	{
		//printf("deviceName: %s\t", p[i][0]);
		//printf("productKey: %s\n", p[i][1]);
	}
	free(p);*/
	
    cJSON_Delete(root);

    return 0;
}

