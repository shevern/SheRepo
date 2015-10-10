#include <stdio.h>
#include <string.h>
#include <signal.h>

int main(int args, char **argv, char **argc)
{
  int ns,id;
  FILE *f;
  char ss[1000],ss2[1000];
  int l,i,j;   

  if(args<2) return;

  //f=popen("ls -l","r");
  f=popen(argv[1],"r");
  if(f==NULL){perror("wwww");return;}
 

  //while((ns=read(f,ss,1000))>0)
  while(fgets(ss,1000,f)!=NULL)
  {
     l=strlen(ss);

     if(ss[l-1]=='\n') {ss[l-1]=0; l--;}

     j=l-1;
     for(i=0;i<l;i++)ss2[j--]=ss[i];
     ss2[l]=0;

     printf("%s\n",ss2);
  }


   if(args==3)
   {
     //ns=atoi(&argv[2][0]);
     //id=atoi(&argv[1][0]);
     //return kill(id,ns); 
   }

}
