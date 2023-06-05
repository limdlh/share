#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cJSON.h"

#define JSON_VALUE_LENGTH 32

/*
[{
		"name": "5Li754Gv5byA5YWz",
		"value":        "1",
		"time": "1670392942239",
		"dataType":     "bool",
		"identifier":   "LightSwitch",
		"unit": "",
		"tag":  ["照明", "电灯", "筒灯"]
}, {
		"name": "5piO5pqX5bqm",
		"value":        "50",
		"time": "1670392942241",
		"dataType":     "int",
		"identifier":   "Brightness",
		"unit": "%",
		"tag":  ["照明", "电灯", "筒灯"]
}, {
		"name": "5Ya35pqW6Imy5rip",
		"value":        "2700",
		"time": "1670392942246",
		"dataType":     "int",
		"identifier":   "ColorTemperature",
		"unit": "K",
		"tag":  ["照明", "电灯", "筒灯"]
}, {
		"name": "57yT54Gt5pe26Ze0",
		"value":        "70",
		"time": "1669959098402",
		"dataType":     "int",
		"identifier":   "CloseTime",
		"tag":  ["照明", "电灯", "筒灯"]
}, {
		"name": "6YCa55S15Yu/5omw5byA5YWz",
		"value":        "0",
		"time": "1669959098407",
		"dataType":     "bool",
		"identifier":   "DisturbSwitch",
		"unit": "",
		"tag":  ["照明", "电灯", "筒灯"]
}, {
		"name": "57yT5ZCv5pe26Ze0",
		"value":        "70",
		"time": "1669959098388",
		"dataType":     "int",
		"identifier":   "OpenTime",
		"tag":  ["照明", "电灯", "筒灯"]
}, {
		"name": "572R57uc54q25oCB",
		"dataType":     "enum",
		"identifier":   "Status",
		"unit": "",
		"tag":  ["照明", "电灯", "筒灯"]
}]
*/

bool parseJsonStr(cJSON* pSubJson, char* subItem, char jsonVal[]){
    if(NULL == pSubJson || NULL == subItem || NULL == jsonVal){
        printf("NULL == pSubJson or NULL == subItem or NULL == jsonVal!!!ERROR!!");
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
				//printf("identifier: %s, value: %s\n", ident_str, value_str);
				int len = strlen(value_str);
				if(len >= JSON_VALUE_LENGTH)
				{
					printf("Value: %s, Length: %d, too large than %d!!! ERROR !!!\n", value_str, len, JSON_VALUE_LENGTH);
					return false;
				}
				memset(jsonVal, 0, JSON_VALUE_LENGTH);
				strcpy(jsonVal, value_str);
				return true;
			}
			else
			{
				printf("ERROR!!! %s found, but no value!!!!\t", subItem);
				return false;
			}
		}
	}
	return false;
}

bool parseJsonInt(cJSON* pSubJson, char* subItem, int* jsonValInt){
    if(NULL == pSubJson || NULL == subItem){
        printf("NULL == pSubJson or NULL == subItem !!!ERROR!!");
		return false;
	}
	
	cJSON* identifier = cJSON_GetObjectItem(pSubJson, "identifier");
	if(identifier) {
		char* ident_str = identifier->valuestring;
		if(!strcmp(ident_str, subItem))
		{
			cJSON* value = cJSON_GetObjectItem(pSubJson, "value");
			if(value && value->type == cJSON_Number) {
				*jsonValInt = value->valueint;
				printf("identifier: %s, jsonValInt: %d, value->valueint: %d\n", ident_str, *jsonValInt, value->valueint);
				return true;
			}
			else
			{
				printf("ERROR!!! %s found, but no value!!!!\t", subItem);
				return false;
			}
		}
	}
	return false;
}
		
int main() {
	char* json_str = "[{\"name\":\"5pyJ5pWI55S15rWB\",\"value\":\"0.0\",\"time\":\"1666838290967\",\"dataType\":\"float\",\"identifier\":\"RMSCurrent\",\"unit\":\"A\",\"tag\":[\"插座\"]},{\"name\":\"56ul6ZSB5byA5YWz\",\"value\":\"0\",\"time\":\"1666838290974\",\"dataType\":\"bool\",\"identifier\":\"ChildLockSwitch\",\"unit\":\"\",\"tag\":[\"插座\"]},{\"name\":\"6IOM5YWJ54Gv5qih5byP\",\"value\":\"1\",\"time\":\"1666838290974\",\"dataType\":\"enum\",\"identifier\":\"BackLightMode\",\"unit\":\"\",\"tag\":[\"插座\"]},{\"name\":\"6L+H5Y6L5L+d5oqk\",\"value\":\"0\",\"time\":\"1666838290966\",\"dataType\":\"bool\",\"identifier\":\"OverVoltOnOff\",\"unit\":\"\",\"tag\":[\"插座\"]},{\"name\":\"5pyJ5pWI55S15Y6L\",\"value\":\"0.0\",\"time\":\"1666838290924\",\"dataType\":\"float\",\"identifier\":\"RMSVoltage\",\"unit\":\"V\",\"tag\":[\"插座\"]},{\"name\":\"6L+H5rWB5L+d5oqk\",\"value\":\"0\",\"time\":\"1666838291583\",\"dataType\":\"bool\",\"identifier\":\"OverCurOnOff\",\"unit\":\"\",\"tag\":[\"插座\"]},{\"name\":\"5oC76ICX55S16YeP\",\"value\":\"0.0\",\"time\":\"1666849112086\",\"dataType\":\"float\",\"identifier\":\"TotalConsumption\",\"unit\":\"kW·h\",\"tag\":[\"插座\"]},{\"name\":\"5pat55S16K6w5b+G\",\"value\":\"0\",\"time\":\"1666838290967\",\"dataType\":\"bool\",\"identifier\":\"FailureProtectionSwitch\",\"unit\":\"\",\"tag\":[\"插座\"]},{\"name\":\"55S15rqQ5byA5YWz\",\"value\":\"1\",\"time\":\"1666838291360\",\"dataType\":\"bool\",\"identifier\":\"PowerSwitch\",\"unit\":\"\",\"tag\":[\"插座\"]},{\"name\":\"6K6+5aSH5Zu65Lu254mI5pys\",\"value\":\"20200706\",\"time\":\"1666589147656\",\"dataType\":\"text\",\"identifier\":\"FirmwareVersion\",\"tag\":[\"插座\"]},{\"name\":\"55S16YeP5riF6Zu2\",\"dataType\":\"enum\",\"identifier\":\"ResetZero\",\"unit\":\"\",\"tag\":[\"插座\"]}]";
	
    cJSON* root;
	cJSON* pSub;
    
	int iCnt = 0;
	//int valueInt = 0;
	char jsonValue[JSON_VALUE_LENGTH];

	printf("json_str: %s\n", json_str);
    root = cJSON_Parse(json_str);
	int cntlist = cJSON_GetArraySize(root);
	printf("cntlist: %d\n", cntlist);
	
	for(iCnt = 0; iCnt < cntlist; iCnt ++ ){
		pSub = cJSON_GetArrayItem(root, iCnt);
		if(NULL == pSub)
		{
			printf("pSub[%d] it NULL\n", iCnt);
			continue ;
		}
		
		/*if(parseMaxLockJsonInt(pSub, "LowPowerState", &valueInt))
		{
			printf("LowPowerState: %d\n", valueInt);
		}*/
		
		if(parseJsonStr(pSub, "PowerSwitch", jsonValue))
		{
			printf("PowerSwitch: %s\n", jsonValue);
			int valueInt = atoi(jsonValue);
			printf("valueInt: %d\n", valueInt);
			//(*value)->PowerSwitch = valueInt;
		}
		else if(parseJsonStr(pSub, "RMSVoltage", jsonValue))
		{
			printf("RMSVoltage: %s\n", jsonValue);
			double valueFloat = atof(jsonValue);
			printf("valueFloat: %.1lf\n", valueFloat);
			//(*value)->ChildLockSwitch = valueFloat;
		}
		else if(parseJsonStr(pSub, "TotalConsumption", jsonValue))
		{
			printf("TotalConsumption: %s\n", jsonValue);
			double valueFloat = atof(jsonValue);
			printf("valueFloat: %.1lf\n", valueFloat);
			//(*value)->ChildLockSwitch = valueFloat;
		}
		else if(parseJsonStr(pSub, "ChildLockSwitch", jsonValue))
		{
			printf("ChildLockSwitch: %s\n", jsonValue);
			int valueInt = atoi(jsonValue);
			printf("valueInt: %d\n", valueInt);
			//(*value)->ChildLockSwitch = valueInt;
		}
	}
	
	char test[10] = "-";
	printf("test: %s\n", test);
	if(!strcmp(test, "+"))
	{
		printf("+++++test: %s\n", test);
	}
	if(!strcmp(test, "-"))
	{
		printf("-----test: %s\n", test);
	}
				
    cJSON_Delete(root);

    return 0;
}