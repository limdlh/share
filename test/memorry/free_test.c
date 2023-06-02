#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main()
{
   char *str;
 
   /* 最初的内存分配 */
   str = (char *) malloc(15);
   strcpy(str, "runoob");
   printf("String = %s,  Address = %p\n", str, str);
 
   /* 重新分配内存 */
   str = (char *) realloc(str, 25);
   strcat(str, ".com");
   printf("String = %s,  Address = %p\n", str, str);
 
   /* 释放已分配的内存 */
   free(str);
   str = NULL;
   free(str);
   str = NULL;

   return(0);
}
