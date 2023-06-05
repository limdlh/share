#include "stdio.h"  
#include "stdlib.h"  
#include "log.h"  
int main(int argv,char**argc){
    printf("%s\n",argc[0]);

    //初始化日志  
    if(initlog(info)!=0)
        return -1;
	
    logwrite(info,"%s","hello world!");
    logwrite(debug,"%s","h.e.l.l.o w.o.r.l.d!"); 
    logwrite(warn,"%s","h e l l o w o r l d!");
    logwrite(error,"%s","hallo world!");
    return 0;
}
