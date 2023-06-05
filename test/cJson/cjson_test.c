#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

//gcc -o cjson_test cJSON.c cjson_test.c

void test_fun(char *jsonStr)
{
	//char* strTmp = NULL;
	cJSON *json = NULL;
	//char string_1[] = "{\"id\":\"1591991830247907330\",\"thingType\":\"DEVICE\",\"gatewayIotId\":\"1630122201869127682\",\"deviceType\":\"0\",\"productKey\":\"a1p1DRaRpWZ\",\"deviceName\":\"0mqjm1PCm7Xv4QOHzwVZ\",\"iotId\":\"0mqjm1PCm7Xv4QOHzwVZ000000\",\"nickName\":\"IOT_单向红外幕帘AC款\",\"groupId\":\"\",\"homeName\":\"\",\"name\":\"SU9UX+WNleWQkee6ouWkluW5leW4mEFD5qy+\",\"icon\":\"a1p1DRaRpWZ.png\",\"online\":1,\"roomName\":\"全屋\",\"rooms\":[\"5YWo5bGL\"],\"homeId\":\"1627918385551712258\",\"childPkAndDns\":[]}";
	
	char string_1[] = "{\"id\":\"1591992396437004289\",\"thingType\":\"DEVICE\",\"gatewayIotId\":\"1630122201869127682\",\"deviceType\":\"0\",\"productKey\":\"a1ig0BA7NMC\",\"deviceName\":\"0IyNSTSi9WClWr622Ys3\",\"iotId\":\"0IyNSTSi9WClWr622Ys3000000\",\"nickName\":\"IOT_二路智能开关模块\",\"groupId\":\"\",\"homeName\":\"\",\"name\":\"SU9UX+S6jOi3r+aZuuiDveW8gOWFs+aooeWdlw==\",\"icon\":\"a1ig0BA7NMC.png\",\"online\":1,\"roomName\":\"全屋\",\"rooms\":[\"5YWo5bGL\"],\"homeId\":\"1627918385551712258\",\"childPkAndDns\":[{\"productKey\":\"a1Nk7FcKSrR\",\"deviceName\":\"test3w_xuni_20220919_146\"},{\"productKey\":\"a1Nk7FcKSrR\",\"deviceName\":\"test3w_xuni_20220919_147\"}]}";
	printf("string_1:%s\n", string_1);
	
	json = cJSON_Parse(string_1);
	if (json == NULL)
	{
		printf("json NULL\n");
		return;
	}

    /*cJSON *array = cJSON_GetObjectItem(json, "code");
	if(array == cJSON_NULL || array == NULL) //判断类型
	{
		printf("array: cJSON_NULL or NULL\n");
	}*/
	cJSON* format = cJSON_GetObjectItem(json, "id");
    if(format == NULL || format->type == cJSON_NULL)
    {
        printf("No item [id] in json\n");
    }
    else
    {
        char* str = format->valuestring;
        //if(codeInt != 200)
	    {
			printf("id: %s\n", str);
		}
    }
	
	cJSON* item = NULL, * productKey = NULL, * deviceName = NULL;
	cJSON *root = NULL;
    cJSON *array = cJSON_GetObjectItem(json, "childPkAndDns");
	if(array->type == cJSON_NULL || array == NULL) //判断类型
	{
		printf("array: cJSON_NULL or NULL\n");
	}
	else if(array->type == cJSON_Array)
	{
		int sceneCntInList = cJSON_GetArraySize(array);
		if(sceneCntInList == 0)
		{
			printf("(全屋API)从服务器获取 0 个场景, 返回\n");
			//return 0;
		}
		else
		{
			printf("(全屋API)从服务器获取 %d 个场景, sizeof(array): %d\n", sceneCntInList, sizeof(array));
			//root = cJSON_Duplicate(array, 1);
			item = cJSON_GetArrayItem(array, 0);
            productKey = cJSON_GetObjectItem(item, "productKey");
			deviceName = cJSON_GetObjectItem(item, "deviceName");
			printf("productKey: %s, deviceName: %s\n", productKey->valuestring, deviceName->valuestring);
			
			item = cJSON_GetArrayItem(array, 1);
            productKey = cJSON_GetObjectItem(item, "productKey");
			deviceName = cJSON_GetObjectItem(item, "deviceName");
			printf("productKey: %s, deviceName: %s\n", productKey->valuestring, deviceName->valuestring);
		}
	}

	//char* strTmp = cJSON_PrintUnformatted(root);
   // printf("strlen(strTmp): %d--------------- \n%s\n", strlen(strTmp), strTmp);
}

int main(void)
{
    cJSON* cjson_test = NULL;
    cJSON* cjson_address = NULL;
    cJSON* cjson_skill = NULL;
    char* str = NULL;

    /* 创建一个JSON数据对象(链表头结点) */
    cjson_test = cJSON_CreateObject();

    /* 添加一条字符串类型的JSON数据(添加一个链表节点) */
    cJSON_AddStringToObject(cjson_test, "name", "mculover666");

    /* 添加一条整数类型的JSON数据(添加一个链表节点) */
    cJSON_AddNumberToObject(cjson_test, "age", 22);

    /* 添加一条浮点类型的JSON数据(添加一个链表节点) */
    cJSON_AddNumberToObject(cjson_test, "weight", 55.5);

    /* 添加一个嵌套的JSON数据（添加一个链表节点） */
    cjson_address = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_address, "country", "China");
    cJSON_AddNumberToObject(cjson_address, "zip-code", 111111);
    cJSON_AddItemToObject(cjson_test, "address", cjson_address);

    /* 添加一个数组类型的JSON数据(添加一个链表节点) */
    cjson_skill = cJSON_CreateArray();
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString( "C" ));
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString( "Java" ));
    cJSON_AddItemToArray(cjson_skill, cJSON_CreateString( "Python" ));
    cJSON_AddItemToObject(cjson_test, "skill", cjson_skill);

    /* 添加一个值为 False 的布尔类型的JSON数据(添加一个链表节点) */
    cJSON_AddFalseToObject(cjson_test, "student");

    /* 打印JSON对象(整条链表)的所有数据 */
    str = cJSON_Print(cjson_test);
    printf("%s\n", str);
	
	char *TTS = cJSON_GetObjectItem(cjson_test, "name")->valuestring;
	printf("TTS: %s\n", TTS);
	
	cJSON *json_tmp = cJSON_GetObjectItem(cjson_test ,"student");
    printf("line: 49\n");
	if(json_tmp == NULL)
	{
	    printf("json_tmp == NULL\n");
	}
	else if(json_tmp->type == cJSON_NULL)
	{
	    printf("json_tmp->type == cJSON_NULL\n");
	}
	else if(json_tmp->type == cJSON_Invalid)
	{
	    printf("json_tmp->type == cJSON_Invalid\n");
	}
	else
	//if(json_tmp != NULL || json_tmp->type != cJSON_NULL) //Crash
	{
		printf("line: 65\n");
		char *tmp_num_chs = json_tmp->valuestring;
		printf("tmp_num_chs: %s\n", tmp_num_chs);
	}
	printf("line: 69\n");
	//cJSON_Delete(cjson_address);
	str = cJSON_Print(cjson_address);
    printf("After cJSON_Delete: %s\n", str);
	
	
	
	cjson_address = cJSON_GetObjectItem(cjson_test, "name");
	char *TTS_str = cjson_address->valuestring;
	printf("TTS_str: %s\n", TTS_str);
	printf("---------------------------------------------------------------------\n");
	
	
	char string[] = "{\"test\":null}";
	printf("string:%s\n", string);
	
	cJSON *json = cJSON_Parse(string);
	if (json == NULL)
	{
		printf("json NULL\n");
		return -1;
	}
 
	cJSON *test =  cJSON_GetObjectItem(json, "test");
	if (test->type == cJSON_NULL) //判断类型
	{
		printf("type : cJSON_NULL\n");
	}
	// printf("%d\n", test->valueint); //打印 0
	// printf("%s\n", test->valuestring); //段错误
	// printf("%ld\n", strlen(test->valuestring)); //段错误
	
	char *jsonStr = NULL;
	test_fun(jsonStr);
    printf(">>>>>>>>>>>>>>>>>>>>%s\n", jsonStr);
	
	/*cJSON *test =  cJSON_GetObjectItem(json, "test");
	if (test->type == cJSON_NULL) //判断类型
	{
		printf("type : cJSON_NULL\n");
	}*/
	
	//printf("cJSON_NULL: %d, cJSON_Number: %d, cJSON_String: %d, cJSON_Array: %d\n", cJSON_NULL, cJSON_Number, cJSON_String, cJSON_Array);
	
    return 0;
}

