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
	char* str_PK_0 = "制冷";
	char* str_DN_0 = "制热";
	char* str_PK_1 = "除湿";
	char* str_DN_1 = "送风";
	char* str_PK_2 = "自动";
	char* str_DN_2 = "低速";
	char* str_PK_3 = "he";
	char* str_DN_3 = "li";
	PK_DN_LIST A;
	PK_DN_LIST * p = &A;
	p->num = 12;
	strcpy(p->PK[0], str_PK_0);
	strcpy(p->DN[0], str_DN_0);
	strcpy(p->PK[1], str_PK_1);
	strcpy(p->DN[1], str_DN_1);
	strcpy(p->PK[2], str_PK_2);
	strcpy(p->DN[2], str_DN_2);
	strcpy(p->PK[3], str_PK_3);
	strcpy(p->DN[3], str_DN_3);
	
	printf("num: %d\n", p->num);
	printf("p->PK[0]: %s, %d\t p->DN[0]: %s, %d\n", p->PK[0], strlen(p->PK[0]), p->DN[0], strlen(p->DN[0]));
	printf("p->PK[1]: %s, %d\t p->DN[1]: %s, %d\n", p->PK[1], strlen(p->PK[1]), p->DN[1], strlen(p->DN[1]));
	printf("p->PK[2]: %s, %d\t p->DN[2]: %s, %d\n", p->PK[2], strlen(p->PK[2]), p->DN[2], strlen(p->DN[2]));
	printf("p->PK[3]: %s, %d\t p->DN[3]: %s, %d\n", p->PK[3], strlen(p->PK[3]), p->DN[3], strlen(p->DN[3]));
	//printf("p->PK[1]: %s, p->DN[1]: %s\n", p->PK[1], p->DN[1]);
	//printf("p->PK[2]: %s, p->DN[2]: %s\n", p->PK[2], p->DN[2]);
}