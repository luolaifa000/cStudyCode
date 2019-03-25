#include <stdio.h>
/*函数声明*/
void digui(int n);
char* readData(int fd);
int main()
{
  int n=10;
  digui(n);
  return 0;
}

void digui(int n)
{
  printf("level1-value of %d\n",n);
  if(n>2){
    digui(n-1);
  } 
  printf("level2-value of %d\n",n);
}



char* readData(int fd)
{
    


    return "sdfsdf";
}
