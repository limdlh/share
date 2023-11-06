#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "netstatus.h"


int check_internet_ping(char* hostname)
{
    int net_res = 0;
    int net_ok_flag = 0;
    int i;
	/*char hostname[32];

	if (getIpFormDomainName("a1H8jN4MJTx.iot-as-mqtt.cn-shanghai.aliyuncs.com", hostname, 31)) {
		//printf("get ip of baidu fail\r\n");
		return 0;
	}*/
 	
    for(i=0; i<5; i++)
    {
        //net_res = NetIsOk();
        #if 1
        if(0 == ping(hostname, 2000))
        {
			net_res = 1;
		}
		#endif
        if(1 == net_res) net_ok_flag++;
    }
    if(net_ok_flag >= 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void check_internet_status(void)
{
	if(0 == check_internet_ping("8.8.8.8"))  /*ping GLOBAL DNS*/
	{
        if(0 == check_internet_ping("223.5.5.5"))  /*ping aliyuncs*/
		{
			if(0 == check_internet_ping("www.baidu.com"))  /*ping baidu*/
			{
			   printf("ping internet fail!!! \n");
			   printf("NOT CONNECTED!!! \n");
			   return;
			}
		}
	}

	printf("CONNECTED\n");
	//return 0;
}

int main(int argc, char *argv[])
{
	/*if(0 != check_internet_status())
	{
	   printf("NOT CONNECTED!!! \n");
	   return -1;
	}
	else
	{
		printf("CONNECTED\n");
	}*/
    check_internet_status();
	return 0;
}
