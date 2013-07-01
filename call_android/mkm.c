#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <asm/syscall.h>
#include <linux/syscalls.h>
#include "mkm_netlink_if.h"
#include "hooks.h"

MODULE_LICENSE("GPL v2");

static unsigned long **ref_sys_call_table;



/*
* 从内核地址空间，寻找sys_call 指针
*/
static unsigned long **acquire_sys_call_table(void)
{
    unsigned long int offset = PAGE_OFFSET;
    unsigned long **sct;

    while(offset < ULLONG_MAX)
    {
        sct = (unsigned long **)offset;
        if(sct[__NR_close] == (unsigned long *) sys_close)
        {
            return sct;
        }
        offset += sizeof(void *);
    }
    printk(KERN_INFO "getting syscall table failed.\n");
    return NULL;
}

/*
*检测CR0写保护位是否设置，如果没有，页写保护已被禁用，并将bitwise-AND的16位置0，并设置CR0
*/
static unsigned long __force_order;
static void disable_page_protection(void)
{
    unsigned long value;
    asm volatile("mov %%cr0, %0\n\t" :"=r"(value));
    if(!(value & 0x00010000))
    {
        return ;
    }

    asm volatile("mov %0, %%cr0" : :"r"(value & ~0x00010000),"m" (__force_order));
}

static void enable_page_protection(void)
{
    unsigned long value;
    asm volatile("mov %%cr0, %0" : "=r"(value),"=m" (__force_order));

    if((value & 0x00010000))
    {
        return ;
    }
    asm volatile("mov %0, %%cr0" : : "r"(value|0x00010000),"m" (__force_order));
}


/*
* 初始化netlink，sys_call_table.hooks
*/
static int __init mkm_mod_start(void)
{
    mkm_nl_init();

    if(!(ref_sys_call_table = acquire_sys_call_table()))
     {
    return -1;
    }
    

//   disable_page_protection();
   reg_hooks(ref_sys_call_table);
  // enable_page_protection();

   printk(KERN_INFO "MKM (monitor kernel module) initiated\n");

   return 0;
}

static void __exit mkm_mod_end(void)
{
    printk(KERN_INFO "MKM (monitor kernel module) stopped 000 \n");
    if(!ref_sys_call_table){
    printk(KERN_ALERT,"MKM (monitor kernel module) stopped return \n");
        return ;
    }
    printk(KERN_ALERT "MKM (monitor kernel module) stopped 111 \n");

   // disable_page_protection();
    printk(KERN_ALERT "MKM (monitor kernel module) stopped 222 \n");
    unreg_hooks(ref_sys_call_table);
    printk(KERN_ALERT "MKM (monitor kernel module) stopped 333 \n");
    //enable_page_protection();
    printk(KERN_ALERT "MKM (monitor kernel module) stopped 444\n");

    mkm_nl_close();

    printk(KERN_ALERT "MKM (monitor kernel module) stopped 555 \n");
}


module_init(mkm_mod_start);
module_exit(mkm_mod_end);
