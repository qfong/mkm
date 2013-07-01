#ifndef MM_H
#define MM_H
extern int mm_init(void);

#define BLOCK_SIZE 64*1024
#define SYSCALLS_PER_BLOCK (BLOCK_SIZE/sizeof(SYSCALL))
extern int block_lim;

#endif
