#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    int sockfd,numbytes;
    char buf[BUFSIZ] = {"/LAPI/V1.0/System/Wakeup{\"DeviceName\":\"H000059200400029\",\"ProductKey\":\"a164msJWX5c\"}"};
    struct sockaddr_in their_addr;
    printf("break!  BUFSIZ: %d\n", BUFSIZ);
    //如果得到返回值不为0，说明socket没创建成功，继续等待，与服务端无关
    while((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1);
    printf("We get the sockfd~\n");
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(10081);
    // 服务器 Socket 的 IP 地址
    their_addr.sin_addr.s_addr=inet_addr("192.168.21.78");
    bzero(&(their_addr.sin_zero), 8);
    
    while(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)) == -1);
    // 当上面建立连接成功后，即connect()返回0，再调用connect()函数，返回就变成-1了
    printf("connect return: %d\n", connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)));
    printf("Get the Server~\n");

    //numbytes = recv(sockfd, buf, BUFSIZ,0);//接收服务器端信息 
    // 在接收到的字符串末尾添加一个0，表示终结，避免乱码
    //buf[numbytes]='\0';  
    //printf("buf: %s\n",buf);
	
    numbytes = send(sockfd, buf, strlen(buf), 0);
    printf("send numbytes: %d\n", numbytes);
		
    while(1)
    {
        printf("Entersome thing:");
        scanf("%[^\n]%*c",buf);
        printf("scanf: %s\n", buf);
        numbytes = send(sockfd, buf, strlen(buf), 0);
        printf("send numbytes: %d\n", numbytes);
        numbytes=recv(sockfd,buf,BUFSIZ,0);  
        printf("recv numbytes: %d\n", numbytes);
        buf[numbytes]='\0'; 
        printf("received:%s\n",buf);  
    }
    close(sockfd);
    return 0;
}
