/* 
 * myhello.c - MYHELLO device driver
 *
 * Copyright (C) 2011 Ericsson AB
 *
 * Author: Luca Contri <luca.contri@ericsson.com>
 *
 */

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#include <linux/device.h>



#include "mybuff.h"
#include "myhello.h"


#define MY_MAX_NUM_DEV      5
#define BUFF_MAX_SIZE    2048

#define MY_NAME_DEV "myhello"
#define MY_NAME_DEVS "myhello%d"


/* Function Prototype */
static int __init myhello_init(void);
static void __exit myhello_exit(void);

static int myhello_open(struct inode *inode, struct file *file);
// static int myhello_close(struct inode *inode, struct file *file);

static struct class *myhello_class;
static struct device *myhello_device;


/* myhello module parameters */
//static unsigned int n_hello = 1;
//module_param(n_hello, uint, 0644);
//MODULE_PARM_DESC(n_hello, "myhello instances (default 1)");

static int nstamp = 1;
module_param(nstamp, int, 0644);
MODULE_PARM_DESC(nstamp, "Number of print message (default 1)");

static char *mymsg = "blah";
module_param(mymsg, charp, 0000);
MODULE_PARM_DESC(mymsg, "A character string - message write in bmesg buffer");

static int myhello_no = 1;
module_param(myhello_no, int, 0644);
MODULE_PARM_DESC(myhello_no, "Number of Device Driver (default 1)");

static int myhello_size = 1024;
module_param(myhello_size, int, 0644);
MODULE_PARM_DESC(myhello_size, "Buffer's Size of Device Driver (default 1 KByte)");


static int err = -1 ;
static unsigned int myhello_major = 0 ;
static unsigned int myhello_minor = 0 ;
static dev_t myhello_id;


static struct myhello_info {
	struct mybuff *mybuff;
	struct cdev cdev;
    //	struct mutex lock;
    //	wait_queue_head_t in_queue; /* queue of processes suspended on read */
    //	wait_queue_head_t out_queue; /* queue of processes suspended on write */
    //	struct fasync_struct *async_queue; /* asynchronous readers */
} *myhello;


static const struct file_operations myhello_fops = {
	.owner = THIS_MODULE,
	.open = myhello_open,
    //    .release = myhello_close,
    //	.read = myhello_read,
    //	.write = myhello_write,
    //	.ioctl = myhello_ioctl,
    //	.poll = myhello_poll,
    //	.fasync = myhello_fasync
};





/**
 *STRUCT FILE
 * 
 *Each open file, socket etcetera is represented by a file data structure.
 *
 *struct file {
 *  mode_t f_mode;
 *  loff_t f_pos;
 *  unsigned short f_flags;
 *  unsigned short f_count;
 *  unsigned long f_reada, f_ramax, f_raend, f_ralen, f_rawin;
 *  struct file *f_next, *f_prev;
 *  int f_owner;         // pid or -pgrp where SIGIO should be sent
 *  struct inode * f_inode;
 *  struct file_operations * f_op;
 *  unsigned long f_version;
 *  void *private_data;  // needed for tty driver, and maybe others 
 *};
 */

/**
 * myhello_open(): Open the MYHELLO device.
 * 
 * Returns 0 if no error, standard error number otherwise.
 */
static int myhello_open(struct inode *inode, struct file *file)
{
    struct myhello_info *myhello ;

    /* make a pointer to myhello struct and save it */
    myhello = container_of(inode->i_cdev, struct myhello_info, cdev) ;
    file->private_data = myhello ;

    return 0;
}





/**
 * myhello_init(): Initializes the MYHELLO device.
 * 
 * Returns 0 if no error, standard error number otherwise.
 */
static int __init myhello_init(void)
{
    int i;
 
    /* check module parameters */
    if(nstamp < 1)
        goto error_1; 

	if (myhello_no == 0 || myhello_no >  MY_MAX_NUM_DEV) {
		pr_err("%s(): invalid myhello_no=%d \n", __func__, myhello_no);
		err = -EINVAL;
		goto error_1;
	}
	if (myhello_size > BUFF_MAX_SIZE) {
		pr_err("%s(): invalid myhello_size=%d \n", __func__, myhello_size);
		err = -EINVAL;
		goto error_1;
	}

	/* register chrdev region, get the major number */
	if (myhello_major) {
		myhello_id = MKDEV(myhello_major, myhello_minor);
		err = register_chrdev_region(myhello_id, myhello_no, MY_NAME_DEV);
	} else {
		err = alloc_chrdev_region(&myhello_id, myhello_minor, myhello_no, MY_NAME_DEV);
		myhello_major = MAJOR(myhello_id);
	}
	if (err < 0) {
		pr_err("%s(): can't get major number %d\n", __func__, myhello_major);
		err = -ENODEV;
		goto error_1;
    } 

	/* init myhello - to be done before calling cdev_add() */
	myhello = kzalloc((myhello_no * sizeof(struct myhello_info)), GFP_KERNEL);
	if (!myhello) {
		pr_err("%s(): can't create myhello\n", __func__);
		err = -ENOMEM;
		goto error_2;
	}

    /* Cicle */
	for (i = 0; i < myhello_no; i++) {
        
		/* Create a Buffur */        
        err =  mybuff_create(&myhello[i].mybuff, myhello_size);
		if (err < 0) {
			pr_err("%s(): can't create mybuff for device %d, %d\n",
                   __func__, myhello_major, myhello_minor + i);
			err = -ENOMEM;
			goto error_3;
        } 

        /* init and add cdev */
		cdev_init(&myhello[i].cdev, &myhello_fops);
		myhello[i].cdev.owner = THIS_MODULE;
		err = cdev_add(&myhello[i].cdev, MKDEV(myhello_major, myhello_minor + i), 1);
		if (err < 0) {
			pr_err("%s(): can't create cdev for device %d, %d\n",
			__func__, myhello_major, myhello_minor + i);
			err = -ENODEV;
			goto error_3;
	        } 
	
    }

        /* register to sysfs and send uevents to create dev nodes */
        myhello_class = class_create(THIS_MODULE, MY_NAME_DEV);
        for (i = 0; i < myhello_no; i++) {
            myhello_device = device_create(myhello_class, NULL, 
                                           MKDEV(myhello_major, myhello_minor + i), NULL, MY_NAME_DEVS, i);
        }
        
	
    
    /* For example Print on Dmesg Buffer */
    for( i=0 ; i < nstamp; i++){
        printk(KERN_ALERT "Hello World !!! I'm Luca's Kernel Driver \n\t"
               "message N.%d\n\t### %s", i, mymsg);
    }
    
    
    return 0;

    /* Memory Release - After Error */
 error_3:
    kfree(myhello);
 error_2:
	unregister_chrdev_region(myhello_id, myhello_no);
 error_1:
    return err;
}



/**
 * myhello_exit(): Terminates the MYHELLO device.
 */
static void __exit myhello_exit(void)
{
	int i;
    for (i = 0; i < myhello_no; i++) {
        
        /* delete cdev */
		cdev_del(&myhello[i].cdev);

        /* delete myhello */
		mybuff_delete(myhello[i].mybuff);
	}
	/* Memory release */
    kfree(myhello);
	
	/* unregister chrdev region, release the major number */
	unregister_chrdev_region(myhello_id, myhello_no);

    /* unregister from sysfs and send uevents to destroy dev nodes */
	for (i = 0; i < myhello_no; i++) {
		device_destroy(myhello_class, MKDEV(myhello_major, myhello_minor + i));
	}
	class_destroy(myhello_class);
    
    printk(KERN_ALERT "Bye Bye World !!! Luca's Kernel Driver Delete\n");
	return;
}






module_init(myhello_init);
module_exit(myhello_exit);

MODULE_DESCRIPTION("Ericsson AB - MYHELLO kernel module");
MODULE_AUTHOR("Luca Contri <luca.contri@ericsson.com>");
MODULE_LICENSE("GPL");


