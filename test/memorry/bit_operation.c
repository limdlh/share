
#include <stdio.h>

int main()
{
    char refresh_com = 0;
	printf("refresh_com(c): %c, refresh_com(d): %d\n", refresh_com, refresh_com);
	
	if((refresh_com & 0x03) == 1)
	{
		printf("(refresh_com & 0x03) == 1\n");
	}
	
	refresh_com |= 1;
	printf("refresh_com |= 1 --> refresh_com(c): %c, refresh_com(d): %d\n", refresh_com, refresh_com);
	
	if((refresh_com & 0x03) == 1)
	{
		printf("(refresh_com & 0x03) == 1\n");
	}
	
	refresh_com |= 2;
	printf("refresh_com |= 2 --> refresh_com(c): %c, refresh_com(d): %d\n", refresh_com, refresh_com);
	
	if((refresh_com & 0x03) == 1)
	{
		printf("(refresh_com & 0x03) == 1\n");
	}
	
	refresh_com &= 0xfd;
	printf("refresh_com &= 0xfd --> refresh_com(c): %c, refresh_com(d): %d\n", refresh_com, refresh_com);
	
	if((refresh_com & 0x03) == 1)
	{
		printf("(refresh_com & 0x03) == 1\n");
	}
	
	refresh_com = 2;
	printf("refresh_com = 2 --> refresh_com(c): %c, refresh_com(d): %d\n", refresh_com, refresh_com);
	if((refresh_com & 0x03) == 1)
	{
		printf("(refresh_com & 0x03) == 1\n");
	}
	
	refresh_com &= 0xfd;
	printf("refresh_com &= 0xfd --> refresh_com(c): %c, refresh_com(d): %d\n", refresh_com, refresh_com);
	
	if((refresh_com & 0x03) == 1)
	{
		printf("(refresh_com & 0x03) == 1\n");
	}
}

