#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


static int check_internet_ping(char* ping_url)
{
	char pingCMD[128]={0};
	char tmpBuf[256]={0};
	FILE *pfile = NULL;
    char* TMP_PING_PATH	= "/tmp/pingInternet";

	snprintf(pingCMD, sizeof(pingCMD), "/bin/ping -c 1 -W 2 %s > %s", ping_url, TMP_PING_PATH);
	printf("pingCMD=%s!\n", pingCMD);
	system(pingCMD);
	if((pfile = fopen(TMP_PING_PATH, "rb")) != NULL)
	{
		fread(tmpBuf, sizeof(tmpBuf), 1, pfile);
		//printf("---------------------\ntmpBuf: %s\n-----------------------\n", tmpBuf);	
		fclose(pfile);
	}
	else
	{
		printf("open ping file error!\n");
	}

	if(strstr(tmpBuf, "1 received"))
	{  /*ping success*/   
        printf("ping [%s] success!\n", ping_url);	
        return 0;
	}
	else
	{  /*ping fail*/
	    printf("ping [%s] fail!\n", ping_url);	
        return -1;
	}
}

void check_internet_status(void)
{
	if(0 != check_internet_ping("8.8.8.8"))  /*ping GLOBAL DNS*/
	{
        if(0 != check_internet_ping("223.5.5.5"))  /*ping aliyuncs*/
		{
			if(0 != check_internet_ping("www.baidu.com"))  /*ping baidu*/
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
