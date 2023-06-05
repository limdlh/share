#include <stdio.h>
#include <stdlib.h>

void swap1(int* a,int* b)
{
	int tmp;   
	tmp = *a;    
	*a = *b;    
	*b = tmp;    
}

int main(){
	int a = 1;    
	int b = 2;
	printf("a: %d, b: %d\n", a, b);
	swap1(&a, &b);
	printf("a: %d, b: %d\n", a, b);
	return 0;
 }
