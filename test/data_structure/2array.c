#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_DEVICE_TYPE_SIZE 64
#define MAX_DEVICE_SERIAL_SIZE 128


typedef struct {
    int num;
    char PK[4][MAX_DEVICE_TYPE_SIZE];
    char DN[4][MAX_DEVICE_SERIAL_SIZE];
} PK_DN_LIST;


int main(void)
{
	char* str_PK_0 = "hello";
	char* str_DN_0 = "limd";
	char* str_PK_1 = "1111";
	char* str_DN_1 = "222222";
	char* str_PK_2 = "3333";
	char* str_DN_2 = "4444444";
	PK_DN_LIST A;
	PK_DN_LIST * p = &A;
	p->num = 12;
	strcpy(p->PK[0], str_PK_0);
	strcpy(p->DN[0], str_DN_0);
	strcpy(p->PK[1], str_PK_1);
	strcpy(p->DN[1], str_DN_1);
	strcpy(p->PK[2], str_PK_2);
	strcpy(p->DN[2], str_DN_2);
	
	printf("num: %d\n", p->num);
	printf("p->PK[0]: %s, p->DN[0]: %s\n", p->PK[0], p->DN[0]);
	printf("p->PK[1]: %s, p->DN[1]: %s\n", p->PK[1], p->DN[1]);
	printf("p->PK[2]: %s, p->DN[2]: %s\n", p->PK[2], p->DN[2]);
}