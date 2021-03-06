#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <math.h>

pthread_mutex_t mx1=PTHREAD_MUTEX_INITIALIZER;
double sm;


struct thri
{
   double i,j,k;
};


void *f1(void *arg)
{
  struct thri *a=arg;

  // much proc
  //a->k=a->i*a->j;
  a->k=sin(a->i)*a->j;

  pthread_mutex_lock(&mx1);
  sm=sm+a->k;
  //printf("-- %d  %d \n",a->i,a->j);
  printf(" %f  %f  %f   \n",a->i,a->j,sm);
  pthread_mutex_unlock(&mx1);

  pthread_exit(arg);
}



int main(int args, char **argv, char **argc)
{
  int nM,i;
  pthread_t t[100];
  struct thri th[100];  
  double nN,a,b;

 // printf(" %0.2f \n",a);

   sm=0;   
   if(args==1)
   { printf("prg3 N n  \n"); }

 
   if(args==3)
   {

     //nN=atof(&argv[1][0]); //N - interval
     nN=atoi(&argv[1][0]); //N - interval
     nM=atoi(&argv[2][0]); //m - threads

     printf("N- %0.1f n- %d  \n",nN,nM);
     
     nN=nN*3.14/180;// Rad->Grad 
     
     if(nM>100)exit(1);
     
     printf("Counting:\n");
     
     for(i=0;i<nM;i++)
     {
        a=nM; b=i;
        th[i].i=nN/a*i;
        th[i].j=nN/a;
        //printf("-- - N  a  %f M b %f    \n",th[i].i,th[i].j);
        pthread_create(&t[i],NULL,f1,&th[i]);
     }

     for(i=0;i<nM;i++)
     {
        pthread_join(t[i],NULL);
     }

     printf("Result - %0.3f\n",sm);

     // pthread_create
     // getpid
     // pthread_cond_wait - ???
     // pthread_exit
    // pthread_join 
 
     return 0; 
   }

}
