#include <stdio.h>
#include <sys/time.h>
#include "log.h"
 
static long long linux_get_sys_time_ms(void)
{
    long long time_ms = 0;
    struct timeval tv;
 
    gettimeofday(&tv, NULL);
    time_ms = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
 
    return (long long)time_ms;
}
 
int main(void)
{
    log_time_register(linux_get_sys_time_ms);
 
    char ch = 'a';
    char str[10] = "ZhengN";
    float float_val = 10.10;
    int num = 88;
    double double_val = 10.123456;
    LOG_D("字符为 %c \n", ch);
    LOG_D("字符串为 %s \n" , str);
    LOG_D("浮点数为 %f \n", float_val);
    LOG_D("整数为 %d\n" , num);
    LOG_R("双精度值为 %lf \n", double_val);
    LOG_R("八进制值为 %o \n", num);
    LOG_D("十六进制值为 %x \n", num);
 
    return 0;
}
