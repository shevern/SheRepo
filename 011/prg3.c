#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>

  int pin,pout; 
  int st;



void *prd(void *a)
{
  char buf[1000];

  while(st)
  {
    if (read(pin,buf,100)>0)
    {
      buf[100]='\0';
      printf("%s\n",buf);
    }
    //sleep(1);
  }

  pthread_exit(NULL);
}







int main(int args, char **argv, char **argc)
{
  int ns,id;
  FILE *f;
  char ss[BUFSIZ],ss2[BUFSIZ];
  int l,i,j;   
  pthread_t th;   



  if(args<3) return;

  //*argv[1] - Pin
  //*argv[2] - Pout 

   printf("pin %s\n",argv[1]);
   printf("pout %s\n",argv[2]);
  
  //Check
  if(access(&argv[1][0],F_OK)<0)
  {
    //Create
    if( mkfifo(&argv[1][0],0777)<0)
    //if( mkfifo("/tmp/a",0777)<0)
    {
       perror("P in"); exit(1);
    }
  }
      

  if(access(&argv[2][0],F_OK)<0)
  {//Create
    if( mkfifo(&argv[2][0],0777)<0)
    {
       perror("P out"); exit(1);
    }
  }

  pin=open(&argv[1][0],O_RDONLY|O_NONBLOCK);
  //pin=open(&argv[1][0],O_RDONLY);
  if(pin==-1){perror("open pin"); exit(1);}
   
  //pout=open(&argv[2][0],O_WRONLY|O_NONBLOCK);
  pout=open(&argv[2][0],O_WRONLY);
  if(pout==-1){perror("open pout"); exit(1);}

  st=1;
  pthread_create(&th,NULL,prd,NULL);
  
 

  //while((ns=read(f,ss,1000))>0)
  //while(fgets(ss,1000,f)!=NULL)
  while(gets(ss)!=NULL)
  {
     l=strlen(ss);
     if (write(pout,ss,l)==-1)
     {
       perror("Write "); exit(1);
     }
      
     //if(ss[l-1]=='\n') {ss[l-1]=0; l--;}

     //printf("%s\n",ss);
  }
  

  st=0;
  pthread_join(th,NULL);



}
