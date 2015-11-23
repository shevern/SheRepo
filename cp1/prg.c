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

//   
#define oops(e) { perror(e);exit(1);}

int st;

void ss1(int s)
{
  st=0;
  //printf(" Gone away \n");

  openlog("My web",LOG_PID |LOG_CONS, LOG_DAEMON);
  syslog(LOG_INFO,"Stop");
  closelog();

  exit(0);
}



void rde(FILE *f)  //  read till - \r\n
{
   char buf[BUFSIZ];
   while( fgets(buf,BUFSIZ,f)!=NULL && strcmp(buf,"\r\n")!=0 );   
}

void hdr(FILE *f, char * t) //  Form header for HTTP  receipt
{
  fprintf(f,"HTTP/1.0 200 OK\r\n");
  if(t) fprintf(f,"Content-type:%s\r\n",t);  //  type of page
}

void cntdo(int fd)  //  Form HTTP reply for  - Can't do
{
   FILE *f = fdopen(fd,"w");
   fprintf(f,"HTTP/1.0 501 Not Impl\r\n");
   fprintf(f,"Content-type: text/plain\r\n");
   fprintf(f,"\r\n");
   fclose(f);
}

void do404(char *it,int fd)   //  Form HTTP reply error 404 (can't find the page)
{
   FILE *f=fdopen(fd,"w");
   fprintf(f,"HTTP/1.0 404 NotFound\r\n");
   fprintf(f,"\r\n");
   fprintf(f,"404 The item %s \r\n is not found \r\n",it);
   fclose(f);
   //printf(" 404 \n");
   exit(0);
}

int isdir(char *f)  // stat && ISDIR  - that's dir
{
  struct stat info;
  return ( stat(f,&info)!=-1 && S_ISDIR(info.st_mode) );  
}

int ntxt(char*f)  // check if it is existed (by - get stat)
{
  struct stat info;
  //printf(" ntxt arg %s \n",f);
  return ( stat(f,&info)==-1 );
}


char * f_typ(char *f) //  find the position of dot
{
   char *cp;
   if ( (cp=strchr(f,'.'))!=NULL ) return cp+1;
   return "";
}



void do_cat(char *f, int fd)
{
  char *ext=f_typ(f+2);  // extension
  char *cnt="text/plain"; 
  FILE *fs,*ff;
  int c;

  //printf(" cat arg %s \n",f);
  //printf(" cat ext %s \n",ext);

  if (strcmp(ext,"html")==0) cnt="text/html"; //form header compatible the file 
  else if (strcmp(ext,"gif")==0) cnt="image/gif";
  else if (strcmp(ext,"jpg")==0) cnt="image/jpeg";
  fs=fdopen(fd,"w");
  ff=fopen(f,"r");
  if(fs!=NULL && ff!=NULL)
  {
//     printf(" cat %s \n",cnt);
     hdr(fs,cnt);      // send HTTP reply
     fprintf(fs,"\r\n"); 
     while( (c=getc(ff))!=EOF ) putc(c,fs); // send file
     fclose(ff);
     fclose(fs);
  }
  exit(0); // exit the child proc
}


void prc(char *rq,int f)
{
   char cmd[BUFSIZ], arg[BUFSIZ];

   if (fork()!=0) return;  //
   strcpy(arg,"./");      // cur dir   - add the path
   if ( sscanf(rq,"%s%s",cmd,arg+2)!=2) return;  //  parse for cmd and arg
   
   if( strcmp(cmd,"GET")!=0 ) cntdo(f);   // there is no GET
   else if (ntxt(arg)) do404(arg,f);  // there are no args in request
   else do_cat(arg,f);                  // is html
}


void * websvr(void *ppp)
{
  int sock_id, fd;  
  FILE *f;
  char req[BUFSIZ];
  
  struct sockaddr_in  saddr,caddr;  
//  socklen_t skln;   
//  struct addrinfo hnt;
  struct hostent *hp;
  char hostnm[256],svc[256];    //NI_MAXHOST  NI_MAXSERV 
//  int sock_id, sock_fd;
//  FILE *sock_fp;
 

   //*************************************************** 
   gethostname(hostnm,256); 
   //printf("Cli HostName- %s\n",hostnm);

   hp=gethostbyname(hostnm);  // hostent - list of addresses, aliases, ...
   // *********************************
   // *********************************
   //  Fill   socketaddr_in from hostent  (h_addr - first addr)
   bzero((void*)&saddr,sizeof(saddr));//Clear
   //bcopy( (void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);// cpy addr
   //inet_aton("192.168.223.131",&saddr.sin_addr);
   inet_aton("127.0.1.1",&saddr.sin_addr);
   saddr.sin_port=htons(13000);  //to NetWk byte order
   saddr.sin_family=AF_INET; // 
//   printf("Server addr %s ,port %d \n",inet_ntoa(saddr.sin_addr),ntohs(saddr.sin_port));
   // socket                        IPPROTO_UDP
   sock_id=socket(PF_INET,SOCK_STREAM,0);
   if(sock_id==-1)oops("Sock");
   if( bind(sock_id, (struct sockaddr*)&saddr,sizeof(saddr))!=0) oops("bind");
   if ( listen(sock_id,5)!=0 ) oops("lst");
   //********************
  
   while(1)
   {
      fd=accept(sock_id, NULL,NULL);  
      f=fdopen(fd,"r");
      fgets(req,BUFSIZ,f);

//      printf("got - %s \n",req);
      rde(f);   



      prc(req,fd);



      fclose(f);
   }
}





//*******************************************************

main(int ar,char ** arv)
{
  int *p,pp[5];
  int opt,d;
  st=1;
  signal(SIGINT,ss1);
  signal(SIGTERM,ss1);

  //p[3]=3;
  //*p=1;

  printf("Server addr 127.0.1.1 ,port 13000 \n");

  d=0; 
  while( (opt=getopt(ar,arv,"d:"))!=-1 ) 
  {
     //printf("- %d \n",opt);
     switch(opt){
     case 'd':
        //printf("- %s \n",optarg); 
        if(strstr(optarg,"aemon")!=NULL ) d=1;
     break;
     default:
     break; 
     }
  }
  
     //printf("d- %d \n",d);

  
  if(d==0)
  {
      websvr(NULL);
      while(st);
  }else{

    if(fork()==0)
    {//child
   
      setsid();//
      close(0);
      close(1);
      close(2);

      //chdir("/");
    
      openlog("myweb",LOG_PID |LOG_CONS, LOG_DAEMON);
      syslog(LOG_INFO,"Start");
      closelog();

      websvr(NULL);
      while(st);
     }
   }

  printf( "---\n");
}




