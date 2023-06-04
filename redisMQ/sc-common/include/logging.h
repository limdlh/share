#ifndef _MY_LOGGING_H_
#define _MY_LOGGING_H_

#define DEFAULT_LOGGING_HISTORY_FILES 8
#define DEFAULT_LOGGING_FILE_SIZE (1024*1024)

#define LOG_SEQ_ESC "\033["
/* Ansi Control character suffixes */
#define LOG_SEQ_HOME_CHAR 'H'
#define LOG_SEQ_HOME_CHAR_STR "H"
#define LOG_SEQ_CLEARLINE_CHAR '1'
#define LOG_SEQ_CLEARLINE_CHAR_STR "1"
#define LOG_SEQ_CLEARLINEEND_CHAR "K"
#define LOG_SEQ_CLEARSCR_CHAR0 '2'
#define LOG_SEQ_CLEARSCR_CHAR1 'J'
#define LOG_SEQ_CLEARSCR_CHAR "2J"
#define LOG_SEQ_DEFAULT_COLOR LOG_SEQ_ESC LOG_SEQ_END_COLOR	/* Reset to Default fg/bg color */
#define LOG_SEQ_AND_COLOR ";"	/* To add multiple color definitions */
#define LOG_SEQ_END_COLOR "m"	/* To end color definitions */
/* Foreground colors values */
#define LOG_SEQ_F_BLACK "30"
#define LOG_SEQ_F_RED "31"
#define LOG_SEQ_F_GREEN "32"
#define LOG_SEQ_F_YELLOW "33"
#define LOG_SEQ_F_BLUE "34"
#define LOG_SEQ_F_MAGEN "35"
#define LOG_SEQ_F_CYAN "36"
#define LOG_SEQ_F_WHITE "37"
/* Background colors values */
#define LOG_SEQ_B_BLACK "40"
#define LOG_SEQ_B_RED "41"
#define LOG_SEQ_B_GREEN "42"
#define LOG_SEQ_B_YELLOW "43"
#define LOG_SEQ_B_BLUE "44"
#define LOG_SEQ_B_MAGEN "45"
#define LOG_SEQ_B_CYAN "46"
#define LOG_SEQ_B_WHITE "47"
/* Preset escape sequences - Change foreground colors only */
#define LOG_SEQ_FBLACK LOG_SEQ_ESC LOG_SEQ_F_BLACK LOG_SEQ_END_COLOR
#define LOG_SEQ_FRED LOG_SEQ_ESC LOG_SEQ_F_RED LOG_SEQ_END_COLOR
#define LOG_SEQ_FGREEN LOG_SEQ_ESC LOG_SEQ_F_GREEN LOG_SEQ_END_COLOR
#define LOG_SEQ_FYELLOW LOG_SEQ_ESC LOG_SEQ_F_YELLOW LOG_SEQ_END_COLOR
#define LOG_SEQ_FBLUE LOG_SEQ_ESC LOG_SEQ_F_BLUE LOG_SEQ_END_COLOR
#define LOG_SEQ_FMAGEN LOG_SEQ_ESC LOG_SEQ_F_MAGEN LOG_SEQ_END_COLOR
#define LOG_SEQ_FCYAN LOG_SEQ_ESC LOG_SEQ_F_CYAN LOG_SEQ_END_COLOR
#define LOG_SEQ_FWHITE LOG_SEQ_ESC LOG_SEQ_F_WHITE LOG_SEQ_END_COLOR
#define LOG_SEQ_BBLACK LOG_SEQ_ESC LOG_SEQ_B_BLACK LOG_SEQ_END_COLOR
#define LOG_SEQ_BRED LOG_SEQ_ESC LOG_SEQ_B_RED LOG_SEQ_END_COLOR
#define LOG_SEQ_BGREEN LOG_SEQ_ESC LOG_SEQ_B_GREEN LOG_SEQ_END_COLOR
#define LOG_SEQ_BYELLOW LOG_SEQ_ESC LOG_SEQ_B_YELLOW LOG_SEQ_END_COLOR
#define LOG_SEQ_BBLUE LOG_SEQ_ESC LOG_SEQ_B_BLUE LOG_SEQ_END_COLOR
#define LOG_SEQ_BMAGEN LOG_SEQ_ESC LOG_SEQ_B_MAGEN LOG_SEQ_END_COLOR
#define LOG_SEQ_BCYAN LOG_SEQ_ESC LOG_SEQ_B_CYAN LOG_SEQ_END_COLOR
#define LOG_SEQ_BWHITE LOG_SEQ_ESC LOG_SEQ_B_WHITE LOG_SEQ_END_COLOR
/* Preset escape sequences */
#define LOG_SEQ_HOME LOG_SEQ_ESC LOG_SEQ_HOME_CHAR_STR
#define LOG_SEQ_CLEARLINE LOG_SEQ_ESC LOG_SEQ_CLEARLINE_CHAR_STR
#define LOG_SEQ_CLEARLINEEND LOG_SEQ_ESC LOG_SEQ_CLEARLINEEND_CHAR
#define LOG_SEQ_CLEARSCR LOG_SEQ_ESC LOG_SEQ_CLEARSCR_CHAR LOG_SEQ_HOME


typedef enum {
	PRIORITY_FATAL = 0,
	PRIORITY_ALERT = 1,
	PRIORITY_CRIT = 2,
	PRIORITY_ERROR = 3,
	PRIORITY_WARN = 4,
	PRIORITY_NOTICE = 5,
	PRIORITY_INFO = 6,
	PRIORITY_DEBUG = 7,
}itwork_priority_t;

#define DEFAULT_LOGGING_LOG_LEVEL ECC_PRIORITY_INFO

#define FATAL(...)  do{log_fatal( __FILE__, __LINE__,__VA_ARGS__);}while(0)
#define ALERT(...)  do{log_alert( __FILE__, __LINE__,__VA_ARGS__);}while(0)
#define CRIT(...)  do{log_crit( __FILE__, __LINE__,__VA_ARGS__);}while(0)
#define ERROR(...)  do{log_error( __FILE__, __LINE__,__VA_ARGS__);}while(0)
#define WARN(...)  do{log_warn( __FILE__, __LINE__,__VA_ARGS__);}while(0)
#define NOTICE(...)  do{log_notice( __FILE__, __LINE__,__VA_ARGS__);}while(0)
#define INFO(...)  do{log_info( __FILE__, __LINE__,__VA_ARGS__);}while(0)
#define DEBUG(...)  do{log_debug( __FILE__, __LINE__,__VA_ARGS__);}while(0)
#define ASSERT(...)  do{log_fatal( __FILE__, __LINE__,__VA_ARGS__);}while(0)


extern "C"{
	void log_fatal(const char *file, int line, const char *format, ...);
	void log_alert(const char *file, int line, const char *format, ...);
	void log_crit(const char *file, int line, const char *format, ...);
	void log_error(const char *file, int line, const char *format, ...);
	void log_warn(const char *file, int line, const char *format, ...);
	void log_notice(const char *file, int line, const char *format, ...);
	void log_info(const char *file, int line, const char *format, ...);
	void log_debug(const char *file, int line, const char *format, ...);

	void init_logging(const char *path, const char *prefix, const char *category, int num_of_history_file, int logglevel, int logfilesize);
	void uninit_logging();
};
#endif
