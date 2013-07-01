#ifndef TIME_H
#define TIME_H
#include <stddef.h>
#include <stdint.h>
#include <time.h>
typedef 	struct tm		 mk_tm_t;
typedef 	uintptr_t 		mk_uptr_t;
typedef 	intptr_t 		mk_ptr_t;
typedef 	unsigned long 	mk_atomic_t;
extern volatile mk_uptr_t  		mk_current_msec;
typedef mk_uptr_t mk_msec_t;

#define mk_gettimeofday(tp) (void) gettimeofday(tp,NULL); 
#define mk_atomic_cmp_set(lock,old,set)\
	 __sync_bool_compare_and_swap(lock,old,set)

#define  mk_atomic_fetch_add(value,add) \
        __sync_fetch_and_add(value,add)
/*
 *  amd64 => x86-64 ,x64
 *  i386 => 32 位操作系统
 */

#if( __i386__ || __i386 || __amd64__ || __amd64)
#define mk_cpu_pause()      __asm__("pause")
#else
#define mk_cpu_pause()
#endif



#define mk_trylock(lock)        (*(lock)== 0 && mk_atomic_cmp_set(lock,0,1))
#define mk_unlock(lock)     *(lock) = 0


#define mk_sched_yield()  sched_yield()


typedef struct{
	time_t		sec;
	mk_uptr_t	msec;
	mk_ptr_t	gmtoff;
}mk_time_t;

void mk_time_update(void);
void mk_time_init(void);

#endif
