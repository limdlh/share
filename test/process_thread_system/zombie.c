#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/prctl.h>

void main(){
	if(!fork()){
		prctl(PR_SET_NAME, "limd", 0, 0);
		printf("child pid=%d\n", getpid());
		exit(12);
	}

	sleep(20);
	printf("parent pid=%d \n", getpid());
	exit(23);
}
