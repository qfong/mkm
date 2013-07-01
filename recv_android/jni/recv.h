#ifndef RECV_H
#define RECV_H
typedef struct{
    int sys_id;
    unsigned long inode;
    pid_t pid;
    unsigned long mem_loc;
}SYSCALL;

#define LOG_FILE	"./out/mkm.log"
#define LOG_W_FILE	"./out/mkm_warn.log"
extern struct hashtab *ht;
extern struct hashtab *sys_ht;
#endif

