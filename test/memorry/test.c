#include <stdio.h>
#include <stdlib.h>
int main()
{
	int num = 123;
	int *p = NULL;
	printf("limd\n");
	p = &num;
	printf("*p: %d\n", *p);
	
	char *pStr;
	if(pStr == NULL)
	{
		printf("pStr == NULL\n");
	}
	free(pStr);
}