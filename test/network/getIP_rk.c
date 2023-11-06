/*
Used for RK device
/output/rockchip_rk312x/host/bin/arm-buildroot-linux-gnueabihf-gcc -o getIP_rk getIP_rk.c
*/

#include <net/if.h>            /* for ifconf */
//#include <linux/sockios.h>     /* for net status mask */
//#include <netinet/in.h>        /* for sockaddr_in */
//#include <sys/socket.h>
//#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
	int inet_sock;
    struct ifreq ifr;
    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, "wlan0");

    if (ioctl(inet_sock, SIOCGIFADDR, &ifr) <  0){
        perror("ioctl wlan0");
        strcpy(ifr.ifr_name, "eth0");
        if (ioctl(inet_sock, SIOCGIFADDR, &ifr) <  0){
		    perror("ioctl eth0");
		}
    }

    char *ip = inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);
    close(inet_sock);

    printf("IP地址: %s\n",ip);
}