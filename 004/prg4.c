#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

 char * GtPPID(char *fNm)
 {
   int *fl;
   char s[2];
   char prNm[50];
   char prVl[1000];
   int st=0;
   int i=0;
// Status format  |name|3A|09| param |0A|name|3A|09| param |0A| ...

   fl=open(fNm,O_RDONLY);

   
   //printf(" rd %d  \n",read(fl,s,1));

   prNm[0]=0;
   prVl[0]=0;
   s[1]=0;

   while(read(fl,s,1)>0)
   {
     //printf(" rd %d \n",s[0]);
   
     if(s[0]==58) //3A
     {
       //printf(" 3A  \n");
       continue;
     }
     if(s[0]==9)
     {
       //printf(" 09 ");
       st=1;
       prVl[0]=0;
       i=0;
       continue;
     } 
     if(s[0]==10)
     {
       //prVl[i++]=0;
       if(strstr(prNm,"PPid")!=0)
       {
         //printf(" pNm  %s   \n",prNm);
         //printf(" pVl  %s   \n",prVl);
         return prVl;
       }
       //printf(" pNm  %s   \n",prNm);
       //printf(" pVl  %s   \n",prVl);

       //---
       prNm[0]=0;
       prVl[0]=0; 
       st=0;

       //return;
       continue;
       //printf(" 0A  \n");
     }
     if(st==0)//fil parNm
     {
       strcat(prNm,s);
     }
     if(st==1)//fil val par
     { 
//printf("Vl-");

//printf(" %s   \n",prVl);

       strcat(prVl,s);
       //if(i<100) prVl[i++]=s;
//printf("Vl \n");
     }

   }
   //b=strstr(a,"PPID");
   //if(strlen(b)==strlen(a))
 }





int main(int args, char **argv, char **argc)
{

   char *b;
   char *a;
   int i;
   char pt[BUFSIZ];
    

   if(args==1)
   {// for all



   }else
   {


   printf(" %s  \n",argv[1]);    
   sprintf(pt,"/proc/%s/status",argv[1]);  
   printf(" %s  \n",pt);    

   //pt="";
   //pt=strcat("/proc/","tt");
   //pt=strcpy("/proc/","tt");

   printf(" ----- %s  \n",GtPPID(pt));



   

      
      //printf(" %s  \n",a);
   }

   //printf(" %c  \n",args);

  printf(" ***************  \n");
  exit(0);

}
