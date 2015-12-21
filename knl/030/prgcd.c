
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

#include <linux/slab.h>

#include <linux/ioctl.h>

#include <linux/sched.h>

#include <linux/wait.h>
#include <linux/poll.h>

#include "prgcd.h"

#define suc 0
#define devnm "chdev"
#define blen 100

//   Stub for Sym Dvr 


  int ret1,ret2;
  dev_t dev;
  int min=0,maj=800;

  struct cdev *mycdev1,mycdev2;


static int Dev_Op=0;

static char Msg[blen];
static char *Msg_Ptr,*MsgP;

static DECLARE_WAIT_QUEUE_HEAD(wq);
static wait_queue_head_t inq, outq;
static int wtflg=0;
static struct fasync_struct *async_q=NULL;


static int d_fasync(int fd,struct file *fl, int on);

static int device_open(struct inode *inode, struct file *fl)
{

   if (Dev_Op) return -EBUSY;
   Dev_Op++;
   Msg_Ptr=Msg;
   
   MsgP=kmalloc(blen*sizeof(char*), GFP_KERNEL);
   if(!MsgP) { return -ENOMEM; }
   memset(MsgP,0,blen*sizeof(char*)); 

   init_waitqueue_head(&inq);//dyn declare
   init_waitqueue_head(&outq);

   try_module_get(THIS_MODULE);
   return suc;
}


static int device_release(struct inode *inode, struct file *fl)
{
   Dev_Op--;
   
   d_fasync(-1,fl,0); // fasycn release

   if(MsgP) kfree(MsgP);
   
   module_put(THIS_MODULE);
   return suc;
}

static int device_flush(struct file *fl)
{
   return suc;
}

static ssize_t device_read(struct file *fl, char __user *buf, size_t ln, loff_t *ofst)
{
   int bt_rd=0;

   DEFINE_WAIT(wt);

   //wait_queue_t wt2;
   //init_wait(&wt2); // manual


   // *****   wait 
   printk(KERN_INFO "prgcd wait_event before \n");

   // I
  // if( wait_event_interruptible(wq, wtflg!=0 ) )
  //      return -ERESTARTSYS;  // signal to fs to handle
  // wtflg=0;
   //  II
   //prepare_to_wait(&wq,&wt,TASK_INTERRUPTIBLE);
   prepare_to_wait_exclusive(&wq,&wt,TASK_INTERRUPTIBLE);
   schedule();
   finish_wait(&wq,&wt);

   printk(KERN_INFO "prgcd wait_event after \n");
   //--------
    

   if(Msg_Ptr==0) return 0;

  //    I ******
 /*  while (ln && *Msg_Ptr)
   {
      //            From     To   put_user(local,ptr)
      put_user(*(Msg_Ptr++),buf++); //from Knl to Usr
      ln--;
      bt_rd++;
   }
*/

  // access_ok(type, void *addr, ulong sz) - verify user data pointer
  // 0-ok  1-fail

   //   II ****** 
   if(copy_to_user(buf,MsgP,ln)) return-EFAULT; 
   bt_rd=ln;

   return bt_rd;
}

static ssize_t device_write(struct file *file, const char __user * buf,
                            size_t ln, loff_t *ofst)
{
  int i;

  // from Usr to Knl - get_user(local,ptr) To   From
  //for(i=0;i<ln && i<blen; i++) get_user(Msg[i],buf+i);
  //Msg_Ptr=Msg; 

  //*****
  if( copy_from_user(MsgP,buf,ln) ) return -EFAULT;
  i=ln;

  wtflg=1;
  wake_up_interruptible(&wq); 
  //wake_up_interruptible_all(&wq); 
  //wake_up(&wq);

  kill_fasync(&async_q,SIGIO,POLL_IN);

  printk(KERN_INFO "prgcd wake_up  after \n");

  return i;
}

static long device_ioctl(struct file *fl, unsigned int cmd, unsigned long par)// file, cmd, arg
{
   int i,e;
   char *tmp;
   char ch;

   //  Eddition checking
/*
  if( _IOC_TYPE(cmd)!=m_num ) return -ENOTTY;
  if( _IOC_DIR(cmd) & _IOC_READ) 
      e=!access_ok(VERIFY_WRITE, (void __user*)par, _IOC_SIZE(cmd) );
  if( _IOC_DIR(cmd) & _IOC_WRITE) 
      e=!access_ok(VERIFY_READ, (void __user*)par, _IOC_SIZE(cmd) );
  if(e) return -EFAULT;
*/

  //  capable (CAP_SYS_ADMIN)

   switch (cmd)
   {

    case IOCTL_SET_MSG:  // Usr to Knl

       tmp= (char*) par; //  (char __user *) par 
       get_user(ch,tmp);
       for(i=0; ch && i<blen; i++,tmp++) get_user(ch,tmp);// get length

       device_write(fl, (char*) par,i,0);
    break;

    case IOCTL_GET_MSG:

       i=device_read(fl, (char*)par, 99, 0); // To Usr
       put_user('\0', (char*) par+1);
    break;

    case IOCTL_GET_NTH_BYTE:

       return Msg[par];
    break;

    default:
    break;
   }

   return suc;
}




static int device_poll(struct file *fl, poll_table *wt)
{
  unsigned int r=0;
//poll_table_struct

   
 //  poll_wait(fl,&wq,wt);

  
   //if(...) r=POLLIN  | POLLRDNORM;
   //if(...) r=POLLOUT | POLLWRNORM;

    //r=POLLIN  | POLLRDNORM; //for read immidiatly
   
   return r;
}


static int d_fasync(int fd,struct file *fl, int on)
{
   int r;
   printk(KERN_INFO "prgcd fasync in \n");
   //async_q

//   if(!async_q) //!=NULL
   {

      printk(KERN_INFO "prgcd fasync in 2 \n");

      r=fasync_helper(fd,fl,1,&async_q);
      if(r>=0)
      {
         printk(KERN_INFO "prgcd fasync Ok \n");
         //return 0;
       }else{

	 printk(KERN_INFO "prgcd fasync Err \n");
         //return -EIO;
       }
    }
  return r;
}


//***

struct file_operations fops={
  .owner=THIS_MODULE,
  .read=device_read,
  .write=device_write,
  .unlocked_ioctl=device_ioctl,
  .compat_ioctl=device_ioctl,
  .open= device_open,
  .release=device_release,
  .flush = device_flush,
  .poll= device_poll,
  .fasync = d_fasync,
};


//****

int init_module()
{


  // new way reg  ***
  //***  get number
  if(maj){
    dev=MKDEV(maj,min);
    ret1=register_chrdev_region(dev,1,devnm);
  }else{
    ret1=alloc_chrdev_region(&dev,min,1,devnm);
    maj=MAJOR(dev);
  }
  if(ret1<0){
    printk(KERN_WARNING "prgcd region err %d \n",ret1);
    return ret1;
  }
  //  reg
 
  // cdev - stantalone
  //mycdev1=cdev_alloc();
  //mycdev1->ops=&fops;
  //mycdev1->owner=THIS_MODULE;

  cdev_init(&mycdev2,&fops);// cdev - already allocated
  mycdev2.owner=THIS_MODULE;
  mycdev2.ops=&fops;

  ret2=cdev_add(&mycdev2,dev,1);
  if(ret2<0){
    printk(KERN_WARNING "prgcd cdev_add err %d \n",ret2);
    return ret2;
  }

  // old way reg  ***  
  //  ret2=register_chrdev(m_num,devnm,&fops);

  printk(KERN_WARNING "prgcd Ok-Up  \n");
  return 0;
}

void cleanup_module()
{
  // del cdev
  if(ret2==0) cdev_del(&mycdev2);
  if(ret1==0) unregister_chrdev_region(dev,1);

  printk(KERN_WARNING "prgcd Ok -down  \n");
// old way reg  *** 
//  unregister_chrdev(m_num,devnm); 
}







// lsmod |grep prg
// insmod ./prgcd.ko
// rmmod prgcd
//  mknod cdev c 100 0




/* macros & functions to use

//  kthread_run   kthread_stop ?????


 *****   Dev numbers  **************************
int min,maj; // numbers of device
dev_t dev; - holds dev nums - major & minor (32 bit)

maj=MAJOR(dev);//get major
min=MINOR(dev);
dev=MKDEV(maj,min); // get dev_t from maj & min

    Allocate dev numbers
int register_chrdev_region(dev_t first, uint count, char * name)
count - numbrs to reguest

    Dynamic-Alloc
int alloc_chrdev_region(dev_t*dev,uint firstminor,uint count,char* name);

    Free numbers
void unregister_chrdev_region(dev_t first, uint count);

//  get by inode
uint iminor(struct inode *ino)
uint imajor(struct inode *ino)

--structures
file_operations
file
inode
cdev - represents char devices
                       
int cdev_add(cdev *dev,dev_t first,uint count);

*****************************************************
*** waiting

  ---  sleep
  wait_event(wq,cond)
  wait_event_interruptible(wq,cond)
  wait_event_timeout(wq,cond,tiou)
  wait_event_interruptible_timeout(wq,cond,tiou)
  --- wake
  wake_up(&wq)
  wake_up_interruptible(&wq)


/proc/devices & sysfs
*/





















