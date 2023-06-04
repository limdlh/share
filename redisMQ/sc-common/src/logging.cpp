#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "logging.h"
static const char * COLORS[] =
	{ LOG_SEQ_FRED, LOG_SEQ_FRED, LOG_SEQ_FRED, LOG_SEQ_FRED, LOG_SEQ_FYELLOW, LOG_SEQ_FCYAN, LOG_SEQ_FGREEN,
LOG_SEQ_DEFAULT_COLOR};

static const char* const priorities[] = {
    "FATAL", 
    "ALERT",
    "CRIT",
    "ERROR",
    "WARN",
    "NOTICE",
    "INFO",
    "DEBUG",
};

#define DEFAULT_LOGGING_FILENAME "itworks.log"
#define MIN_LOGGING_FILE_SIZE	(1024 * 1024)
#define LOGGING_BUFFER_SIZE	32768

const char *logging_format_string = "%s %6s [%s] %s:%d:%d ";
char logging_filename[32];
char logging_directory[256];
int  logging_history_files = DEFAULT_LOGGING_HISTORY_FILES;
long logging_file_size = DEFAULT_LOGGING_FILE_SIZE;

long  current_file_size = 0;
FILE *logging_stream = NULL;
char catalog[32];
bool logging_consle=true;
int logging_level = PRIORITY_INFO;
char filename_part[32];
char filename_ext[8];
char log_filename[512];
bool logging_initialized = false;
pthread_mutex_t logging_mutex = PTHREAD_MUTEX_INITIALIZER;


/*
 * 将日期时间转换为字符串
 * 按缺省格式将日期值转换为SQL格式的字符串
 * @param  tlong 时间戳
 * @return 日期字符串
 */

const char *myctime(const time_t *tlong) {
	struct tm *localtm = localtime(tlong);
	static char tBuff[64];

	memset(tBuff, 0, 64);
	if (*tlong == 0)
		return tBuff;

	sprintf(tBuff, "%04d-%02d-%02d %02d:%02d:%02d", localtm->tm_year + 1900,
			localtm->tm_mon + 1, localtm->tm_mday, localtm->tm_hour,
			localtm->tm_min, localtm->tm_sec);
	return tBuff;
}
void logging_close_consle(){logging_consle=false;};
void logging_start_consle(){logging_consle=true;}
void init_logging(const char *path,const char *prefix,const char *category,int num_of_history_file,int loglevel,int logfilesize)
{
	char *ptr = NULL;
	
	strcpy(logging_filename,prefix);
	strcpy(catalog,category);
	logging_file_size = logfilesize;	
	if (logging_file_size < MIN_LOGGING_FILE_SIZE)
		logging_file_size = MIN_LOGGING_FILE_SIZE;

	strcpy(logging_directory,path);

	if (logging_directory[strlen(logging_directory) -1] != '/')
		strcat(logging_directory,"/");

	logging_history_files = num_of_history_file;
	if (logging_history_files < 0)
		logging_history_files = 0;
	
	logging_level = loglevel;
	if (logging_level < PRIORITY_FATAL)
		logging_level = PRIORITY_FATAL;

	if (logging_level > PRIORITY_DEBUG)
		logging_level = PRIORITY_DEBUG;

	if (ptr = rindex(logging_filename,'/'))
	{
		strcpy(logging_filename,ptr + 1);
	}
	
	if (ptr = rindex(logging_filename,'.'))
	{
		*ptr = '\0';
		strcpy(filename_part,logging_filename);
		strcpy(filename_ext,ptr + 1);
		*ptr = '.';
	}else
	{
		strcpy(filename_part,logging_filename);
		strcpy(filename_ext,"log");
	}
	
	memset(log_filename,0,sizeof(log_filename));
	strcpy(log_filename,logging_directory);
	strcat(log_filename,logging_filename);

	logging_stream = fopen(log_filename,"a+");
	
	if (logging_stream == NULL)
	{
		fprintf(stderr,"Error occurred when opening logging file:%s for writing,errno=%d.\n",log_filename,errno);
		exit(-1);
	}
	setbuf(logging_stream,NULL);
	current_file_size = ftell(logging_stream);
	logging_initialized = true;
}
void uninit_logging()
{
	if (logging_stream) {
		fclose(logging_stream);
		logging_stream = NULL;
	}
}

const char* priority_to_string(int a_priority)
{
    if ( (a_priority < 0) || (a_priority > 7) ) 
    	a_priority = 7;

    return priorities[a_priority];
}    

void cycle_logging_files()
{
	char source[512];
	char dest[512];
	const char *filename_format_string="%s%s.%03d";
	struct stat f_stat;

	char cmdBuf[1024];
	if (logging_stream) {
		fclose(logging_stream);
		logging_stream = NULL;
	}

	for(int i = (logging_history_files - 1);i >= -1;i--)
	{
		if (i == (logging_history_files - 1))
		{
			snprintf(dest,sizeof(dest) - 1,filename_format_string,logging_directory,filename_part,i);
			continue;
		}else if (i == -1)
		{
			snprintf(source,sizeof(source) - 1,"%s%s",logging_directory,logging_filename);			
			if (!stat(source,&f_stat))
			{
				rename(source,dest);
			}
		}else
		{
			snprintf(source,sizeof(source) - 1,filename_format_string,logging_directory,filename_part,i);
			if (!stat(source,&f_stat))
			{
				rename(source,dest);
			}
			strcpy(dest,source);
		}
	}
	logging_stream = fopen(log_filename,"w");
	if (logging_stream == NULL)
	{
		fprintf(stderr,"Error occured when opening logging file:%s for writing,errno=%d.\n",log_filename,errno);
		exit(-1) ;//todo
	}
	setbuf(logging_stream,NULL);
}

void log(const char *file, int line, itwork_priority_t priority, const char *format, va_list vl)
{
	char logging_buffer[LOGGING_BUFFER_SIZE];

	char message[LOGGING_BUFFER_SIZE - 32];
	char ts_string[32];
	
	pthread_mutex_lock(&logging_mutex);
	time_t curtime;
	time(&curtime);

	strcpy(ts_string,ctime(&curtime));
	ts_string[strlen(ts_string) - 1] = '\0';

	if (!logging_initialized)
	{
		vsnprintf(message,LOGGING_BUFFER_SIZE-32,format,vl);
		fprintf(stderr,"logging service has not been initialized.\n");
		fprintf(stderr,logging_format_string,
		ts_string,priority_to_string(priority),catalog,file,line,pthread_self());
		fprintf(stderr, "%s", message);
		pthread_mutex_unlock(&logging_mutex);
		return ;
	}

	if (priority > logging_level) {
		pthread_mutex_unlock(&logging_mutex);
		return ;
	}

	

	if (logging_stream)
	{
		vsnprintf(message, LOGGING_BUFFER_SIZE - 32, format, vl);

		snprintf(logging_buffer, LOGGING_BUFFER_SIZE, logging_format_string,
			ts_string, priority_to_string(priority), catalog, file,line,pthread_self());
		fwrite(logging_buffer,strlen(logging_buffer),1,logging_stream);
		fwrite(message, strlen(message), 1, logging_stream);
		fwrite("\n",strlen("\n"),1,logging_stream);


		// print log to consle.  for test
		if(logging_consle)
			fprintf(stdout,"%s%s%s%s\n",COLORS[priority], logging_buffer, message,LOG_SEQ_DEFAULT_COLOR);

		
		current_file_size = ftell(logging_stream);
		if (current_file_size >= logging_file_size)
			cycle_logging_files();
	}
	pthread_mutex_unlock(&logging_mutex);
}
void log_fatal(const char *file, int line, const char *format, ...)
{
	va_list vl;
	va_start( vl, format );

	log(file, line, PRIORITY_FATAL, format, vl);
	va_end( vl );
}

void log_alert(const char *file, int line, const char *format, ...)
{
	va_list vl;
	va_start( vl, format );

	log(file, line, PRIORITY_ALERT, format, vl);
	va_end( vl );
}
void log_crit(const char *file, int line, const char *format, ...)
{
	va_list vl;
	va_start( vl, format );

	log(file, line, PRIORITY_CRIT, format, vl);
	va_end( vl );
}
void log_error(const char *file, int line, const char *format, ...)
{
	va_list vl;
	va_start( vl, format );

	log(file, line, PRIORITY_ERROR, format, vl);
	va_end( vl );
}
void log_warn(const char *file, int line, const char *format, ...)
{
	va_list vl;
	va_start( vl, format );

	log(file, line, PRIORITY_WARN, format, vl);
	va_end( vl );
}
void log_notice(const char *file, int line, const char *format, ...)
{
	va_list vl;
	va_start( vl, format );

	log(file, line, PRIORITY_NOTICE, format, vl);
	va_end( vl );
}
void log_info(const char *file, int line, const char *format, ...)
{
	va_list vl;
	va_start( vl, format );

	log(file, line, PRIORITY_INFO, format, vl);
	va_end( vl );
}
void log_debug(const char *file, int line, const char *format, ...)
{
	va_list vl;
	va_start( vl, format );

	log(file,line,PRIORITY_DEBUG,format,vl);
	va_end( vl );
}
