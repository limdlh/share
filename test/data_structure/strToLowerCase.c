#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main()
{
	char s[15] = "ADJFLJEAJF";
	printf("s: %s\n", s);
	int k = strlen(s);
	for(int i = 0; i < k; i++)
	{
		s[i] = tolower(s[i]);
	}
	printf("s: %s\n", s);
	return 0;
}

