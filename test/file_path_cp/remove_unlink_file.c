#include <stdio.h>
#include <string.h>
//#include <fcntl.h>
#include <unistd.h>

int main ()
{
   FILE *fp;
   char filename[] = "file.txt";

   fp = fopen(filename, "w");

   fprintf(fp, "%s", "这里是 runoob.com");
   fclose(fp);
   
   int ret = remove(filename);
   if(ret == 0) 
   {
      printf("文件file.txt删除成功\n");
   }
   else 
   {
      printf("错误：不能删除该文件file.txt\n");
   }


  int result = unlink("test.txt");
  if(result == 0)
  {
    printf("文件test.txt删除成功\n");
  }
  else
  {
    printf("文件test.txt删除失败\n");
  }

  if(access("./dir/", F_OK) < 0)
  {
    printf("文件夹 ./dir/ 不存在\n");
  }
  else
  {
    printf("文件夹 ./dir/ 存在\n");
  }

  result = unlink("./dir/");
  if(result == 0)
  {
    printf("文件夹 ./dir/ 删除成功\n");
  }
  else
  {
    printf("文件夹 ./dir/ 删除失败\n");
  }

   return(0);
}

