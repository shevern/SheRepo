#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>


 //   List of Threads

 char * GtPPID(char *fNm,char *Nm)
 {
   int *fl;
   char s[2];
   char prNm[50];
   char prVl[1000];
   int st=0;
   int i=0;

// Syntax analizer
// Status format  |name|3A|09| param |0A|name|3A|09| param |0A| ...

 
   fl=open(fNm,O_RDONLY);
   if( fl ==-1 ) { return NULL;}

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
 }


 char * GetPrc(char *id)
 {
    char pt[BUFSIZ];
    char pt3[BUFSIZ];
    char *pt2;
    int i;


   // Proc Pid:  PPid  Tgid

   sprintf(pt3,"Proc %s :",id);
   sprintf(pt,"/proc/%s/status",id);
   
   if( (i=atoi(id))==NULL )  return NULL;
   if((pt2=GtPPID(pt,"PPid"))!=NULL){
      sprintf(pt3,"%s PPid- %s ", pt3,pt2);}
   else {return NULL;}
  
   if((pt2=GtPPID(pt,"Tgid"))!=NULL)
      sprintf(pt3,"%s Tgid- %s ", pt3,pt2);

   if((pt2=GtPPID(pt,"Threads"))!=NULL)
      sprintf(pt3,"%s Threads- %s ", pt3,pt2);

 //  while ((pt2=GtPPID(pt,"PPid"))!=NULL)
   //   sprintf(pt,"/proc/%s/status",pt2);
     // sprintf(pt3,"%s PPid-%s ", pt3,pt2);
  
    




   return pt3; 
 }





void GetThr(char *id)
{
  DIR *dir_p;
  struct dirent *direp;
  char buf[100],buf2[100],buf3[100],*pt2,buf4[100];
  int i;

  sprintf(buf3,"/proc/%s/task",id);  

  dir_p=opendir(buf3);
  if(dir_p==NULL) exit(1);
  
  while( (direp=readdir(dir_p))!=NULL)
  {
    if(direp->d_type==4)
    {
       strncpy(buf,direp->d_name,100);
       buf[99]=0;
       
       //printf(" d - %s    ",buf); 
       if( (i=atoi(buf))!=NULL )
       {
    
          // printf("%s \n",buf3);
          sprintf(buf4,"%s/%s/status",buf3,buf);
          //printf("%s \n",buf4);

          sprintf(buf2,"Thr %s : ",buf);

          //if((pt2=GtPPID(buf4,"Pid"))!=NULL)
          //    sprintf(buf2,"%s Pid- %s ",buf2,pt2);        
          if((pt2=GtPPID(buf4,"PPid"))!=NULL)
              sprintf(buf2,"%s PPid- %s ",buf2,pt2);
          if((pt2=GtPPID(buf4,"Tgid"))!=NULL)
              sprintf(buf2,"%s Tgid- %s \n",buf2,pt2);

          printf(buf2);
        }
    }

  }
}




int main(int args, char **argv, char **argc)
{
   char *a;
   int p;

   if(args<2) return 1;

   if( (a=GetPrc(argv[1]))!=NULL )
      printf("%s\n",a);
   GetThr(argv[1]);  


  //printf(" ***************  \n");
  exit(0);
}
