#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/cdev.h>


//static int mjr;
struct cdev *my_cdev;

static int my_open( struct inode *ino, struct file *fl)
{
   printk(KERN_INFO "Open \n");
  return 0;
}


static int my_close(struct inode *ino, struct file *fl)
{
   printk(KERN_INFO "prg3 Close \n");
  return 0;
}


static ssize_t my_write(struct file *fl, const char __user *buf, size_t cou,loff_t *pos)
{
  //copy_from_user
  return 0;
}

static ssize_t my_read(struct file *fl, char __user *buf, size_t cou,loff_t *pos)
{
  //  copy_to_user
  return 0;
}


  //

static const struct file_operations my_fops={
  .owner   = THIS_MODULE,
  .open    = my_open,
  .release = my_close,
  .write   = my_write,
  .read    = my_read,
};



		
static int __init myinit(void)
{
   int e;
   dev_t f_node;

   printk(KERN_INFO "prg3 Start \n");

   e=register_chrdev(800,"mydev",&my_fops);
   //mjr=register_chrdev(0,"mydev",&my_fops);
   if( e <0)
   {
       printk(KERN_INFO "prg3 Err register_chrdev %d \n",e);
       return 0;
   }


/*   if( (e=register_chrdev_region(MKDEV(800,0),1,"mydev")) <0)
   {
       printk(KERN_INFO "prg3 Errr register_chrdev_region   %d \n",e);
       return 0;
   }
*/


   
   f_node=MKDEV(800,0);
/*
   if( (e=register_chrdev_region(f_node,1,"mydev")) <0)
   {
       printk(KERN_INFO "Err register_chrdev_region %d \n",e);
       return 0;
   }
*/
   my_cdev=cdev_alloc();
   cdev_init(my_cdev,&my_fops);

   e=cdev_add(my_cdev,f_node,1);
   if( e <0)
   {
       printk(KERN_INFO "Err cdev_add %d \n",e);
       return 0;
   }

/*
   register_chrdev(800,"mydev",&my_fops);
   if( e <0)
   {
       printk(KERN_INFO "prg3 Err register_chrdev %d \n",e);
       return 0;
   }
*/

       printk(KERN_INFO "prg3 Ok \n");

   return 0;
}

static void __exit myexi(void)
{
 //  cdev_del(my_cdev);
//   unregister_chrdev(800,"mydev");
   
   printk(KERN_INFO "prg3 Stop \n");  
}

module_init(myinit);
module_exit(myexi);



//http://www.xatlantis.ch/education/linux_driver_example.html





















