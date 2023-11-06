#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#define PING_CMD	"/bin/ping -c 1 -W 2 %s > %s"
#define TMP_PING_PATH	"/tmp/pingInternet"
#define RECV1ICMPPKT	"1 received"
#define PING_URL	"8.8.8.8"

static int check_internet_ping(void)
{
	char pingCMD[128]={0};
	char tmpBuf[256]={0};
	FILE *pfile = NULL;

	snprintf(pingCMD, sizeof(pingCMD), PING_CMD, PING_URL, TMP_PING_PATH);  /*ping GLOBAL DNS*/
	printf("pingCMD=%s!\n", pingCMD);
	system(pingCMD);
	if((pfile = fopen(TMP_PING_PATH, "rb")) != NULL) {
	       fread(tmpBuf, sizeof(tmpBuf), 1, pfile);
	       fclose(pfile);
	}
	else {
		printf("open ping file error!\n");
	}

	if(strstr(tmpBuf, RECV1ICMPPKT)) {  /*ping success*/    
            return 0;
	} else {                           /*ping fail*/
            return -1;
	}
}
 
int main(int argc, char *argv[])
{
	if(0 == check_internet_ping())
	{		   
	   printf("CONNECTED\n");
	}

	return 0;
}
 
