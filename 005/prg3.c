#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>

extern char **environ;


//*********************  Find file

int fnd(char *fl,char * fl2)
{
  DIR *dr;
  struct dirent *dre;
  char buf[100];
  char hm[100];

  if (genv("PWD=",hm)==1) return 1;  

  //printf("******* %s \n",hm);
  
  dr=opendir(hm);
  if(dr==NULL){perror("dir ---"); return 1;}
  
  while(( dre=readdir(dr))!=NULL)
  {
    if(dre->d_type==8)
    {
       dre->d_name[99]=0;
       if(strstr(dre->d_name,fl)!=NULL)
      { 
        sprintf(fl2,"%s/%s",hm,fl);
        return 0;
       }
      // printf("%s \n",dre->d_name);
    }
  }

  return 1;

}

int genv(char *nm,char *vl)
{
  //  getenv  -  putenv
  char *s,*ss;
  int i;
  
  i=0;
  vl[0]=0;
  while( (s=&environ[i++][0])!=NULL )
  {
    if((ss=strstr(s,nm))!=NULL)
    {
      if(strlen(s)==strlen(ss))
      {
        //printf("%s",s);
        sprintf(vl,"%s",&s[strlen(nm)]);
        return 0;
      }
    }
  }
  return 1;
}


//**********************************************


//****************  Start Proc

int f1(char *cmd)
{
  int status;
  int pid;

  pid=fork();
  if(pid<0) {printf("Err\n");} 
      else
      if(pid==0)  //Child
      {
         //setpgid(getpid(),getpid());
         //setsid();
         signal(SIGINT,SIG_DFL); 
         signal(SIGTERM,SIG_DFL);

         //execle("/bin/sh","sh","-c",cmd,0,environ);
         execl("/bin/sh","sh","-c",cmd,0);
         //printf("Err \n");
     //arr[0]="pwd"; arr[1]=0; arr[2]=0;
     //if(execvp("pwd",arr)==-1) perror(" Er -");    
     //system("cd / | pwd");
     }else
     {     //  Prnt
       if(waitpid(pid,&status,0)==pid)
       {
         if(WIFEXITED(status))
         {return WIFEXITED(status);}
         else
         //if(WIFSIGNALLED(status))
         {return WTERMSIG(status);}
       }
     }
}
//************************************************


//****************  Signal
void * setsig(int nsig, void (*hndl)(int))
{
  struct sigaction act, oact;

  act.sa_handler=hndl;
  sigemptyset(&act.sa_mask);

  act.sa_flags=0;
  
  if( sigaction(nsig,&act, &oact)<0) return SIG_ERR;

  return oact.sa_handler;
}

void ha(int i)
{
   printf(" Bay  \n");
   setsig(SIGINT,SIG_DFL);
   kill(getpid(),SIGINT);
}

void ha2(int i)
{
   printf(" Bay  \n");
   setsig(SIGTERM,SIG_DFL);
   kill(getpid(),SIGTERM);
}
//******************************************



main(int ar,char ** arv)
{
   
int i,i2;

char * pt, *obrk, *nbrk, *naddr, *arr[3];
char buf[200], var[200],buf2[100],hm[100];   //  Allocate 

//i=BUFSIZ; //bufsiz;  //  Max size of bufs

    
 
    setsig(SIGINT,ha);
    setsig(SIGTERM,ha2);
  //setsig(SIGINT,SIG_IGN);  
  //signal(SIGINT,SIG_DFL);
    
   //if(genv("HOME=",hm)==0) printf("--- %s \n",hm);

  
  

  while(9)
  {
     write(1,"A- ",3);
     i=read(0,buf,100);
     buf[i-1]=0;
     
     //printf(" got  \n");
 
     if(strstr(buf,"/")!=&buf[0]) //not abs addr
     { 
       //    printf(" not abs \n");
       if((i=(int)strstr(buf," "))!=NULL)  //has space
       {
       //     printf(" has space  \n");
         i=i-(int)&buf[0];
         strncpy(var,buf,i);
         var[i]=0;
         //printf("- %s",var); 
         if(fnd(var,buf2)==0)             // found
         { 
          sprintf(buf2,"%s%s",buf2,&buf[i]);
          f1(buf2);         
      //    printf("  found  %s \n",buf2);
         }else                           // not  found
         {
      //     printf(" not found  %s \n",var);
           f1(buf);
         }
         // var
       }else
       {
        //     printf(" no space  \n");
         if(fnd(buf,buf2)==0)
         { 
       //    printf(" found  %s\n",buf2);
           f1(buf2);
         }else
         {
        //   printf("  not found  %s\n",buf);
           f1(buf);
         }
         // buf
       }
     }else
     {
      //    printf(" abs addr  \n");
          f1(buf);
     }

     //i=f1(buf);    
     //printf(" trm  %d\n",i);
  }
 

 printf( " ---------------------------------------- \n");

  exit(0);
}




