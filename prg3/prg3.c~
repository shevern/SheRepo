#include <stdio.h>
#include <string.h>

int main(int args, char **argv, char **argc)
{

// To get Envirenment  -  3 Arg  
// getenv

   char *b;
   char *a;
   int i;
 
   i=0;
   while(1)
   {
      a=argc[i++];

      if(a==0) break;

      b=strstr(a,"PWD");
      if(b!=0)
      {
        if(strlen(b)==strlen(a))
        {
      printf(" %s  \n",b);    
        }

      } 


      
      //printf(" %s  \n",a);
   }

   //printf(" %c  \n",args);

}
