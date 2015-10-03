#include <stdio.h>
#include <string.h>
#include <signal.h>

int main(int args, char **argv, char **argc)
{
  int ns,id;

   if(args==3)
   {
     ns=atoi(&argv[2][0]);
     id=atoi(&argv[1][0]);
     return kill(id,ns); 
   }

}
