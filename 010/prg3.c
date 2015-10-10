#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>

int main(int args, char **argv, char **argc)
{
  int ns,id;
  FILE *f;
  char ss[1000],ss2[1000],pp[100];
  int l,i,j,cou,ii;   
  int ptr;

  //if(args<2) return;

  //f=popen(argv[1],"r");
  //if(f==NULL){perror("wwww");return;}
 
  cou=0;

  //while((ns=read(f,ss,1000))>0)
  while(fgets(ss,1000,stdin)!=NULL)   // From stdin
  {
     l=strlen(ss);

     if(ss[l-1]=='\n') {ss[l-1]=0; l--;}

     j=l-1;
     for(i=0;i<l;i++)ss2[j--]=ss[i];
     ss2[l]=0;

     //printf("[%s]\n",ss2);

     sprintf(pp,argv[1]);
     //pp="p";

     j=0; 
     while (j<(l-1))
     {
         //printf("1\n");
         if( (i=strstr(&ss[j],pp))!=NULL )
         {
           ii=&ss[j]; 
           j=j+strlen(pp)+(i-ii);
           //printf("2\n");
           cou++;
         } else {
          //printf("3\n");
          break;}
      }

     sprintf(ss2,"%d",cou);     
     f=creat(argv[2],S_IRUSR|S_IWUSR);
     write(f,ss2,strlen(ss2));

     puts(ss);  // To stdout
  }

  //printf("*** %d\n",cou);

}
