#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int args, char **argv, char **argc)
{

// To get Envirenment  -  3 Arg  
// getenv

   char *b;
   char *a;
   int i;
    //printf(" %c  \n",args);
   int f;

   f=open("./mydev",O_RDONLY);
   if(f==-1) printf("aaaa\n");

  // i=write(f,"qqq",3);
  // if(i==-1) printf("bbbb\n");
 
    

 //  close(f);


}
