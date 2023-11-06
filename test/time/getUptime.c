#include <stdio.h>
 
unsigned long getUptime()
{
    unsigned long uptime = 0;
    FILE *fp = fopen("/proc/uptime", "r");
    if(fp)
    {
        double runTime = 0;
        fscanf(fp, "%lf", &runTime);
        uptime = (unsigned long)runTime;
        fclose(fp);
    }
    return uptime;
}
 
int main()
{
    printf("uptime is %ld\n", getUptime());
	printf("uptime is %ld\n", getUptime()/60/60);
    return 0;   
}