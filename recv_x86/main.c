#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <sys/stat.h>
#include <poll.h>

#include <fcntl.h>

#include <pwd.h>
#include <sched.h>

#include "recv.h"
#include "mm.h"
#include "util.h"
#include "hashtab.h"
#include "mk_time.h"

#define NETLINK_MKM 24
#define MAX_PAYLOAD 1024



int rev(void)
{
    struct sockaddr_nl src_addr,dest_addr;
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    int    sock_fd;
    struct msghdr msg;
    unsigned long long count = 0;

    if(getuid()!=0){
        sprintf(stderr,"This needs to be run as root!");
        exit(1);
    }
    if(set_rr_scheduler() != 0){
        sprintf(stderr,"Failed to change the schedular");
        exit(1);
    }
    /*
    if(drop_privileges()!=0){
        sprintf(stderr,"Failed to drop privileges, Is system account \%s\"available?\n",SYSACCOUNT);
        exit(1);
    }
    */
   
    if(mm_init()!= 0){
    sprintf(stderr,"Failed to initialize the memory manager.\n");
    exit(1);
   }
   
    sock_fd = socket(AF_NETLINK,SOCK_DGRAM,NETLINK_MKM);

    memset(&src_addr,0,sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    bind(sock_fd,(struct sockaddr*)&src_addr,sizeof(src_addr));

    memset(&dest_addr,0,sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;             
    dest_addr.nl_pid = 0;   //kernel
    dest_addr.nl_groups = 0;

    nlh = (struct nlmsghdr*)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    strcpy(NLMSG_DATA(nlh),"mkm-syn");
    iov.iov_base = (void*) nlh;
    iov.iov_len = nlh->nlmsg_len;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    printf ("send message to kernel \n");
    int rets;
    rets=sendmsg(sock_fd,&msg,0);
    printf("waiting for message from kernel\n");

    recvmsg(sock_fd,&msg,0);
    printf("recvied message: %s \n",(char *)NLMSG_DATA(nlh));
    memset(NLMSG_DATA(nlh),0,strlen((char*)NLMSG_DATA(nlh)));
    
    static struct pollfd  fds[2];
    

    fds[0].fd = sock_fd;
    fds[0].events = POLLIN;
    int fd,ret;
    //fd = open("list",O_RDWR|O_CREAT);
    //if (fd < 0) printf("can not open file\n");

    //fds[1].fd = fd;
    //fds[1].events = POLLOUT;
    int i =1;
	mk_uptr_t	cur;
	cur = mk_current_msec;
    while(1){
			
			
			ret = poll(fds,2,-1);
			if(ret == 0){
				printf("time out \n");
				continue ;
			}
			mk_time_update();
			for (i=0;i<2;i++){
				if(fds[i].revents & POLLIN){
				recvmsg(sock_fd,&msg,0);
				SYSCALL *data = (SYSCALL *)NLMSG_DATA(nlh);
           // 	printf("%i,%lu,%i,%llu\n",data->sys_id,data->inode,data->pid,count);
				analysis_handler(data);
				//printf("%i,%lu,%i,%llu\n",data->sys_id,data->inode,data->pid,count);
				memset(NLMSG_DATA(nlh),0,sizeof(SYSCALL));
				count ++;
				}
		     		
			}
            
			if(( mk_current_msec - cur) >50){
				store_flags = 1;
				hashtab_show(sys_ht);
				hashtab_destroy(sys_ht);
				sys_ht = hashtab_create(symhash,symcmp,1024);
				cur = mk_current_msec;
			}
            
           
    }
    return 0;



}

struct hashtab *ht;
struct hashtab *sys_ht;

int mk_daemon(void)
{
    int fd;
    pid_t pid;
    pid = fork();
    switch(pid){
    case -1:
        return -1;
    case 0:
        break;
    default : 
        exit(0);
    }
    umask(0);
    if ( setsid() <0) return -1;
    fd = open("/dev/null",O_RDWR);
    if(fd  == -1) return -1;

    dup2(fd,STDIN_FILENO);
    dup2(fd,STDOUT_FILENO);
    return 0;
}


int main()
{
	mk_daemon();
	mk_time_init();
    mk_open_logfile();
		sys_ht = hashtab_create(symhash,symcmp,1024);
	printf("HELLO WORLD\n");
	rev();
}
