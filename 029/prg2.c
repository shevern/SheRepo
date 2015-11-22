#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/moduleparam.h>

#include "prg.h"

extern struct tst ff[20];
extern int myget(int a);
static int mydt __initdata=3;

//locals
static short int mysho=1;
static int myi=222;
static long int mlo=111;
static char *mcha="ppp";
static int ar[4];

// MODULE_PARAM(var,"b"); //byte
// MODULE_PARAM(var,"h"); //short
// MODULE_PARAM(var,"i");
// MODULE_PARAM(var,"l"); //long
// MODULE_PARAM(var,"s"); //str
// MODULE_PARAM(ar,"4i"); // array of int - len 4

// module_param(nam,int,0000); // with  access mod
//module_param(myi,int,0644);
//MODULE_PARM_DESC(myi,"my int");


static int __init myinit(void)
{
   printk(KERN_INFO "aaa \n");

   //printk(KERN_INFO "par - %d \n",myi);
   printk(KERN_INFO "ff - %d \n",ff[0].a);
   printk(KERN_INFO "get - %d \n",myget(99));
   return 0;
}

static void __exit myexi(void)
{
   printk(KERN_INFO "bbb \n");  
}

module_init(myinit);
module_exit(myexi);



