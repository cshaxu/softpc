#include "lib/host/sync.h"

#include "lib/types/win32/sync.h"

struct host_sync_platform_event { HANDLE handle; };
struct host_sync_platform_task { HANDLE thread; };
typedef struct host_sync_platform_start {
    host_sync_platform_task_entry entry;
    void *context;
} host_sync_platform_start;

static DWORD WINAPI host_sync_platform_main(LPVOID opaque)
{
    host_sync_platform_start *start = (host_sync_platform_start *)opaque;
    host_sync_platform_task_entry entry = start->entry;
    void *context = start->context;
    lib_release(start);
    entry(context);
    return 0u;
}

void host_sync_platform_sleep_milliseconds(lib_u32 milliseconds)
{ Sleep((DWORD)milliseconds); }
void host_sync_platform_yield(void) { Sleep(0u); }

lib_status host_sync_platform_event_create(lib_bool manual_reset,
    host_sync_platform_event **out_event)
{
    host_sync_platform_event *event;
    if (out_event == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_event = LIB_NULL;
    event = lib_allocate_zero(1u, sizeof(*event));
    if (event == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    event->handle = CreateEventA(LIB_NULL, manual_reset != LIB_FALSE, FALSE, LIB_NULL);
    if (event->handle == NULL) { lib_release(event); return LIB_STATUS_IO_ERROR; }
    *out_event = event;
    return LIB_STATUS_OK;
}

void host_sync_platform_event_destroy(host_sync_platform_event *event)
{ if (event != LIB_NULL) { if (event->handle != NULL) (void)CloseHandle(event->handle); lib_release(event); } }
void host_sync_platform_event_signal(host_sync_platform_event *event)
{ if (event != LIB_NULL && event->handle != NULL) (void)SetEvent(event->handle); }
void host_sync_platform_event_reset(host_sync_platform_event *event)
{ if (event != LIB_NULL && event->handle != NULL) (void)ResetEvent(event->handle); }

lib_status host_sync_platform_event_wait_many(
    const host_sync_platform_event *const *events, lib_u32 event_count,
    lib_u32 timeout_milliseconds, lib_bool *out_signaled,
    lib_u32 *out_event_index)
{
    HANDLE handles[MAXIMUM_WAIT_OBJECTS];
    DWORD result;
    lib_u32 index;
    if (events == LIB_NULL || out_signaled == LIB_NULL || event_count == 0u ||
        event_count > (lib_u32)MAXIMUM_WAIT_OBJECTS) return LIB_STATUS_INVALID_ARGUMENT;
    *out_signaled = LIB_FALSE;
    for (index = 0u; index < event_count; ++index) {
        if (events[index] == LIB_NULL || events[index]->handle == NULL)
            return LIB_STATUS_INVALID_ARGUMENT;
        handles[index] = events[index]->handle;
    }
    result = WaitForMultipleObjects((DWORD)event_count, handles, FALSE,
        (DWORD)timeout_milliseconds);
    if (result < WAIT_OBJECT_0 + event_count) {
        if (out_event_index != LIB_NULL) *out_event_index = result - WAIT_OBJECT_0;
        *out_signaled = LIB_TRUE;
        return LIB_STATUS_OK;
    }
    return result == WAIT_TIMEOUT ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status host_sync_platform_task_create(host_sync_platform_task_entry entry,
    void *context, host_sync_platform_task **out_task)
{
    host_sync_platform_task *task;
    host_sync_platform_start *start;
    if (entry == LIB_NULL || out_task == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_task = LIB_NULL;
    task = lib_allocate_zero(1u, sizeof(*task));
    start = lib_allocate_zero(1u, sizeof(*start));
    if (task == LIB_NULL || start == LIB_NULL) { lib_release(start); lib_release(task); return LIB_STATUS_NO_MEMORY; }
    start->entry = entry; start->context = context;
    task->thread = CreateThread(LIB_NULL, 0u, host_sync_platform_main, start, 0u, LIB_NULL);
    if (task->thread == NULL) { lib_release(start); lib_release(task); return LIB_STATUS_IO_ERROR; }
    *out_task = task;
    return LIB_STATUS_OK;
}

void host_sync_platform_task_join(host_sync_platform_task *task)
{ if (task != LIB_NULL && task->thread != NULL) (void)WaitForSingleObject(task->thread, INFINITE); }
void host_sync_platform_task_destroy(host_sync_platform_task *task)
{ if (task != LIB_NULL) { host_sync_platform_task_join(task); if (task->thread != NULL) (void)CloseHandle(task->thread); lib_release(task); } }
