#include "lifecycle.h"

#include <windows.h>

#if defined(_MSC_VER)
#define SOFTPC_CCPU_THREAD_LOCAL __declspec(thread)
#else
#define SOFTPC_CCPU_THREAD_LOCAL __thread
#endif

static SOFTPC_CCPU_THREAD_LOCAL unsigned long softpc_ccpu_frame_depth;
static volatile LONG softpc_ccpu_exit_requested;

extern void ccpu386UnsimulateOuter(void);

void softpc_ccpu_lifecycle_enter(void)
{
    ++softpc_ccpu_frame_depth;
}

void softpc_ccpu_lifecycle_leave(void)
{
    if (softpc_ccpu_frame_depth != 0ul)
        --softpc_ccpu_frame_depth;
}

void softpc_ccpu_lifecycle_request_exit(void)
{
    InterlockedExchange(&softpc_ccpu_exit_requested, 1);
}

void softpc_ccpu_lifecycle_clear_exit(void)
{
    InterlockedExchange(&softpc_ccpu_exit_requested, 0);
}

int softpc_ccpu_lifecycle_exit_requested(void)
{
    return softpc_ccpu_frame_depth != 0ul &&
        InterlockedCompareExchange(&softpc_ccpu_exit_requested, 0, 0) != 0;
}

void softpc_ccpu_lifecycle_return_outer(void)
{
    /* The generated adapter returns to the first c_cpu_simulate frame.  The
       matching generated c_main leave hook then restores this depth to zero. */
    softpc_ccpu_frame_depth = 1ul;
    ccpu386UnsimulateOuter();
}
