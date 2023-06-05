#ifndef LOG_H
#define LOG_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
#define LOG_BUF_SIZE 1024
 
typedef long long (*get_sys_time_ms_def)(void);
 
enum log_color
{
    COLOR_NULL  = 0,
    RED    = 1,
    GREEN  = 2,
    YELLOW = 3,
    BLUE   = 4
};
 
void log_print(enum log_color color, const char *file, int line, const char *func, const char* fmt, ...);
void log_time_register(get_sys_time_ms_def p_get_sys_time_ms);
 
#define LOG_D(...) log_print(BLUE, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_R(...) log_print(RED, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif
