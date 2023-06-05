#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define MYPORT 10080
//char* SERVERIP = "127.0.0.1";
char* SERVERIP = "192.168.124.12";

#define ERR_EXIT(m) \
    do \
    { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while(0)

void echo_cli(int sock)
{
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVERIP);

    int ret;
	char sendbuf[1024] = {"{\"topic\":\"/LAPI/V1.0/System/Wakeup\",\"data\":{\"DeviceName\": \"H000059200400029\",\"ProductKey\": \"a164msJWX5c\"}}"};
	printf("向服务器[%s:%d]发送：%s\n", SERVERIP, MYPORT, sendbuf);
	sendto(sock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	memset(sendbuf, 0, sizeof(sendbuf));

    close(sock); 
}
 
int main(void)
{
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        ERR_EXIT("socket");
    
    echo_cli(sock);
    
    return 0;
}

