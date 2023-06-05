#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int makeFile(){
   FILE *fileP=NULL;
 
   fileP=fopen("hello.txt","w");
   fputs("Hello,World!\n",fileP);
   fclose(fileP);
    return 0;
}

int changeFileName(){
    int ret=access("hello.txt",F_OK);
    if(ret==0){
        printf("文件存在\n");
    }else if(ret==-1){
        printf("文件不存在\n");
    }
	
	ret = rename("hello.txt","helloTest.txt");//int res = rename(oldpath, newpath);
	printf("ret: %d\n", ret);
	return 0;
}

int main() {
    changeFileName();
    return 0;
}
 
int changemod()
{
  chmod("hello.txt", S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
  return 0;
}

int chmodOnlyRead(){
  chmod("hello.txt", S_IRUSR|S_IRGRP|S_IROTH);
  return 0;
}
