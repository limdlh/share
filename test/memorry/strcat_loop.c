#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FREE_CLEAR(x) if(x){free(x); x = NULL; printf("FREE_CLEAR %s\n",#x);}
	
#define PRINT_ENUM_STEP(action) printf(“Cur action - #action (%d)\r\n”,action);

int main() {
    char *firstName = "111";
    char *lastName = " 222";
	char *type = "333";
	int n = 123;
    //char *name = (char *) malloc(strlen(firstName) + strlen(lastName));
	char *name = (char *) malloc(512);
	strcat(name, "type: ");
	strcat(name, type);
	char string[16] = {0};
    //itoa(n,string,10);
	snprintf(string, 16, "%d setThing", n);
	strcat(name, "\n444");
	strcat(name, firstName); // 把firstName复制到name中
	strcat(name, "\n555");
	strcat(name, lastName); // 把lastName追加到name中
	strcat(name, "\n666");
	strcat(name, "\n777");
    printf("%s\n", name);
	//name = NULL;
	FREE_CLEAR(name);
	if(name == NULL)
	{
		 printf("name == NULL\n");
	}
	
	char *strTest = "[{\"heartbeat\":\"request\",\"value\":\"16\"}]";
	printf("strTest: %s\n", strTest);
    return 0;
}

