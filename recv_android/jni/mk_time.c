#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mk_time.h"

#define MK_TIME_SLOTS		64
static mk_uptr_t			slot;

volatile mk_uptr_t  		mk_current_msec;
volatile  mk_time_t		*mk_cached_time;
static 	mk_atomic_t			mk_time_lock;

static	mk_time_t		cached_time[MK_TIME_SLOTS];

void mk_time_init(void)
{
	mk_cached_time = &cached_time[0];
	mk_time_update();
}

void mk_time_update(void)
{
	time_t	sec;
	mk_uptr_t		msec;
	mk_time_t		*tp;
	struct timeval tv;
	
	if(!mk_trylock(&mk_time_lock)){
		return ;	
	}
	mk_gettimeofday(&tv);
	sec = tv.tv_sec;
	msec = tv.tv_usec/1000;

	mk_current_msec = (mk_msec_t) sec*1000 + msec;
	__sync_synchronize();
	mk_unlock(&mk_time_lock);
	
}
