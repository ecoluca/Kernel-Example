/* 
 * myhello.h - MYHELLO device driver interface
 *
 * Copyright (C) 2011 Ericsson AB
 *
 * Author: Luca Contri <luca.contri@ericsson.com>
 *
 */

#ifndef __MYHELLO_H__
#define __MYHELLO_H__

#define MYHELLO_IOCTL_MAGIC 'm'
#define MYHELLO_IOCTL_SIZE_GET _IOR(MYHELLO_IOCTL_MAGIC, 0, int)
#define MYHELLO_IOCTL_FREE_GET _IOR(MYHELLO_IOCTL_MAGIC, 1, int)
#define MYHELLO_IOCTL_READY_GET _IOR(MYHELLO_IOCTL_MAGIC, 2, int)

#endif /* __MYHELLO_H__ */

