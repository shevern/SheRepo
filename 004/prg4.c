#include <stdio.h>
#include <string.h>

 char * GtPPID(char* pid)
 {

   i=0;
   while(1)
   {
      a=argc[i++];

      if(a==0) break;

      b=strstr(a,"PPID");
      if(b!=0)
      {
        if(strlen(b)==strlen(a))
        {
          printf(" %s  \n",b); 
          return b;   
        }
      } 
 }





int main(int args, char **argv, char **argc)
{

   char *b;
   char *a;
   int i;
   char pt[BUFSIZ];
   FILE *fl;
    

   if(args<2)  return;
   printf(" %s  \n",argv[1]);    

   //pt="";
   //pt=strcat("/proc/","tt");
   //pt=strcpy("/proc/","tt");
   
   sprintf(pt,"/proc/%s/status",argv[1]);  

   printf(" %s  \n",pt);    

   fl=fopen(pt,"r");
   fread(





   return 0;
      
      //printf(" %s  \n",a);
   }

   //printf(" %c  \n",args);

}
