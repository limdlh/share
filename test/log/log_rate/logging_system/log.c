/* =====================================================================================
 *       Filename:  logging.c
 *    Description:  日志打印模块，可分级打印到串口，也可以将日志写到本地文件
                    配置文件./conf/logging.conf
 *        Version:  1.0
 *        Created:  2022年09月5日
 *       Revision:  none
 *         Author:  Li Mingdong, 13674@hongyan.com.cn
 *   Organization:  Honyar IOT
 * ===================================================================================== */

#include <sys/types.h>
//#include <sys/stat.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
//#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "log.h"


static char *logPrintBuf = NULL;
//static char timeStr[LOG_TIME_BUF_SIZE];
static int size = LOG_BUF_SIZE;

uint8_t debug_type = STD_OUT;
uint8_t debug_level = LEVEL_LOG;
int fd_log_file;//日志文件的句柄


int initlogfile(char *file_name)
{
    int fd = -1;
    struct stat buf;

    if(!file_name)
    {
        WARN("empty file_name: %s", file_name);
        return -1;
    }

    //将日志输入到对应的文件
    int rv = access(file_name, F_OK);//判断文件是否存在
    if(rv < 0)
    {
        INFO("%s doesn't exist, need to create a log file", file_name);


        char logPath[MAX_FILE_PATH] = {0};
        char* pch = strrchr(file_name, '/');
        if(pch != NULL)
        {
            int nPos = pch - file_name;
            LOG("Last occurence of '/' found at %d", nPos);
            //buff[nPos] = '\0';
            strncpy(logPath, file_name, nPos);
            LOG("logPath: %s", logPath);
            if(access(logPath, F_OK) < 0)
            {
                LOG("logPath[%s] doesn't exist, now create it", logPath);
                fd = mkdir(logPath, RWRWRW);
                if(fd < 0)
                {
                    WARN("create logPath: %s failure", logPath);
                    return -2;
                }
            }
        }


        umask(0);//为进程设置文件模式创建屏蔽字, 
        fd = open(file_name, O_RDWR|O_CREAT|O_APPEND, RWRWRW);//屏蔽字为1的位，文件mode中的相应位一定关闭
        if(fd < 0)
        {
            WARN("open %s failure", file_name);
            return -2;
        }
        DEBUG("open %s successful", file_name);
    }
    else
    {
        DEBUG("%s exist", file_name);
        fd = open(file_name, O_RDWR|O_CREAT|O_APPEND, RWRWRW);//打开文件，设置对应的权限
        if(fd < 0)
        {
            WARN("open %s failure", file_name);
            return -2;
        }
        DEBUG("open %s successful", file_name);
    }

    rv = dup2(fd, STDOUT_FILENO);//将标准输出重定向到文件fd
    if(rv < 0)
    {
        WARN("dup2 %d to stdout failure", fd);
        return -4;
    }
    rv = fstat(fd, &buf);//获取日志文件的信息
    if(rv < 0)
    {
        WARN("get last time used failure");
    }
    else
    {
        INFO("File size: %lld bytes", (long long) buf.st_size);//打印截止到现在的文件大小
        INFO("Time of last access: %s, last modification: %s", ctime((const time_t *)&buf.st_atim), ctime((const time_t *)&buf.st_mtim));
    }
    return fd;
}

void get_local_time(char* timeStr)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm* ptm = localtime (&(tv.tv_sec));
    //strftime(timeStr, LOG_TIME_BUF_SIZE, "%Y/%m/%d %H:%M:%S", ptm); //输出格式为: 2018/12/09 10:48:31.391
    //strftime(timeStr, LOG_TIME_BUF_SIZE, "%Y|%m|%d %H-%M-%S", ptm); //输出格式为: 2018|12|09 10-52-28.302
    strftime(timeStr, LOG_TIME_BUF_SIZE, "%Y-%m-%d %H:%M:%S", ptm); //输出格式为: 2018-12-09 10:52:57.200
    //strftime(timeStr, LOG_TIME_BUF_SIZE, "%Y\\%m\\%d %H-%M-%S", ptm); //输出格式为: 2018\12\09 10-52-28.302

    int milliseconds = tv.tv_usec / 100; //从微秒计算毫秒

    char buffer[16] = {0};
    sprintf(buffer, ".%04d", milliseconds); //以秒为单位打印格式化后的时间日期，小数点后为毫秒
    strcat(timeStr, buffer);
}

void log_print(enum log_color color, const char *file, int line, const char *func, const char* fmt, ...)
{
    if (debug_type == NO_OUTPUT)
    {
        return;
    }

    if(logPrintBuf == NULL)
    {
        if ((logPrintBuf = (char *) malloc(LOG_BUF_SIZE * sizeof(char))) == NULL)
        {
            ERROR("malloc error!!!");
            return;
        }
        size = LOG_BUF_SIZE;
    }

    char timeStr[LOG_TIME_BUF_SIZE] = {0};
    get_local_time(timeStr);
    va_list ap;
    va_start(ap, fmt);
    int nLogStrLen = vsnprintf(logPrintBuf, size, fmt, ap);
    if(nLogStrLen >= size)
    {
        if(nLogStrLen < LOG_BUF_BIG_SIZE)
        {
            size = LOG_BUF_BIG_SIZE * sizeof(char);
        }
        else
        {
            size = (nLogStrLen + 1) * sizeof(char);
        }

        INFO("log content: %d, equal or more than %d, now extand logPrintBuf to be: %d", nLogStrLen, LOG_BUF_SIZE, size);

        if ((logPrintBuf = (char *)realloc(logPrintBuf, size)) == NULL)
        {
            ERROR("realloc error!!!");
            return;
        }

        nLogStrLen = vsnprintf(logPrintBuf, size, fmt, ap);
        if (nLogStrLen < 0 || nLogStrLen >= size)
        {
            WARN("vsnprintf: %d, size: %d, log content buffer vsnprintf() error, please check!!!!", nLogStrLen, size);
        }
    }
    else if(nLogStrLen < 0)
    {
        WARN("vsnprintf: %d, size: %d, log content buffer vsnprintf() error, please check!!!!", nLogStrLen, size);
    }
    va_end(ap);

    switch(color)
    {
        case HIGHLIGHT:
            if(debug_level >= LEVEL_FATAL)
            {
                if (debug_type == STD_OUT)
                {
                    printf("\033[5;7;31m[F][%s,%s:%d %s()] %s\033[0m\n", timeStr, file, line, func, logPrintBuf);
                }
                else if (debug_type == WRITE_FILE)
                {
                    printf("[F][%s,%s:%d %s()] %s\n", timeStr, file, line, func, logPrintBuf);
                }
            }
            break;
        case RED:
            if(debug_level >= LEVEL_ERROR)
            {
                if (debug_type == STD_OUT)
                {
                    printf("\033[31m[E][%s,%s:%d %s()] %s\033[0m\n", timeStr, file, line, func, logPrintBuf);
                }
                else if (debug_type == WRITE_FILE)
                {
                    printf("[E][%s,%s:%d %s()] %s\n", timeStr, file, line, func, logPrintBuf);
                }
            }
            break;
        case YELLOW:
            if(debug_level >= LEVEL_WARN)
            {
                if (debug_type == STD_OUT)
                {
                    printf("\033[33m[W][%s,%s:%d %s()] %s\033[0m\n", timeStr, file, line, func, logPrintBuf);
                }
                else if (debug_type == WRITE_FILE)
                {
                    printf("[W][%s,%s:%d %s()] %s\n", timeStr, file, line, func, logPrintBuf);
                }
            }
            break;
        case BLUE:
            if(debug_level >= LEVEL_INFO)
            {
                if (debug_type == STD_OUT)
                {
                    printf("\033[34m[I][%s,%s:%d %s()] %s\033[0m\n", timeStr, file, line, func, logPrintBuf);
                }
                else if (debug_type == WRITE_FILE)
                {
                    printf("[I][%s,%s:%d %s()] %s\n", timeStr, file, line, func, logPrintBuf);
                }
            }
            break;
        case GREEN:
            if(debug_level >= LEVEL_DEBUG)
            {
                if (debug_type == STD_OUT)
                {
                    printf("\033[32m[D][%s,%s:%d %s()] %s\033[0m\n", timeStr, file, line, func, logPrintBuf);
                }
                else if (debug_type == WRITE_FILE)
                {
                    printf("[D][%s,%s:%d %s()] %s\n", timeStr, file, line, func, logPrintBuf);
                }
            }
            break;
        case BLACK:
            if(debug_level >= LEVEL_LOG)
            {
                printf("[L][%s,%s:%d %s()] %s\n", timeStr, file, line, func, logPrintBuf);
            }
            break;
        default:
            WARN("No operation for color: %d !!!!!", color);
            break;
    }

    if (nLogStrLen >= LOG_BUF_BIG_SIZE)
    {
        LOG("FREE_CLEAR --> logPrintBuf, nLogStrLen: %d, size: %d", nLogStrLen, size);
        size = LOG_BUF_SIZE;
        FREE_CLEAR(logPrintBuf);
    }
}

long get_file_size(char* filename)
{
    long length = 0;
    FILE *fp = NULL;

    fp = fopen(filename, "rb");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
    }

    if (fp != NULL)
    {
        fclose(fp);
        fp = NULL;
    }
    return length;
}

int setlog(logset *logsetting)
{
    if(NULL == logsetting)
    {
        ERROR("NULL == logsetting");
        return -1;
    }

    debug_type = logsetting->outputtype;
    debug_level = logsetting->loglevel;
    logsetting->maxfilelen = logsetting->maxfilelen*MB;  //unit of maxfilelen in logging.conf is MB

    if (debug_type == WRITE_FILE)
    {
        long length = get_file_size(logsetting->filepath); // 文件超过最大限制, 删除
        if (length > logsetting->maxfilelen)
        {
            INFO("file: %s, length: %ld > maxfilelen: %ld, log file is larger than size in logging.conf, remove it!!!", \
                logsetting->filepath, length, logsetting->maxfilelen);
            unlink(logsetting->filepath); // 删除文件
        }

        fd_log_file = initlogfile(logsetting->filepath);
        if (fd_log_file < 0)
        {   //日志文件初始化失败，转为终端打印日志
            debug_type = STD_OUT;
        }
    }
    return fd_log_file;
}

void destorylogging(void)
{
    INFO("destory logging >>");
    close(fd_log_file);
    FREE_CLEAR(logPrintBuf);
}

void initlogging(void)
{
    INFO("init log module");
    logset logsetting;
    if(getLogConf(&logsetting) == false)
    {
        WARN("get log set fail!");
    }

    if(setlog(&logsetting) < 0)
    {
        WARN("init logging data fail!");
    }
    INFO("init logging finished!");
}

/* 日志设置信息 */
bool getLogConf(logset *logsetting)
{
    if(NULL == logsetting)
    {
        return false;
    }
    char path[MAX_FILE_PATH] = {0x0};
    getcwd(path, sizeof(path));
    strcat(path, "/conf/logging.conf");
    if(access(path, F_OK)==0)
    {
        if(readconfig(path, logsetting) != 0)
        {//无法打印到日志文件，则打印到终端
            logsetting->loglevel = LEVEL_ALL;
            logsetting->outputtype = STD_OUT;
            logsetting->maxfilelen = 1*MB;
            memcpy(logsetting->filepath, "./", strlen("./"));
        }
    }
    else
    {//无法打印到日志文件，则打印到终端
        logsetting->loglevel = LEVEL_ALL;
        logsetting->outputtype = STD_OUT;
        logsetting->maxfilelen = 1*MB;
        memcpy(logsetting->filepath, "./", strlen("./"));
    }
    return true;
}

uint8_t getloglevel(char *levelstr)
{
    uint8_t level = 255;
    if(strcmp("fatal", levelstr) == 0)
    {
        level = LEVEL_FATAL;
    }
    else if(strcmp("error", levelstr) == 0)
    {
        level = LEVEL_ERROR;
    }
    else if(strcmp("warn", levelstr) == 0)
    {
        level = LEVEL_WARN;
    }
    else if(strcmp("info", levelstr) == 0)
    {
        level = LEVEL_INFO;
    }
    else if(strcmp("debug", levelstr) == 0)
    {
        level = LEVEL_DEBUG;
    }
    else if(strcmp("log", levelstr) ==  0)
    {
        level = LEVEL_LOG;
    }
    else if(strcmp("all", levelstr) ==  0)
    {
        level = LEVEL_LOG;
    }
    else
    {
        WARN("Data configure error in logging.conf, item \'level\' could be: fatal, error, warn, info, debug, log or all");
        level = LEVEL_WARN;
    }
    return level;
}

uint8_t getoutputtype(char *typestr)
{
    uint8_t type = NO_OUTPUT;
    if(strcmp("NO_OUTPUT", typestr) == 0)
    {
        type = NO_OUTPUT;
        INFO("You input NO_OUTPUT, no log print anymore!");
    }
    else if(strcmp("N", typestr) == 0)
    {
        type = NO_OUTPUT;
        INFO("You input N, no log print anymore!");
    }
    else if(strcmp("n", typestr) == 0)
    {
        type = NO_OUTPUT;
        INFO("You input n, no log print anymore!");
    }
    else if(strcmp("WRITE_FILE", typestr) == 0)
    {
        type = WRITE_FILE;
        INFO("You input WRITE_FILE, write log to local file!");
    }
    else if(strcmp("W", typestr) == 0)
    {
        type = WRITE_FILE;
        INFO("You input W, write log to local file!");
    }
    else if(strcmp("w", typestr) == 0)
    {
        type = WRITE_FILE;
        INFO("You input w, write log to local file!");
    }
    else if(strcmp("s", typestr) == 0)
    {
        type = STD_OUT;
        INFO("You input s, print log to Standard Out!");
    }
    else if(strcmp("S", typestr) == 0)
    {
        type = STD_OUT;
        INFO("You input S, print log to Standard Out!");
    }
    else if(strcmp("STD_OUT", typestr) == 0)
    {
        type = STD_OUT;
        INFO("You input STD_OUT, print log to Standard Out!");
    }
    else
    {
        WARN("Data configure error in logging.conf, item \'outputtype\' could be: NO_OUTPUT(N/n), WRITE_FILE(W/w) or STD_OUT(S/s)");
        type = STD_OUT;
    }
    return type;
}

static char* getdate(char *date)
{
    time_t timer=time(NULL);
    strftime(date, 11, "%y-%m-%d", localtime(&timer));
    return date;
}

uint8_t readconfig(char *path, logset *logsetting)
{
    char value[MAX_FILE_PATH] = {0x0};
    char data[50] = {0x0};

    FILE *fpath = fopen(path, "r");
    if(fpath == NULL)
    {
        return 1;
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




