/*
本程序用于监控配置文件
启动的时候读取配置文件（INPUT_FILE_PATH），并且存储到输出文件（OUTPUT_FILE_PATH）中；之后当文件内容有出现修改，立马读取，并且存储在输出文件中
limd, 2023.6.11
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>

#define MAX_STRINT_SIZE       1024
#define LOG_TIME_BUF_SIZE     32
#define TIME_INTERVAL         5
#define INPUT_FILE_PATH       "./test_input.txt"
#define OUTPUT_FILE_PATH      "./test_output.txt"


void get_local_time(char *timeStr)
{
	char buffer[24] = {0};
	struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm* ptm = localtime (&(tv.tv_sec));
    strftime(buffer, LOG_TIME_BUF_SIZE, "%Y-%m-%d %H:%M:%S\n\n", ptm); //输出格式为: 2018-12-09 10:52:57

    strcat(timeStr, buffer);
	//strcat(timeStr, "\n");
	printf("%s", timeStr);
}

char readFile(char *str)
{
	int fd = open(INPUT_FILE_PATH, O_RDONLY);
	if(fd < 0)
	{
		printf("open failed!\n");
		return -1;
	}

	int rLen = read(fd, str, MAX_STRINT_SIZE);
	printf("rLen: %d, read: {%s}\n", rLen, str);
	if(rLen < 0)
	{
		printf("read failed!\n");
		return -1;
	}
	else if (rLen >= MAX_STRINT_SIZE)
	{
		printf("File content is too long!!!Part of content maybe fail to copy\n");
	}
	close(fd);

	return 1;
}

char writeFile(char *str)
{
	static int times = 1;
	if(NULL == str)
	{
		printf("NULL == str!\n");
		return -1;
	}
	
	int len = strlen(str);
	if(len <= 0)
	{
		printf("No content to write!! len: %d\n", len);
		return -1;
	}
	
	int fd = open(OUTPUT_FILE_PATH, O_WRONLY|O_CREAT|O_APPEND, 0644);
	if(fd < 0)
	{
		printf("open failed!\n");
		return -1;
	}

	printf("len: %d, output str: {%s}", len, str);
	write(fd, str, len);
	
    char timeStr[LOG_TIME_BUF_SIZE] = {0};
    char buffer[8] = {0};
    sprintf(buffer, "\n[%d] ", times);
	times++;
    strcpy(timeStr, buffer);
	
    get_local_time(timeStr);
	len = strlen(timeStr);
	write(fd, timeStr, len);
	
    close(fd);
	return 0;
}

char checkFileModified()
{
	static unsigned long lastModifiedTime = 0;
	struct stat buf;
    int result = stat(INPUT_FILE_PATH, &buf);
	if( result != 0 )
	{
	    perror( "显示文件状态信息出错" );
		return -1;
	}
    else
    {
        /*printf("文件大小: %ld\n", buf.st_size);
        printf("文件创建时间: %s", ctime(&buf.st_ctime));
        printf("访问日期: %s", ctime(&buf.st_atime));*/
        //printf("最后修改日期: %s", ctime(&buf.st_mtime));
    }
	
	if(lastModifiedTime != buf.st_mtime)
	{
		printf("lastModifiedTime: %lu ", lastModifiedTime);
		lastModifiedTime = buf.st_mtime;
		printf("--> lastModifiedTime: %lu, st_mtime: %lu, st_mtime: %s", lastModifiedTime, buf.st_mtime, ctime(&buf.st_mtime));
		return 1;
	}
	
	return 0;
}

int main()
{
	do{
		char cRet = checkFileModified();
		if(cRet < 0)
		{
			printf("checkFileModified failed!\n");
		}
		else if(cRet == 0)
		{
			//printf("checkFileModified file NO change!\n");
		}
		else
		{
			printf("checkFileModified file DID changed!\n");
			char arrStr[MAX_STRINT_SIZE] = {0};
			if(readFile(arrStr) < 0)
			{
				printf("readFile failed!\n");
			}
			
			if(writeFile(arrStr) < 0)
			{
				printf("writeFile failed!\n");
			}
		}
		sleep(TIME_INTERVAL);
	}while(1);

	return 0;
}