#ifndef LIB_TYPES_LINUX_SYNC_H
#define LIB_TYPES_LINUX_SYNC_H

/* POSIX threading, scheduling and wait-error declarations. */
#include <errno.h>
#include <pthread.h>
#include <sched.h>

typedef pthread_mutex_t lib_linux_pthread_mutex_t;
typedef pthread_cond_t lib_linux_pthread_cond_t;
typedef pthread_t lib_linux_pthread_t;
#define lib_linux_pthread_mutex_init pthread_mutex_init
#define lib_linux_pthread_mutex_destroy pthread_mutex_destroy
#define lib_linux_pthread_mutex_lock pthread_mutex_lock
#define lib_linux_pthread_mutex_unlock pthread_mutex_unlock
#define lib_linux_pthread_cond_init pthread_cond_init
#define lib_linux_pthread_cond_destroy pthread_cond_destroy
#define lib_linux_pthread_cond_signal pthread_cond_signal
#define lib_linux_pthread_cond_broadcast pthread_cond_broadcast
#define lib_linux_pthread_create pthread_create
#define lib_linux_pthread_join pthread_join
#define lib_linux_sched_yield sched_yield

#endif
