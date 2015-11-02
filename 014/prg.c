#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <string.h>

#include <pthread.h>
#include <signal.h>

//   
#define oops(e) { perror(e);exit(1);}

int st;

void exit(int i);

// Client for Listening  (UDP svr)
void *cli(void * par)
{
   int i,j,prt;
   char * pt;
   char buf[200], var[200];   //  Allocate 

   unsigned char inadr[100];
   struct sockaddr_in  saddr,caddr;
   struct sockaddr_storage peer_a; // address to communicate
   socklen_t peer_ln, skln; 
   struct addrinfo hnt,*ares,*arp;
   struct hostent *hp;
   char hostnm[256],svc[256];    //NI_MAXHOST  NI_MAXSERV 
   int sock_id, sock_fd;
   FILE *sock_fp;
   int opt=1;

   prt=*(int*)par;

   //  Get (choose one of ) address of host
   gethostname(hostnm,256); 
   //printf("Cli HostName- %s\n",hostnm);

   hp=gethostbyname(hostnm);  // hostent - list of addresses, aliases, ...
   //hp=gethostbyaddr( );  // hostent - list of addresses, aliases, ...
   //printf("Cli h_nm  %s ",hp->h_name);
   //printf("addr typ %d ",hp->h_addrtype);
   //printf("addr len   %d \n",hp->h_length);
   //for(i=0;i<hp->h_length;i++) printf("%d ",hp->h_addr_list[0][i]);printf("\n");

   //  Fill   socketaddr_in from hostent  (h_addr - first addr)
   bzero((void*)&saddr,sizeof(saddr));//Clear
   //bcopy( (void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);// cpy addr
   inet_aton("127.0.1.255",&saddr.sin_addr);
   saddr.sin_port=htons(prt);  //to NetWk byte order
   saddr.sin_family=AF_INET; // 
   //printf("Recv addr %s prt %d \n",inet_ntoa(saddr.sin_addr),ntohs(saddr.sin_port));
   printf("Recv addr 127.0.1.255 prt %d \n",ntohs(saddr.sin_port));
   // *********************************


   // socket                        IPPROTO_UDP
   sock_id=socket(PF_INET,SOCK_DGRAM,0);
   if(sock_id==-1)oops("Sock");

   setsockopt(sock_id,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt));
 
   // bind
   if( bind(sock_id, (struct sockaddr*)&saddr,sizeof(saddr))!=0) oops("bind");

   while(st)
   {

     // 1   recvfrom
     bzero(buf,200);
     i=recvfrom(sock_id,buf,200,0,(struct sockaddr*)&peer_a,&peer_ln);
     if(i<0) oops("recFrm");  
     j=getnameinfo( (struct sockaddr*)&peer_a,peer_ln,     //get name by address
                     hostnm,255,svc,255,NI_NUMERICSERV);
     //if(j==0)printf("From %s  %s  ",hostnm,svc);
     
     //j=sendto(sock_id,buf,i,0,(struct sockaddr*)&peer_a,peer_ln);
     buf[199]=0;
     printf("Got - %s \n",buf); 

     sleep(1);
  }

   pthread_exit(NULL);
}



//  
void *svr(void * par)
{
   int i,j,prt;
   char * pt;
   char buf[200], var[200];   //  Allocate 
   unsigned char inadr[100];
   struct sockaddr_in  saddr,caddr;
   struct sockaddr_storage peer_a; // address to communicate
   socklen_t peer_ln, skln;   
   struct addrinfo hnt,*ares,*arp;
   struct hostent *hp;
   char hostnm[256],svc[256];    //NI_MAXHOST  NI_MAXSERV 
   int sock_id, sock_fd;
   FILE *sock_fp;
   int opt=1;

   prt=*(int*)par;

   //  Get (choose one of ) address of host
   gethostname(hostnm,256); 
   printf("svr HostName- %s\n",hostnm);

   hp=gethostbyname(hostnm);  // hostent - list of addresses, aliases, ...
   //hp=gethostbyaddr( );  // hostent - list of addresses, aliases, ...
   //printf("svr h_nm  %s  ",hp->h_name);
   //printf("addr typ %d  ",hp->h_addrtype);
   //printf("addr len   %d \n",hp->h_length);
   //for(i=0;i<hp->h_length;i++) printf("%d ",hp->h_addr_list[0][i]);      
   //printf("\n");   

   //  Fill   socketaddr_in from hostent  (h_addr - first addr)
   bzero((void*)&saddr,sizeof(saddr));//Clear
   //bcopy( (void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);// cpy addr
   inet_aton("127.0.1.255",&saddr.sin_addr);
   saddr.sin_port=htons(prt);  //to NetWk byte order
   saddr.sin_family=AF_INET; // 
   //printf("Remote addr %s %d \n",inet_ntoa(saddr.sin_addr),ntohs(saddr.sin_port));
   printf("Remote addr 127.0.1.255 %d \n",ntohs(saddr.sin_port));
   //*********************************

   // socket                        IPPROTO_UDP
   sock_id=socket(PF_INET,SOCK_DGRAM,0);
   if(sock_id==-1)oops("Sock");
 
   setsockopt(sock_id,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt));

   // bind
   //if( bind(sock_id, (struct sockaddr*)&saddr,sizeof(saddr))!=0) oops("bind");

   while(st)
   {
     bzero(buf,200);
     gets(buf);         
     j=sendto(sock_id,buf,strlen(buf),0,(struct sockaddr*)&saddr,sizeof(saddr));
     //printf("%d  %d \n",i,j); 

     sleep(1);
   }
   pthread_exit(NULL);
  
}

void ss1(int s)
{
  st=0;
  //printf("SIGINT \n");
}





main(int ar,char ** arv)
{
  pthread_t t1,t2;   

  int i,j,prt1,prt2;
  char * pt;
  char buf[200], var[200];   //  Allocate 

  if(ar<3){printf(" Prt1 Prt2   \n");  return; }
  prt1=atoi((char*)&arv[1][0]);
  prt2=atoi((char*)&arv[2][0]);

  st=1;
  signal(SIGINT,ss1);


  pthread_create(&t1,NULL,cli,(void*)&prt1);
  pthread_create(&t2,NULL,svr,(void*)&prt2);

  
  while(st);
  //pthread_join(t1,NULL);
  //pthread_join(t2,NULL);



  // **********************************

  i=BUFSIZ; //bufsiz;  //  Max size of bufs

  printf( "---\n");

}




