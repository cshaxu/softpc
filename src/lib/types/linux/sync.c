#define _POSIX_C_SOURCE 200809L

#include "lib/types/native_sync.h"

#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>

struct lib_native_event {
    pthread_mutex_t mutex;
    lib_bool signaled;
    lib_bool manual_reset;
};

struct lib_native_task {
    pthread_t thread;
    lib_bool joined;
};

typedef struct lib_native_wait_hub {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    lib_u64 generation;
    lib_status status;
} lib_native_wait_hub;

typedef struct lib_native_task_start {
    lib_native_task_entry entry;
    void *context;
} lib_native_task_start;

static lib_native_wait_hub lib_native_hub;
static pthread_once_t lib_native_hub_once = PTHREAD_ONCE_INIT;

static void lib_native_hub_initialize(void)
{
    pthread_condattr_t attributes;

    lib_native_hub.status = LIB_STATUS_IO_ERROR;
    if (pthread_mutex_init(&lib_native_hub.mutex, LIB_NULL) != 0 ||
        pthread_condattr_init(&attributes) != 0) return;
    if (pthread_condattr_setclock(&attributes, CLOCK_MONOTONIC) != 0 ||
        pthread_cond_init(&lib_native_hub.condition, &attributes) != 0) {
        (void)pthread_condattr_destroy(&attributes);
        return;
    }
    (void)pthread_condattr_destroy(&attributes);
    lib_native_hub.status = LIB_STATUS_OK;
}

static lib_status lib_native_hub_ready(void)
{
    (void)pthread_once(&lib_native_hub_once, lib_native_hub_initialize);
    return lib_native_hub.status;
}

static void lib_native_hub_notify(void)
{
    if (lib_native_hub_ready() != LIB_STATUS_OK) return;
    (void)pthread_mutex_lock(&lib_native_hub.mutex);
    ++lib_native_hub.generation;
    (void)pthread_cond_broadcast(&lib_native_hub.condition);
    (void)pthread_mutex_unlock(&lib_native_hub.mutex);
}

static lib_bool lib_native_deadline(lib_u32 milliseconds,
    struct timespec *deadline)
{
    if (clock_gettime(CLOCK_MONOTONIC, deadline) != 0) return LIB_FALSE;
    deadline->tv_sec += (time_t)(milliseconds / 1000u);
    deadline->tv_nsec += (long)(milliseconds % 1000u) * 1000000L;
    if (deadline->tv_nsec >= 1000000000L) {
        ++deadline->tv_sec;
        deadline->tv_nsec -= 1000000000L;
    }
    return LIB_TRUE;
}

static lib_bool lib_native_event_take_signal(const lib_native_event *event)
{
    lib_bool signaled;

    if (event == LIB_NULL) return LIB_FALSE;
    (void)pthread_mutex_lock((pthread_mutex_t *)&event->mutex);
    signaled = event->signaled;
    if (signaled && !event->manual_reset)
        ((lib_native_event *)event)->signaled = LIB_FALSE;
    (void)pthread_mutex_unlock((pthread_mutex_t *)&event->mutex);
    return signaled;
}

static void *lib_native_task_main(void *opaque)
{
    lib_native_task_start *start = (lib_native_task_start *)opaque;
    lib_native_task_entry entry = start->entry;
    void *context = start->context;

    lib_release(start);
    entry(context);
    return LIB_NULL;
}

void lib_native_sleep_milliseconds(lib_u32 milliseconds)
{
    struct timespec duration = { (time_t)(milliseconds / 1000u),
        (long)(milliseconds % 1000u) * 1000000L };

    (void)nanosleep(&duration, LIB_NULL);
}

void lib_native_yield(void)
{
    (void)sched_yield();
}

lib_status lib_native_event_create(lib_bool manual_reset,
    lib_native_event **out_event)
{
    lib_native_event *event;

    if (out_event == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_event = LIB_NULL;
    event = (lib_native_event *)lib_allocate_zero(1u, sizeof(*event));
    if (event == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    if (pthread_mutex_init(&event->mutex, LIB_NULL) != 0) {
        lib_release(event);
        return LIB_STATUS_IO_ERROR;
    }
    event->manual_reset = manual_reset != LIB_FALSE;
    *out_event = event;
    return LIB_STATUS_OK;
}

void lib_native_event_destroy(lib_native_event *event)
{
    if (event == LIB_NULL) return;
    (void)pthread_mutex_destroy(&event->mutex);
    lib_release(event);
}

void lib_native_event_signal(lib_native_event *event)
{
    if (event == LIB_NULL) return;
    (void)pthread_mutex_lock(&event->mutex);
    event->signaled = LIB_TRUE;
    (void)pthread_mutex_unlock(&event->mutex);
    lib_native_hub_notify();
}

void lib_native_event_reset(lib_native_event *event)
{
    if (event == LIB_NULL) return;
    (void)pthread_mutex_lock(&event->mutex);
    event->signaled = LIB_FALSE;
    (void)pthread_mutex_unlock(&event->mutex);
    lib_native_hub_notify();
}

lib_status lib_native_event_wait_many(
    const lib_native_event *const *events, lib_u32 event_count,
    lib_u32 timeout_milliseconds, lib_bool *out_signaled,
    lib_u32 *out_event_index)
{
    struct timespec deadline;
    lib_u32 index;

    if (events == LIB_NULL || out_signaled == LIB_NULL || event_count == 0u)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (lib_native_hub_ready() != LIB_STATUS_OK ||
        !lib_native_deadline(timeout_milliseconds, &deadline))
        return LIB_STATUS_IO_ERROR;
    *out_signaled = LIB_FALSE;
    for (index = 0u; index < event_count; ++index) {
        if (events[index] == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    }
    for (;;) {
        lib_u64 observed;
        int result;

        (void)pthread_mutex_lock(&lib_native_hub.mutex);
        observed = lib_native_hub.generation;
        (void)pthread_mutex_unlock(&lib_native_hub.mutex);
        for (index = 0u; index < event_count; ++index) {
            if (lib_native_event_take_signal(events[index])) {
                if (out_event_index != LIB_NULL) *out_event_index = index;
                *out_signaled = LIB_TRUE;
                return LIB_STATUS_OK;
            }
        }
        if (timeout_milliseconds == 0u) return LIB_STATUS_OK;
        (void)pthread_mutex_lock(&lib_native_hub.mutex);
        if (lib_native_hub.generation != observed) {
            (void)pthread_mutex_unlock(&lib_native_hub.mutex);
            continue;
        }
        result = pthread_cond_timedwait(&lib_native_hub.condition,
            &lib_native_hub.mutex, &deadline);
        (void)pthread_mutex_unlock(&lib_native_hub.mutex);
        if (result == ETIMEDOUT) return LIB_STATUS_OK;
        if (result != 0) return LIB_STATUS_IO_ERROR;
    }
}

lib_status lib_native_event_wait(const lib_native_event *event,
    lib_u32 timeout_milliseconds, lib_bool *out_signaled)
{
    return lib_native_event_wait_many(&event, 1u, timeout_milliseconds,
        out_signaled, LIB_NULL);
}

lib_status lib_native_event_wait_messages(const lib_native_event *event,
    lib_u32 timeout_milliseconds, lib_bool *out_wake, lib_bool *out_message)
{
    (void)event;
    (void)timeout_milliseconds;
    if (out_wake == LIB_NULL || out_message == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_wake = LIB_FALSE;
    *out_message = LIB_FALSE;
    return LIB_STATUS_UNSUPPORTED;
}

lib_status lib_native_task_create(lib_native_task_entry entry, void *context,
    lib_native_task **out_task)
{
    lib_native_task *task;
    lib_native_task_start *start;

    if (entry == LIB_NULL || out_task == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_task = LIB_NULL;
    task = (lib_native_task *)lib_allocate_zero(1u, sizeof(*task));
    start = (lib_native_task_start *)lib_allocate_zero(1u, sizeof(*start));
    if (task == LIB_NULL || start == LIB_NULL) {
        lib_release(start);
        lib_release(task);
        return LIB_STATUS_NO_MEMORY;
    }
    start->entry = entry;
    start->context = context;
    if (pthread_create(&task->thread, LIB_NULL, lib_native_task_main, start) != 0) {
        lib_release(start);
        lib_release(task);
        return LIB_STATUS_IO_ERROR;
    }
    *out_task = task;
    return LIB_STATUS_OK;
}

void lib_native_task_join(lib_native_task *task)
{
    if (task != LIB_NULL && !task->joined) {
        (void)pthread_join(task->thread, LIB_NULL);
        task->joined = LIB_TRUE;
    }
}

void lib_native_task_destroy(lib_native_task *task)
{
    if (task == LIB_NULL) return;
    lib_native_task_join(task);
    lib_release(task);
}
