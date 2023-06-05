#include <stdio.h>
#include <string.h>
#include "cJSON.h"

void parseMaxLockJson(cJSON* pSub, char* subItem){
    if(NULL == pSub || NULL == subItem){
        printf("NULL == pSub or NULL == subItem !!!ERROR!!");
		return;
	}
	
	cJSON* identifier = cJSON_GetObjectItem(pSub, "identifier");
	if(identifier) {
		char* ident_str = identifier->valuestring;
		if(!strcmp(ident_str, subItem))
		{
			cJSON* value = cJSON_GetObjectItem(pSub, "value");
			if(value) {
				char* value_str = value->valuestring;
				printf("identifier: %s, value: %s\n", ident_str, value_str);
			}
			else
			{
				printf("ERROR!!! %s found, but no value!!!!\t", subItem);
			}
		}
	}
}
		
int main() {
	char* json_str = "[{\"name\":\"55S76Z2i57+76L2s54q25oCB\",\"value\":\"0\",\"time\":\"1666859385017\",\"dataType\":\"enum\",\"identifier\":\"ImageFlipState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5Yqg5a+G57G75Z6L5YiX6KGo\",\"value\":\"[0]\",\"time\":\"1666859385030\",\"dataType\":\"array\",\"identifier\":\"EncryptTypeList\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5qih57uE56uv5L2O5Yqf6ICX5b6F5py655qE54q25oCB\",\"value\":\"1\",\"time\":\"1666859405319\",\"dataType\":\"bool\",\"identifier\":\"LowPowerState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"V2lmaeeJiOacrA==\",\"value\":\"DFDM-2105.1.2.221027-udp\",\"time\":\"1666859385017\",\"dataType\":\"text\",\"identifier\":\"WifiVersion\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6KeG6aKR5Yqg5a+G5byA5YWz\",\"value\":\"1\",\"time\":\"1666859385017\",\"dataType\":\"bool\",\"identifier\":\"EncryptSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6L6F56CB5rWB6KeG6aKR6LSo6YeP\",\"value\":\"0\",\"time\":\"1666859385017\",\"dataType\":\"enum\",\"identifier\":\"SubStreamVideoQuality\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5oql6K2m5byA5YWz\",\"value\":\"1\",\"time\":\"1666859385017\",\"dataType\":\"bool\",\"identifier\":\"AlarmSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6bqm5YWL6aOO5byA5YWz\",\"value\":\"1\",\"time\":\"1666859385017\",\"dataType\":\"bool\",\"identifier\":\"MicSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5Li756CB5rWB6KeG6aKR6LSo6YeP\",\"value\":\"0\",\"time\":\"1666859385017\",\"dataType\":\"enum\",\"identifier\":\"StreamVideoQuality\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6Zeo54q25oCB\",\"value\":\"68\",\"time\":\"1666858452586\",\"dataType\":\"enum\",\"identifier\":\"DoorState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6Ziy5pKs5qOA5rWL\",\"value\":\"1\",\"time\":\"1666772967272\",\"dataType\":\"bool\",\"identifier\":\"AntiPicking\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"55S15rGg55S16YeP\",\"value\":\"93\",\"time\":\"1666869236253\",\"dataType\":\"int\",\"identifier\":\"batpt\",\"unit\":\"%\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6ZSB54q25oCB\",\"value\":\"0\",\"time\":\"1666606542980\",\"dataType\":\"enum\",\"identifier\":\"LockerState\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5pys5Zyw5LqL5Lu25o6o6YCBSVA=\",\"value\":\"192.168.8.154\",\"time\":\"1666607989821\",\"dataType\":\"text\",\"identifier\":\"LocalEventIP\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"54yr55y855qE5pys5ZywSVDlkozmnKzlnLDmtYHnq6/lj6M=\",\"value\":\"192.168.8.153\",\"time\":\"1666859385017\",\"dataType\":\"text\",\"identifier\":\"LocalIP\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6ZSB5o6n54mI5pys\",\"dataType\":\"text\",\"identifier\":\"LockerVersion\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"56e75Yqo5L6m5rWL6K6+572u\",\"dataType\":\"struct\",\"identifier\":\"MotionDetect\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5LyR55yg6K6+572u\",\"dataType\":\"struct\",\"identifier\":\"Dormant\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6JOd54mZ5bm/5pKt5bi45byA\",\"dataType\":\"bool\",\"identifier\":\"BLEBoardcast\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6K+t6KiA6K6+572u\",\"dataType\":\"enum\",\"identifier\":\"Language\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5pys5Zyw6K6+572u5Yqf6IO9\",\"dataType\":\"bool\",\"identifier\":\"LocalMode\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"d2lmaei/nuaOpeaooeW8jw==\",\"dataType\":\"enum\",\"identifier\":\"LinkMode\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"55S15a2Q5Y+N6ZSB\",\"dataType\":\"bool\",\"identifier\":\"DigitalLocked\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5oql6K2m6aKR546H\",\"dataType\":\"enum\",\"identifier\":\"AlarmFrequencyLevel\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5a+86Iiq6Z+z6YeP\",\"dataType\":\"enum\",\"identifier\":\"VoiceVolumn\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5pSv5oyB5Y2h54mH57G75Z6L\",\"dataType\":\"enum\",\"identifier\":\"RFIDType\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5oms5aOw5Zmo5byA5YWz\",\"dataType\":\"bool\",\"identifier\":\"SpeakerSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"56a75a625biD6Ziy\",\"dataType\":\"bool\",\"identifier\":\"ArmingSwitch\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6ZSB5bi45byA\",\"dataType\":\"int\",\"identifier\":\"AlwaysOpen\",\"unit\":\"min\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"5Yu/5omw5qih5byP\",\"dataType\":\"struct\",\"identifier\":\"DoNotDisturb\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"6Zeo6ZOD6Z+z5LmQ6K6+572u\",\"dataType\":\"struct\",\"identifier\":\"DoorbellSet\",\"unit\":\"\",\"tag\":[\"开关\",\"门锁\"]},{\"name\":\"TkZDdGFn5YaF5a65\",\"dataType\":\"text\",\"identifier\":\"NFCtag\",\"tag\":[\"开关\",\"门锁\"]}]";
    cJSON* root;
	cJSON* pSub;
    
	
    int value_int;
	int iCnt = 0;
    char* value_string;

    //解析json_str, 通过cJSON_Parse解析接收到的字符串，再通过cJSON_GetObjectItem获取指定键的值，最后释放该JSON结点的内存
	printf("json_str: %s\n", json_str);
    root = cJSON_Parse(json_str);
	int cntlist = cJSON_GetArraySize(root);
	printf("cntlist: %d\n", cntlist);
	
	for(iCnt = 0; iCnt < cntlist; iCnt ++ ){
		//printf("iCnt: %3d\t", iCnt);
		pSub = cJSON_GetArrayItem(root, iCnt);
		if(NULL == pSub)
		{
			printf("pSub[%d] it NULL\n", iCnt);
			continue ;
		}
		
		parseMaxLockJson(pSub, "LowPowerState");
		parseMaxLockJson(pSub, "LocalIP");
		parseMaxLockJson(pSub, "LocalEventIP");
		
		/*identifier = cJSON_GetObjectItem(pSub, "identifier");
        if(identifier) {
			char* ident_str = identifier->valuestring;
			if(!strcmp(ident_str, "LocalEventIP"))
			{
				value = cJSON_GetObjectItem(pSub, "value");
				if(value) {
					char* value_str = value->valuestring;
					printf("identifier: %s, value: %s\n", ident_str, value_str);
				}
				else
				{
					printf("ERROR!!! LocalEventIP found, but no value!!!!\t");
				}
			}
		}
		
		identifier = cJSON_GetObjectItem(pSub, "identifier");
        if(identifier) {
			char* ident_str = identifier->valuestring;
			if(!strcmp(ident_str, "LocalIP"))
			{
				value = cJSON_GetObjectItem(pSub, "value");
				if(value) {
					char* value_str = value->valuestring;
					printf("identifier: %s, value: %s\n", ident_str, value_str);
				}
				else
				{
					printf("ERROR!!! LocalIP found, but no value!!!!\t");
				}
			}
		}
		
		identifier = cJSON_GetObjectItem(pSub, "identifier");
        if(identifier) {
			char* ident_str = identifier->valuestring;
			if(!strcmp(ident_str, "LowPowerState"))
			{
				value = cJSON_GetObjectItem(pSub, "value");
				if(value) {
					char* value_str = value->valuestring;
					printf("identifier: %s, value: %s\n", ident_str, value_str);
				}
				else
				{
					printf("ERROR!!! LowPowerState found, but no value!!!!\t");
				}
			}
		}*/
	}
				
    cJSON_Delete(root);

    return 0;
}