#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>

#define oops(e) { perror(e);exit(1);}

int si,sa;

int max(int i,int j)
{
  if(i>j)return i;
  return j;
}

static int cmpr(const void *p1,const void *p2) //point to point to char
{
  int i,r=0;
  char a,b;
  //a=((char*)p1);
  //b=((char*)p2);
  //printf("cmp \n");
  for(i=0;i<si;i++)
  { 
    a=*((char*)(p1+i));
    b=*((char*)(p2+i));
    //printf("%d   %d \n",a,b);
    if(a>b) return 1; 
    if(a<b) return -1; 
  }
  return 0;
} 

int main(int args, char **argv, char **argc)
{
  int nM,i;
  int fds,fdr,fdz;
  caddr_t ar,as; 
  double nN,a,b;
  size_t fsz,msz,psz;
  struct stat st; //st.st_size
  char c=1,cc[100];





   if(args!=3) return;
   si=atoi(argv[2]);

  // sa=15;
  // si=2;
  //fsz=sa*si;
  fdr=open(argv[1],O_RDWR,0644); //
  //for(i=0;i<fsz;i++)  {write(fdr,(void *)&c,1);c--;}
  if(fdr<0) oops("open");
  if(fstat(fdr,&st)==-1)oops("stat");
  fsz=st.st_size;
 
  ar=mmap(0,fsz,PROT_READ|PROT_WRITE,MAP_SHARED,fdr,0);

  if(ar==NULL) oops("mmap");

  //for(i=0;i<fsz;i++) printf("%d ",*(ar+i)); 
  sa=fsz/si;
  qsort(ar,sa,si,cmpr);
  //for(i=0;i<fsz;i++) printf("%d ",*(ar+i)); 

  printf("-- \n");
   





exit(0);



   if(args!=4) return;
   fsz=atoi(argv[2]);
   msz=atoi(argv[3]);

// mmap  munmap  msync
// fstat

   fdz=open("/dev/zero",O_RDWR);
   fdr=open(argv[1],O_RDWR|O_CREAT); //
   //fdr=open(argv[1],O_RDWR|O_CREAT|O_TRUNC); // auto  increase size
   lseek(fdr,fsz-1,SEEK_SET);   //set size
   write(fdr,"",1); 
   ar=mmap(NULL,msz,PROT_READ|PROT_WRITE,MAP_SHARED,fdr,0);
   //ar=mmap(NULL,msz,PROT_READ|PROT_WRITE,MAP_SHARED,fdz,0);// init /dev/zero
   //  MAP_SHARED   -  for parent/child proc
   close(fdz);
   close(fdr);

   psz=sysconf(_SC_PAGESIZE);
   printf("psz %d \n",psz);

   //test
   for(i=0;i<max(fsz,msz);i+=psz)
   {
     printf("- %d  %d \n",i,ar[i]);     
     ar[i]=1;
     printf("- %d  %d \n",i+psz-1,ar[i+psz-1]);     
     ar[i+psz-1]=1;
   }
 // printf(" %0.2f \n",a);

}
