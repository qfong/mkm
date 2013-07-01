#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include "mkm.h"
#include "hooks.h"
#include <linux/skbuff.h>


#define NETLINK_MKM 24

DEFINE_MUTEX(mkm_nl_mutex);
static struct sock *nl_sock;
pid_t mkm_userspace_pid;

int mkm_nl_send_syscall(SYSCALL *msg)
{
    struct sk_buff *skb = NULL;
    struct nlmsghdr *nlh = NULL;
    int res;

    if(NULL == (skb = alloc_skb(NLMSG_SPACE(sizeof(SYSCALL)),GFP_ATOMIC)))
    {
        printk(KERN_WARNING "MKM(mointor kernel module) Failed to allocate memory for skb\n");
        return -1;
    }
    nlh = (struct nlmsghdr *)skb_put(skb,NLMSG_SPACE(sizeof(SYSCALL)));
    nlh->nlmsg_type = 0;
    nlh->nlmsg_len = NLMSG_LENGTH(sizeof(SYSCALL));
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_pid = 0;
    nlh->nlmsg_seq = 0;
    memset(NLMSG_DATA(nlh)+sizeof(SYSCALL),0,NLMSG_SPACE(sizeof(SYSCALL))-NLMSG_LENGTH(sizeof(SYSCALL)));
    memcpy(NLMSG_DATA(nlh),msg,sizeof(SYSCALL));
    mutex_lock(&mkm_nl_mutex);
    res = netlink_unicast(nl_sock,skb,mkm_userspace_pid,1);
    mutex_unlock(&mkm_nl_mutex);
    return 0;
}

static void recv_msg(struct sk_buff *skb)
{
    printk(KERN_INFO "MKM(monitor kernel module) recv msg\n");
    struct nlmsghdr *nlh;
    pid_t   pid;
    struct sk_buff *skb_out;
    int msg_size;
    char *msg="mkm-ack";
    int res;
    printk(KERN_INFO "MKM(monitor kernel module) recv msg\n");
    mutex_lock(&mkm_nl_mutex);
    msg_size = strlen(msg);
    nlh = nlmsg_hdr(skb);

    if(strncmp((char *)nlmsg_data(nlh),"mkm-syn",strlen("mkm-syn")) != 0){
        printk(KERN_WARNING "MKM(monitor kernel module),received invaild mkm-syn messgae: %s\n",(char *)nlmsg_data(nlh));
        mutex_unlock(&mkm_nl_mutex);
        return;
    }

    if((pid = nlh->nlmsg_pid)<= 0){
        printk(KERN_WARNING "MKM(monitor kernel module),received invalid PID from mkm-syn message %i\n",pid);
        mutex_unlock(&mkm_nl_mutex);
        return ;
    }

    skb_out = nlmsg_new(msg_size,0);

    if(!skb_out){
        printk(KERN_WARNING "MKM(monitor kernel module),failed to allocate new skb\n");
        mutex_unlock(&mkm_nl_mutex);
        return ;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE,msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0;
    strncpy(nlmsg_data(nlh), msg,msg_size);
    nlmsg_end(skb_out,nlh);

    res = nlmsg_unicast(nl_sock,skb_out,pid);
    if(res != 0){
        printk(KERN_WARNING "MKM(monitor kernel module),failed to send mkm-ack message to %i\n",pid);
        mutex_unlock(&mkm_nl_mutex);
        return ;
    }

    mkm_userspace_pid = pid;

    mutex_unlock(&mkm_nl_mutex);
    printk("MKM(monitor kernel module) set ulevel pud  to %i\n",mkm_userspace_pid);


}

/*
* 初始化init-function for netlink
*/
void mkm_nl_init(void)
{
    
    mkm_userspace_pid = -1;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22))
    nl_sock = netlink_kernel_create( NETLINK_MKM, 0, recv_msg, THIS_MODULE);
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
    nl_sock = netlink_kernel_create( NETLINK_MKM, 0, recv_msg, NULL, THIS_MODULE);
#elif(LINUX_VERSION_CODE  < KERNEL_VERSION(3,6,0))
    nl_sock = netlink_kernel_create(&init_net, NETLINK_MKM, 0, recv_msg, NULL, THIS_MODULE);
#else 
	{
	struct netlink_kernel_cfg cfg ={
        .input = recv_msg,
        .groups = 1,
    };

#if(LINUX_VERSION_CODE  < KERNEL_VERSION(3,7,0))
    nl_sock = netlink_kernel_create(&init_net, NETLINK_MKM,THIS_MODULE, &cfg);
#else
	nl_sock = netlink_kernel_create(&init_net, NETLINK_MKM, &cfg);

#endif 
	}
#endif
    if(!nl_sock){
        printk(KERN_WARNING "%s,failed to create netlink socket\n",__FUNCTION__);
    }
    printk(KERN_INFO "MKM (monitor kernel module) Create socket\n");
}


void mkm_nl_close(void)
{
    mutex_lock(&mkm_nl_mutex);
    netlink_kernel_release(nl_sock);
    mutex_unlock(&mkm_nl_mutex);
}
