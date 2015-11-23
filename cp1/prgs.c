#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdlib.h>

#include <malloc.h>

#define oops(e) { perror(e);exit(1);}

//   Server

 struct scli
 { 
    int t;//
    int f;//  0-empty  1-
    char nm[200];
    char adr[200],prt[200];
    pthread_t th;
    int   sock_fd,sock_fp;
 };

 struct tmsg
 {
    int cmd;
    char nm[50];
    char buf[200];
    int c; 
 };


 int cou1=0,cou2=0; 

 int st; 
 struct scli clis[200];
 char  **inLst=0;
 int numIn;
 pthread_mutex_t mx1=PTHREAD_MUTEX_INITIALIZER; 

 int gtc()
 {
   int i;
   for(i=0;i<200;i++) 
     if(clis[i].f==0) return i;
   return -1;
 }  

 void tof(char *a)
 {
   int fd,p;
   FILE *f;

   fd=strlen(a);
   a[fd]='\n';
   a[fd+1]=0;

   pthread_mutex_lock(&mx1); 
   f=fopen("a","a+");
   if(f==NULL)oops("file");
   fputs(a,f);
   //p=ftell(fd);
   fclose(f);
   pthread_mutex_unlock(&mx1); 
 } 
  
 int ldf()
 {  
   char buf[256];
   int mx_sz=0;
   FILE *f;

   f=fopen("a","r");
   if(f==NULL)oops("file");

   inLst=0;
   numIn=0;
   //while(feof(f)!=0)
   while(fgets(buf,256,f)!=NULL)
   {
      //printf(buf);
      if(++numIn>mx_sz) // alloc for pointer on char
      { 
        mx_sz +=2;
        if(!inLst)  
           inLst=(char**)calloc(mx_sz,sizeof(char*)); //for the first time  
        else
           inLst=(char**)realloc(inLst,sizeof(char*) * mx_sz); // for others
      }
      // alloc for buf
      inLst[numIn-1]=(char*)malloc( strlen(buf)+1 );
      strcpy( inLst[numIn-1],buf );
      //sleep(1);
   }
   fclose(f);
/* 
   while(--numIn>=0)
   {
      //printf(inLst[numIn]);
      free( inLst[numIn] );
   }
   free(inLst); 
*/

 }

 void * fcli(void * pr)
 { 
   struct scli *cl,*cl2;
   int sock_fd,sock_fd2,i,j;
   char buf[300];
   struct tmsg msg;
   

   cl= (struct scli *)pr;
   sock_fd=cl->sock_fd;

   while(st)
   {
      bzero((void*)&msg,sizeof(msg));//Clear
      bzero((void*)&buf[0],300);//Clear
      i=recv(sock_fd,buf,300,0);
      if(i==0)break;
      //if(i>0) send(sock_fd,buf,100,0);
      bcopy( (void*)buf, (void*)&msg, sizeof(msg));// cpy 

      if(msg.cmd==1)//Name
      {
         //printf("cmd - %d \n",msg.cmd);
         //printf("Connected  - %s \n",msg.nm); 
         for(j=0;j<200;j++) 
           if((clis[j].sock_fd==sock_fd)&& (clis[j].f==1))
         {
            strcpy(clis[j].nm,msg.nm); 
         }
      }

      if(msg.cmd==2)//Message to all
      {
         //printf("cmd - %d \n",msg.cmd);
         //printf("Msg to all - %s \n",msg.buf); 
         msg.cmd=102;
         bzero((void*)&buf[0],300);//Clear
         bcopy( (void*)&msg, (void*)buf, sizeof(msg));// cpy addr

         for(j=0;j<200;j++) 
           if((clis[j].sock_fd!=sock_fd)&&(clis[j].f==1))
         {
            send(clis[j].sock_fd,buf,300,0);
         }
         tof(msg.buf);
      }    
 
      if(msg.cmd==3)
      {
         //printf("cmd - %d \n",msg.cmd);
         //printf("Msg to %s - %s \n",msg.nm,msg.buf); 

         //printf("Msg to %d - %d \n",cou1,cou2); 
         sprintf(msg.buf,"Msg to USR1 %d - USR2 %d \n",cou1,cou2); 


         msg.cmd=102;
         bzero((void*)&buf[0],300);//Clear
         bcopy( (void*)&msg, (void*)buf, sizeof(msg));// cpy addr

         for(j=0;j<200;j++) 
           if(clis[j].f==1)
           if ( (strstr(clis[j].nm,msg.nm)!=NULL) &&
                (strlen(clis[j].nm)==strlen(msg.nm)))
         {
            send(clis[j].sock_fd,buf,300,0);
         }
         tof(msg.buf);
      }    
 
      if(msg.cmd==4)
      {
         //printf("cmd - %d \n",msg.cmd);
         //printf("Count - %s \n",msg.buf);
         i=0;j=0;
         i=atoi(msg.buf);
         //printf("Count - %d \n",i);

         pthread_mutex_lock(&mx1); 
         ldf();
         while(--numIn>=0)
         {
           //printf(inLst[numIn]);
           if(j++<i)
           {
              inLst[numIn][ strlen(inLst[numIn])-1]=0;
              strcpy(msg.buf,inLst[numIn]);
              msg.cmd=102;
              bzero((void*)&buf[0],300);//Clear
              bcopy( (void*)&msg, (void*)buf, sizeof(msg));// cpy addr
              send(sock_fd,buf,300,0);
           }
           free( inLst[numIn] );
         }
         free(inLst); 
         pthread_mutex_unlock(&mx1); 
      }     

      if(msg.cmd==5)//list
      {
         //printf("cmd - %d \n",msg.cmd);

         for(j=0;j<200;j++) 
           if(clis[j].f==1)
         {
            strcpy(msg.nm,clis[j].nm);
            msg.cmd=103;
            bzero((void*)&buf[0],300);//Clear
            bcopy( (void*)&msg, (void*)buf, sizeof(msg));// cpy addr
            send(sock_fd,buf,300,0);
         }
      }     
//      sleep(1);
    }
    //printf("Conn Closed %s %s Name %s \n",cl->adr,cl->prt,cl->nm);

    close(sock_fd);
    cl->f=0;//Clear;
 } 


 void sgnl(int n)
 {
    cou1++;
    signal(SIGUSR1,sgnl);
 }

 void sgnl2(int n)
 {
    cou2++;
    signal(SIGUSR1,sgnl2);
 }
 void sgnl3(int n)
 {
    cou2++;
    //signal(SIGINT,sgnl3);
    //signal(SIGINT,SIG_IGN);
 }


main(int ar,char ** arv)
{
   int i,j,nprt;
   char * pt;
   char buf[200], var[200];   //  Allocate 
   unsigned char inadr[100];
   struct sockaddr_in  saddr,caddr;
   socklen_t skln;
   struct hostent *hp;
   char hostnm[256],svc[256],svrnm[256];    //NI_MAXHOST  NI_MAXSERV 
   int sock_id, sock_fd;
   FILE *sock_fp;
   
   struct sigaction a1,a2;    

  //tof("qqq",3);
   //ldf();
   //exit(0);

   if(ar<3) {printf("Name Port \n");exit(0);}
 
   sprintf(var,"%s",&arv[2][0]);//prt
   sprintf(svrnm,"%s",&arv[1][0]);//svr nm
   nprt=13000;
   nprt=atoi(var);

   printf("Svr Name-  127.0.1.1  %s \n",svrnm);


if(fork()==0)
{
   setsid();
 //  close(0);
 //  close(1);
 //  close(2);



   signal(SIGUSR1,sgnl);
   signal(SIGUSR2,sgnl2);
   //signal(SIGINT,sgnl3);


  // a1.sa_handler=sgnl3;
  // a1.sa_flags=SA_RESTART;
  // sigaction(SIGINT,&a1,&a2); 



   //while(9);


   //  Get (choose one of ) address of host
   gethostname(hostnm,256); 
   //printf("HostName- %s\n",hostnm);

   hp=gethostbyname(hostnm);  // hostent - list of addresses, aliases, ...
   //hp=gethostbyaddr( );  // hostent - list of addresses, aliases, ...
//   printf("h_nm  %s\n",hp->h_name);
//   printf("addr typ %d\n",hp->h_addrtype);
//   printf("addr len   %d\n",hp->h_length);
//   for(i=0;i<hp->h_length;i++) printf("%d   ",hp->h_addr_list[0][i]);      
//   printf("\n");
   
   //  Fill   socketaddr_in from hostent  (h_addr - first addr)
   bzero((void*)&saddr,sizeof(saddr));//Clear
   bcopy( (void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);// cpy addr
   saddr.sin_port=htons(nprt);  //to NetWk byte order
   saddr.sin_family=AF_INET; // 
  // printf("svr addr %s prt %d \n",(char*)inet_ntoa(saddr.sin_addr),ntohs(saddr.sin_port));
   //*********************************

   // ***** STREAM  (TCP)  part
   sock_id=socket(PF_INET,SOCK_STREAM,0);
   if(sock_id==-1)oops("Sock");
   if( bind(sock_id, (struct sockaddr*)&saddr,sizeof(saddr))!=0) oops("bind");
   //for(i=0;i<1000;i++)
   // if( bind(sock_id, (struct sockaddr*)&saddr,sizeof(saddr))==0) break;

   if( listen(sock_id,5)!=0) oops("lsn");

   st=1;
   while(st)
   { 
      bzero(&caddr,sizeof(caddr));  //  Client addr      
      skln=sizeof(caddr);
      sock_fd=accept(sock_id,(struct sockaddr*)&caddr,&skln);  // accept
      //sock_fd=accept(sock_id,NULL,NULL);  // accept
      if(sock_id==-1) oops("acce");
    //  printf("Client Connected - %s  %d \n",(char*)inet_ntoa(caddr.sin_addr ) ,                                            caddr.sin_port);       
/*
      //  var 1
      sock_fp=fdopen(sock_fd,"w");
      if(sock_fp==NULL) oops("fdop");   
 //     fprintf(sock_fp, "dd server  dddd");
 //     fclose(sock_fp); 
  
      //  var 2
  //    i=recv(sock_fd,buf,100,0);
  //    if(i>0) send(sock_fd,buf,100,0);
     // close(sock_fd);
*/
      i=gtc();
    //  if(i<0){printf("Too many cli");continue;} 
    //  sprintf(clis[i].adr, "Addr %s",(char*) inet_ntoa(caddr.sin_addr));       
      sprintf(clis[i].prt,"Prt %d", ntohs(caddr.sin_port));
      clis[i].f=1;    
      clis[i].sock_fd=sock_fd;  
      pthread_create(&clis[i].th,NULL,fcli,(void*)&clis[i]);
   }
   close(sock_id);


}



   // **********************************

  i=BUFSIZ; //bufsiz;  //  Max size of bufs
  printf( " -------%d  \n",i);
}




