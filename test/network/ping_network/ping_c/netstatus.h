#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <errno.h>


#ifndef NETSTATUS
#define NETSTATUS
int NetIsOk();
int net_status_check();
#endif

#define MAX_WAIT_TIME   1
#define MAX_NO_PACKETS  1
#define ICMP_HEADSIZE 8 
#define PACKET_SIZE     4096


static struct timeval tvsend,tvrecv;
static struct sockaddr_in dest_addr,recv_addr;
static int sockfd;
static pid_t pid;
static char sendpacket[PACKET_SIZE];
static char recvpacket[PACKET_SIZE];
 
//函数定义
static void timeout(int signo);
static unsigned short cal_chksum(unsigned short *addr,int len);
static int pack(int pkt_no,char *sendpacket);
static int send_packet(int pkt_no,char *sendpacket);
static int recv_packet(int pkt_no,char *recvpacket);
static int unpack(int cur_seq,char *buf,int len);
static void tv_sub(struct timeval *out,struct timeval *in);
static void _CloseSocket();

int ping(char *ipaddr, int timeout);