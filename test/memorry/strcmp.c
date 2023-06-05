//20230317


#include <stdio.h>
#include <string.h>
 
int main ()
{
   //char* str = "20230317V01";
   char str1[15];
   char str2[15];
   int ret;
 
 
   strcpy(str1, "20230317V03");
   strcpy(str2, "20230317V11");
 
   ret = strncmp(str1, str2, 8);
 
   if(ret < 0)
   {
      printf("str1[%s] 小于 str2[%s]\n", str1, str2);
   }
   else if(ret > 0) 
   {
      printf("str1[%s] 大于 str2[%s]\n", str1, str2);
   }
   else 
   {
      printf("str1[%s] 等于 str2[%s]\n", str1, str2);
   }
   
   return(0);
}