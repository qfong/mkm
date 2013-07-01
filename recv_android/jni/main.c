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


#include "mm.h"
#include "util.h"


#define NETLINK_MKM 24
#define MAX_PAYLOAD 1024

typedef struct{
    int sys_id;
    unsigned long inode;
    pid_t pid;
    unsigned long mem_loc;
}SYSCALL;

int main(void)
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
    /*
    if(mm_init()!= 0){
    sprintf(stderr,"Failed to initialize the memory manager.\n");
    exit(1);
   }
   */
    sock_fd = socket(AF_NETLINK,SOCK_DGRAM,NETLINK_MKM);
    perror("socket kkkkk1\n");

    memset(&src_addr,0,sizeof(src_addr));
    perror("socket kkkkk2\n");
    src_addr.nl_family = AF_NETLINK;
    perror("socket kkkkk3\n");
    src_addr.nl_pid = getpid();
    perror("socket kkkkk4\n");

    bind(sock_fd,(struct sockaddr*)&src_addr,sizeof(src_addr));
    perror("bind kkkkk\n");

    memset(&dest_addr,0,sizeof(dest_addr));
    perror("msmset dest_addr kkkkk\n");
    dest_addr.nl_family = AF_NETLINK;             
    perror("bind kkkkk1\n");
    dest_addr.nl_pid = 0;   //kernel
    perror("bind kkkkk2\n");
    dest_addr.nl_groups = 0;
    perror("bind kkkkk3\n");

    nlh = (struct nlmsghdr*)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    perror("bind kkkkk4\n");
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    perror("memset NLMSG_SPCAE kkkkk\n");

    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    strcpy(NLMSG_DATA(nlh),"mkm-syn");
    perror("kkkkk\n");
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
    perror("kkkkk\n");
    printf("waiting for message from kerddd %d\n",rets);
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
    while(1){
        
       ret = poll(fds,2,-1);
     if(ret == 0) printf("time out \n");
      for (i=0;i<2;i++){
            if(fds[i].revents & POLLIN){

            recvmsg(fds[i].fd,&msg,0);
        //    recvmsg(sock_fd,&msg,-1);
            SYSCALL *data = (SYSCALL *)NLMSG_DATA(nlh);
            printf("%i,%lu,%i,%llu\n",data->sys_id,data->inode,data->pid,count);
         //store
            memset(NLMSG_DATA(nlh),0,sizeof(SYSCALL));
            count ++;

           }
           // if(fds[i].revents & POLLOUT){
           //     printf("1222222222222\n");
            }
       // }
    }
    return 0;



}
