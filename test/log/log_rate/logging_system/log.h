/* =====================================================================================
 *       Filename:  logging.h
 *    Description:  日志打印模块，可分级打印到串口，也可以将日志写到本地文件
                    配置文件./conf/logging.conf
 *        Version:  1.0
 *        Created:  2022年09月5日
 *       Revision:  none
 *         Author:  Li Mingdong, 13674@hongyan.com.cn
 *   Organization:  Honyar IOT
 * ===================================================================================== */

#ifndef _LOGGING_H_
#define _LOGGING_H_

//#include <time.h>
#include <stdio.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/time.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
//#include "comHeader.h"


typedef unsigned char uint8_t;

//LOG("Now free and clear: %s", #x);//Add this line before free() in FREE_CLEAR() to trace memory crash
#define FREE_CLEAR(x) do {                  \
    if(x != NULL){                          \
        free(x);                            \
        x = NULL;}                          \
    } while(0)


#ifdef RKXXX
#define LOG_BUF_SIZE       512
#else
#define LOG_BUF_SIZE       51200
#endif
#define LOG_BUF_BIG_SIZE   5120
#define LOG_TIME_BUF_SIZE  32
#define MAX_FILE_PATH      256
#define MB                 1024*1024

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#define RWRWRW  (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH|S_IRWXU)//创建文件的权限

#define FATAL(...) log_print(HIGHLIGHT, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define ERROR(...) log_print(RED, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define WARN(...)  log_print(YELLOW, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define INFO(...)  log_print(BLUE, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define DEBUG(...) log_print(GREEN, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG(...)   log_print(BLACK, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)

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
    RED        = 1,
    YELLOW     = 2,
    BLUE       = 3,
    GREEN      = 4,
    BLACK      = 5
};

enum log_type
{
    NO_OUTPUT  = 0,
    WRITE_FILE = 1,
    STD_OUT    = 2
};

typedef struct logseting
{
    char filepath[MAX_FILE_PATH];
    long maxfilelen;
    uint8_t loglevel;
    uint8_t outputtype;
}logset;

void log_print(enum log_color color, const char *file, int line, const char *func, const char* fmt, ...);
int initlogfile(char *file_name);
bool getLogConf(logset *logsetting);
int setlog(logset *logsetting);
void destorylogging(void);
void initlogging(void);
void get_local_time(char* timeStr);
long get_file_size(char* filename);
uint8_t readconfig(char *path, logset *logset);
uint8_t getloglevel(char *levelstr);
uint8_t getoutputtype(char *typestr);


#endif


