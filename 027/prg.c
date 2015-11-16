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

#include <sys/mount.h>


//   
#define oops(e) { perror(e);exit(1);}

//*******************************************************

main(int ar,char ** arv)
{
  char *p,pp[50];
  int f; 
  

   mkdir("a1",0777);
   mkdir("a2",0777);
   mkdir("a3",0777);

   //mount("proc","./a1","proc",0,NULL);


    if((f=fork())==0)
    {//child
       //unshare(CLONE_NEWNS);
       //unshare(CLONE_NEWUTS);
       unshare(CLONE_NEWPID);

       mount("proc","./a1","proc",0,NULL);


//***********
       if((f=fork())==0)
       {//child
         //unshare(CLONE_NEWNS);
         //unshare(CLONE_NEWUTS);
         unshare(CLONE_NEWPID);

         mount("proc","./a2","proc",0,NULL);


//************
          if((f=fork())==0)
          {//child
             //unshare(CLONE_NEWNS);
             //unshare(CLONE_NEWUTS);
             unshare(CLONE_NEWPID);

             mount("proc","./a3","proc",0,NULL);

             while(9);
             exit(0);
           }
//*************



          while(9);
          exit(0);
      }
//*****************

       while(9);
       exit(0);
    }

       //waitpid(f,NULL,0);
       while(9);
}




