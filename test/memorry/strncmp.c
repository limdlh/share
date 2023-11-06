//20230317


#include <stdio.h>
#include <string.h>
 
int main ()
{
   //char* str = "20230317V01";
   char criticalVer[8] = "23.2.0";
   char strCurVer[32];
   int ret;
 
   //strcpy(criticalVer, "4.3.2-v1.0.0-6-gf608052");
   //strcpy(criticalVer, "3.2");
   strcpy(strCurVer, "3.2.1-v1.0.0-6-gf608052");
 
   ret = strncmp(strCurVer, criticalVer, 5);
   //ret = strncmp("99", criticalVer, 5);
   printf("ret[%d]\n", ret);
   if(ret > 0)
   {
      printf("criticalVer[%s] 大于 strCurVer[%s]\n", criticalVer, strCurVer);
   }
   else if(ret < 0) 
   {
      printf("criticalVer[%s] 小于 strCurVer[%s]\n", criticalVer, strCurVer);
   }
   /*else if(ret == 0) 
   {
      printf("criticalVer[%s] 等于 strCurVer[%s]\n", criticalVer, strCurVer);
   }*/
   
   char szCmd[520] = {0};
   printf("sizeof(szCmd)[%d]\n", sizeof(szCmd));
   
   return(0);
}