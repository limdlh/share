#include <stdio.h>
#include <stdlib.h>

#define MAINTENANCE_PATH              "/ubi/conf/cgi-bin/maintenance/"
#define MAINTENANCE_SERVERINFO              MAINTENANCE_PATH "ServerInfo.config"

#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)
 
int main(int argc, char *argv[])
{
    int a;
 
    /*获取GCC无法优化的值*/
    a  = atoi(argv[1]);
 
    //if (likely(a==2)) {
	if (a==2) {
        a++;
    } else {
        a--;
    }
 
    printf("%d\n", a);
	printf("MAINTENANCE_PATH: %s\n", MAINTENANCE_PATH);
    printf("MAINTENANCE_SERVERINFO: %s\n", MAINTENANCE_SERVERINFO);

    return 0;
}