obj-m += hellodev.o
hellodev-objs := myhello.o mybuff.o

KERNELDIR := "/usr/src/kernels/"$(shell uname -r)
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

distclean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -fr *~
	rm -fr *#
