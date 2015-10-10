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
    
    if(kill(id,ns)<0) {perror("Err mykill -"); exit(1);} 
   }else {printf("Args - ? \n");exit(1);}

  exit(0);
}
