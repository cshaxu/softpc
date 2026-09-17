#include "lifecycle.h"
#include "abi.h"

#include <windows.h>

#if defined(_MSC_VER)
#define SOFTPC_CCPU_THREAD_LOCAL __declspec(thread)
#else
#define SOFTPC_CCPU_THREAD_LOCAL __thread
#endif

static SOFTPC_CCPU_THREAD_LOCAL unsigned long softpc_ccpu_frame_depth;
static SOFTPC_CCPU_THREAD_LOCAL softpc_ccpu_checkpoint_observer checkpoint_observer;
static SOFTPC_CCPU_THREAD_LOCAL void *checkpoint_context;
static volatile LONG softpc_ccpu_exit_requested;


void softpc_ccpu_lifecycle_enter(void)
{
    ++softpc_ccpu_frame_depth;
}

void softpc_ccpu_lifecycle_leave(void)
{
    if (softpc_ccpu_frame_depth != 0ul)
        --softpc_ccpu_frame_depth;
}

void softpc_ccpu_lifecycle_observe(softpc_ccpu_checkpoint_observer observer,
    void *context)
{
    checkpoint_observer = observer;
    checkpoint_context = context;
}

void softpc_ccpu_lifecycle_checkpoint(int halted)
{
    if (checkpoint_observer == NULL) return;
    checkpoint_observer(checkpoint_context, softpc_ccpu_frame_depth, halted);
    /* A stop issued while the observer was parked must unwind before another
     * guest instruction, just like the existing executor callback exit. */
    if (softpc_ccpu_lifecycle_exit_requested())
        softpc_ccpu_lifecycle_return_outer();
}

void softpc_ccpu_lifecycle_request_exit(void)
{
    InterlockedExchange(&softpc_ccpu_exit_requested, 1);
}

void softpc_ccpu_lifecycle_clear_exit(void)
{
    InterlockedExchange(&softpc_ccpu_exit_requested, 0);
}

void softpc_ccpu_lifecycle_clear_pending_interrupts(void)
{
    unsigned long *interrupt_map = softpc_ccpu_interrupt_map_address();
    if (interrupt_map != NULL) *interrupt_map = 0u;
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
