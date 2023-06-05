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
#define PATHNAME 	"./msg.txt"				//文件路径
//debug宏
#define DEBUG 		 
//#undef DEBUG
#ifdef DEBUG  
#define debug(format,...) printf("[debug]""File: "__FILE__", Line: %05d: "format"", __LINE__, ##__VA_ARGS__)  
#else  
#define debug(format,...)  
#endif


//全局变量
int sockfd = -1;
int ret = -1;

typedef struct msg_S
{
	char target_buf[256];
	char time[32];
}msg_S;

	//客户端函数
int client_init(void)
{
	//第一步，先socket打开文件描述符
	struct sockaddr_in cliaddr = {0};
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sockfd)
	{
		perror("socket");
		return -1;
	}
	debug("打开文件描述符成功\n");
	
	//第二步 通过connect连接服务器
	cliaddr.sin_family = AF_INET; //设置地址族为ipv4
	cliaddr.sin_port = htons(SERPORT);//设置端口号 
	cliaddr.sin_addr.s_addr = inet_addr(SERADDR);	//设置IP地址
	ret = connect(sockfd,(const struct sockaddr*)&cliaddr,sizeof(cliaddr));
	if (ret < 0)
	{
		perror("connect");
		return -1;
	}
	debug("与服务器connect成功\n");
}


//抓取文件
int msg_parse(msg_S *pMsg)
{
	//发送信息
	msg_S *ptarget_msg = pMsg;
	int num = -1;
	char sendbuf[1024] = {0};//HTTP协议的语法格式
	char recvBuf[1024 * 2] = {0};
	sprintf(sendbuf, "GET / HTTP/1.1\r\n"
	"Host:www.baidu.com\r\n"
	"User-Agent:whatever\r\n"
	"Accept-Type:*/*\r\n"
	"Connection:close\r\n"
	"X-Power:XXX\r\n"
	"\r\n");
    
	ret  = send(sockfd,sendbuf,strlen(sendbuf),MSG_NOSIGNAL);
	if (-1 == ret)
	{
		perror("send");
		return -1;
	}

	debug("正在接收信息...\n");
	//接收信息
	num = recv(sockfd, recvBuf, sizeof(recvBuf),0);
	if(-1 == num)
	{
		perror("recv msg fail\n");
		return -1;
	}
	//debug("the msg I recv is \n%s\n",recvBuf);
	debug("接收信息接收成功...\n");
	
	
	//创建一个文件，用来存储信息
	FILE *fp = NULL;
	fp = fopen(PATHNAME,"w+");
	if(NULL == fp)
	{
		perror("fopen");
		return -1;
	}
	debug("存储信息文件创建成功\n");
	//存储信息
	fwrite(recvBuf,sizeof(recvBuf),1,fp);
	debug("信息存储成功\n");
	//关闭文件
	fclose(fp);
	
	//抓取关键信息
	//重新打开文件
	debug("重新打开文件，抓取时间Date\n");
	char file_str[256];
	//FILE *fp = NULL;
	fp = fopen(PATHNAME,"r");
	if(NULL == fp)
	{
		perror("fopen");
		return -1;
	}
	
	//抓取时间Date
	while(fgets(file_str,sizeof(file_str),fp))
	{
		if(strstr(file_str,"Date:"))
		{
			debug("getmsg:\n%s\n",file_str);
			strcpy(ptarget_msg->target_buf,file_str);
			debug("target_buf:\n%s\n",ptarget_msg->target_buf);
		}
	}
}

	//日、年、时、分、秒解析函数
	/***要解析的字符串Date: Sun, 24 Jan 2021 14:33:55 GMT **/
	/*len:日期值的长度，比如2021-01-23 12:23:45 日 时 分 秒 的长度都是2，年的长度是4
	offset: 偏移量，例如，以GMT的G为基准，往前减3就是秒的起始地址，往前减去6就是分的起始地址
	char src[]: 用来存放年月日时分秒的字符串，最后统一用atoi转换成整型
	msg_S *pMsg：定义的一个结构体，从这里面获取target_msg，解析出GMT的首地址*/
int date_parse(const int len, const int offset , char src[], msg_S *pMsg)
{
	int k = 0;
	char *gmt_ptr = NULL;
	msg_S *ptarget_msg = pMsg;
	//找到"GMT",以GMT为基准计算年、日、时、分、秒
	gmt_ptr = strstr((*ptarget_msg).target_buf,"GMT");
	while(k < len)
	{
		src[k] = *(gmt_ptr - offset);
		gmt_ptr++;
		k++;
	}
	return atoi(src);
}

	//月份解析函数
int month_parse(msg_S *pMsg)
{
	msg_S *ptarget_msg = pMsg;
	char *month_ptr = NULL;
	int month;
	char month_arr[12][5] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"};	
	int i = 0;
	//找月份  不能以GMT为基准 因为thur 或者sept都是4个字节的，跟其他的3字节不一样，这样会出错的。
	for(i = 0;i < 12; i++)
	{
		month_ptr = strstr((*ptarget_msg).target_buf,month_arr[i]);
		if(NULL != month_ptr)
		{
			break;
		}
	}
	//debug("month i is %d\n",i);
	//debug("month_ptr is %s\n",month_ptr);
	month = i + 1 ;
	return month;
}

//把时间整合成一个字符串
int catenate_time(msg_S *pst_time)
{
	int sec, min ,hour, year ,day, month;
	char r_year[5] = {0};
	char r_month[5] = {0};
	char r_day[3] = {0};
	char r_hour[3] = {0};
	char r_min[3] = {0};
	char r_sec[3] = {0};
	
	msg_S *p = pst_time;
	
	/***要解析的字符串Date: Sun, 24 Jan 2021 14:33:55 GMT **/
	month = month_parse(p);				//月
	day = date_parse(2, 21, r_day, p);		//日
	year = date_parse(4, 14, r_year, p);	//年
	min = date_parse(2, 6, r_min, p);		//分
	sec = date_parse(2, 3, r_sec, p);		//秒
	hour = date_parse(2, 9, r_hour, p);	//时
	hour = hour + 8;					//时+8 北京时间
	if(hour >= 24)
		hour = hour - 24;
	sprintf(p->time,"%02d-%02d-%02d %02d:%02d:%02d",year,month,day,hour,min,sec);//格式化月日时分秒
}
//整合出设置时间的命令，然后执行命令
void func_settime(msg_S * const pst_time)
{
	msg_S *p = pst_time;
	
	debug("time is %s\n",p->time);
	char cmd[256] = "date -s";
	strcat(cmd," \"");
	strcat(cmd,p->time);
	strcat(cmd,"\"");
	debug("cmd is %s\n",cmd);
	system(cmd);
}

int main(void)
{
	msg_S st_time;
	memset(&st_time, 0, sizeof(msg_S));
	client_init();				//http 客户端
	msg_parse(&st_time);		//保存并获取信息
	catenate_time(&st_time);	//整合时间
	func_settime(&st_time);		//调用system命令去执行
	return 0;
}

