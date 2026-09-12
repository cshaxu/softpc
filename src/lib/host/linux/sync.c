#define _POSIX_C_SOURCE 200809L

#include "lib/host/sync.h"

#include "lib/types/posix.h"

struct host_sync_platform_event {
    pthread_mutex_t lock;
    pthread_cond_t changed;
    lib_bool signaled;
    lib_bool manual_reset;
};
struct host_sync_platform_task { pthread_t thread; lib_bool joined; };
typedef struct host_sync_platform_start {
    host_sync_platform_task_entry entry;
    void *context;
} host_sync_platform_start;

static lib_bool host_sync_platform_deadline(lib_u32 milliseconds,
    struct timespec *deadline)
{
    if (clock_gettime(CLOCK_REALTIME, deadline) != 0) return LIB_FALSE;
    deadline->tv_sec += (time_t)(milliseconds / 1000u);
    deadline->tv_nsec += (long)(milliseconds % 1000u) * 1000000L;
    if (deadline->tv_nsec >= 1000000000L) {
        ++deadline->tv_sec; deadline->tv_nsec -= 1000000000L;
    }
    return LIB_TRUE;
}

static lib_bool host_sync_platform_expired(const struct timespec *deadline)
{
    struct timespec now;

    if (clock_gettime(CLOCK_REALTIME, &now) != 0) return LIB_TRUE;
    return now.tv_sec > deadline->tv_sec ||
        (now.tv_sec == deadline->tv_sec && now.tv_nsec >= deadline->tv_nsec) ?
        LIB_TRUE : LIB_FALSE;
}

static lib_bool host_sync_platform_take(host_sync_platform_event *event)
{
    lib_bool signaled;
    (void)pthread_mutex_lock(&event->lock);
    signaled = event->signaled;
    if (signaled != LIB_FALSE && event->manual_reset == LIB_FALSE)
        event->signaled = LIB_FALSE;
    (void)pthread_mutex_unlock(&event->lock);
    return signaled;
}

static void *host_sync_platform_main(void *opaque)
{
    host_sync_platform_start *start = opaque;
    host_sync_platform_task_entry entry = start->entry;
    void *context = start->context;
    lib_release(start);
    entry(context);
    return LIB_NULL;
}

void host_sync_platform_sleep_milliseconds(lib_u32 milliseconds)
{
    struct timespec duration = { (time_t)(milliseconds / 1000u),
        (long)(milliseconds % 1000u) * 1000000L };
    (void)nanosleep(&duration, LIB_NULL);
}
void host_sync_platform_yield(void) { (void)sched_yield(); }

lib_status host_sync_platform_event_create(lib_bool manual_reset,
    host_sync_platform_event **out_event)
{
    host_sync_platform_event *event;
    if (out_event == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_event = LIB_NULL;
    event = lib_allocate_zero(1u, sizeof(*event));
    if (event == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    if (pthread_mutex_init(&event->lock, LIB_NULL) != 0 ||
        pthread_cond_init(&event->changed, LIB_NULL) != 0) {
        (void)pthread_mutex_destroy(&event->lock); lib_release(event);
        return LIB_STATUS_IO_ERROR;
    }
    event->manual_reset = manual_reset != LIB_FALSE;
    *out_event = event;
    return LIB_STATUS_OK;
}
void host_sync_platform_event_destroy(host_sync_platform_event *event)
{
    if (event == LIB_NULL) return;
    (void)pthread_cond_destroy(&event->changed);
    (void)pthread_mutex_destroy(&event->lock);
    lib_release(event);
}
void host_sync_platform_event_signal(host_sync_platform_event *event)
{
    if (event == LIB_NULL) return;
    (void)pthread_mutex_lock(&event->lock); event->signaled = LIB_TRUE;
    (void)pthread_cond_broadcast(&event->changed); (void)pthread_mutex_unlock(&event->lock);
}
void host_sync_platform_event_reset(host_sync_platform_event *event)
{
    if (event == LIB_NULL) return;
    (void)pthread_mutex_lock(&event->lock); event->signaled = LIB_FALSE;
    (void)pthread_mutex_unlock(&event->lock);
}

lib_status host_sync_platform_event_wait_many(
    const host_sync_platform_event *const *events, lib_u32 event_count,
    lib_u32 timeout_milliseconds, lib_bool *out_signaled,
    lib_u32 *out_event_index)
{
    lib_u32 index;
    struct timespec deadline;
    if (events == LIB_NULL || event_count == 0u || out_signaled == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_signaled = LIB_FALSE;
    if (!host_sync_platform_deadline(timeout_milliseconds, &deadline)) return LIB_STATUS_IO_ERROR;
    for (;;) {
        for (index = 0u; index < event_count; ++index) {
            if (events[index] == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
            if (host_sync_platform_take((host_sync_platform_event *)events[index]) != LIB_FALSE) {
                if (out_event_index != LIB_NULL) *out_event_index = index;
                *out_signaled = LIB_TRUE; return LIB_STATUS_OK;
            }
        }
        if (timeout_milliseconds == 0u) return LIB_STATUS_OK;
        if (host_sync_platform_expired(&deadline) != LIB_FALSE)
            return LIB_STATUS_OK;
        host_sync_platform_sleep_milliseconds(1u);
    }
}

lib_status host_sync_platform_task_create(host_sync_platform_task_entry entry,
    void *context, host_sync_platform_task **out_task)
{
    host_sync_platform_task *task;
    host_sync_platform_start *start;
    if (entry == LIB_NULL || out_task == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_task = LIB_NULL;
    task = lib_allocate_zero(1u, sizeof(*task)); start = lib_allocate_zero(1u, sizeof(*start));
    if (task == LIB_NULL || start == LIB_NULL) { lib_release(start); lib_release(task); return LIB_STATUS_NO_MEMORY; }
    start->entry = entry; start->context = context;
    if (pthread_create(&task->thread, LIB_NULL, host_sync_platform_main, start) != 0) {
        lib_release(start); lib_release(task); return LIB_STATUS_IO_ERROR;
    }
    *out_task = task; return LIB_STATUS_OK;
}
void host_sync_platform_task_join(host_sync_platform_task *task)
{ if (task != LIB_NULL && task->joined == LIB_FALSE) { (void)pthread_join(task->thread, LIB_NULL); task->joined = LIB_TRUE; } }
void host_sync_platform_task_destroy(host_sync_platform_task *task)
{ if (task != LIB_NULL) { host_sync_platform_task_join(task); lib_release(task); } }
