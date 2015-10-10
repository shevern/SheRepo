#include "prg2lb.h"

void f2()
{

  printf(" f2 call  \n");

}

void * f3(char *a)
{
  printf(" f3 call  %s\n",a);
  //a="f3";
  return "f3 call";
}



