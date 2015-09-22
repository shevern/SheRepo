#include<stdio.h>
#include<dlfcn.h>

int main (int argc, char **ar2, char **ar3)
{
  void *ha;
  int (*f1)();
  char *er;

  ha=dlopen("/home/study/SheRepo/003/libprg2lb.so", RTLD_LAZY);

  if(!ha)
  {
   printf(" err LD  %s  \n", dlerror());
   return 1;
  }

 f1=dlsym(ha,"f2");
 er=dlerror();
 if(er!=NULL) 
 {
 printf(" err sym  %s  \n", er);
 return 1;
 }
 (*f1)();

 dlclose(ha);


}



