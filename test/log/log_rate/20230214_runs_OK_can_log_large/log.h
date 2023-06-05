
#ifndef _LOG_H_
#define _LOG_H_

#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>



/*-----------------------------------------------------------------------------
 *  打印输出级别
 *-----------------------------------------------------------------------------*/
/*#define LEVEL_LOG           5
#define LEVEL_DEBUG         4
#define LEVEL_INFO          3
#define LEVEL_WARN          2
#define LEVEL_ERROR         1
#define LEVEL_FATAL         0*/


#define LOG_BUF_SIZE 16

#define RWRWRW  (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)//创建文件的权限

enum log_level
{
    LEVEL_FATAL = 0,
	LEVEL_ERROR = 1,
	LEVEL_WARN  = 2,
	LEVEL_INFO  = 3,
	LEVEL_DEBUG = 4,
	LEVEL_LOG   = 5,
	LEVEL_ALL
};

enum log_color
{
	HIGHLIGHT  = 0,
    RED    = 1,
    YELLOW = 2,
    BLUE   = 3,
    GREEN  = 4,
    BLACK  = 5
};

enum log_type
{
    NO_OUTPUT  = 0,
    WRITE_FILE = 1,
	STD_OUT    = 2
};

#define FATAL(...) log_print(HIGHLIGHT, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define ERROR(...) log_print(RED, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define WARN(...) log_print(YELLOW, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define INFO(...) log_print(BLUE, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define DEBUG(...) log_print(GREEN, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG(...) log_print(BLACK, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

int debug_type;
int debug_levet;

void log_print(enum log_color color, const char *file, int line, const char *func, const char* fmt, ...);


#endif

