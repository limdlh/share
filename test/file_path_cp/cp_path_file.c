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
#define READ_LENGTH        2048


int Tools_Cp(const char *src, const char *des)
{
    if (src == NULL || des == NULL)
    {
        printf("Parames src or des is NULL");
        return -2;
    }

    int fd_src = -1, fd_des = -1;
    if ((fd_src = open(src, O_RDONLY)) == -1)
    {
        printf("open src path error! src[%s]", src);
        return -2;
    }

    if (access(des, F_OK) == 0)
    {
		if(0 != unlink(des))
		{
			printf("文件 %s 删除失败\n", des);
			close(fd_src);
			return -2;
		}
    }

    if ((fd_des = open(des, O_CREAT | O_EXCL | O_WRONLY, 0644)) == -1)
    {
        printf("open des file error!, des[%s]", des);
        close(fd_src);
        return -2;
    }

    int num = -1, res = -1;
    char buf[MAX_PATH_LEN] = {0};
    while ((num = read(fd_src, buf, READ_LENGTH)) > 0)
    {
        if((res = write(fd_des, buf, num)) < 0)
        {
            printf("write [%s] failed",des);
            close(fd_src);
            close(fd_des);
            return -1;
        }
    }
    close(fd_src);
    close(fd_des);
    return 0;
}

int main()
{
    char path[160] = {0};
    getcwd(path, 160);
    printf("current work path:%s\n", path);

    if(Tools_Cp("/home/limd/work/github/share/test/file_path_cp/dir/test.txt", "/home/limd/work/github/share/test/file_path_cp/test.txt") < 0)
    {
      printf("Tools_Cp error!\n");
    }
    getcwd(path, 160);
    printf("current work path:%s\n", path);
}