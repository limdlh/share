#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

int Tools_Mkdir(const char *path)
{
    int ret = 0;
    if(path == NULL)
        return -2;
    DIR* dir = opendir(path);
    if(dir == NULL)
    {
        ret = mkdir(path, 0755);
    }
    else
    {
        closedir(dir);
    }
    return ret;
}

int main()
{
    Tools_Mkdir("/home/limd/work/github/share/test/file_net/limd/test/");
}

