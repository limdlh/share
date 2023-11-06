#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>

#define MAX_PATH_LEN               256


int Tools_Mkdir(const char *dir)
{
	char str[MAX_PATH_LEN] = {0};
	strncpy(str, dir, strlen(dir));
	int len = strlen(str);
	printf("len: %d\n", len);
	for (int i = 1; i < len; i++)
	{
		if (str[i] == '/')
		{
			printf("11111 i: %d\n", i);
			str[i] = '\0';
			if (access(str, F_OK) != 0)
			{
			    mkdir(str, S_IRWXU | S_IRWXG | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
				printf("11111 str: %s\n", str);
				char path[160] = {0};
				getcwd(path, 160);
                printf("pwd:%s\n", path);
			}
			str[i] = '/';
		}
	}
	if (len > 0 && access(str, F_OK) != 0) // 检测是否创建成功
	{
		mkdir(str, S_IRWXU | S_IRWXG | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
		printf("2222 str: %s\n", str);
	}

    printf("----------> dir: %s\n", dir);
	printf("----------> str: %s\n", str);
    if(access(dir, F_OK) < 0) //判断文件夹是否创建成功
    {
        return -1;
    }

    return 1;
}

int main()
{
    char path[160] = {0};
    getcwd(path, 160);
    printf("current work path:%s\n", path);

    if(Tools_Mkdir("/home/limd/work/github/share/test/file_net/limd/test/hello/") < 0)
    //if(Tools_Mkdir("./limd/test/hello/") < 0)
    //if(Tools_Mkdir("/") < 0)
    {
      printf("Tools_Mkdir error!\n");
    }
    getcwd(path, 160);
    printf("current work path:%s\n", path);
}

