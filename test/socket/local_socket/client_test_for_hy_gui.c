#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
 
#define SOCK_PATH_LVGL "/home/limd/work/git_dir/hy_4_inch_gui/GUI/test_socket"

int main()
{
	printf("limd, this is client\n");
	int iSocketFD = -1;
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
	//sprintf(stRemoteAddr.sun_path,"/userdata/srceen_app_server"); //注意用sprintf
	strcpy (stRemoteAddr.sun_path, SOCK_PATH_LVGL);
    int len = sizeof (stRemoteAddr);
	int result;
	
	//注意connect的第二个参数
	/*if(0 > connect(iSocketFD, (void *)&stRemoteAddr, sizeof(stRemoteAddr)))
	{
		close(iSocketFD);
		printf("连接服务器失败！\n");
		return -1;
	}*/
	
	while (1)
    {
        result = connect(iSocketFD, (struct sockaddr *)&stRemoteAddr, len);
        if (result == 0)
        {
            return 0;
        }
        else 
        {
            //close(iSocketFD);
			printf("连接服务器失败！\n");
            sleep(3);
            continue;
        }
    }
	
	char cmd_buf[128] = {0};
	//sprintf(cmd_buf, "[{\"ctrlType\":\"music\",\"method\":\"prev\",\"offset\":\"0\"}]");
	sprintf(cmd_buf, "limd");
	int length = sizeof(cmd_buf);
	printf("length of cmd_buf: %d\n", length);
	if(0 > send(iSocketFD, cmd_buf, length+1, 0))
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
