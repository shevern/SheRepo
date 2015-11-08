#include <stdio.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>

#define oops(e) { perror(e);exit(1);}


int pin,pout; 
int st;



int main(int args, char **argv, char **argc)
{
  int ns,id;
  FILE *f;
  char ss[BUFSIZ],ss2[BUFSIZ];
  int l,i,j;   
  pthread_t th[2];   

  fd_set rds;
  struct timeval tv;
  int rt,nfds;

  tv.tv_sec=5;
  tv.tv_usec=0;  


  if(args<3) return;

  //*argv[1] - Pin
  //*argv[2] - Pout 

   printf("pin %s\n",argv[1]);
   printf("pout %s\n",argv[2]);
  
  
  // 1. Make FIFO  

  if(access(&argv[1][0],F_OK)<0) //Check
  {
    if( mkfifo(&argv[1][0],0777)<0)
    oops("P in");
  }    

  if(access(&argv[2][0],F_OK)<0) //Check
  {
    if( mkfifo(&argv[2][0],0777)<0)
    oops("P out");
  }


  // 2.  Open Files
  st=1;

  pin=open(&argv[1][0],O_RDONLY|O_NONBLOCK);
  if(pin==-1)oops("open pin");

  pout=open(&argv[2][0],O_WRONLY);
  if(pout==-1)oops("open pout");


   
  //  3.  Main 

  nfds=0+pin+100;
  while(7)
  {

    sleep(1);

    FD_ZERO(&rds);
    FD_SET(0,&rds); //   for read consol  
    FD_SET(pin,&rds); // for read pipe
 
    rt=select(nfds,&rds,NULL,NULL,NULL);
    if(rt>0)
    {
       if(FD_ISSET(0,&rds))  // console
       {   
           if( (i=read(0,ss,100))>0)
           {        
              ss[i]='\0';
              printf("out-  %s\n",ss);
              l=strlen(ss);
              if (write(pout,ss,l)==-1)
              {
                perror("Write "); exit(1);
              }
        
           }else{
               printf("E 1 \n");
           }
        }
       

        if(FD_ISSET(pin,&rds))  // pipe
        {   
           if ( (i=read(pin,ss,100))>0)
           {
             ss[i]='\0';
             printf("In - %s\n",ss);
           }
        }
    
    }else{
          printf("E 2  %d  \n",rt);
           if(rt==0)
           {  }
   }

  
  }
 

  st=0;
  //pthread_join(th,NULL);

}
