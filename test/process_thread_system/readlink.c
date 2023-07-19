#include <stdio.h>
#include <unistd.h>
char * get_exe_path( char * buf, int count)
{
    int i;
	//int rslt = readlink("/home/limd/work/git_dir/test/process_thread_system/readlink", buf, count - 1);
    int rslt = readlink("/proc/self/exe", buf, count - 1);
    if (rslt < 0 || (rslt >= count - 1))
    {
        return NULL;
    }
    buf[rslt] = '\0';
    /*for (i = rslt; i >= 0; i--)
    {
        printf("buf[%d] %c\n", i, buf[i]);
        if (buf[i] == '/')
        {
            buf[i + 1] = '\0';
            break;
        }
    }*/
    return buf;
}

int main(int argc, char ** argv)
{
    char path[1024];
	char* strRet = get_exe_path(path, 1024);
	if(strRet == NULL)
	{
		printf("strRet == NULL\n");
	}
	else
	{
        printf("strRet: %s\n", strRet);
	}
    return 0;
}