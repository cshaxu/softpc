#include "lib/types/types_interface.h"

#include <windows.h>

struct lib_native_event {
    HANDLE handle;
};

struct lib_native_task {
    HANDLE thread;
};

typedef struct lib_native_task_start {
    lib_native_task_entry entry;
    void *context;
} lib_native_task_start;

static DWORD WINAPI lib_native_task_main(LPVOID opaque)
{
    lib_native_task_start *start = (lib_native_task_start *)opaque;
    lib_native_task_entry entry = start->entry;
    void *context = start->context;

    lib_release(start);
    entry(context);
    return 0u;
}

void lib_native_sleep_milliseconds(lib_u32 milliseconds)
{
    Sleep((DWORD)milliseconds);
}

void lib_native_yield(void)
{
    Sleep(0u);
}

lib_status lib_native_event_create(lib_bool manual_reset,
    lib_native_event **out_event)
{
    lib_native_event *event;

    if (out_event == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_event = LIB_NULL;
    event = (lib_native_event *)lib_allocate_zero(1u, sizeof(*event));
    if (event == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    event->handle = CreateEventA(LIB_NULL, manual_reset != LIB_FALSE,
        FALSE, LIB_NULL);
    if (event->handle == NULL) {
        lib_release(event);
        return LIB_STATUS_IO_ERROR;
    }
    *out_event = event;
    return LIB_STATUS_OK;
}

void lib_native_event_destroy(lib_native_event *event)
{
    if (event == LIB_NULL) return;
    if (event->handle != NULL) (void)CloseHandle(event->handle);
    lib_release(event);
}

void lib_native_event_signal(lib_native_event *event)
{
    if (event != LIB_NULL && event->handle != NULL)
        (void)SetEvent(event->handle);
}

void lib_native_event_reset(lib_native_event *event)
{
    if (event != LIB_NULL && event->handle != NULL)
        (void)ResetEvent(event->handle);
}

lib_native_wait_result lib_native_event_wait_many(
    const lib_native_event *const *events, lib_u32 event_count,
    lib_u32 timeout_milliseconds, lib_u32 *out_event_index)
{
    HANDLE handles[MAXIMUM_WAIT_OBJECTS];
    DWORD result;
    lib_u32 index;

    if (events == LIB_NULL || event_count == 0u ||
        event_count > (lib_u32)MAXIMUM_WAIT_OBJECTS)
        return LIB_NATIVE_WAIT_FAULT;
    for (index = 0u; index < event_count; ++index) {
        if (events[index] == LIB_NULL || events[index]->handle == NULL)
            return LIB_NATIVE_WAIT_FAULT;
        handles[index] = events[index]->handle;
    }
    result = WaitForMultipleObjects((DWORD)event_count, handles, FALSE,
        (DWORD)timeout_milliseconds);
    if (result < WAIT_OBJECT_0 + event_count) {
        if (out_event_index != LIB_NULL) *out_event_index = result - WAIT_OBJECT_0;
        return LIB_NATIVE_WAIT_SIGNALED;
    }
    return result == WAIT_TIMEOUT ? LIB_NATIVE_WAIT_TIMED_OUT : LIB_NATIVE_WAIT_FAULT;
}

lib_native_wait_result lib_native_event_wait(const lib_native_event *event,
    lib_u32 timeout_milliseconds)
{
    return lib_native_event_wait_many(&event, 1u, timeout_milliseconds,
        LIB_NULL);
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
    task->thread = CreateThread(LIB_NULL, 0u, lib_native_task_main, start,
        0u, LIB_NULL);
    if (task->thread == NULL) {
        lib_release(start);
        lib_release(task);
        return LIB_STATUS_IO_ERROR;
    }
    *out_task = task;
    return LIB_STATUS_OK;
}

void lib_native_task_join(lib_native_task *task)
{
    if (task != LIB_NULL && task->thread != NULL)
        (void)WaitForSingleObject(task->thread, INFINITE);
}

void lib_native_task_destroy(lib_native_task *task)
{
    if (task == LIB_NULL) return;
    lib_native_task_join(task);
    if (task->thread != NULL) (void)CloseHandle(task->thread);
    lib_release(task);
}
