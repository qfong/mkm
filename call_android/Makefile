obj-m := kmonitor.o
KERNELDIR:=/root/android/system/kernel/samsung/smdk4412/
CROSS_COMPILE=/root/android/system/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-
#CC=$(CROSS_COMPILE)gcc
#LD=$(CROSS_COMPILE)ld
CFLAGS_MODULE=-fno-pic
ARCH:=arm
kmonitor-objs := mkm.o hooks.o mkm_netlink_if.o utils.o
EXTRA_CFLAGS=-I$(PWD)/include
all:
	make -C $(KERNELDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
