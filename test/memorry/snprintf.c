#include<stdio.h>
#include<stdlib.h>
int main()
{
	char str[10]={0};
	int nLen=snprintf(str, sizeof(str), "123456789012345678");
	printf("str=%s\n", str);
	printf("nLen=%d\n", nLen);
	
	char locationBody[384]= {0};
	char smac[32] = "f0:b0:40:c7:d8:cc";
    //char macs[128]= "fe:84:8d:a5:ac:3e,-40,[WPA-PSK-CCMP][WPA2-PSK-CCMP][ESS]|f4:84:8d:a5:ac:3e,-40,TP-LINK_AC3E|f4:84:8d:af:36:84,-49,TP-LINK_3684";
	char macs[256]= "f4:84:8d:a5:ac:3e,-38,TP-LINK_AC3E|fe:84:8d:a5:ac:3e,-38,[WPA-PSK-CCMP][WPA2-PSK-CCMP][ESS]|a0:a3:b9:60:f9:f3,-46,HONYAR-GW-F9F1";
	int bodyLen = snprintf(locationBody, sizeof(locationBody), "{\"smac\":\"%s\", \"macs\":\"%s\"} ", smac, macs);
    printf("smac: %s, macs: %s, len: %d, locationBody: %s\n", smac, macs, sizeof(locationBody), locationBody);
	
	int batptInt = 12;
	char cmd[24] = {0};
	snprintf(cmd, 24, "%s %d%s", "#DB5037", batptInt, "%");
	printf("cmd=%s\n", cmd);
	return 0;
}