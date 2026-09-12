#include "lib/types/types_interface.h"

#include <windows.h>

struct lib_sync_event {
    HANDLE handle;
};

struct lib_sync_task {
    HANDLE thread;
    HANDLE cancellation;
    lib_sync_task_entry entry;
    void *context;
};

static DWORD WINAPI lib_sync_task_main(LPVOID opaque)
{
    lib_sync_task *task = opaque;

    if (task != LIB_NULL && task->entry != LIB_NULL)
        task->entry(task->context, task);
    return 0u;
}

lib_sync_wait_result lib_sync_wait_any(lib_sync_event *const *events,
    lib_u32 event_count, const lib_sync_task *cancel_task,
    lib_u32 timeout_milliseconds, lib_u32 *out_event_index)
{
    HANDLE handles[MAXIMUM_WAIT_OBJECTS] = { NULL };
    DWORD count = 0u;
    DWORD result;
    lib_u32 index;

    if ((event_count != 0u && events == LIB_NULL) ||
        (event_count == 0u && cancel_task == LIB_NULL) ||
        event_count > (lib_u32)(MAXIMUM_WAIT_OBJECTS -
            (cancel_task != LIB_NULL ? 1u : 0u)))
        return LIB_SYNC_WAIT_INVALID_ARGUMENT;
    if (out_event_index != LIB_NULL) *out_event_index = UINT32_MAX;
    if (cancel_task != LIB_NULL) {
        if (cancel_task->cancellation == NULL) return LIB_SYNC_WAIT_INVALID_ARGUMENT;
        handles[count++] = cancel_task->cancellation;
    }
    for (index = 0u; index < event_count; ++index) {
        if (events[index] == LIB_NULL || events[index]->handle == NULL)
            return LIB_SYNC_WAIT_INVALID_ARGUMENT;
        handles[count++] = events[index]->handle;
    }
    result = WaitForMultipleObjects(count, handles, FALSE, timeout_milliseconds);
    if (result < WAIT_OBJECT_0 + count) {
        if (cancel_task != LIB_NULL && result == WAIT_OBJECT_0)
            return LIB_SYNC_WAIT_CANCELLED;
        if (out_event_index != LIB_NULL)
            *out_event_index = result - WAIT_OBJECT_0 -
                (cancel_task != LIB_NULL ? 1u : 0u);
        return LIB_SYNC_WAIT_SIGNALED;
    }
    return result == WAIT_TIMEOUT ? LIB_SYNC_WAIT_TIMED_OUT : LIB_SYNC_WAIT_FAULT;
}

void lib_sync_sleep_milliseconds(lib_u32 milliseconds) { Sleep(milliseconds); }

void lib_sync_yield(void) { Sleep(0u); }

lib_status lib_sync_event_create(lib_sync_event **out_event)
{
    lib_sync_event *event;

    if (out_event == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_event = LIB_NULL;
    event = lib_allocate_zero(1u, sizeof(*event));
    if (event == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    event->handle = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (event->handle == NULL) {
        lib_release(event);
        return LIB_STATUS_IO_ERROR;
    }
    *out_event = event;
    return LIB_STATUS_OK;
}

void lib_sync_event_destroy(lib_sync_event *event)
{
    if (event == LIB_NULL) return;
    if (event->handle != NULL) (void)CloseHandle(event->handle);
    lib_release(event);
}

void lib_sync_event_signal(lib_sync_event *event)
{
    if (event != LIB_NULL && event->handle != NULL) (void)SetEvent(event->handle);
}

void lib_sync_event_reset(lib_sync_event *event)
{
    if (event != LIB_NULL && event->handle != NULL) (void)ResetEvent(event->handle);
}

lib_sync_wait_result lib_sync_event_wait(lib_sync_event *event,
    lib_u32 timeout_milliseconds)
{
    return lib_sync_wait_any(&event, 1u, LIB_NULL, timeout_milliseconds, LIB_NULL);
}

lib_status lib_sync_task_create(lib_sync_task_entry entry, void *context,
    lib_sync_task **out_task)
{
    lib_sync_task *task;

    if (entry == LIB_NULL || out_task == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_task = LIB_NULL;
    task = lib_allocate_zero(1u, sizeof(*task));
    if (task == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    task->entry = entry;
    task->context = context;
    task->cancellation = CreateEventA(NULL, TRUE, FALSE, NULL);
    task->thread = task->cancellation == NULL ? NULL : CreateThread(NULL, 0u,
        lib_sync_task_main, task, 0u, NULL);
    if (task->thread == NULL) {
        if (task->cancellation != NULL) (void)CloseHandle(task->cancellation);
        lib_release(task);
        return LIB_STATUS_IO_ERROR;
    }
    *out_task = task;
    return LIB_STATUS_OK;
}

void lib_sync_task_request_cancel(lib_sync_task *task)
{
    if (task != LIB_NULL && task->cancellation != NULL)
        (void)SetEvent(task->cancellation);
}

int lib_sync_task_cancelled(const lib_sync_task *task)
{
    return task != LIB_NULL && task->cancellation != NULL &&
        WaitForSingleObject(task->cancellation, 0u) == WAIT_OBJECT_0;
}

lib_sync_wait_result lib_sync_task_wait_cancel(const lib_sync_task *task,
    lib_u32 timeout_milliseconds)
{
    if (task == LIB_NULL) return LIB_SYNC_WAIT_INVALID_ARGUMENT;
    return lib_sync_wait_any(LIB_NULL, 0u, task, timeout_milliseconds, LIB_NULL);
}

void lib_sync_task_join(lib_sync_task *task)
{
    if (task != LIB_NULL && task->thread != NULL)
        (void)WaitForSingleObject(task->thread, INFINITE);
}

void lib_sync_task_destroy(lib_sync_task *task)
{
    if (task == LIB_NULL) return;
    lib_sync_task_request_cancel(task);
    lib_sync_task_join(task);
    if (task->thread != NULL) (void)CloseHandle(task->thread);
    if (task->cancellation != NULL) (void)CloseHandle(task->cancellation);
    lib_release(task);
}
