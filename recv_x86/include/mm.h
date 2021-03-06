#ifndef MM_H
#define MM_H
extern int mm_init(void);

#define BLOCK_SIZE 4*1024
#define SYSCALLS_PER_BLOCK (BLOCK_SIZE/sizeof(SYSCALL_STORE))
extern int block_lim;
extern int store_flags;
extern int mkm_fd;
extern int mkm_wfd;
int mk_open_logfile(void);
#endif
