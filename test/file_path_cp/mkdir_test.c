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

int createdir(const char *dir)
{
    if (!dir || strlen(dir) == 0)
    {
        return -1;
    }

    if( mkdir(dir, S_IRWXU | S_IRWXG | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == -1 )
    {
        if(errno != EEXIST) {
            printf("Webconfig create  dir %s failed errno %d, errmsg %s\n", dir, errno, strerror(errno));
            return -1;
        }
    }
    return 1;
}

void makedir(const char *dir)
{
  char *p;
  if (!dir || strlen(dir) == 0)
  {
    return;
  }

  char tmp[160] = {0};
  strncpy(tmp, dir, strlen(dir));
  if (tmp[0] == '/')
  {
    p = strchr(tmp + 1, '/');
  }
  else
  {
    p = strchr(tmp, '/');
  }

  if (p)
  {
    *p = 0x00;
    if(0 > createdir(tmp))
    {
        return;
    }
    chdir(tmp);
  }
  else
  {
    if(0 > createdir(tmp)) 
    {
        return;
    }
    chdir(tmp);
    return;
  }
  makedir(p + 1);
}

int Tools_Mkdir(const char *dir)
{
  char curpath[160] = {0};
  getcwd(curpath, 160);
  printf("dir: %s\n", dir);
  makedir(dir);
  printf("curpath: %s\n", curpath);
  chdir(curpath);

  if(access(dir, F_OK) < 0)
  {
    return -1;
  }
  else
  {
    return 1;
  }
}

int create_multistage_dir(char *dir)
{
	int i, len;
	char str[1024] = {0};
	strcpy(str, dir);
	len = strlen(str);
	printf("len: %d\n", len);
	for (i = 0; i < len; i++)
	{
		if (str[i] == '/')
		{
			str[i] = '\0';
			if (access(str, F_OK) != 0)
			{
			    mkdir(str, S_IRWXU | S_IRWXG | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
				printf("str: %s\n", str);
			}
			str[i] = '/';
		}
	}
	if (len > 0 && access(str, F_OK) != 0) // 检测是否创建成功
	{
		mkdir(str, S_IRWXU | S_IRWXG | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
		printf("str: %s\n", str);
	}

	return 1;
}

int main()
{
    char path[160] = {0};
    getcwd(path, 100);
    printf("current work path:%s\n", path);
    //if(Tools_Mkdir("/home/limd/work/github/share/test/file_net/limd/test/hello/") < 0)
    if(create_multistage_dir("/home/limd/work/github/share/test/file_net/limd/test/hello") < 0)
    {
      printf("Tools_Mkdir error!\n");
    }
    getcwd(path, 100);
    printf("current work path:%s\n", path);
}

