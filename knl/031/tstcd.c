#include "prgcd.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <pthread.h>

#include <poll.h>

// lsmod |grep prg
// insmod ./prgcd.ko
// rmmod prgcd
//  mknod cdev c 800 0

struct ss
{
  
};
 
 int f; 

void *frd(void* ar)
{
  struct ss *a=ar;
  char msg2[100];
  int rt;
  struct pollfd pfd[1];

     printf("reed S  \n");

    /*
     //sleep(2);     
     msg2[99]='\0';
     if ( read(f,msg2,10)!=-1){ printf("read - %s \n",msg2); }
     else {printf("E read \n");}
    */

   pfd[0].fd=f;
   pfd[0].events=POLLIN;
   pfd[0].revents=0;
   
   rt=poll(pfd,1,-1);
   if(rt>0)
   {
      if(pfd[0].revents & POLLIN)
      {
         printf("read - \n");
      }
   }

  pthread_exit(ar);
}





main(int args, char **argv, char **argc)
{

  int r,i;
  char *msg="dddddddddddd\n";
  char msg2[100];
  int pid;
  pthread_t trd[100];  


  //ns=atoi(&argv[2][0]);

   f=open("./cdev",O_RDWR);
   if( f<0 ) {perror("Err open \n"); exit(0);}
   printf("open f  %d \n",f);
   
   //     through thread   **************

   for (i=0;i<2;i++)  // read 
      pthread_create(&trd[i],NULL,frd,NULL);

   sleep(5);
   printf("write b sleep  \n");
   write(f,msg,10);
   sleep(1);
   write(f,msg,10);
   printf("write  a sleep  \n");
   //sleep(2);
  
   for (i=0;i<2;i++)  // 
     pthread_join(trd[i],NULL);

   //----------------------------------



/*   //   through fork  ****************
   pid=fork();
   if(pid==0)   
   {       
     printf("reed  \n");
     //sleep(2);     
     msg2[99]='\0';
     if ( read(f,msg2,10)!=-1){ printf("read - %s \n",msg2); }
     else {printf("E read \n");}
   }else{ //  write
     printf("write b sleep  \n");
     sleep(5);
     write(f,msg,10);
     printf("write  a sleep  \n");
    }
*/   //---------------------------------------


    close(f);
}





