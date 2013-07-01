#include <asm/unistd.h>
#include <linux/syscalls.h>
#include <asm/thread_info.h>
#include "mkm_netlink_if.h"
#include "mkm.h"
#include "utils.h"
#include "hooks.h"

extern pid_t mkm_userspace_pid;
long (*ref_sys_read)(unsigned int fd, char __user *buf,size_t count) = NULL;
long (*ref_sys_write)(unsigned int fd, const char __user *buf,size_t count) = NULL;
long (*ref_sys_open)(const char __user *filename,int flags,int mode) = NULL;
long (*ref_sys_close)(unsigned int fd) = NULL;
long (*ref_sys_newstat)(char __user *filename,struct stat __user *statbuf) = NULL;
long (*ref_sys_newlstat)(char __user *filename,struct stat __user *statbuf) = NULL;
long (*ref_sys_poll)(struct pollfd __user *ufds,unsigned int nfds,int timeout) = NULL;
long (*ref_sys_lseek)(unsigned int fd,off_t offset,unsigned int origin)= NULL;
long (*ref_sys_mkdir)(const char __user *pathname,int mode) = NULL;
long (*ref_sys_rmdir)(const char __user *pathname) = NULL;
long (*ref_sys_exit_group)(int error_code) = NULL;
long (*ref_sys_exit)(int error_code) = NULL;
long (*ref_sys_getpid)(void) = NULL;
long (*ref_sys_gettid)(void) = NULL;
long (*ref_sys_kill)(int pid,int sig) = NULL;
long (*ref_sys_prctl)(int option,unsigned long arg2,unsigned long arg3,unsigned long arg4,unsigned long arg5) = NULL;
long (*ref_sys_setuid)(uid_t uid) = NULL;
long (*ref_sys_setgid)(gid_t gid) = NULL;
long (*ref_sys_waitid)(int which,pid_t pid, struct siginfo __user *infop,int options,struct rusage __user *ru) = NULL;
long (*ref_sys_shutdown)(int,int) = NULL;
long (*ref_sys_geteuid)(void) = NULL;
long (*ref_sys_getegid)(void) = NULL;
long (*ref_sys_ioctl)(unsigned int fd,unsigned int cmd,unsigned long arg) = NULL;
long (*ref_sys_mount)(char __user *dev_name,char __user *dir_name,char __user *type,unsigned long flags,void __user *data) = NULL;
long (*ref_sys_umount)(char __user *name, int flags) = NULL;
long (*ref_sys_bind)(int,struct sockaddr __user *,int) = NULL;
long (*ref_sys_connect)(int,struct sockaddr __user *,int) = NULL;
long (*ref_sys_accept)(int,struct sockaddr __user *,int __user *) = NULL;
long (*ref_sys_sendto)(int,void __user *,size_t,unsigned,struct sockaddr __user *,int) = NULL;
long (*ref_sys_recvfrom)(int,void __user *,size_t,unsigned, struct sockaddr __user *,int __user *) = NULL;
long (*ref_sys_sendmsg)(int fd,struct msghdr __user *msg,unsigned flags) = NULL;
long (*ref_sys_recvmsg)(int fd,struct msghdr __user *msg,unsigned flags) = NULL;
long (*ref_sys_listen)(int,int) = NULL;
long (*ref_sys_socket)(int , int ,int )= NULL;




long hooks_sys_read(unsigned int fd,char __user *buf, size_t count)
{
    long retval = ref_sys_read(fd,buf,count);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 0;
        
        //printk(KERN_INFO "sys_call_table [read]: 0x%02X",buf[0]);
        
        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}
long hooks_sys_write(unsigned int fd,char __user *buf, size_t count)
{
    long retval = ref_sys_write(fd,buf,count);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 1;
        
        //printk(KERN_INFO "sys_call_table [write]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}


long hooks_sys_open(const char __user *filename, int flags,int mode)
{
    long retval = ref_sys_open(filename,flags,mode);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 2;
        
       // //printk(KERN_INFO "sys_call_table [open]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_close(unsigned int fd)
{
    long retval = ref_sys_close(fd);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 3;
        
        ////printk(KERN_INFO "sys_call_table [close]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}
long hooks_sys_newstat(char __user *filename,struct stat __user *statbuf)
{
    long retval = ref_sys_newstat(filename,statbuf);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 4;
        
        ////printk(KERN_INFO "sys_call_table [newstat]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}
long hooks_sys_newlstat(char __user *filename,struct stat __user *statbuf)
{
    long retval = ref_sys_newlstat(filename,statbuf);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 6;
        
       // //printk(KERN_INFO "sys_call_table [newlstat]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}
long hooks_sys_poll(struct pollfd __user *ufds,unsigned int nfds,int timeout)
{
    long retval = ref_sys_poll(ufds,nfds,timeout);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 7;
        
        //printk(KERN_INFO "sys_call_table [poll]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_lseek(unsigned int fd,off_t offset,unsigned int origin)
{
    long retval = ref_sys_lseek(fd,offset,origin);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 8;
        
        //printk(KERN_INFO "sys_call_table [lseek]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_ioctl(unsigned int fd,unsigned int cmd,unsigned long arg)
{
    long retval = ref_sys_ioctl(fd,cmd,arg);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 16;
        
        //printk(KERN_INFO "sys_call_table [ioctl]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_getpid(void)
{
    long retval = ref_sys_getpid();
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 39;
        
        //printk(KERN_INFO "sys_call_table [getpid]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_socket(int arg0, int arg1,int arg2)
{
    long retval = ref_sys_socket(arg0,arg1,arg2);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 41;
        
        //printk(KERN_INFO "sys_call_table [socket]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_connect(int arg0,struct sockaddr __user *arg1,int arg2)
{
    long retval = ref_sys_connect(arg0,arg1,arg2);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 42;
        
        //printk(KERN_INFO "sys_call_table [connect]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_accept(int arg0,struct sockaddr __user *arg1,int __user *arg2)
{
    long retval = ref_sys_accept(arg0,arg1,arg2);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 43;
        
        //printk(KERN_INFO "sys_call_table [accept]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_sendto(int arg0,void __user *arg1,size_t arg2,unsigned arg3,struct sockaddr __user *arg4,int arg5)
{
    long retval = ref_sys_sendto(arg0,arg1,arg2,arg3,arg4,arg5);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 44;
        
        //printk(KERN_INFO "sys_call_table [sendto]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_recvfrom(int arg0,void __user *arg1,size_t arg2,unsigned arg3,struct sockaddr __user *arg4,int __user *arg5)
{
    long retval = ref_sys_recvfrom(arg0,arg1,arg2,arg3,arg4,arg5);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 45;
        
        //printk(KERN_INFO "sys_call_table [recvfrom]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_sendmsg(int fd,struct msghdr __user *msg,unsigned flags)
{
    long retval = ref_sys_sendmsg(fd,msg,flags);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 46;
        
        //printk(KERN_INFO "sys_call_table [sendmsg]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_recvmsg(int fd,struct msghdr __user *msg,unsigned flags)
{
    long retval = ref_sys_recvmsg(fd,msg,flags);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 47;
        
        //printk(KERN_INFO "sys_call_table [recvmsg]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_shutdown(int arg0,int arg1)
{
    long retval = ref_sys_shutdown(arg0,arg1);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 48;
        
        //printk(KERN_INFO "sys_call_table [shutdown]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_bind(int arg0,struct sockaddr __user *arg1,int arg2)
{
    long retval = ref_sys_bind(arg0,arg1,arg2);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 49;
        
        //printk(KERN_INFO "sys_call_table [bind]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_listen(int arg0,int arg1)
{
    long retval = ref_sys_listen(arg0,arg1);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 50;
        
        //printk(KERN_INFO "sys_call_table [listen]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_exit(int error_code)
{
    long retval = ref_sys_exit(error_code);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 60;
        
        //printk(KERN_INFO "sys_call_table [exit]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}
long hooks_sys_kill(int pid, int sig)
{
    long retval = ref_sys_kill(pid,sig);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 62;
        
        //printk(KERN_INFO "sys_call_table [kill]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_mkdir(const char __user *pathname,int mode)
{
    long retval = ref_sys_mkdir(pathname,mode);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 83;
        
        //printk(KERN_INFO "sys_call_table [mkdir]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_rmdir(const char __user *pathname)
{
    long retval = ref_sys_rmdir(pathname);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 84;
        
        //printk(KERN_INFO "sys_call_table [rmdir]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_setuid(uid_t uid)
{
    long retval = ref_sys_setuid(uid);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 105;
        
        //printk(KERN_INFO "sys_call_table [setuid]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_setgid(gid_t gid)
{
    long retval = ref_sys_setgid(gid);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 106;
        
        //printk(KERN_INFO "sys_call_table [setgid]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_geteuid(void)
{
    long retval = ref_sys_geteuid();
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 107;
        
        //printk(KERN_INFO "sys_call_table [geteuid]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_getegid(void)
{
    long retval = ref_sys_getegid();
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 108;
        
        //printk(KERN_INFO "sys_call_table [getegid]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_prctl(int option,unsigned long arg2,unsigned long arg3,unsigned long arg4,unsigned long arg5)
{
    long retval = ref_sys_prctl(option,arg2,arg3,arg4,arg5);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 157;
        
        //printk(KERN_INFO "sys_call_table [prctl]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_mount(char __user *dev_name,char __user *dir_name,char __user *type,unsigned long flags,void __user *data)
{
    long retval = ref_sys_mount(dev_name,dir_name,type,flags,data);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 165;
        
        //printk(KERN_INFO "sys_call_table [mount]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_umount(char __user *name, int flags)
{
    long retval = ref_sys_umount(name,flags);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 166;
        
        //printk(KERN_INFO "sys_call_table [umount]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_gettid(void)
{
    long retval = ref_sys_gettid();
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 186;
        
        //printk(KERN_INFO "sys_call_table [gettid]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}

long hooks_sys_exit_group(int error_code)
{
    long retval = ref_sys_exit_group(error_code);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 231;
        
        //printk(KERN_INFO "sys_call_table [exit_group]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}
long hooks_sys_waitid(int which,pid_t pid, struct siginfo __user *infop,int options,struct rusage __user *ru)
{
    long retval = ref_sys_waitid(which,pid,infop,options,ru);
    if(mkm_userspace_pid > 0 && current->pid != mkm_userspace_pid)
    {
        SYSCALL data;
        data.sys_id = 247;
        
        //printk(KERN_INFO "sys_call_table [waitid]: 0x%02X",buf[0]);

        data.inode = get_inode();
        
        data.pid = current->pid;
        data.mem_loc = 0;
        mkm_nl_send_syscall(&data);
        
    }
    return retval;
}
void reg_hooks(unsigned long **syscall_table)
{
//    ref_sys_read = (void *) syscall_table[__NR_read];
//    syscall_table[__NR_read] = (unsigned long *)hooks_sys_read;
    ref_sys_write= (void *) syscall_table[__NR_write];
    syscall_table[__NR_write] = (unsigned long *)hooks_sys_write;
    ref_sys_open= (void *) syscall_table[__NR_open];
    syscall_table[__NR_open] = (unsigned long *)hooks_sys_open;
    ref_sys_close= (void *) syscall_table[__NR_close];
    syscall_table[__NR_close] = (unsigned long *)hooks_sys_close;

    ref_sys_newstat= (void *) syscall_table[__NR_stat];
    syscall_table[__NR_stat] = (unsigned long *)hooks_sys_newstat;
    ref_sys_newlstat= (void *) syscall_table[__NR_lstat];
    syscall_table[__NR_lstat] = (unsigned long *)hooks_sys_newlstat;
//    ref_sys_poll= (void *) syscall_table[__NR_poll];
 //   syscall_table[__NR_poll] = (unsigned long *)hooks_sys_poll;
    ref_sys_lseek= (void *) syscall_table[__NR_lseek];
    syscall_table[__NR_lseek] = (unsigned long *)hooks_sys_lseek;
    ref_sys_ioctl= (void *) syscall_table[__NR_ioctl];
    syscall_table[__NR_ioctl] = (unsigned long *)hooks_sys_ioctl;
    ref_sys_getpid= (void *) syscall_table[__NR_getpid];
    syscall_table[__NR_getpid] = (unsigned long *)hooks_sys_getpid;
    ref_sys_socket= (void *) syscall_table[__NR_socket];
    syscall_table[__NR_socket] = (unsigned long *)hooks_sys_socket;
    ref_sys_connect= (void *) syscall_table[__NR_connect];
    syscall_table[__NR_connect] = (unsigned long *)hooks_sys_connect;
    ref_sys_accept= (void *) syscall_table[__NR_accept];
    syscall_table[__NR_accept] = (unsigned long *)hooks_sys_accept;
    ref_sys_sendto= (void *) syscall_table[__NR_sendto];
    syscall_table[__NR_sendto] = (unsigned long *)hooks_sys_sendto;
    ref_sys_recvfrom= (void *) syscall_table[__NR_recvfrom];
    syscall_table[__NR_recvfrom] = (unsigned long *)hooks_sys_recvfrom;
    ref_sys_sendmsg= (void *) syscall_table[__NR_sendmsg];
    syscall_table[__NR_sendmsg] = (unsigned long *)hooks_sys_sendmsg;
    ref_sys_recvmsg= (void *) syscall_table[__NR_recvmsg];
    syscall_table[__NR_recvmsg] = (unsigned long *)hooks_sys_recvmsg;
    ref_sys_shutdown= (void *) syscall_table[__NR_shutdown];
    syscall_table[__NR_shutdown] = (unsigned long *)hooks_sys_shutdown;
    ref_sys_bind= (void *) syscall_table[__NR_bind];
    syscall_table[__NR_bind] = (unsigned long *)hooks_sys_bind;
    ref_sys_listen= (void *) syscall_table[__NR_listen];
    syscall_table[__NR_listen] = (unsigned long *)hooks_sys_listen;
    ref_sys_exit= (void *) syscall_table[__NR_exit];
    syscall_table[__NR_exit] = (unsigned long *)hooks_sys_exit;
    ref_sys_kill= (void *) syscall_table[__NR_kill];
    syscall_table[__NR_kill] = (unsigned long *)hooks_sys_kill;
    ref_sys_mkdir= (void *) syscall_table[__NR_mkdir];
    syscall_table[__NR_mkdir] = (unsigned long *)hooks_sys_mkdir;
    ref_sys_rmdir= (void *) syscall_table[__NR_rmdir];
    syscall_table[__NR_rmdir] = (unsigned long *)hooks_sys_rmdir;
    ref_sys_setuid= (void *) syscall_table[__NR_setuid];
    syscall_table[__NR_setuid] = (unsigned long *)hooks_sys_setuid;
    ref_sys_setgid= (void *) syscall_table[__NR_setgid];
    syscall_table[__NR_setgid] = (unsigned long *)hooks_sys_setgid;
    ref_sys_geteuid= (void *) syscall_table[__NR_geteuid];
    syscall_table[__NR_geteuid] = (unsigned long *)hooks_sys_geteuid;
    ref_sys_getegid= (void *) syscall_table[__NR_getegid];
    syscall_table[__NR_getegid] = (unsigned long *)hooks_sys_getegid;
    ref_sys_prctl= (void *) syscall_table[__NR_prctl];
    syscall_table[__NR_prctl] = (unsigned long *)hooks_sys_prctl;
    ref_sys_mount= (void *) syscall_table[__NR_mount];
    syscall_table[__NR_mount] = (unsigned long *)hooks_sys_mount;
    ref_sys_umount= (void *) syscall_table[__NR_umount2];
    syscall_table[__NR_umount2] = (unsigned long *)hooks_sys_umount;
    ref_sys_gettid= (void *) syscall_table[__NR_gettid];
    syscall_table[__NR_gettid] = (unsigned long *)hooks_sys_gettid;
    ref_sys_exit_group= (void *) syscall_table[__NR_exit_group];
    syscall_table[__NR_exit_group] = (unsigned long *)hooks_sys_exit_group;
    ref_sys_waitid= (void *) syscall_table[__NR_waitid];
    syscall_table[__NR_waitid] = (unsigned long *)hooks_sys_waitid;
}

void unreg_hooks(unsigned long **syscall_table)
{
    
//    syscall_table[__NR_read] = (unsigned long *)ref_sys_read;
	syscall_table[__NR_write] = (unsigned long *)ref_sys_write;
	syscall_table[__NR_open] = (unsigned long *)ref_sys_open;
	syscall_table[__NR_close] = (unsigned long *)ref_sys_close;
    syscall_table[__NR_stat] = (unsigned long *)ref_sys_newstat;
	syscall_table[__NR_lstat] = (unsigned long *)ref_sys_newlstat;
//	syscall_table[__NR_poll] = (unsigned long *)ref_sys_poll;
	syscall_table[__NR_lseek] = (unsigned long *)ref_sys_lseek;
	syscall_table[__NR_ioctl] = (unsigned long *)ref_sys_ioctl; 
	syscall_table[__NR_getpid] = (unsigned long *)ref_sys_getpid;
	syscall_table[__NR_socket] = (unsigned long *)ref_sys_socket;
	syscall_table[__NR_connect] = (unsigned long *)ref_sys_connect;
	syscall_table[__NR_accept] = (unsigned long *)ref_sys_accept;
	syscall_table[__NR_sendto] = (unsigned long *)ref_sys_sendto;
	syscall_table[__NR_recvfrom] = (unsigned long *)ref_sys_recvfrom;
	syscall_table[__NR_sendmsg] = (unsigned long *)ref_sys_sendmsg;
	syscall_table[__NR_recvmsg] = (unsigned long *)ref_sys_recvmsg;
	syscall_table[__NR_shutdown] = (unsigned long *)ref_sys_shutdown;
	syscall_table[__NR_bind] = (unsigned long *)ref_sys_bind;
	syscall_table[__NR_listen] = (unsigned long *)ref_sys_listen;
	syscall_table[__NR_exit] = (unsigned long *)ref_sys_exit;
	syscall_table[__NR_kill] = (unsigned long *)ref_sys_kill;
	syscall_table[__NR_mkdir] = (unsigned long *)ref_sys_mkdir;
	syscall_table[__NR_rmdir] = (unsigned long *)ref_sys_rmdir;
	syscall_table[__NR_setuid] = (unsigned long *)ref_sys_setuid;
	syscall_table[__NR_setgid] = (unsigned long *)ref_sys_setgid;
	syscall_table[__NR_geteuid] = (unsigned long *)ref_sys_geteuid;
	syscall_table[__NR_getegid] = (unsigned long *)ref_sys_getegid;
	syscall_table[__NR_prctl] = (unsigned long *)ref_sys_prctl;
	syscall_table[__NR_mount] = (unsigned long *)ref_sys_mount;
	syscall_table[__NR_umount2] = (unsigned long *)ref_sys_umount;
	syscall_table[__NR_gettid] = (unsigned long *)ref_sys_gettid;
	syscall_table[__NR_exit_group] = (unsigned long *)ref_sys_exit_group;
	syscall_table[__NR_waitid] = (unsigned long *)ref_sys_waitid;

}
