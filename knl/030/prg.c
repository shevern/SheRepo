#include <linux/module.h>
#include <linux/kernel.h>
#include "prg.h"



struct tst ff[20];
EXPORT_SYMBOL(ff);

int myget(int a)
{
  return a+44;
}
EXPORT_SYMBOL(myget);


int init_module(void)
{
  printk(KERN_INFO "H W \n");

  ff[0].a=11;

  return 0;
}

void cleanup_module(void)
{

   printk(KERN_INFO "G B \n");
}
