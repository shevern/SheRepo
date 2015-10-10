#include<stdio.h>
#include<dlfcn.h>

int main (int argc, char **ar2, char **ar3)
{
  void *ha;
  void (*f1)();
  char * (*f2)(char* a);   //  return char*
  char *er;
  char a[10],b[10];

  ha=dlopen("./libprg2lb.so", RTLD_LAZY);

  if(!ha)
  {
   printf(" err LD  %s  \n", dlerror());
   return 1;
  }

  f1=dlsym(ha,"f2");
  er=dlerror();
  if(er!=NULL) 
  {
     printf(" err sym f2  %s  \n", er);
     return 1;
  }

  f2=dlsym(ha,"f3");
  er=dlerror();
  if(er!=NULL) 
  {
     printf(" err sym f3 %s  \n", er);
     return 1;
  }

  (*f1)();
  //a="QQQ";
  printf("f3- %s\n",f2("QQQ"));
  //printf("par- %s\n",a);
  //f2("WWW");

 dlclose(ha);


}



