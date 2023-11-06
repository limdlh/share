#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
	int fd;
	fd=open("./file", O_RDWR);//打开文件file，读写方式
	if(fd < 0){
		printf("open failure\n");
		perror("why");//打印失败的原因
		//printf("prefix: %s\n", strerror(errno));
		//printf("Error: %d\n", perror(errno));
		// ERROR("Error: %s\n", strerror(errno));
		printf("Error: %s\n", strerror(errno));
	}
	close(fd);
	return 0;
}

