obj-m := kmonitor.o
kmonitor-objs := mkm.o hooks.o mkm_netlink_if.o utils.o
EXTRA_CFLAGS=-I$(PWD)/include
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
