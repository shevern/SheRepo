

#ifndef chdev
#define chdev

#include <linux/ioctl.h>

#define m_num 100
//----------------
#define IOCTL_SET_MSG _IOR(m_num,0,char *)
#define IOCTL_GET_MSG _IOR(m_num,1,char *)
#define IOCTL_GET_NTH_BYTE _IOWR(m_num,2,int)
//-----------------
#define DEVICE_FILE_NAME "cdev"

#endif
