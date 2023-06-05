#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int i = 0;
    while(i < 1000000){
        malloc(1024000);
	usleep(1);
	printf("i: %d\n", i++);
    }
    printf("------------------------------->>>>>>>>>>>>>>>>>>>>>>>>");
    return 0;
}

