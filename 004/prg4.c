#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

 char * GtPPID(char *fNm,char *Nm)
 {
   int *fl;
   char s[2];
   char prNm[50];
   char prVl[1000];
   int st=0;
   int i=0;

// Status format  |name|3A|09| param |0A|name|3A|09| param |0A| ...
// Syntax analizer

 
   fl=open(fNm,O_RDONLY);
   if( fl ==-1 ) { return 0;}

   //printf(" ****** %d \n",fl);

   prNm[0]=0;
   prVl[0]=0;
   s[1]=0;

   while(read(fl,s,1)>0)
   {
     //printf(" rd %d \n",s[0]);
   
     if(s[0]==58) //3A
     {
       continue;
     }
     if(s[0]==9)
     {
       st=1;
       prVl[0]=0;
       i=0;
       continue;
     } 
     if(s[0]==10)
     {
       if(strstr(prNm,Nm)!=0)
       {
         //printf(" pNm  %s   \n",prNm);
         //printf(" pVl  %s   \n",prVl);
         return prVl;
       }
       prNm[0]=0;
       prVl[0]=0; 
       st=0;

       continue;
     }
     if(st==0)//fil parNm
     {
       strcat(prNm,s);
     }
     if(st==1)//fil val par
     { 
       strcat(prVl,s);
     }
   }
   //b=strstr(a,"PPID");
   //if(strlen(b)==strlen(a))
 }


 char * PrcChain(char *id)
 {
    char pt[BUFSIZ];
    char pt3[BUFSIZ];
    char *pt2;

   sprintf(pt3,"%s",id);
   sprintf(pt,"/proc/%s/status",id);

   while ((pt2=GtPPID(pt,"PPid"))!=NULL)
   {
      sprintf(pt,"/proc/%s/status",pt2);
      //printf(" ----- %s  \n",pt2);
      sprintf(pt3,"%s->%s", pt3,pt2);
   }
   return pt3; 
 }




int main(int args, char **argv, char **argc)
{

   char *b;
   char *a;
   int i;
   char pt[BUFSIZ];
   char prNm[50];
 
int *fl;
char s[2];
char fNm[50];
sprintf(fNm,"/proc/10");
s[1]=0;
fl=open(fNm,O_RDONLY);
printf(" %d  \n",fl);
   while(read(fl,s,1)>0)
   {
     printf("%s",s[0]);
   }



   if(args==1)
   {// for all



   }else
   {
   //printf(" %s  \n",argv[1]);    
   //sprintf(pt,"/proc/%s/status",argv[1]);  
   //printf(" %s  \n",pt);    
      printf(" %s  \n",PrcChain(argv[1]));
   }
  printf(" ***************  \n");
  exit(0);
}