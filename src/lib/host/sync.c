#include "lib/host/sync_interface.h"

struct host_sync_event {
    lib_native_event *native;
};

struct host_sync_task {
    lib_native_task *native;
    host_sync_event *cancellation;
    host_sync_task_entry entry;
    void *context;
};

static void host_sync_task_main(void *opaque)
{
    host_sync_task *task = (host_sync_task *)opaque;

    if (task != LIB_NULL && task->entry != LIB_NULL)
        task->entry(task->context, task);
}

static host_sync_wait_result host_sync_wait_result_from_native(
    lib_native_wait_result result)
{
    switch (result) {
    case LIB_NATIVE_WAIT_SIGNALED: return HOST_SYNC_WAIT_SIGNALED;
    case LIB_NATIVE_WAIT_TIMED_OUT: return HOST_SYNC_WAIT_TIMED_OUT;
    default: return HOST_SYNC_WAIT_FAULT;
    }
}

void host_sync_sleep_milliseconds(lib_u32 milliseconds)
{
    lib_native_sleep_milliseconds(milliseconds);
}

void host_sync_yield(void)
{
    lib_native_yield();
}

lib_status host_sync_event_create(host_sync_event **out_event)
{
    host_sync_event *event;
    lib_status status;

    if (out_event == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_event = LIB_NULL;
    event = (host_sync_event *)lib_allocate_zero(1u, sizeof(*event));
    if (event == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    status = lib_native_event_create(LIB_TRUE, &event->native);
    if (status != LIB_STATUS_OK) {
        lib_release(event);
        return status;
    }
    *out_event = event;
    return LIB_STATUS_OK;
}

void host_sync_event_destroy(host_sync_event *event)
{
    if (event == LIB_NULL) return;
    lib_native_event_destroy(event->native);
    lib_release(event);
}

void host_sync_event_signal(host_sync_event *event)
{
    if (event != LIB_NULL) lib_native_event_signal(event->native);
}

void host_sync_event_reset(host_sync_event *event)
{
    if (event != LIB_NULL) lib_native_event_reset(event->native);
}

host_sync_wait_result host_sync_wait_any(host_sync_event *const *events,
    lib_u32 event_count, const host_sync_task *cancel_task,
    lib_u32 timeout_milliseconds, lib_u32 *out_event_index)
{
    const lib_native_event *native_events[64];
    lib_u32 native_count = 0u;
    lib_u32 native_index = 0u;
    lib_u32 index;
    lib_native_wait_result result;

    if ((event_count != 0u && events == LIB_NULL) ||
        (event_count == 0u && cancel_task == LIB_NULL) || event_count > 63u)
        return HOST_SYNC_WAIT_INVALID_ARGUMENT;
    if (out_event_index != LIB_NULL) *out_event_index = UINT32_MAX;
    if (cancel_task != LIB_NULL) {
        if (cancel_task->cancellation == LIB_NULL ||
            cancel_task->cancellation->native == LIB_NULL)
            return HOST_SYNC_WAIT_INVALID_ARGUMENT;
        native_events[native_count++] = cancel_task->cancellation->native;
    }
    for (index = 0u; index < event_count; ++index) {
        if (events[index] == LIB_NULL || events[index]->native == LIB_NULL)
            return HOST_SYNC_WAIT_INVALID_ARGUMENT;
        native_events[native_count++] = events[index]->native;
    }
    result = lib_native_event_wait_many(native_events, native_count,
        timeout_milliseconds, &native_index);
    if (result != LIB_NATIVE_WAIT_SIGNALED)
        return host_sync_wait_result_from_native(result);
    if (cancel_task != LIB_NULL && native_index == 0u)
        return HOST_SYNC_WAIT_CANCELLED;
    if (out_event_index != LIB_NULL)
        *out_event_index = native_index - (cancel_task != LIB_NULL ? 1u : 0u);
    return HOST_SYNC_WAIT_SIGNALED;
}

host_sync_wait_result host_sync_event_wait(host_sync_event *event,
    lib_u32 timeout_milliseconds)
{
    return host_sync_wait_any(&event, 1u, LIB_NULL, timeout_milliseconds,
        LIB_NULL);
}

lib_status host_sync_task_create(host_sync_task_entry entry, void *context,
    host_sync_task **out_task)
{
    host_sync_task *task;
    lib_status status;

    if (entry == LIB_NULL || out_task == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_task = LIB_NULL;
    task = (host_sync_task *)lib_allocate_zero(1u, sizeof(*task));
    if (task == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    task->entry = entry;
    task->context = context;
    status = host_sync_event_create(&task->cancellation);
    if (status == LIB_STATUS_OK)
        status = lib_native_task_create(host_sync_task_main, task, &task->native);
    if (status != LIB_STATUS_OK) {
        host_sync_event_destroy(task->cancellation);
        lib_release(task);
        return status;
    }
    *out_task = task;
    return LIB_STATUS_OK;
}

void host_sync_task_request_cancel(host_sync_task *task)
{
    if (task != LIB_NULL) host_sync_event_signal(task->cancellation);
}

int host_sync_task_cancelled(const host_sync_task *task)
{
    return task != LIB_NULL && task->cancellation != LIB_NULL &&
        lib_native_event_wait(task->cancellation->native, 0u) ==
            LIB_NATIVE_WAIT_SIGNALED;
}

host_sync_wait_result host_sync_task_wait_cancel(const host_sync_task *task,
    lib_u32 timeout_milliseconds)
{
    if (task == LIB_NULL) return HOST_SYNC_WAIT_INVALID_ARGUMENT;
    return host_sync_wait_any(LIB_NULL, 0u, task, timeout_milliseconds,
        LIB_NULL);
}

void host_sync_task_join(host_sync_task *task)
{
    if (task != LIB_NULL) lib_native_task_join(task->native);
}

void host_sync_task_destroy(host_sync_task *task)
{
    if (task == LIB_NULL) return;
    host_sync_task_request_cancel(task);
    host_sync_task_join(task);
    lib_native_task_destroy(task->native);
    host_sync_event_destroy(task->cancellation);
    lib_release(task);
}
