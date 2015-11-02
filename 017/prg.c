#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <math.h>

#include <stdlib.h>

//#define _GNU_SOURCE

#include <linux/sched.h>

#define oops(e) { perror(e);exit(1);}

double sm;
 

struct thri
{
  double i,j,k;
  char *stk,*stkTop;
  pid_t pid;

};


 int f2(void *arg)
{
  struct thri *a=arg;

  a->k=sin(a->i)*a->j;
  //printf(" %f  %f  %f   \n",a->i,a->j,a->k);
  //sleep(1);
  
  return 0;		
}

int main(int args, char **argv, char **argc)
{
  int nM,i;
  struct thri th[100];  
  double nN,a,b;


 // printf(" %0.2f \n",a);

   sm=0;   
   if(args==1) { printf("prg3 interval  threads \n"); }
 
   if(args==3)
   {
     //nN=atof(&argv[1][0]); //N - interval
     nN=atoi(&argv[1][0]); //N - interval
     nM=atoi(&argv[2][0]); //m - threads

     //printf("N- %0.1f n- %d  \n",nN,nM);
     
     nN=nN*3.14/180;// Rad->Grad 
     
     if(nM>100) exit(1);
     
     printf("Counting:\n");
     
     for(i=0;i<nM;i++)
     {
        a=nM; b=i;
        th[i].i=nN/a*i;
        th[i].j=nN/a;
        //printf("-- - i %f j  %f    \n",th[i].i,th[i].j);
 
        th[i].stk=malloc(100000);
        th[i].stkTop=th[i].stk+100000;
        th[i].pid= clone(f2,th[i].stkTop,SIGCHLD|CLONE_VM,&th[i]);
        if (th[i].pid==-1) {oops("Err Clone");}
     }

     for(i=0;i<nM;i++)
     {
        if( waitpid(th[i].pid,NULL,0)==-1) {oops("ErrW");}
        //printf("--- %f %f  %f   \n",th[i].i,th[i].j,th[i].k);
        sm=sm+th[i].k;
        free(th[i].stk);
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
