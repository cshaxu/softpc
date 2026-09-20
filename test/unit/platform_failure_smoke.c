#include <windows.h>
#include "core/machine/machine.h"
#include <assert.h>
#include <stdio.h>
#include "../lib/cleanup.h"

static int fail_event, fail_timer, fail_wait, fake_timer, fail_delete;
static unsigned wait_calls, timer_calls;
static unsigned delete_calls;
static WAITORTIMERCALLBACK saved_tick;
static HANDLE WINAPI create_event(LPSECURITY_ATTRIBUTES a, BOOL manual,
    BOOL signaled, LPCSTR name)
{ return fail_event ? NULL : CreateEventA(a, manual, signaled, name); }
static BOOL WINAPI create_timer(PHANDLE timer, HANDLE queue,
    WAITORTIMERCALLBACK callback, PVOID context, DWORD due, DWORD period, ULONG flags)
{
    ++timer_calls;
    if (fake_timer && !fail_timer) {
        assert(due == 50u && period == 50u);
        saved_tick = callback;
        *timer = (HANDLE)(ULONG_PTR)1u;
        return TRUE;
    }
    return fail_timer ? FALSE :
        CreateTimerQueueTimer(timer, queue, callback, context, due, period, flags);
}
static BOOL WINAPI delete_timer(HANDLE queue, HANDLE timer, HANDLE completion)
{
    ++delete_calls;
    assert(completion == INVALID_HANDLE_VALUE); /* Return is a join barrier. */
    if (fail_delete) return FALSE;
    if (fake_timer) {
        assert(timer == (HANDLE)(ULONG_PTR)1u);
        saved_tick(NULL, FALSE); /* Last in-flight tick completes before join. */
        saved_tick = NULL;
        return TRUE;
    }
    return DeleteTimerQueueTimer(queue, timer, completion);
}
static DWORD WINAPI wait_event(HANDLE event, DWORD timeout)
{
    ++wait_calls;
    assert(timeout == INFINITE);
    if (fail_wait) {
        assert(wait_calls == 1u); /* Never retry an immediately failed HLT. */
        return WAIT_FAILED;
    }
    return WaitForSingleObject(event, timeout);
}
#define CreateEventA create_event
#define CreateTimerQueueTimer create_timer
#define DeleteTimerQueueTimer delete_timer
#define WaitForSingleObject wait_event
#include "core/compat/platform.c"
#undef WaitForSingleObject
#undef CreateTimerQueueTimer
#undef DeleteTimerQueueTimer
#undef CreateEventA

int main(void)
{
    const char *path = "softpc-platform-failure.img";
    unsigned char sector[512] = {0};
    const unsigned char halt[] = {0xfa, 0xf4, 0xeb, 0xfd}; /* cli; hlt; loop */
    softpc_machine_options options = {0};
    softpc_machine *machine = NULL;
    FILE *file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
    options.floppy_path = path;
    options.floppy_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    options.hard_disk_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);

    fail_event = 1;
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_IO_ERROR);
    assert(softpc_executor_event == NULL && timer_calls == 0u);
    fail_event = 0; fail_timer = 1;
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_IO_ERROR);
    assert(softpc_executor_event != NULL && softpc_clock_timer == NULL);
    host_timer_shutdown(); /* Partial initialization is disposable. */
    assert(softpc_executor_event == NULL);

    fail_timer = 0;
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_platform_executor_ready());
    assert(SetEvent(softpc_executor_event));
    softpc_platform_wait_for_executor_event();
    assert(wait_calls == 1u && softpc_platform_executor_ready());

    /* Capture freezes only the producer. Do not clear accepted ticks or the
       wake endpoint. Fake native callbacks make the join ordering deterministic. */
    assert(softpc_platform_set_clock_running(0));
    {
        HANDLE event = softpc_executor_event;
        unsigned deletes = delete_calls;
        InterlockedExchange(&softpc_clock_pending_ticks, 3);
        assert(softpc_platform_set_clock_running(0));
        assert(delete_calls == deletes);
        fake_timer = 1;
        assert(softpc_platform_set_clock_running(1));
        saved_tick(NULL, FALSE);
        fail_delete = 1;
        assert(!softpc_platform_set_clock_running(0));
        assert(softpc_clock_timer != NULL && saved_tick != NULL);
        assert(softpc_clock_pending_ticks == 4);
        fail_delete = 0;
        assert(softpc_platform_set_clock_running(0));
        assert(saved_tick == NULL && softpc_clock_timer == NULL);
        assert(softpc_executor_event == event && softpc_clock_pending_ticks == 5);
        fail_timer = 1;
        assert(!softpc_platform_set_clock_running(1));
        assert(softpc_executor_event == event && softpc_clock_pending_ticks == 5);
        fail_timer = 0;
        assert(softpc_platform_set_clock_running(1));
        assert(softpc_clock_pending_ticks == 5); /* No catch-up ticks on resume. */
        assert(softpc_platform_set_clock_running(0));
        fake_timer = 0;
        InterlockedExchange(&softpc_clock_pending_ticks, 0);
        assert(softpc_platform_set_clock_running(1));
    }

    /* Execute the real CCPU HLT and its existing outer unwind, not a mock jump. */
    assert(softpc_machine_write_physical(machine, 0x500u, halt, sizeof(halt)) ==
        SOFTPC_MACHINE_OK);
    assert(c_setCS(0u) == 0);
    c_setEIP(0x500u);
    fail_wait = 1; wait_calls = 0u;
    assert(softpc_machine_run(machine, UINT64_MAX) == SOFTPC_MACHINE_IO_ERROR);
    assert(wait_calls == 1u && !softpc_platform_executor_ready());
    assert(softpc_machine_run(machine, 1u) == SOFTPC_MACHINE_IO_ERROR);
    assert(wait_calls == 1u); /* Reject execution before entering the CPU again. */
    softpc_machine_destroy(machine);
    assert(softpc_executor_event == NULL && softpc_clock_timer == NULL);
    assert(!softpc_executor_wait_failed);
    assert(softpc_test_remove_image(path));
    return 0;
}
