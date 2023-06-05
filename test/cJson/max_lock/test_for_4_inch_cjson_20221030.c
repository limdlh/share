#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cJSON.h"

#define JSON_VALUE_LENGTH 32

bool parseMaxLockJsonStr(cJSON* pSubJson, char* subItem, char jsonVal[]){
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

bool parseMaxLockJsonInt(cJSON* pSubJson, char* subItem, int* jsonValInt){
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
	char* json_str = "[{\"name\":\"55S76Z2i57+76L2s54q25oCB\",\"value\":\"0\",\"time\":\"1666859385017\",\"dataType\":\"enum\",\"identifier\":\"ImageFlipState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5Yqg5a+G57G75Z6L5YiX6KGo\",\"value\":\"[0]\",\"time\":\"1666859385030\",\"dataType\":\"array\",\"identifier\":\"EncryptTypeList\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5qih57uE56uv5L2O5Yqf6ICX5b6F5py655qE54q25oCB\",\"value\":\"1\",\"time\":\"1666859405319\",\"dataType\":\"bool\",\"identifier\":\"LowPowerState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"V2lmaeeJiOacrA==\",\"value\":\"DFDM-2105.1.2.221027-udp\",\"time\":\"1666859385017\",\"dataType\":\"text\",\"identifier\":\"WifiVersion\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6KeG6aKR5Yqg5a+G5byA5YWz\",\"value\":\"1\",\"time\":\"1666859385017\",\"dataType\":\"bool\",\"identifier\":\"EncryptSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6L6F56CB5rWB6KeG6aKR6LSo6YeP\",\"value\":\"0\",\"time\":\"1666859385017\",\"dataType\":\"enum\",\"identifier\":\"SubStreamVideoQuality\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5oql6K2m5byA5YWz\",\"value\":\"1\",\"time\":\"1666859385017\",\"dataType\":\"bool\",\"identifier\":\"AlarmSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6bqm5YWL6aOO5byA5YWz\",\"value\":\"1\",\"time\":\"1666859385017\",\"dataType\":\"bool\",\"identifier\":\"MicSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5Li756CB5rWB6KeG6aKR6LSo6YeP\",\"value\":\"0\",\"time\":\"1666859385017\",\"dataType\":\"enum\",\"identifier\":\"StreamVideoQuality\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6Zeo54q25oCB\",\"value\":\"68\",\"time\":\"1666858452586\",\"dataType\":\"enum\",\"identifier\":\"DoorState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6Ziy5pKs5qOA5rWL\",\"value\":\"1\",\"time\":\"1666772967272\",\"dataType\":\"bool\",\"identifier\":\"AntiPicking\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"55S15rGg55S16YeP\",\"value\":\"93\",\"time\":\"1666869236253\",\"dataType\":\"int\",\"identifier\":\"batpt\",\"unit\":\"%\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6ZSB54q25oCB\",\"value\":\"0\",\"time\":\"1666606542980\",\"dataType\":\"enum\",\"identifier\":\"LockerState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5pys5Zyw5LqL5Lu25o6o6YCBSVA=\",\"value\":\"192.168.8.154\",\"time\":\"1666607989821\",\"dataType\":\"text\",\"identifier\":\"LocalEventIP\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"54yr55y855qE5pys5ZywSVDlkozmnKzlnLDmtYHnq6/lj6M=\",\"value\":\"192.168.8.153\",\"time\":\"1666859385017\",\"dataType\":\"text\",\"identifier\":\"LocalIP\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6ZSB5o6n54mI5pys\",\"dataType\":\"text\",\"identifier\":\"LockerVersion\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"56e75Yqo5L6m5rWL6K6+572u\",\"dataType\":\"struct\",\"identifier\":\"MotionDetect\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5LyR55yg6K6+572u\",\"dataType\":\"struct\",\"identifier\":\"Dormant\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6JOd54mZ5bm/5pKt5bi45byA\",\"dataType\":\"bool\",\"identifier\":\"BLEBoardcast\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6K+t6KiA6K6+572u\",\"dataType\":\"enum\",\"identifier\":\"Language\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5pys5Zyw6K6+572u5Yqf6IO9\",\"dataType\":\"bool\",\"identifier\":\"LocalMode\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"d2lmaei/nuaOpeaooeW8jw==\",\"dataType\":\"enum\",\"identifier\":\"LinkMode\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"55S15a2Q5Y+N6ZSB\",\"dataType\":\"bool\",\"identifier\":\"DigitalLocked\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5oql6K2m6aKR546H\",\"dataType\":\"enum\",\"identifier\":\"AlarmFrequencyLevel\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5a+86Iiq6Z+z6YeP\",\"dataType\":\"enum\",\"identifier\":\"VoiceVolumn\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5pSv5oyB5Y2h54mH57G75Z6L\",\"dataType\":\"enum\",\"identifier\":\"RFIDType\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5oms5aOw5Zmo5byA5YWz\",\"dataType\":\"bool\",\"identifier\":\"SpeakerSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"56a75a625biD6Ziy\",\"dataType\":\"bool\",\"identifier\":\"ArmingSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6ZSB5bi45byA\",\"dataType\":\"int\",\"identifier\":\"AlwaysOpen\",\"unit\":\"min\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5Yu/5omw5qih5byP\",\"dataType\":\"struct\",\"identifier\":\"DoNotDisturb\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6Zeo6ZOD6Z+z5LmQ6K6+572u\",\"dataType\":\"struct\",\"identifier\":\"DoorbellSet\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"TkZDdGFn5YaF5a65\",\"dataType\":\"text\",\"identifier\":\"NFCtag\",\"tag\":[\"开关\",\"门锁\"]}]";
	
	//char* json_str = "[{\"name\":\"55S76Z2i57+76L2s54q25oCB\",\"value\":\"0\",\"time\":\"1666859385017\",\"dataType\":\"enum\",\"identifier\":\"ImageFlipState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5Yqg5a+G57G75Z6L5YiX6KGo\",\"value\":\"[0]\",\"time\":\"1666859385030\",\"dataType\":\"array\",\"identifier\":\"EncryptTypeList\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5qih57uE56uv5L2O5Yqf6ICX5b6F5py655qE54q25oCB\",\"value\":1,\"time\":\"1666859405319\",\"dataType\":\"bool\",\"identifier\":\"LowPowerState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"V2lmaeeJiOacrA==\",\"value\":\"DFDM-2105.1.2.221027-udp\",\"time\":\"1666859385017\",\"dataType\":\"text\",\"identifier\":\"WifiVersion\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6KeG6aKR5Yqg5a+G5byA5YWz\",\"value\":\"1\",\"time\":\"1666859385017\",\"dataType\":\"bool\",\"identifier\":\"EncryptSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6L6F56CB5rWB6KeG6aKR6LSo6YeP\",\"value\":\"0\",\"time\":\"1666859385017\",\"dataType\":\"enum\",\"identifier\":\"SubStreamVideoQuality\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5oql6K2m5byA5YWz\",\"value\":\"1\",\"time\":\"1666859385017\",\"dataType\":\"bool\",\"identifier\":\"AlarmSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6bqm5YWL6aOO5byA5YWz\",\"value\":\"1\",\"time\":\"1666859385017\",\"dataType\":\"bool\",\"identifier\":\"MicSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5Li756CB5rWB6KeG6aKR6LSo6YeP\",\"value\":\"0\",\"time\":\"1666859385017\",\"dataType\":\"enum\",\"identifier\":\"StreamVideoQuality\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6Zeo54q25oCB\",\"value\":\"68\",\"time\":\"1666858452586\",\"dataType\":\"enum\",\"identifier\":\"DoorState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6Ziy5pKs5qOA5rWL\",\"value\":\"1\",\"time\":\"1666772967272\",\"dataType\":\"bool\",\"identifier\":\"AntiPicking\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"55S15rGg55S16YeP\",\"value\":\"93\",\"time\":\"1666869236253\",\"dataType\":\"int\",\"identifier\":\"batpt\",\"unit\":\"%\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6ZSB54q25oCB\",\"value\":\"0\",\"time\":\"1666606542980\",\"dataType\":\"enum\",\"identifier\":\"LockerState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5pys5Zyw5LqL5Lu25o6o6YCBSVA=\",\"value\":\"192.168.8.154\",\"time\":\"1666607989821\",\"dataType\":\"text\",\"identifier\":\"LocalEventIP\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"54yr55y855qE5pys5ZywSVDlkozmnKzlnLDmtYHnq6/lj6M=\",\"value\":\"192.168.8.153\",\"time\":\"1666859385017\",\"dataType\":\"text\",\"identifier\":\"LocalIP\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6ZSB5o6n54mI5pys\",\"dataType\":\"text\",\"identifier\":\"LockerVersion\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"56e75Yqo5L6m5rWL6K6+572u\",\"dataType\":\"struct\",\"identifier\":\"MotionDetect\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5LyR55yg6K6+572u\",\"dataType\":\"struct\",\"identifier\":\"Dormant\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6JOd54mZ5bm/5pKt5bi45byA\",\"dataType\":\"bool\",\"identifier\":\"BLEBoardcast\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6K+t6KiA6K6+572u\",\"dataType\":\"enum\",\"identifier\":\"Language\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5pys5Zyw6K6+572u5Yqf6IO9\",\"dataType\":\"bool\",\"identifier\":\"LocalMode\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"d2lmaei/nuaOpeaooeW8jw==\",\"dataType\":\"enum\",\"identifier\":\"LinkMode\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"55S15a2Q5Y+N6ZSB\",\"dataType\":\"bool\",\"identifier\":\"DigitalLocked\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5oql6K2m6aKR546H\",\"dataType\":\"enum\",\"identifier\":\"AlarmFrequencyLevel\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5a+86Iiq6Z+z6YeP\",\"dataType\":\"enum\",\"identifier\":\"VoiceVolumn\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5pSv5oyB5Y2h54mH57G75Z6L\",\"dataType\":\"enum\",\"identifier\":\"RFIDType\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5oms5aOw5Zmo5byA5YWz\",\"dataType\":\"bool\",\"identifier\":\"SpeakerSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"56a75a625biD6Ziy\",\"dataType\":\"bool\",\"identifier\":\"ArmingSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6ZSB5bi45byA\",\"dataType\":\"int\",\"identifier\":\"AlwaysOpen\",\"unit\":\"min\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5Yu/5omw5qih5byP\",\"dataType\":\"struct\",\"identifier\":\"DoNotDisturb\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6Zeo6ZOD6Z+z5LmQ6K6+572u\",\"dataType\":\"struct\",\"identifier\":\"DoorbellSet\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"TkZDdGFn5YaF5a65\",\"dataType\":\"text\",\"identifier\":\"NFCtag\",\"tag\":[\"开关\",\"门锁\"]}]";
    cJSON* root;
	cJSON* pSub;
    
	int iCnt = 0;
	int valueInt = 0;
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
		if(parseMaxLockJsonStr(pSub, "LowPowerState", jsonValue))
		{
			printf("LowPowerState: %s\n", jsonValue);
			int stateInt = atoi(jsonValue);
			printf("stateInt: %d\n", stateInt);
		}
		if(parseMaxLockJsonStr(pSub, "LocalIP", jsonValue))
		{
			printf("LocalIP: %s\n", jsonValue);
		}
		if(parseMaxLockJsonStr(pSub, "LocalEventIP", jsonValue))
		{
			printf("LocalEventIP: %s\n", jsonValue);
		}
	}
				
    cJSON_Delete(root);

    return 0;
}