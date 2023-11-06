//202301012

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
int checkVersion(char *chCurVer, int criVer[])
{
    printf ("criVer[0]: %d, criVer[1]: %d\n", criVer[0], criVer[1]);

    char* chVer = strtok(chCurVer, ".");
    if(chVer == NULL)
    {
        printf ("chVer == NULL\n");
        return 0;
    }

    int major_version = atoi(chVer);
    printf("chVer: %s, major_version: %d\n", chVer, major_version);
    if(major_version > criVer[0])
    {
        printf("major_version > 3\n");
        return 1;
    }
    else if(major_version < criVer[0])
    {
        return -1;
    }

    chVer = strtok(NULL, ".");
    if(chVer == NULL)
    {
        printf ("22 chVer == NULL\n");
        return 0;
    }

    int minor_version = atoi(chVer);
    printf("chVer: %s, minor_version: %d\n", chVer, minor_version);
    if(minor_version >= criVer[1])
    {
        printf("minor_version >= 2\n");
        return 1;
    }

    return -1;
}

int main ()
{
    //char strCurVer[] = "3.2.1-v1.0.0-6-gf608052";
    char strCurVer[] = "123";
    printf ("strCurVer %s\n", strCurVer);
    int criticalVer[] = {3, 2};//Since v3.2 internally the timeout wait is ignored, the lws scheduler is smart enough to stay asleep until an event is queued.
    int ret = checkVersion(strCurVer, criticalVer);
    if(ret > 0)
    {
        printf("strCurVer[%s] 大于等于 criticalVer[%d.%d]\n", strCurVer, criticalVer[0], criticalVer[1]);
    }
    else if(ret < 0) 
    {
        printf("strCurVer[%s] 小于 criticalVer[%d.%d]\n", strCurVer, criticalVer[0], criticalVer[1]);
    }
    else if(ret == 0) 
    {
        printf("strCurVer[%s] invalid!!!\n", strCurVer);
    }

   return(0);
}