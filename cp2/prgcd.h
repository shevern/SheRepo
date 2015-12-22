
#ifndef chdev
#define chdev

#include <linux/ioctl.h>

//  definition of numbers for ioctl
#define m_num 100   // my number for the dev
//----------------
#define IOCTL_SET_MSG _IOR(m_num,0,char *)
#define IOCTL_GET_MSG _IOR(m_num,1,char *)
#define IOCTL_GET_NTH_BYTE _IOWR(m_num,2,int)
#define IOCTL_GET_NTH_BYTE2 _IOW(m_num,3,int *)

//-----------------
#define DEVICE_FILE_NAME "cdev"

#endif

// _IO(type,nr)  - no args
// _IOR(type,nr,datTyp)  - for reading
// _IOW(type,nr,datTyp)  - for writing
// _IOWR(type,nr,datTyp)  - for wri / rea



