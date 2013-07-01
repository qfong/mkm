#ifndef MKM_NETLINK_IF_H
#define MKM_NETLINK_IF_H
#ifndef MKM_NEW_H
#include "mkm.h"
#endif

extern pid_t mkm_userspace_pid;

extern int mkm_nl_send_syscall(SYSCALL *data);
extern void mkm_nl_init(void); 
extern void mkm_nl_close(void); 
#endif
