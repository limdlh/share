#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void showTime(){
    time_t now;
    struct tm date;
    now = time(NULL)+ 8*3600;
    localtime_r(&now, &date);
    printf("%04d年%02d月%02d日 %02d时%02d分%02d秒 星期%d \n", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec, date.tm_wday);
	sleep(1);
}

int main(){
	printf("start to print date, time\n");
	for(int i = 0; i < 1000; i++){
		showTime();
	}
}
