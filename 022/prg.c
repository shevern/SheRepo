#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define oops(e) { perror(e);exit(1);}


pthread_mutex_t mx1=PTHREAD_MUTEX_INITIALIZER;
int st;

int  vec; 
//int vec2[10000];
int *ar;


void *f1(void *arg)
{
  int i,j;

  while(st)
  {
    pthread_mutex_lock(&mx1);
    //  Monitor
    j=0;
    for(i=1;i<1000;i++)
       if (ar[i]==1)j++;
    //printf("Vec      %d  \n",vec);
    printf("Vec      %d  %d  \n",j,ar[0]);
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
  //  sleep(1);     

    pthread_mutex_lock(&mx1);
    //read
    //printf("rd \n");
    vec--; 
    ar[0]--;
    for(i=1;i<1000;i++)
       if (ar[i]==1){ar[i]=0;break;}

    pthread_mutex_unlock(&mx1);
  }

  pthread_exit(arg);
}

void *fw(void *arg)
{
  int i;

  while(st)
  {
    //sleep(1);     
    pthread_mutex_lock(&mx1);
    //write
    //printf("wr \n");
    vec++;
    ar[0]++;
    for(i=1;i<1000;i++)
       if (ar[i]==0){ar[i]=1;break;}

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
  int nM,nN,i,shmid;
  pthread_t t;
  pthread_t tr[100];
  pthread_t tw[100];
  struct stat sta;
  struct ipc_perm perm;
  struct shmid_ds ds;
  key_t kk;
  

       // syst V - IPC
  // shmget       - create/open
  // shmctl       - control
  // shmat shmdt  - oper
  //  key_t ftok( PathNam, id)     (PathNam + 8 bit of id)

  // how to form - key_t  -   8 bit + 12 bit + 12 bit
  stat("prg.o",&sta);
  kk=ftok("prg.o",0x57);
  printf( "id--  %lx  %lx  %x  \n", (u_long)sta.st_dev,(u_long)sta.st_ino,(u_int)kk);
  //kk=99;
  //IPC_PRIVATE ,  IPC_CREATE|IPC_EXCL  SVSHM_MODE
  shmid=shmget(kk,1000*sizeof(int),IPC_CREAT| 0644  );  
  if(shmid==-1) oops("shmget");
  //SHM_R | SHM_W
  ar=shmat(shmid,0,0);  //attach
  if(ar==NULL) oops("shmat");
   // IPC_RMID  IPC_SET IPC_STAT
  //shmctl(shmid,IPC_RMID,NULL); // Del shm
  // ds.shm_segsz ds.shm_perm
  //shmctl(shmid,IPC_STAT,&ds); // stat


  for(i=0;i<1000;i++) ar[i]=0;

 // printf(" %0.2f \n",a);

   if(args==1)
   { printf("prg N n  \n"); }

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

     shmdt(ar); // detach

     printf("Ok \n");

     // pthread_create
     // getpid
     // pthread_cond_wait - ???
     // pthread_exit
     // pthread_join 
 
     return 0; 
   }

}
