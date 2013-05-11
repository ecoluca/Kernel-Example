/* 
 * myhello.c - MYHELLO device driver
 *
 * Copyright (C) 2011 Ericsson AB
 *
 * Author: Luca Contri <luca.contri@ericsson.com>
 *
 */

#include <linux/module.h>

#define MAX_NUM_DEV 5


/* myhello module parameters */
static unsigned int n_hello = 1;
module_param(n_hello, uint, 0644);
MODULE_PARM_DESC(n_hello, "myhello instances (default 1)");

static int nstamp = 1;
module_param(nstamp, int, 0644);
MODULE_PARM_DESC(nstamp, "Number of print message (default 1)");

static char *mymsg = "blah";
module_param(mymsg, charp, 0000);
MODULE_PARM_DESC(mymsg, "A character string - message write in bmesg buffer");

int err = -1 ;

static int __init myhello_init(void)
{
    int i;
    if(nstamp < 1)
        goto error_1; 
    else
        for( i=0 ; i < nstamp; i++){
            printk(KERN_ALERT "Hello World !!! I'm Luca's Kernel Driver \n\t"
                   "message N.%d\n\t### %s", i, mymsg);
        }
    // BUGS(); ??? Funzione esplicita non trovata    
    return 0;
    
 error_1:
    return err;
}

static void __exit myhello_exit(void)
{
    printk(KERN_ALERT "Bye Bye World !!! Luca's Kernel Driver Delete\n");
	return;
}

module_init(myhello_init);
module_exit(myhello_exit);

MODULE_DESCRIPTION("Ericsson AB - MYHELLO kernel module");
MODULE_AUTHOR("Luca Contri <luca.contri@ericsson.com>");
MODULE_LICENSE("GPL");



