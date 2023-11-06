#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

pid_t getProcessPidByName(const char *proc_name)
{
    FILE *fp;
    char buf[100];
    char cmd[200] = {'\0'};
    pid_t pid = -1;
    sprintf(cmd, "pidof %s", proc_name);

    if((fp = popen(cmd, "r")) != NULL)
    {
        if(fgets(buf, 255, fp) != NULL)
        {
            pid = atoi(buf);
        }
    }

    printf("pid = %d \n", pid);

    pclose(fp);
    return pid;
}

int main(int argc,char *argv[])
{
	/*if(argc != 2)
	{
		printf("please input PID, for example:\n    \r./kill_fun PID\n");
		return -1;
	}
    printf("argc: %d, argv[1]: %d\n", argc, atoi(argv[1]));*/

	pid_t pid = getProcessPidByName("loop");
    printf("Now kill pid: %d \n", pid);
	int res = kill(pid, SIGINT);
	printf("res: %d \n", res);
	if(res == -1)
	{
		printf("kill error\n");
	}
	if(res == 0)
	{
		printf("kill succeed\n");
	}

	return 1;
}

