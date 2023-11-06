#include <stdio.h>
#include <string.h>
//#include <fcntl.h>
#include <unistd.h>

int main ()
{
	char* oldName = "./test.txt";
	char* newName = "./dir/test.txt";
	//char* newName = "./dir/";
	//int ret = link(oldName, newName);
	int ret = symlink(oldName, newName);  //ln -s oldName newName
	printf("ret: %d\n", ret);
	if(ret != 0)
	{
		printf("soft link failed, src[%s], des[%s]\n", oldName, newName);
	}
	else
	{
		printf("soft link Succeed, src[%s], des[%s]\n", oldName, newName);
	}

   return(0);
}

