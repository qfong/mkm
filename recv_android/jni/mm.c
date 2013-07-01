#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include "mm.h"



static unsigned long syscall_n;
static unsigned long block_n;
static void **block_p;
int block_lim;

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


int mm_init(void)
{
    block_n = 0;
    if(add_block()!= 0)
    {
        return -1;
    }
    return 0;
}
