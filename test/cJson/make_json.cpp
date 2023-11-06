/*
{
	"int_test": 22,
	"int":  55,
	"skill":[{
					"country":      "US",
					"zip-code":     222
			}, {
					"country":      "China",
					"zip-code":     111111
			}]
}\
*/

#include <stdio.h>
#include "cJSON.h"
int main(void)
{
    cJSON* cjson_test = NULL;
    cJSON* cjson_address = NULL;
    cJSON* cjson_skill = NULL;
    char* str = NULL;

    /* 创建一个JSON数据对象(链表头结点) */
    cjson_test = cJSON_CreateObject();

    /* 添加一条整数类型的JSON数据(添加一个链表节点) */
    cJSON_AddNumberToObject(cjson_test, "int_test", 22);

    /* 添加一条浮点类型的JSON数据(添加一个链表节点) */
    cJSON_AddNumberToObject(cjson_test, "int", 55);

    /* 添加一个嵌套的JSON对象（添加一个链表节点） 
    cjson_address = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_address, "country", "China");
    cJSON_AddNumberToObject(cjson_address, "zip-code", 111111);
    cJSON_AddItemToObject(cjson_test, "address", cjson_address); */

    /* 添加一个数组类型的JSON数据(添加一个链表节点) */
    cjson_skill = cJSON_CreateArray();
	cjson_address = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_address, "country", "US");
    cJSON_AddNumberToObject(cjson_address, "zip-code", 222);
	cJSON_AddItemToObject(cjson_skill, "", cjson_address);
	
	cjson_address = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_address, "country", "China");
    cJSON_AddNumberToObject(cjson_address, "zip-code", 111111);
	cJSON_AddItemToObject(cjson_skill, "", cjson_address);
	//cJSON_AddItemToObject(cjson_skill, "address", cjson_address);
	
    cJSON_AddItemToObject(cjson_test, "skill", cjson_skill);

    /* 打印JSON对象(整条链表)的所有数据 */
    str = cJSON_Print(cjson_test);
    printf("%s\n", str);

    return 0;
}