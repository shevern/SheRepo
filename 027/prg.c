#define _GNU_SOURCE


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>

#include <stdlib.h>
#include <string.h>

#include <sched.h>



//   
#define oops(e) { perror(e);exit(1);}

//*******************************************************

main(int ar,char ** arv)
{
  char *p,pp[50];
  int f; 
  
    if((f=fork())==0)
    {//child
      // unshare(CLONE_NEWNS);
       unshare(CLONE_NEWUTS);

       gethostname((char *)&pp[0],10); 
       pp[10]=0;
       printf( "child hst %s \n",pp); 


       //if(sethostname("ubuntu",6)<0) oops("st");
       if(sethostname("qqqqqq",6)<0) oops("st");

       gethostname((char *)&pp[0],10); 

       pp[10]=0;
       printf( " child hst  %s \n",(char *)pp); 
       //while(9);
       exit(0);

    }

       sleep(1);
       waitpid(f,NULL,0);
       gethostname(&pp[0],10); 
       pp[10]=0;
       printf( "prn hst %s \n",(char *)pp); 

}




