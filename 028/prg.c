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


static int fu(int l)
{
  char pp[20];
  int f;

   if(l==0)
   {
      //********
      sprintf(pp,"./a%d",l);
      printf("pid %d =%d\n",l,getpid());
      mkdir("a2",0777);
      mount("proc","./a2","proc",0,NULL);
      //********
      sleep(10);
   }else
   { 
      if((f=fork())==0)
      {//child
       //unshare(CLONE_NEWNS);
       //unshare(CLONE_NEWUTS);
         unshare(CLONE_NEWPID  );

         //*********
         sprintf(pp,"./a%d",l);
         printf("pid %d =%d\n",l,getpid());
         mkdir("a2",0777);
         mount("proc","./a2","proc",0,NULL);
         //*********
         f=fu(--l);
       } 
       waitpid(f,NULL,0);
       exit(0);
    }

}






main(int ar,char ** arv)
{
   char *p,pp[50];
   int f1,f2,f3; 
  

    printf("main pid=%d\n",getpid());


    //   ************   Recursive

    //fu(5);
    //exit(0);



    //   ***********  Extended 

    //unshare(CLONE_NEWPID  );
    if((f1=fork())==0)
    {//child
       //unshare(CLONE_NEWNS);
       //unshare(CLONE_NEWUTS);
       unshare(CLONE_NEWPID  );

       printf("pid 1 =%d\n",getpid());
       mkdir("a1",0555);
       mount("proc","./a1","proc",0,NULL);

//***********
      //unshare(CLONE_NEWPID  );
       if((f2=fork())==0)
       {//child
         //unshare(CLONE_NEWNS);
         //unshare(CLONE_NEWUTS);
         unshare(CLONE_NEWPID  );

         printf("pid 2 =%d\n",getpid());
         mkdir("a2",0555);
         mount("proc","./a2","proc",0,NULL);


//************
          if((f3=fork())==0)
          {//child
             //unshare(CLONE_NEWNS);
             //unshare(CLONE_NEWUTS);
             unshare(CLONE_NEWPID  );

             printf("pid 3 =%d\n",getpid());
             mkdir("a3",0555);
             mount("proc","./a3","proc",0,NULL);

             sleep(10);
             //while(9);
             exit(0);
           }
//*************



          waitpid(f3,NULL,0);
          exit(0);
      }
//*****************

       waitpid(f2,NULL,0);
       exit(0);
    }
    waitpid(f1,NULL,0);
    //while(9);
}




