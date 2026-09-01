#ifndef SOFTPC_VM_RUNNER_PACER_H
#define SOFTPC_VM_RUNNER_PACER_H

#include <stdint.h>

/*
 * Standalone host courtesy scheduler.  This is deliberately not a guest
 * clock: original SoftPC timer, PIT and quick-event state remain in Core.
 * It simply gives the desktop a bounded share between executor quanta.
 */
typedef struct softpc_vm_runner_pacer {
    uint64_t frequency;
    uint64_t quantum_start;
    int available;
} softpc_vm_runner_pacer;

void softpc_vm_runner_pacer_init(softpc_vm_runner_pacer *pacer);
void softpc_vm_runner_pacer_wait(softpc_vm_runner_pacer *pacer);

#endif
