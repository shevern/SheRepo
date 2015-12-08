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
  char *msg="dddddddddddd\n";
  char msg2[100];

  msg2[99]='\0';

  if( open("cdev",0)<0 ) {printf("Err open \n"); exit(0);}

  write(f,msg,10);
  if ( read(f,msg2,10)!=-1){ printf("%s\n",msg2); }
  else {printf("E read \n");}



  close(f);
}





