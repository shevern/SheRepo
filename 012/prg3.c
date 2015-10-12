#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <math.h>

pthread_mutex_t mx1=PTHREAD_MUTEX_INITIALIZER;
int st;

int  vec; 
int vec2[10000];

void *f1(void *arg)
{
  int i,j;
  while(st)
  {
    pthread_mutex_lock(&mx1);
    //  Monitor
    j=0;
    for(i=0;i<10000;i++)
       if (vec2[i]==1)j++;
    //printf("Vec      %d  \n",vec);
    printf("Vec      %d  \n",j);
    //sleep(1);     

    pthread_mutex_unlock(&mx1);
    sleep(1);     

  }
  pthread_exit(arg);
}


void *fr(void *arg)
{
  int i;
  while(st)
  {
    pthread_mutex_lock(&mx1);
    //read
    //printf("rd \n");
    //sleep(2);     
    vec--; 
    for(i=0;i<10000;i++)
       if (vec2[i]==1){vec2[i]=0;break;}

    pthread_mutex_unlock(&mx1);
  }
  pthread_exit(arg);
}

void *fw(void *arg)
{
  int i;
  while(st)
  {
    pthread_mutex_lock(&mx1);
    //write
    //printf("wr \n");
    //sleep(3);     
    vec++;
    for(i=0;i<10000;i++)
       if (vec2[i]==0){vec2[i]=1;break;}

    pthread_mutex_unlock(&mx1);
  }
  pthread_exit(arg);
}

 void ha(int i)
 {
  st=0;
  signal(SIGINT,ha);
 }




int main(int args, char **argv, char **argc)
{
  int nM,nN,i;
  pthread_t t;
  pthread_t tr[100];
  pthread_t tw[100];


 // printf(" %0.2f \n",a);

   if(args==1)
   { printf("prg3 N n  \n"); }

   vec=0;
   if(args==3)
   {

     signal(SIGINT,ha);
    
     //nN=atof(&argv[1][0]); //N - interval
     nN=atoi(&argv[1][0]); //N - Rd
     nM=atoi(&argv[2][0]); //m - Wr

     if(nN>100)exit(1);
     if(nM>100)exit(1);

     st=1; 
     for(i=0;i<nN;i++)
        pthread_create(&tr[i],NULL,fr,NULL);
     for(i=0;i<nM;i++)
        pthread_create(&tw[i],NULL,fw,NULL);
        pthread_create(&t,NULL,f1,NULL);

     //---------- 
     
     for(i=0;i<nM;i++) pthread_join(tw[i],NULL);
     for(i=0;i<nN;i++) pthread_join(tr[i],NULL);
     pthread_join(t,NULL);

     printf("Ok \n");

     // pthread_create
     // getpid
     // pthread_cond_wait - ???
     // pthread_exit
     // pthread_join 
 
     return 0; 
   }

}
