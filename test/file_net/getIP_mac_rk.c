/*
Used for RK device
/output/rockchip_rk312x/host/bin/arm-buildroot-linux-gnueabihf-gcc -o getIP_mac_rk getIP_mac_rk.c
*/


#include <net/if.h>            /* for ifconf */
#include <linux/sockios.h>     /* for net status mask */
#include <netinet/in.h>        /* for sockaddr_in */
#include <sys/socket.h>
#include <sys/types.h>
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
    ioctl(inet_sock,SIOCGIFHWADDR,&ifr);
    static char mac[32];
    sprintf(mac,"%02x-%02x-%02x-%02x-%02x-%02x",
        ifr.ifr_hwaddr.sa_data[0]&0x000000ff,ifr.ifr_hwaddr.sa_data[1]&0x000000ff,ifr.ifr_hwaddr.sa_data[2]&0x000000ff,
        ifr.ifr_hwaddr.sa_data[3]&0x000000ff,ifr.ifr_hwaddr.sa_data[4]&0x000000ff,ifr.ifr_hwaddr.sa_data[5]&0x000000ff);
    close(inet_sock);

    printf("IP地址: %s\n",ip);
    printf("MAC地址: %s\n", mac);
}