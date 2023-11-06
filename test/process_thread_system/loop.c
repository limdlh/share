#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	int count = 0;
	while(1)
	{
		sleep(3);
	    printf("count: %d\n", count++);
	}
}

