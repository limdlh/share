
#include "log.h"


static char *logPrintBuf = NULL;

void initLogBuf()
{
    if ((logPrintBuf = (char *) malloc(LOG_BUF_SIZE)) == NULL){
		printf("malloc error!!!");
        return;
    }
}

int log_model(char *file_name)
{
    int         rv = -1;
    int         fd = -1;
    //mode_t         mode;
    struct stat buf;

    if( !file_name ) {
        perror("Only standard output\n");
        return 0;
    }

    //将日志输入到对应的文件
    rv=access(file_name , F_OK);//判断文件是否存在
    if( rv < 0) {
        printf("%s doesn't exist, need to create a log file\n", file_name);
        umask(0);//为进程设置文件模式创建屏蔽字, 
        fd=open(file_name, O_RDWR|O_CREAT|O_APPEND, RWRWRW );//屏蔽字为1的位，文件mode中的相应位一定关闭
        if( fd < 0) {
            printf("open %s failure\n", file_name);
            return -2;
        }
        printf("open %s successful\n", file_name);
    }
    else {   
        printf("%s exist\n", file_name);
        fd=open(file_name, O_RDWR|O_CREAT|O_APPEND);//打开文件，设置对应的权限
        if( fd < 0) {            
            printf("open %s failure\n", file_name);
            return -2;
        }            
        printf("open %s successful\n", file_name);
    }

	printf("enter FILE_ONLY\n");
	rv=dup2(fd, STDOUT_FILENO);//将标准输出重定向到文件fd
	if( rv < 0) {
		printf("dup2 %d to stdout failure\n", fd);
		return -4;
	}
	rv=fstat(fd, &buf);//获取日志文件的信息
	if( rv < 0) {
		printf("get last time used failure\n");
	} else {
		printf("File size:                %lld bytes\n",(long long) buf.st_size);//打印截止到现在的文件大小
		printf("Time of last access:%sTime of last modification:%s\n",ctime((const time_t *)&buf.st_atim), ctime((const time_t *)&buf.st_mtim));//打印上次进入的时间， 打印上次进入且修改日志的时间
	}
	return fd;
}


static char * get_local_time(char *time_str, int len, struct timeval *tv)
{
    struct tm* ptm;
    char time_string[40];
    long milliseconds;
    
    ptm = localtime (&(tv->tv_sec));
 
    //strftime (time_string, sizeof(time_string), "%Y/%m/%d %H:%M:%S", ptm); //输出格式为: 2018/12/09 10:48:31.391
    //strftime (time_string, sizeof(time_string), "%Y|%m|%d %H-%M-%S", ptm); //输出格式为: 2018|12|09 10-52-28.302
    strftime (time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", ptm); //输出格式为: 2018-12-09 10:52:57.200
    //strftime (time_string, sizeof(time_string), "%Y\\%m\\%d %H-%M-%S", ptm); //输出格式为: 2018\12\09 10-52-28.302
 
    /* 从微秒计算毫秒。*/
    milliseconds = tv->tv_usec / 1000;
 
    /* 以秒为单位打印格式化后的时间日期，小数点后为毫秒。*/
    snprintf (time_str, len, "%s.%03ld", time_string, milliseconds);
 
    return time_str;
}
 
void log_print(enum log_color color, const char *file, int line, const char *func, const char* fmt, ...)
{
    if (debug_type == NO_OUTPUT){
		return;
    }
	
    va_list ap;
	char local_time_str[128];
    char *time = NULL;
    struct timeval tv;
 
    gettimeofday(&tv, NULL);
    time = get_local_time(local_time_str, sizeof(local_time_str), &tv);

	if(logPrintBuf == NULL)
	{
	    initLogBuf();
	}
	else
	{
        printf("--> sizeof(logPrintBuf): %d, strlen(logPrintBuf): %d\n", sizeof(logPrintBuf), strlen(logPrintBuf));
	}
	
    va_start(ap, fmt);
    int n = vsnprintf(logPrintBuf, LOG_BUF_SIZE, fmt, ap);
	printf("-----------------------> n: %d \n", n);
	if (n > LOG_BUF_SIZE)
	{
		if ((logPrintBuf = (char *)realloc(logPrintBuf, (n+1)*sizeof(char))) == NULL)
		{
			printf("realloc error!!!\n");
            return;
		}
		n = vsnprintf(logPrintBuf, n+1, fmt, ap);
		printf("=========== n: %d, logPrintBuf: %s\n", n, logPrintBuf);
	}
	va_end(ap);

	switch(color)
    {
		case HIGHLIGHT:
			if(debug_levet >= LEVEL_FATAL){
				if (debug_type == STD_OUT){
					printf("\033[5;7;31m<%s>[%s:%d %s] %s\033[0m\n", time, file, line, func, logPrintBuf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[HIGHLIGHT] <%s>[%s:%d %s] %s\n", time, file, line, func, logPrintBuf);
			    }
			}
			break;
		case RED:
			if(debug_levet >= LEVEL_ERROR){
				if (debug_type == STD_OUT){
					printf("\033[31m<%s>[%s:%d %s] %s\033[0m\n", time, file, line, func, logPrintBuf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[ERROR] <%s>[%s:%d %s] %s\n", time, file, line, func, logPrintBuf);
			    }
			}
			break;
		case YELLOW:
			if(debug_levet >= LEVEL_WARN){
				if (debug_type == STD_OUT){
					printf("\033[33m<%s>[%s:%d %s] %s\033[0m\n", time, file, line, func, logPrintBuf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[WARN] <%s>[%s:%d %s] %s\n", time, file, line, func, logPrintBuf);
			    }
			}
			break;
		case BLUE:
			if(debug_levet >= LEVEL_INFO){
				if (debug_type == STD_OUT){
					printf("\033[34m<%s>[%s:%d %s] %s\033[0m\n", time, file, line, func, logPrintBuf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[INFO] <%s>[%s:%d %s] %s\n", time, file, line, func, logPrintBuf);
			    }
			}
			break;
		case GREEN:
			if(debug_levet >= LEVEL_DEBUG){
				if (debug_type == STD_OUT){
					printf("\033[32m<%s>[%s:%d %s] %s\033[0m\n", time, file, line, func, logPrintBuf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[DEBUG] <%s>[%s:%d %s] %s\n", time, file, line, func, logPrintBuf);
			    }
			}
			break;
		case BLACK:
            if(debug_levet >= LEVEL_LOG){
				if (debug_type == STD_OUT){
					printf("<%s>[%s:%d %s] %s\n", time, file, line, func, logPrintBuf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[LOG] <%s>[%s:%d %s] %s\n", time, file, line, func, logPrintBuf);
			    }
			}
			break;
        default:
            break;
    }

	if (n > 32)
	{
	    printf("free logPrintBuf\n");
        free(logPrintBuf);
		logPrintBuf = NULL;
	}
}


