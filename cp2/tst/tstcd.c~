#include "prgcd.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>



// lsmod |grep prg
// insmod ./prgcd.ko
// rmmod prgcd
//  mknod cdev c 100 0


main()
{

  int f,r;
  char *msg="ddddddddddddoooopppppppppp \0\n";
  char msg2[100];

  msg2[99]='\0';

  if( (f=open("cdev",0))<0 ) {printf("Err open \n"); exit(0);}

  write(f,(void *)msg,10);
  r=ioctl(f,IOCTL_SET_MSG,(unsigned long)msg); 
  //printf("ioctl %d\n",r);

 // if ( read(f,msg2,10)!=-1){ printf("%s\n",msg2); }
 // else {printf("E read \n");}

  ioctl(f,IOCTL_GET_MSG,(unsigned long)msg2);
  printf("---%s \n",(char *)msg2);

  





  close(f);
}





