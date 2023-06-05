#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
 
int main()
{
	int iSocketFD = 0;
	int iNewFD = 0;
	struct sockaddr_un stLocalAddr = {0};
	char acBuf[1024]= {0};
 
	iSocketFD = socket(AF_UNIX, SOCK_STREAM, 0);
	if(0 > iSocketFD)	
	{
		printf("创建socket失败！\n");
		return 0;
	}
 
	stLocalAddr.sun_family = AF_UNIX;
	sprintf(stLocalAddr.sun_path, "/home/limd/work/git_dir/hy_4_inch_gui/GUI/test_socket");
 
	remove("/home/limd/work/git_dir/hy_4_inch_gui/GUI/test_socket");
 
	if(0 > bind(iSocketFD, (void *)&stLocalAddr, sizeof(stLocalAddr)))
	{
		close(iSocketFD);
		printf("绑定地址失败！\n");
		return -1;
	}
 
	if(0 > listen(iSocketFD, 5))
	{
		close(iSocketFD);
		printf("监听失败！\n");
		return -1;
	}
 
	while(1)
	{
		iNewFD = accept(iSocketFD, NULL, NULL);
		if(0 > iNewFD)
		{
			close(iSocketFD);
			printf("接收数据失败！\n");
			return -1;
		}
		
		if(0 > send(iNewFD, "Hello, here is 服务器",  sizeof("Hello, here is 服务器"), 0))
		{
			close(iSocketFD);
			close(iNewFD);
			printf("发送失败！\n");
			return -1;
		}
		
		if(0 > recv(iNewFD, acBuf, sizeof(acBuf), 0))
		{
			close(iSocketFD);
			close(iNewFD);
			printf("接收失败！\n");
			return -1;
		}
		else
		{
			printf("接收到的客户端发来的信息是：%s\n", acBuf);
		}
 
		if(0 > send(iNewFD, "服务器",  sizeof("服务器"), 0))
		{
			close(iSocketFD);
			close(iNewFD);
			printf("发送失败！\n");
			return -1;
		}
	}
	return 0;
}
