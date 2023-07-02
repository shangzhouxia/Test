
ifneq  ($(KERNELRELEASE),)
obj-m:=rcu_call_run.o
else
KERDIR := /lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
all:
	make -C $(KERDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.symvers *.cmd *.cmd.o modules.* *.mod.c
endif