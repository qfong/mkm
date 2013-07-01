#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "mm.h"
#include <sys/time.h>
#include <mk_time.h>
#include "recv.h"
#include "hashtab.h"


static unsigned long syscall_n;
static unsigned long block_n;
static void **block_p;
int block_lim;
int store_flags;

typedef struct{
	int 			pid;
	unsigned long	inode;
	int 			sys_id;
	unsigned long long count;
}SYSCALL_STORE;
int mkm_fd;
int mkm_wfd;
int mk_open_logfile(void)
{
    
	mkm_fd = open(LOG_FILE,O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IRGRP|S_IROTH);
	if(mkm_fd == NULL) {printf("---------------------\n");exit(1);}
	mkm_wfd = open(LOG_W_FILE,O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IRGRP|S_IROTH);
	if(mkm_wfd == NULL) {printf("---------------------\n");exit(1);}
}

int write_to_blocks_to_file(void)
{
	int 		fd;
	int 		i,j;
	char 		filename[50];
	char 		buffer[200];
    char        fmt[64],buf[64];
	SYSCALL_STORE *tmpcall;
    time_t  now;
    struct tm    *tm;
	//sprintf(filename,"./out/mkm_%lu.out");
	
	//fd = open(LOG_FILE,O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IRGRP|S_IROTH);
	//if(fd == NULL) {printf("---------------------\n");exit(1);}
    time(&now);
    tm = localtime(&now);
   // strftime(fmt, sizeof fmt, "%Y-%m-%d %H:%M:%S.%%06u %z", tm);
    //snprintf(buf,sizeof buf,fmt,tv.tv_usec);
	sprintf(buffer,"========================================\nCurrent Time %s\n pid\tinode\t\sys_id\tcount\n",asctime(tm));
	if(write(mkm_fd,buffer,strlen(buffer)) == -1)
	{
			perror("write error");
				exit(1);
	}
	for(i = 0;i< block_n;i++){
		
		for(j = 0; j<(i == block_n -1 ? syscall_n:SYSCALLS_PER_BLOCK);j++){
			memset(buffer,0,200);
			tmpcall = (SYSCALL_STORE *)(block_p[i] + (j * sizeof(SYSCALL_STORE)));
			sprintf(buffer,"%d\t%lu\t%d\t\%lu\n",tmpcall->pid,tmpcall->inode,tmpcall->sys_id,tmpcall->count);
			if(write(mkm_fd,buffer,strlen(buffer)) == -1)
			{
				perror("write error");
				exit(1);
			}
		}	
	}
	//close(fd);
	return 0;
}




static int add_block(void)
{
    if(block_n != 0){
        block_p = realloc(block_p,(block_n + 1)* sizeof(void *));
        if(!block_p){
            return -1;
        }
    }else{
        block_p = calloc(1,sizeof(void *));
        if(!block_p){
            return -1;
        }
    }

    block_p[block_n] = calloc(1,BLOCK_SIZE);
    
    if(!block_p[block_n]){
    return -1;
   }

   block_n ++;
   syscall_n = 0;

   return 0;
}


int store_syscall(struct hashtab_node *input,struct id_node *inode)
{
	SYSCALL_STORE *sys_store;
	sys_store = malloc(sizeof(SYSCALL_STORE));
	sys_store->pid = input->pid;
	sys_store->inode = input->datum;
	sys_store->sys_id = inode->id;
	sys_store->count = inode->count;

	if(syscall_n < SYSCALLS_PER_BLOCK)
	{
		memcpy(block_p[block_n -1] + (syscall_n * sizeof(SYSCALL_STORE)),sys_store,sizeof(SYSCALL_STORE));
		syscall_n ++;
	}
	else{
		if(add_block()!=0) return -1;
		memcpy(block_p[block_n - 1] + (syscall_n * sizeof(SYSCALL_STORE)),sys_store,sizeof(SYSCALL_STORE));
		syscall_n ++;
	}

	if(store_flags)
	{
		int i;
		write_to_blocks_to_file();
		for(i = 0; i< block_n;i++){
			free(block_p[i]);	
		}
		free(block_p);
		mm_init();
		store_flags = 0;
		//exit(1);
	}
	return 0;
}

int mm_init(void)
{
    block_n = 0;
    if(add_block()!= 0)
    {
        return -1;
    }
    return 0;
}

 int analysis_handler(SYSCALL *data)
 {
	char key[5];
	sprintf(key,"%d",data->pid);
	hashtab_insert(sys_ht,key,data->pid,data->inode,data->sys_id);

 }
