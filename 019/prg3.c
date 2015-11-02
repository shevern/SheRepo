#include <stdio.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>


int pin,pout; 
int st;


void *prd(void *a)
{
  char buf[1000];
  int i;
  while(st)
  {
    if ((i=read(pin,buf,100))>0)
    {
      buf[i]='\0';
      printf("%s\n",buf);
    }
    //sleep(1);
  }
  pthread_exit(NULL);
}


void *f1(void *a)
{
  //pin=open((char*)a,O_RDONLY|O_NONBLOCK);
  pin=open((char*)a,O_RDONLY);
  if(pin==-1){perror("open pin"); pthread_exit(NULL);}
  pthread_exit(NULL);
}

void *f2(void *a)
{
  //pout=open((char*)a,O_WRONLY|O_NONBLOCK);
  pout=open((char*)a,O_WRONLY);
  if(pout==-1){perror("open pout"); pthread_exit(NULL);}
  pthread_exit(NULL);
}







int main(int args, char **argv, char **argc)
{
  int ns,id;
  FILE *f;
  char ss[BUFSIZ],ss2[BUFSIZ];
  int l,i,j;   
  pthread_t th[2];   

  fd_set rds;
  struct timeval tv;
  int rt;

  tv.tv_sec=5;
  tv.tv_usec=0;  


  if(args<3) return;

  //*argv[1] - Pin
  //*argv[2] - Pout 

   printf("pin %s\n",argv[1]);
   printf("pout %s\n",argv[2]);
  
  //Check
  if(access(&argv[1][0],F_OK)<0)
  {
    if( mkfifo(&argv[1][0],0777)<0)
    //if( mkfifo("/tmp/a",0777)<0)
    {  perror("P in"); exit(1);}
  }    

  if(access(&argv[2][0],F_OK)<0)
  {//Create
    if( mkfifo(&argv[2][0],0777)<0)
    {  perror("P out"); exit(1);}
  }

  st=1;
  pthread_create(&th[0],NULL,f1,&argv[1][0]);
  pthread_create(&th[1],NULL,f2,&argv[2][0]);
  pthread_join(th[0],NULL);
  pthread_join(th[1],NULL); 

  pthread_create(&th[0],NULL,prd,NULL);

  printf("qq3 \n");


  FD_ZERO(&rds);
  //FD_SET(0,&rds); //   for read consol  
  FD_SET(pin,&rds); // for read pipe
  
  while(7)
  {

    //sleep(1);
 
    //rt=select(2,&rds,NULL,NULL,&tv);
    rt=select(3,&rds,NULL,NULL,NULL);
    if(rt>0)
    {
       //printf("qq1  \n");

       if(FD_ISSET(0,&rds))  // console
       {   
        //   printf("qq2 Cnsl \n");
           if( (i=read(0,ss,100))>0)
           //while((ns=read(0,ss,1000))>0)
           //if(gets(ss)!=NULL)
           {        
       //     printf("qq3  \n");
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
           printf("qq 4  \n");
           if ( (i=read(pin,ss,100))>0)
           {
             printf("qq 5  \n");
             ss[i]='\0';
             printf("pp--- %s\n",ss);
           }
        }
    
    }else{
          //printf("E 2  %d  \n",rt);
           if(rt==0)
           {
                FD_ZERO(&rds);            
                FD_SET(pin,&rds); // for read pipe
                FD_SET(0,&rds); //   for read consol  
                tv.tv_sec=5;
           }
   }

  
  }
 

  st=0;
  //pthread_join(th,NULL);

}
