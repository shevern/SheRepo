#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h> // for get_user put_user
#include <linux/fs.h>
#include <linux/types.h>

#define MsgLn  80
#define FNm "rwTst2"


//  ***   chapter 5.2  ****************

//struct proc_dir_entry;

static char Msg[MsgLn];
static struct proc_dir_entry *PrcFl, *dir;

//struct proc_dir_entry *PrcFl;
//{
//  loff_t size;
//} *PrcFl;

extern struct proc_dir_entry *create_proc_entry(const char *name, umode_t mode,
          struct proc_dir_entry *parent);

// To make a Dev   drv=12 device=2
//   mknod /dev/coffoo c 13 2

//   /usr/include/linux/fs.h
//  struct file_operations
//  struct file


// *******************************************************************

// in usr mode   it is as read
static ssize_t module_output(struct file *filp,char *bf,size_t ln,loff_t *ofst)
{
   static int fnsh=0;
   int i;
   char mes[MsgLn+30];

   if(fnsh) // indicate end of file
   {
      fnsh=0;
      return 0;
   }

   // dt knl->usr
   sprintf(mes,"ssq %s",Msg);

   // ****  Data To User   *******
   for(i=0; i<ln && mes[i]; i++)  
     put_user(mes[i],bf+i); // mes[i] -> bf+i

   fnsh=1;
   return i;  // count of read
}

// in usr mode   it is as write
static ssize_t module_input(struct file *filp,const char *bf, size_t ln, loff_t *of)
{
   int i;

   // ***  Data From User   *********
   for(i=0;i<MsgLn-1 && i<ln;i++)
     get_user(Msg[i],bf+i);   //  Msg[i] <- bf+1

   Msg[i]='\0';
   
   return i;
}


static int module_permission (struct inode * ino, int op, struct nameidata *foo)
{
  // 0- exec  2-wr (usr->knl)  4-rd (knl -> usr)

                //  root wr only
// if(op==4 || (op==2 && current->euid==0)) return 0;
 
 return -EACCES; 
 //  return =0 - permit    <>0 - not permit
}


// counter add     when module  is opened
int module_open(struct inode *inode, struct file *fl)
{
   try_module_get(THIS_MODULE);
   return 0;
}

// counter down     when module is closed
int module_close(struct inode *ino, struct file *fl)
{
   module_put(THIS_MODULE);
   return 0;
}


// *********************************************************************


//   fill structure   (syntaxis C99)
static struct file_operations MyFl_4_PrcFl={
  .owner = THIS_MODULE,
  .read  = module_output,
  .write = module_input,
  .open  = module_open,
  .release = module_close,
};

static struct inode_operations MyInod_4_PrcFl={
   .permission = module_permission,
};


//  *****************   Module   ***************************************


int init_module()
{
  int rv=0;
  
  //dir=proc_mkdir("aaa",NULL);
  //PrcFl=proc_create(FNm, 0644, dir,&MyFl_4_PrcFl);

  //PrcFl = create_proc_entry(FNm, 0644, NULL);
  PrcFl=proc_create(FNm, 0644, NULL,&MyFl_4_PrcFl);

/*  PrcFl->owner = THIS_MODULE;
  PrcFl->proc_iops = &MyInod_4_PrcFl;
  PrcFl->proc_fops = &MyFl_4_PrcFl;
  PrcFl->mode = S_IFREG | S_IRUGO | S_IWUSR;
  PrcFl->uid = 0;
  PrcFl->gid = 0;
  PrcFl->size =80;
*/

  if(PrcFl==NULL)
  {
     rv=-ENOMEM;
     //remove_proc_entry(FNm, NULL);
     printk(KERN_INFO "eeeeeee \n");
  }
  return rv;
}


void cleanup_module()
{
   remove_proc_entry(FNm, NULL);
}


























