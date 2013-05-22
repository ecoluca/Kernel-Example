obj-m += hellodev.o
hellodev-objs := myhello.o mybuff.o

KERNELDIR := "/usr/src/kernels/"$(shell uname -r)
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

fclean:
	rm -fr *~
	rm -fr *#
