#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define oops(e) { perror(e);exit(1);}


int main(int args, char **argv, char **argc)
{
  int nM,nN,i,shmid;
  struct stat sta;
  struct ipc_perm perm;
  struct shmid_ds ds;
  struct shm_info inf;
  key_t kk;
  int *ar;

       // syst V - IPC
  // shmget       - create/open
  // shmctl       - control
  // shmat shmdt  - oper
  //  key_t ftok( PathNam, id)     (PathNam + 8 bit of id)

  // how to form - key_t  -   8 bit + 12 bit + 12 bit
  stat("prg.o",&sta);
  kk=ftok("prg.o",0x57);
  printf( "ftok id -  %lx  %lx  %x  \n", (u_long)sta.st_dev,(u_long)sta.st_ino,(u_int)kk);
  //kk=99;
  //IPC_PRIVATE ,  IPC_CREATE|IPC_EXCL  SVSHM_MODE
  shmid=shmget(kk,1000*sizeof(int),IPC_CREAT| 0644  );  
  if(shmid==-1) oops("shmget");
  //SHM_R | SHM_W
  //ar=shmat(shmid,0,0);  //attach
  //if(ar==NULL) oops("shmat");
   // IPC_RMID  IPC_SET IPC_STAT
  //shmctl(shmid,IPC_RMID,NULL); // Del shm
  // ds.shm_segsz ds.shm_perm

  if (shmctl(shmid,IPC_STAT,&ds)==-1) oops("shmctl"); // stat
  if (shmctl(shmid,SHM_INFO,&inf)==-1) oops("shmctl"); // stat

  printf("shm_segsz %d \n",ds.shm_segsz);
  printf("shm_nattch %d \n",ds.shm_nattch);
  printf("shm_perm.mode %d \n",ds.shm_perm.mode);

  printf("shm_rss %d \n",inf.shm_rss);
  printf("shm_swp %d \n",inf.shm_swp);
  printf("shm_tot %d \n",inf.shm_tot);




     printf("Ok \n");

     return 0; 
}

