#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
 
int main()
{
	int iSocketFD = 0;
	struct sockaddr_un stRemoteAddr = {0};	
	char acBuf[1024] = {0};
 
	iSocketFD = socket(AF_UNIX, SOCK_STREAM, 0);
	if(0 > iSocketFD)
	{
		printf("创建socket失败！\n");
		return -1;
	}
 
	//填充对端地址
	stRemoteAddr.sun_family = AF_UNIX;
	sprintf(stRemoteAddr.sun_path,"/home/limd/work/git_dir/hy_4_inch_gui/GUI/test_socket"); //注意用sprintf
	
	//注意connect的第二个参数
	if(0 > connect(iSocketFD, (void *)&stRemoteAddr, sizeof(stRemoteAddr)))
	{
		close(iSocketFD);
		printf("连接服务器失败！\n");
		return -1;
	}	
	
	if(0 > send(iSocketFD, "客户端", sizeof("客户端"), 0))
	{
		close(iSocketFD);
		printf("发送数据失败！\n");
		return -1;
	}
	
	if(0 > recv(iSocketFD, acBuf, sizeof(acBuf), 0))
	{
		close(iSocketFD);
		printf("接收数据失败！\n");
		return -1;
	}
 
	printf("客户端接收到服务器发来的消息是:%s\n", acBuf);
	
	return 0;
}
