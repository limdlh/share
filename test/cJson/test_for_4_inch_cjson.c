#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cJSON.h"

#define JSON_VALUE_LENGTH 32

/*
{
	"StreamVideoQuality":   {
			"time": 1670398484425,
			"value":        0
	},	"MicSwitch":    {
			"time": 1670398484425,
			"value":        1
	},	"EncryptSwitch":        {
			"time": 1670398484425,
			"value":        1
	},	"LocalIP":      {
			"time": 1670398484425,
			"value":        "192.168.124.5"
	},	"WifiVersion":  {
			"time": 1670398484425,
			"value":        "DFDM-2105.1.2.221111-TEST03"
	},	"ImageFlipState":       {
			"time": 1670398484425,
			"value":        0
	},	"SubStreamVideoQuality":        {
			"time": 1670398484425,
			"value":        0
	},	"LowPowerState":        {
			"time": 1670398484425,
			"value":        0
	},	"AlarmSwitch":  {
			"time": 1670398484425,
			"value":        1
	}, "batpt":  {
                "time": 1670399008219,
                "value":        45
    }, "MicSwitch":    {
			"time": 1670398484425,
			"value":        1
	}
}
*/

bool parseDualJsonInt(cJSON* pSubJson, char* subItem, int* jsonValInt){
    cJSON* tmpJson = cJSON_GetObjectItem(pSubJson, subItem);
    if(tmpJson == NULL || tmpJson->type == cJSON_NULL){
		printf("No item [%s] in json\n", subItem);
    } else {
        cJSON* format = cJSON_GetObjectItem(tmpJson, "value");
        if(format == NULL || format->type != cJSON_Number){
            printf("No item [value] in json or format->type is NOT cJSON_Number");
        }else {
			*jsonValInt = format->valueint;
        }
    }
}

bool parseDualJsonStr(cJSON* pSubJson, char* subItem, char jsonVal[]){
    cJSON* tmpJson = cJSON_GetObjectItem(pSubJson, subItem);
    if(tmpJson == NULL || tmpJson->type == cJSON_NULL){
		printf("No item [%s] in json\n", subItem);
    } else {
        cJSON* format = cJSON_GetObjectItem(tmpJson, "value");
		if(format) {
			char* value_str = format->valuestring;
			//printf("subItem: %s, value: %s\n", subItem, value_str);
			int len = strlen(value_str);
			if(len >= JSON_VALUE_LENGTH)
			{
				printf("Value: %s, Length: %d, too large than %d!!! ERROR !!!\n", value_str, len, JSON_VALUE_LENGTH);
				return false;
			}
			memset(jsonVal, 0, JSON_VALUE_LENGTH);
			strcpy(jsonVal, value_str);
        }
    }
}

int main() {
	char* json_str = "{\"StreamVideoQuality\":{\"time\":1670398484425,\"value\":0},\"MicSwitch\":{\"time\":1670398484425,\"value\":1},\"EncryptSwitch\":{\"time\":1670398484425,\"value\":1},\"LocalIP\":{\"time\":1670398484425,\"value\":\"192.168.124.5\"},\"WifiVersion\":{\"time\":1670398484425,\"value\":\"DFDM-2105.1.2.221111-TEST03\"},\"ImageFlipState\":{\"time\":1670398484425,\"value\":0},\"SubStreamVideoQuality\":{\"time\":1670398484425,\"value\":0},\"LowPowerState\":{\"time\":1670398484425,\"value\":0},\"AlarmSwitch\":{\"time\":1670398484425,\"value\":1},\"batpt\":{\"time\":1670399008219,\"value\":45},\"MicSwitch\":{\"time\":1670398484425,\"value\":1}}";
	
    cJSON* root;
	int valueInt = 0;
	char jsonValue[JSON_VALUE_LENGTH];

	printf("json_str: %s\n", json_str);
    root = cJSON_Parse(json_str);
	int cntlist = cJSON_GetArraySize(root);
	printf("cntlist: %d\n", cntlist);
		
	if(parseDualJsonStr(root, "LocalIP", jsonValue))
	{
		printf("LocalIP: %s\n", jsonValue);
	}
	
	if(parseDualJsonInt(root, "LowPowerState", &valueInt))
	{
		printf("LowPowerState: %d\n", valueInt);
	}
	
	if(parseDualJsonInt(root, "batpt", &valueInt))
	{
		printf("batpt: %d\n", valueInt);
	}
	if(parseDualJsonInt(root, "MicSwitch", &valueInt))
	{
		printf("MicSwitch: %d\n", valueInt);
	}
	if(parseDualJsonStr(root, "LocasslIP", jsonValue))
	{
		printf("LocalIP: %s\n", jsonValue);
	}
	
	if(parseDualJsonInt(root, "LowPosswerState", &valueInt))
	{
		printf("LowPowerState: %d\n", valueInt);
	}
	
    cJSON_Delete(root);

    return 0;
}