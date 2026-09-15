#include "lib/types/types_interface.h"
#include "lib/types/win32/sync.h"
#include "lib/base/sync_interface.h"
#include <assert.h>

static unsigned allocations, releases, joins;
static int fail_create, fail_signal, fail_reset;
static HANDLE WINAPI create_event(LPSECURITY_ATTRIBUTES attributes, BOOL manual,
    BOOL initial, LPCSTR name)
{ return fail_create ? NULL : CreateEventA(attributes, manual, initial, name); }
static BOOL WINAPI signal_event(HANDLE handle)
{ return fail_signal ? FALSE : SetEvent(handle); }
static BOOL WINAPI reset_event(HANDLE handle)
{ return fail_reset ? FALSE : ResetEvent(handle); }
static void *allocate(lib_size count, lib_size size)
{ ++allocations; return lib_allocate_zero(count, size); }
static void release(void *memory)
{ if (memory) ++releases; lib_release(memory); }
static DWORD WINAPI join_wait(HANDLE handle, DWORD timeout)
{ ++joins; return WaitForSingleObject(handle, timeout); }
#undef lib_allocate_zero
#undef lib_release
#undef lib_win32_wait_for_single_object
#define lib_allocate_zero allocate
#define lib_release release
#define lib_win32_wait_for_single_object join_wait
#undef lib_win32_create_event_a
#undef lib_win32_set_event
#undef lib_win32_reset_event
#define lib_win32_create_event_a create_event
#define lib_win32_set_event signal_event
#define lib_win32_reset_event reset_event
#include "lib/base/win32/sync.c"

static void task_entry(void *context, const base_sync_task *task)
{
    unsigned *completed = context;
    assert(base_sync_task_wait_cancel(task, LIB_UINT32_MAX) == BASE_SYNC_WAIT_CANCELLED);
    *completed = 1;
}

int main(void)
{
    base_sync_event *event;
    base_sync_task *task;
    unsigned completed = 0;
    assert(base_sync_event_create(BASE_SYNC_EVENT_MANUAL_RESET, &event) == LIB_STATUS_OK);
    assert(allocations == 1);
    assert(base_sync_event_signal(event) == LIB_STATUS_OK);
    assert(base_sync_event_wait(event, 0) == BASE_SYNC_WAIT_SIGNALED);
    assert(base_sync_event_wait(event, 0) == BASE_SYNC_WAIT_SIGNALED);
    assert(base_sync_event_reset(event) == LIB_STATUS_OK);
    assert(base_sync_event_wait(event, 0) == BASE_SYNC_WAIT_TIMED_OUT);
    base_sync_event_destroy(event);
    assert(releases == 1);
    assert(base_sync_task_create(task_entry, &completed, &task) == LIB_STATUS_OK);
    assert(allocations == 3); /* cancellation event + thread, no startup allocation */
    base_sync_task_destroy(task);
    assert(completed && joins == 1 && allocations == releases);
    event = (void *)1;
    assert(base_sync_event_create((base_sync_event_mode)99, &event) ==
        LIB_STATUS_INVALID_ARGUMENT && event == NULL);
    assert(base_sync_event_create(BASE_SYNC_EVENT_AUTO_RESET, NULL) ==
        LIB_STATUS_INVALID_ARGUMENT);
    fail_create = 1;
    assert(base_sync_event_create(BASE_SYNC_EVENT_AUTO_RESET, &event) ==
        LIB_STATUS_IO_ERROR && event == NULL);
    fail_create = 0;
    assert(base_sync_event_create(BASE_SYNC_EVENT_AUTO_RESET, &event) == LIB_STATUS_OK);
    assert(base_sync_event_signal(NULL) == LIB_STATUS_INVALID_ARGUMENT);
    assert(base_sync_event_reset(NULL) == LIB_STATUS_INVALID_ARGUMENT);
    assert(base_sync_event_wait(NULL, 0) == BASE_SYNC_WAIT_INVALID_ARGUMENT);
    assert(base_sync_event_signal(event) == LIB_STATUS_OK);
    assert(base_sync_event_signal(event) == LIB_STATUS_OK);
    assert(base_sync_event_wait(event, 0) == BASE_SYNC_WAIT_SIGNALED);
    assert(base_sync_event_wait(event, 0) == BASE_SYNC_WAIT_TIMED_OUT);
    fail_signal = fail_reset = 1;
    assert(base_sync_event_signal(event) == LIB_STATUS_IO_ERROR);
    assert(base_sync_event_reset(event) == LIB_STATUS_IO_ERROR);
    fail_signal = fail_reset = 0;
    assert(base_sync_event_signal(event) == LIB_STATUS_OK);
    assert(base_sync_event_reset(event) == LIB_STATUS_OK);
    assert(base_sync_event_wait(event, 0) == BASE_SYNC_WAIT_TIMED_OUT);
    base_sync_event_destroy(event);
    assert(allocations == releases);
    return 0;
}
