#include <stdio.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>

#include <poll.h>

#define oops(e) { perror(e);exit(1);}

int pin,pout; 
int st;


int main(int args, char **argv, char **argc)
{
  int ns,id;
  FILE *f;
  char ss[BUFSIZ],ss2[BUFSIZ];
  int l,i,j;   

  fd_set rds;
  struct timeval tv;
  int rt;
   
  struct pollfd pfd[2];  
 
  tv.tv_sec=5;
  tv.tv_usec=0;  

  if(args<3) return;

   printf("pin %s\n",argv[1]);
   printf("pout %s\n",argv[2]);
  
  //Check
  if(access(&argv[1][0],F_OK)<0)  
    if( mkfifo(&argv[1][0],0666)<0)    {  perror("P in"); exit(1);}
      

  if(access(&argv[2][0],F_OK)<0)
    if( mkfifo(&argv[2][0],0666)<0)    {  perror("P out"); exit(1);}

  st=1;

  pin=open(&argv[1][0],O_RDONLY|O_NONBLOCK);
  if(pin==-1)oops("open pin");

  pout=open(&argv[2][0],O_WRONLY);
  if(pout==-1)oops("open pout");

  while(7)
  {

    sleep(1);

    pfd[0].fd=0;
    pfd[0].events=POLLIN;
    pfd[0].revents=0;
    pfd[1].fd=pin;
    pfd[1].events=POLLIN;
    pfd[1].revents=0;

    rt=poll(pfd,2,-1);
 
    if(rt>0)
    {
       if(pfd[0].revents & POLLIN)  // console
       {   
           if( (i=read(0,ss,100))>0)
           //while(  (  i=read(0,ss,100)  )>0   )
           //if(gets(ss)!=NULL)
           {        
              ss[i]='\0';
              printf("out-  %s\n",ss);
              l=strlen(ss);
              if (write(pout,ss,l)==-1)oops("Write ");
           }
           else{ printf("E 1 \n");  }
        }

        if(pfd[1].revents & POLLIN)  // pipe
        {   
           if ( (i=read(pin,ss,100))>0)
           {
             ss[i]='\0';
             printf("in - %s\n",ss);
           }
        }
    
    }else{
           printf("E 2  %d  \n",rt);
           if(rt==0) printf("Tout  %d  \n",rt);
   }
  }
 

  st=0;
 
}
