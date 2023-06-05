#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SERPORT		80						//服务器开放的端口号
#define SERADDR		"183.232.231.174"   	//服务器开放的IP地址
#define PATHNAME 	"./msg.txt"
int main(void)
{
	struct sockaddr_in cliaddr = {0};
	int ret = -1;
	//第一步，先socket打开文件描述符
	int sockfd = -1;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sockfd)
	{
		perror("socket");
		return -1;
	}
	printf("socket\n打开文件描述符成功\n");
	
	//第二步 通过connect连接服务器
	cliaddr.sin_family = AF_INET; //设置地址族为ipv4
	cliaddr.sin_port = htons(SERPORT);//设置端口号 大于5000的端口是其他服务器预留的，还要设置字节序
	cliaddr.sin_addr.s_addr = inet_addr(SERADDR);	//　设置IP地址
	ret = connect(sockfd,(const struct sockaddr*)&cliaddr,sizeof(cliaddr));
	if (ret < 0)
	{
		perror("connect");
		return -1;
	}
	printf("与服务器connect成功\n");

	char recvBuf[1024 * 2] = {0};
	int num = -1;
	char sendbuf[1024] = {0};//HTTP协议的语法格式
	 sprintf(sendbuf, "GET / HTTP/1.1\r\n"
	 "Host:www.baidu.com\r\n"
	 "User-Agent:whatever\r\n"
	 "Accept-Type:*/*\r\n"
	 "Connection:close\r\n"
	 "X-Power:XXX\r\n"
	 "\r\n");
    
	//printf("sendbuf is : %s\nn",sendbuf);
	
	ret  = send(sockfd,sendbuf,strlen(sendbuf),MSG_NOSIGNAL);
	if (-1 == ret)
	{
		perror("send");
		return -1;
	}

	printf("正在接收信息...\n");
	
	num = recv(sockfd, recvBuf, sizeof(recvBuf),0);
	if(-1 == num)
	{
		perror("recv msg fail\n");
		return -1;
	}
	printf("the msg I recv is \n%s\n",recvBuf);
	printf("接收信息接收成功...\n");
	
	//创建一个文件，用来存储信息
	FILE *fp = NULL;
	fp = fopen(PATHNAME,"w+");
	if(NULL == fp)
	{
		perror("fopen");
		return -1;
	}
	printf("存储信息文件创建成功\n");
	//存储信息
	fwrite(recvBuf,sizeof(recvBuf),1,fp);
	printf("信息存储成功\n");
	//关闭文件
	fclose(fp);
	
	//重新打开文件，抓取时间Date
	fp = fopen(PATHNAME,"r");
	if(NULL == fp)
	{
		perror("fopen");
		return -1;
	}
	printf("重新打开文件，抓取时间Date\n");
	
	char file_str[256];
	int line = 0;	
	char target_buf[256] = {0};
	while(fgets(file_str,sizeof(file_str),fp))
	{
		line++;
		
		if(strstr(file_str,"Date:"))
		{
			printf("file_str %s\n",file_str);
			strcpy(target_buf,file_str);
			printf("target_buf %s\n",target_buf);
		}
	}
	printf("strlen of target_buf is %ld\n",strlen(target_buf));
	
	char month[12][5] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"};
	char week[7][5] = {"Mon","Thu","Wed","Thur","Fri","Sat","Sun"};
	char r_year[5] = {0};
	char r_month[5] = {0};
	char r_week[5] = {0};
	char r_day[3] = {0};
	char r_hour[3] = {0};
	char r_min[3] = {0};
	char r_sec[3] = {0};
	char *temp;
	int i,j,k;
	int sec, min ,hour, year ,day,i_month;
	char *month_ptr = NULL,*week_ptr = NULL,*gmt_ptr = NULL;
	char time[32] = {0};
	/***要解析的字符串Date: Sun, 24 Jan 2021 14:33:55 GMT **/
	//找月份
	for(i = 0;i < 12; i++)
	{
		month_ptr = strstr(target_buf,month[i]);
		if(NULL != month_ptr)
		{
			break;
		}
	}
	i_month = i + 1 ;
	k = 0;
	temp = month_ptr;
	while(k<3)
	{
		r_month[k] = *temp;
		temp++;
		k++;
	}
	//printf("real month is %s\n",r_month);
	//printf("real month is %d\n",i_month);
	//星期
	for(j = 0;j < 7; j++)
	{
		week_ptr = strstr(target_buf,week[j]);
		if(NULL != week_ptr)
		{
			break;
		}
	}
	k = 0;
	while(k<3)
	{
		r_week[k] = *week_ptr;
		week_ptr++;
		k++;
	}
	//printf("real week is %s\n",r_week);
	
	//找到"GMT",以GMT为基准计算年、日、时、分、秒
	gmt_ptr = strstr(target_buf,"GMT");
	printf("gmt is %s\n",gmt_ptr);
	
	//日
	k = 0;
	temp = gmt_ptr;
	while(k<2)
	{
		r_day[k] = *(temp - 21);
		temp++;
		k++;
	}
	day = atoi(r_day);
	//printf("real day is %d\n",day);
	
	//年
	k = 0;
	temp = gmt_ptr;
	temp = temp - 14;
	while(k<4)
	{
		r_year[k] = *temp;
		temp++;
		k++;
	}
	year = atoi(r_year);
	//printf("real year is %d\n",year);
	
	//时
	k = 0;
	temp = gmt_ptr;
	temp = temp - 9;
	while(k<2)
	{
		r_hour[k] = *temp;
		temp++;
		k++;
	}
	hour = atoi(r_hour) + 8; //东八区
	if(hour >= 24)
	{
		hour = hour - 24;
	}
		
	//printf("real hour is %d\n",hour);
	
	//分
	k = 0;
	temp = gmt_ptr;
	temp = temp - 6;
	while(k<2)
	{
		r_min[k] = *temp;
		temp++;
		k++;
	}
	min = atoi(r_min);
	//printf("real min is %d\n",min);
	
	//秒
	k = 0;
	temp = gmt_ptr;
	temp = temp - 3;
	while(k<2)
	{
		r_sec[k] = *temp;
		temp++;
		k++;
	}
	sec = atoi(r_sec);
	//printf("real sec is %d\n",sec);  
	sprintf(time,"%02d-%02d-%02d %02d:%02d:%02d",year,i_month,day,hour,min,sec);//月日时分秒>10
		
	
	printf("time is %s\n",time);
	char cmd[256] = "date -s";
	
	printf("cmd is %s\n",cmd);
	strcat(cmd," \"");
	strcat(cmd,time);
	strcat(cmd,"\"");
	printf("cmd is %s\n",cmd);
	system(cmd);
	return 0;
}
