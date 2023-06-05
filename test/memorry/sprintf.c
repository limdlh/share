
#include <stdio.h>

int main()
{
    char buf[128] = { 0 };

    sprintf(buf,"www.codersrc.com\n");
    printf("%s", buf);

    sprintf(buf,"www.codersrc.com age:%d\n",17);
    printf("%s", buf);

    sprintf(buf,"www.codersrc.com age:%d name:%s\n",17, "zhangsan");
    printf("%s", buf);

    sprintf(buf,"www.codersrc.com age:%d name:%s height:%f\n",17, "zhangsan",1.75);
    printf("%s", buf);
	
	sprintf(buf,"%s%s/switch_open.png\n","--DEVICE_INFO_ICON--","--device>type--","--device>type--");//warning: too many arguments for format
	printf("%s", buf);
	
	sprintf(buf,"%s%s/switch_open.png\n","--DEVICE_INFO_ICON--","--device>type--");
	printf("%s", buf);
}

