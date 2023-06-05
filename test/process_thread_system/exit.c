#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{	
	pid_t res;
	res = fork();
	sleep(3);
	if(res == -1)
	{
		printf("fork err\r\n");
	}
	if(res == 0)
	{
		//不加回车换行符，让输出的字符串数据保留在io缓存区里面
		printf("son");
		//退出状态设置为0，表示正常退出。
		_exit(0);
	}
	else if(res > 0)
	{
		printf("parent\n");
		exit(123);
		//_exit(188);
	}
}


