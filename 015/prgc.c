#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <signal.h>
#include <pthread.h>

#define oops(e) { perror(e);exit(1);}

//    Client 




 struct tmsg
 {
    int cmd;
    char nm[50];
    char buf[200]; 
    int c;
 };

   int sock_id, st;

 void * fcli(void * pr)
 { 
   char buf[300];
   struct tmsg msg;
   int i;

   st=1; 
   while(st)
   {
      bzero((void*)&msg,sizeof(msg));//Clear
      bzero((void*)&buf[0],300);//Clear
      i=recv(sock_id,buf,300,0);
      if(i==0)break;
      //if(i>0) send(sock_fd,buf,100,0);
      bcopy( (void*)buf, (void*)&msg, sizeof(msg));// cpy addr

      if(msg.cmd==102)//Message 
      {
         //printf("cmd - %d \n",msg.cmd);
         printf("Messsage - %s \n",msg.buf); 
      }
      if(msg.cmd==103)//list 
      {
         //printf("cmd - %d \n",msg.cmd);
         printf("cli - %s \n",msg.nm); 
      }

   }

    printf("Conn Closed \n");
    close(sock_id);
    exit(0);
 }

 void sgnl(int n)
 {
    st=0;
    printf("\n Term \n");
    sleep(1);
    exit(0);
 }


main(int ar,char ** arv)
{
   int i,j,nprt;
   char * pt;
   char buf[300], var[200], nm[50];   //  Allocate 
   struct sockaddr_in  saddr;
   struct addrinfo hnt,*ares,*arp;
   struct sockaddr_storage peer_a; // address to communicate
   socklen_t peer_ln;
   struct hostent *hp;
   char hostnm[256],svc[256];    //NI_MAXHOST  NI_MAXSERV 
   FILE *sock_fp;
   struct tmsg msg;
   pthread_t th;
   unsigned char inadr[100];
   struct in_addr inp;

   signal(SIGINT,sgnl);

   if(ar<4) exit(0);

   sprintf(nm,"%s",&arv[1][0]);
  
   i=inet_aton(arv[2],&inp);
   if(i<0) oops("adr");   

   sprintf(var,"%s",&arv[3][0]);//prt
   nprt=13000;
   nprt=atoi(var);

   printf("Cli Name- %s \n",nm);

//exit(0);

   //Var I  *************************

   // 1   Get (choose one of ) address of host
   gethostname(hostnm,256); hostnm[255]=0;
   printf(" HostName %s\n",hostnm);
   hp=gethostbyname(hostnm);  // hostent - list of addresses, aliases, ...

   //Svr Addr 
   bzero((void*)&saddr,sizeof(saddr));//Clear
   //bcopy( (void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);// cpy addr
   i=inet_aton(&arv[2][0],&saddr.sin_addr);
   if(i<0) oops("adr");
   saddr.sin_port=htons(nprt);  //to NetWk byte order
   saddr.sin_family=AF_INET; // 

   // *****  CLIENT  STREAM  (TCP)  part
   sock_id=socket(PF_INET,SOCK_STREAM,0);
   if(sock_id==-1)oops("Sock");
 
   //saddr.sin_port=htons(13001);  //to NetWk byte order
   //if( bind(sock_id, (struct sockaddr*)&saddr,sizeof(saddr))==-1) oops("bind");
   //saddr.sin_port=htons(13000);  //to NetWk byte order
   if( connect(sock_id, (struct sockaddr*)&saddr,sizeof(saddr))==-1)
         oops("Conn");
   //if( connect(sock_id,arp->ai_addr,arp->ai_addrlen)!=-1) break; 
   printf("Conn to svr %s %d \n",inet_ntoa(saddr.sin_addr),ntohs(saddr.sin_port));

   pthread_create(&th,NULL,fcli,NULL);


   bzero((void*)&msg,sizeof(msg));//Clear
   msg.cmd=1;
   strcpy(msg.nm,nm);
   bzero((void*)&buf[0],300);//Clear
   bcopy( (void*)&msg, (void*)buf, sizeof(msg));// cpy addr
   send(sock_id,buf,sizeof(msg),0);



   while(1)
   {  

      printf("1 - disconnect \n");
      printf("2 - send to all \n");
      printf("3 - send to \n");
      printf("4 - get last \n");
      printf("5 - list of cli \n");

      gets(var);
      if ( (strstr(var,"1")!=NULL) && (strlen(var)==1))
      {
         close(sock_id);
         exit(0);

      }
      if ( (strstr(var,"2")!=NULL) && (strlen(var)==1))
      {
          printf("Message to all \n");
          gets(buf);
          bzero((void*)&msg,sizeof(msg));//Clear
          msg.cmd=2;
          strcpy(msg.buf,buf);
          bzero((void*)&buf[0],300);//Clear
          bcopy( (void*)&msg, (void*)buf, sizeof(msg));// cpy addr
          send(sock_id,buf,sizeof(msg),0);
      }
      if ( (strstr(var,"3")!=NULL) && (strlen(var)==1))
      {
          bzero((void*)&msg,sizeof(msg));//Clear
          msg.cmd=3;

          printf("Cli name \n");
          gets(buf);
          strcpy(msg.nm,buf);

          printf("Message to cli \n");
          gets(buf);
          strcpy(msg.buf,buf);
          bzero((void*)&buf[0],300);//Clear
          bcopy( (void*)&msg, (void*)buf, sizeof(msg));// cpy addr
          send(sock_id,buf,sizeof(msg),0);
      }
     
      if ( (strstr(var,"4")!=NULL) && (strlen(var)==1))
      {
          printf("Get last (count) \n");
          gets(buf);
          bzero((void*)&msg,sizeof(msg));//Clear
          msg.cmd=4;
          strcpy(msg.buf,buf);
          bzero((void*)&buf[0],300);//Clear
          bcopy( (void*)&msg, (void*)buf, sizeof(msg));// cpy addr
          send(sock_id,buf,sizeof(msg),0);
      }
      if ( (strstr(var,"5")!=NULL) && (strlen(var)==1))
      {
          bzero((void*)&msg,sizeof(msg));//Clear
          msg.cmd=5;
          bzero((void*)&buf[0],300);//Clear
          bcopy( (void*)&msg, (void*)buf, sizeof(msg));// cpy addr
          send(sock_id,buf,sizeof(msg),0);
      }






      continue;

      sprintf(buf,"qqqqqqqqqqqqqqqqqqqqq");
   
      //write(sock_id,buf,strlen(buf));
      //sprintf(buf,"");
      //read(sock_id,buf,200);

      printf(buf);
      send(sock_id,buf,100,0);
      sprintf(buf,"");
      //recv(sock_id,buf,100,0);
      buf[200]=0;    
      printf(buf);

      sleep(1);
   }

   // **********************************

  i=BUFSIZ; //bufsiz;  //  Max size of bufs
  printf( " -------%d  \n",i);
}




