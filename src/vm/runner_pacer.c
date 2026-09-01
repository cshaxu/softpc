#include "runner_pacer.h"

#include <windows.h>

static uint64_t softpc_vm_runner_counter(void)
{
    LARGE_INTEGER value;
    if (!QueryPerformanceCounter(&value)) return 0u;
    return (uint64_t)value.QuadPart;
}

void softpc_vm_runner_pacer_init(softpc_vm_runner_pacer *pacer)
{
    LARGE_INTEGER frequency;
    if (pacer == NULL) return;
    pacer->frequency = 0u;
    pacer->quantum_start = 0u;
    pacer->available = QueryPerformanceFrequency(&frequency) &&
        frequency.QuadPart > 0;
    if (pacer->available) {
        pacer->frequency = (uint64_t)frequency.QuadPart;
        pacer->quantum_start = softpc_vm_runner_counter();
    }
}

void softpc_vm_runner_pacer_wait(softpc_vm_runner_pacer *pacer)
{
    uint64_t now;
    uint64_t active;
    uint64_t deadline;

    if (pacer == NULL || !pacer->available) {
        Sleep(1u);
        return;
    }
    now = softpc_vm_runner_counter();
    if (now < pacer->quantum_start) {
        pacer->quantum_start = now;
        return;
    }
    active = now - pacer->quantum_start;
    if (active == 0u) {
        /* Never turn a fast CCPU return into a tight host spin. */
        Sleep(1u);
        pacer->quantum_start = softpc_vm_runner_counter();
        return;
    }
    deadline = now + active;
    for (;;) {
        now = softpc_vm_runner_counter();
        if (now >= deadline) break;
        /* Sleeping, rather than busy-yielding the final fraction, is the
           point of this host scheduler: it must remain RDP-friendly. */
        Sleep(1u);
    }
    pacer->quantum_start = softpc_vm_runner_counter();
}
