#include <stdio.h>
#include <string.h>
#include <signal.h>

int main(int args, char **argv, char **argc)
{
  int ns,id;
  FILE *f;
  char ss[1000],ss2[1000],pp[100];
  int l,i,j,cou;   

  //if(args<2) return;

  //f=popen(argv[1],"r");
  //if(f==NULL){perror("wwww");return;}
 

  //while((ns=read(f,ss,1000))>0)
  while(fgets(ss,1000,stdin)!=NULL)
  {
     l=strlen(ss);

     if(ss[l-1]=='\n') {ss[l-1]=0; l--;}

     j=l-1;
     for(i=0;i<l;i++)ss2[j--]=ss[i];
     ss2[l]=0;

     printf("[%s]\n",ss2);

     pp="p";

     j=0; cou=0;
     while (j<(l-1))
     {
         printf("1\n");
         if( (i=strstr(&ss[j],pp))>0 )
         {
           j=j+strlen(pp);
           printf("2\n");
         } else { printf("3\n"); break;}
      }



     puts(ss);
  }


}
