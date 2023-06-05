//从共享内存中读取数据
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define sem_name "mysem"

int main(){
	int shmid;
	sem_t *sem;
	int i = 1;
	key_t shmkey;
	shmkey = ftok("ff_player_status", 0);
      struct Status{
		char msg[10];
		int status;
	};
	
	sem = sem_open(sem_name, 0, 0644, 0);
	if(sem == SEM_FAILED){
		printf("unable open semapore\n");
		sem_close(sem);
		exit(-1);
	}
	shmid = shmget(shmkey, 0, 0666);
	if(shmid == -1){
		printf("shmget failed\n");
		exit(0);
	}
	
	struct Status* addr;
	addr = (struct Status*)shmat(shmid, 0, 0);
	if(addr == (struct Status*)-1){
		printf("shm shmat is failed\n");
		exit(0);
	}
	printf("读进程映射的共享内存地址 = %p\n", addr);
	
	while(1){
		sem_wait(sem);
		if(addr->status >= 0){//说明共享内存已经写入数据
		  printf("读进程：绑定到共享内存 %p : msg %d %s, status: %d \n", addr, i, addr->msg, addr->status);
		}
		sem_post(sem);
		sleep(1);
	}
	
	sem_close(sem);
	
	if(shmdt(addr) == -1) printf("shmdt failed\n");
	if(shmctl(shmid, IPC_RMID, NULL) == -1) printf("shmctl delete error\n");
}