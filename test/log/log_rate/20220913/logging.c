
#include "logging.h"


int initlogfile(char *file_name){
    int rv = -1;
    int fd = -1;
    struct stat buf;

    if(!file_name) {
        ERROR("file_name: %s", file_name);
        return -1;
    }

    //将日志输入到对应的文件
    rv = access(file_name, F_OK);//判断文件是否存在
    if( rv < 0) {
        INFO("%s doesn't exist, need to create a log file", file_name);
        umask(0);//为进程设置文件模式创建屏蔽字, 
        fd = open(file_name, O_RDWR|O_CREAT|O_APPEND, RWRWRW);//屏蔽字为1的位，文件mode中的相应位一定关闭
        if( fd < 0) {
            ERROR("open %s failure", file_name);
            return -2;
        }
        DEBUG("open %s successful", file_name);
    } else {   
        DEBUG("%s exist", file_name);
        fd = open(file_name, O_RDWR|O_CREAT|O_APPEND, RWRWRW);//打开文件，设置对应的权限
        if( fd < 0) {            
            ERROR("open %s failure", file_name);
            return -2;
        }            
        DEBUG("open %s successful", file_name);
    }

	rv = dup2(fd, STDOUT_FILENO);//将标准输出重定向到文件fd
	if(rv < 0) {
		ERROR("dup2 %d to stdout failure", fd);
		return -4;
	}
	rv = fstat(fd, &buf);//获取日志文件的信息
	if(rv < 0) {
		ERROR("get last time used failure");
	} else {
		INFO("File size:   %lld bytes", (long long) buf.st_size);//打印截止到现在的文件大小
		INFO("Time of last access:%s, last modification:%s", ctime((const time_t *)&buf.st_atim), ctime((const time_t *)&buf.st_mtim));
	}
	return fd;
}

static char * get_local_time(char *time_str, int len, struct timeval *tv){
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
 
void log_print(enum log_color color, const char *file, int line, const char *func, const char* fmt, ...){
    if (debug_type == NO_OUTPUT){
		return;
    }
	
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
	char local_time_str[128];
    char *time = NULL;
    struct timeval tv;
 
    gettimeofday(&tv, NULL);
    time = get_local_time(local_time_str, sizeof(local_time_str), &tv);
	
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
 
	switch(color) {
		case HIGHLIGHT:
			if(debug_level >= LEVEL_FATAL){
				if (debug_type == STD_OUT){
					printf("\033[5;7;31m<%s>[%s:%d %s] %s\033[0m\n", time, file, line, func, buf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[HIGHLIGHT] <%s>[%s:%d %s] %s\n", time, file, line, func, buf);
			    }
			}
			break;
		case RED:
			if(debug_level >= LEVEL_ERROR){
				if (debug_type == STD_OUT){
					printf("\033[31m<%s>[%s:%d %s] %s\033[0m\n", time, file, line, func, buf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[ERROR] <%s>[%s:%d %s] %s\n", time, file, line, func, buf);
			    }
			}
			break;
		case YELLOW:
			if(debug_level >= LEVEL_WARN){
				if (debug_type == STD_OUT){
					printf("\033[33m<%s>[%s:%d %s] %s\033[0m\n", time, file, line, func, buf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[WARN] <%s>[%s:%d %s] %s\n", time, file, line, func, buf);
			    }
			}
			break;
		case BLUE:
			if(debug_level >= LEVEL_INFO){
				if (debug_type == STD_OUT){
					printf("\033[34m<%s>[%s:%d %s] %s\033[0m\n", time, file, line, func, buf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[INFO] <%s>[%s:%d %s] %s\n", time, file, line, func, buf);
			    }
			}
			break;
		case GREEN:
			if(debug_level >= LEVEL_DEBUG){
				if (debug_type == STD_OUT){
					printf("\033[32m<%s>[%s:%d %s] %s\033[0m\n", time, file, line, func, buf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[DEBUG] <%s>[%s:%d %s] %s\n", time, file, line, func, buf);
			    }
			}
			break;
		case BLACK:
            if(debug_level >= LEVEL_LOG){
				if (debug_type == STD_OUT){
					printf("<%s>[%s:%d %s] %s\n", time, file, line, func, buf);
			    }
				else if (debug_type == WRITE_FILE){
					printf("[LOG] <%s>[%s:%d %s] %s\n", time, file, line, func, buf);
			    }
			}
			break;
        default:
            break;
    }
}

long get_file_size(char* filename){
	long length = 0;
	FILE *fp = NULL;
 
	fp = fopen(filename, "rb");
	if (fp != NULL) {
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
	}
 
	if (fp != NULL) {
		fclose(fp);
		fp = NULL;
	}
	return length;
}

int initlogging(logset *logsetting){
    if(NULL == logsetting) {
        ERROR("NULL == logsetting");
		return -1;
	}
	
    debug_type = logsetting->outputtype;
    debug_level = logsetting->loglevel;
	logsetting->maxfilelen = logsetting->maxfilelen*MB;  //unit of maxfilelen in logging.conf is MB

	if (debug_type == WRITE_FILE) {
		long length = get_file_size(logsetting->filepath); // 文件超过最大限制, 删除
		if (length > logsetting->maxfilelen) {
			INFO("file: %s, length: %ld > maxfilelen: %ld, log file is larger than size in logging.conf, remove it!!!!!!!!!!!\n", \
				logsetting->filepath, length, logsetting->maxfilelen);
			unlink(logsetting->filepath); // 删除文件
		}
	
		fd_log_file = initlogfile(logsetting->filepath);
		if (fd_log_file < 0){ //日志文件初始化失败，转为终端打印日志
            debug_type = STD_OUT;
		}
	}
	return fd_log_file;
}

void destorylogging(void){
    INFO("destory logging >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n");
    close(fd_log_file);
}


/* 日志设置信息 */  
bool getlogset(logset *logsetting){
    if(NULL == logsetting) {
	    return false;
	}
    char path[MAX_FILE_PATH] = {0x0};
    getcwd(path, sizeof(path));  
    strcat(path, "/logging.conf");  
    if(access(path, F_OK)==0) {  
        if(readconfig(path, logsetting) != 0){//无法打印到日志文件，则打印到终端
            logsetting->loglevel = LEVEL_ALL;
		    logsetting->outputtype = STD_OUT;
		    logsetting->maxfilelen = 1*MB;
			memcpy(logsetting->filepath, "./", strlen("./"));
        }
    } else {//无法打印到日志文件，则打印到终端
        logsetting->loglevel = LEVEL_ALL;  
		logsetting->outputtype = STD_OUT;
		logsetting->maxfilelen = 1*MB;
		memcpy(logsetting->filepath, "./", strlen("./"));
    }  
    return true;  
}  

unsigned char getloglevel(char *levelstr){
    unsigned char level = 255;
	if(strcmp("fatal", levelstr) == 0){
        level = LEVEL_FATAL;
	}else if(strcmp("error", levelstr) == 0){
        level = LEVEL_ERROR;
	}else if(strcmp("warn", levelstr) == 0){
        level = LEVEL_WARN;
	}else if(strcmp("info", levelstr) == 0){
        level = LEVEL_INFO;
	}else if(strcmp("debug", levelstr) == 0){
        level = LEVEL_DEBUG;
 	}else if(strcmp("log", levelstr) ==  0){
        level = LEVEL_LOG;
 	}else if(strcmp("all", levelstr) ==  0){
        level = LEVEL_LOG;
 	}else{
		printf("Data configure error in logging.conf, item \'level\' could be: fatal, error, warn, info, debug, log or all\n");
        level = LEVEL_ALL;
    }
    return level;
}

unsigned char getoutputtype(char *typestr){
    unsigned char type = NO_OUTPUT;
	if(strcmp("NO_OUTPUT", typestr) == 0){
        type = NO_OUTPUT;
	}else if(strcmp("WRITE_FILE", typestr) == 0){
        type = WRITE_FILE;
	}else if(strcmp("STD_OUT", typestr) == 0){
        type = STD_OUT;
	}else{
		printf("Data configure error in logging.conf, item \'outputtype\' could be: NO_OUTPUT, WRITE_FILE or STD_OUT\n");
		type = STD_OUT;
	}
    return type;
}

static char * getdate(char *date){  
    time_t timer=time(NULL);  
    strftime(date,11,"%y-%m-%d",localtime(&timer));  
    return date;  
}

unsigned char readconfig(char *path, logset *logsetting){
    char value[MAX_FILE_PATH] = {0x0};
    char data[50] = {0x0};
  
    FILE *fpath = fopen(path,"r");
    if(fpath == NULL){
        return -1;
	}
	
    fscanf(fpath, "path=%s\n", value);
    getdate(data);
    strcat(data, ".log");  
    strcat(value, "/");  
    strcat(value, data);  
    memset(logsetting->filepath, 0, sizeof(logsetting->filepath));
    memcpy(logsetting->filepath, value, strlen(value));

    memset(value, 0, sizeof(value));
    fscanf(fpath, "level=%s\n", value);
    logsetting->loglevel = getloglevel(value);

	memset(value, 0, sizeof(value));
    fscanf(fpath, "maxfilelen=%s\n", value);
    logsetting->maxfilelen = atoi(value);

	memset(value, 0, sizeof(value));
    fscanf(fpath, "outputtype=%s\n", value);
    logsetting->outputtype = getoutputtype(value);
	
    fclose(fpath);
    return 0;
}  



