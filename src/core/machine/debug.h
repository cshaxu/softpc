#ifndef VM_DEBUG_H
#define VM_DEBUG_H

#include "core/machine/machine.h"
#include "x86/debug/protocol_interface.h"

/* Product driver owns this state; only its executor accesses it. */
typedef struct softpc_debug_state {
    x86_debug_execution_plan_kind kind;
    lib_u64 target_count;
    lib_u32 address, executed, stopped_address;
    lib_bool stop_pending, result_ready, skip_first;
    lib_bool in_instruction;
    struct { lib_bool enabled; lib_u32 address; } watches[3];
    x86_debug_observation observation;
} softpc_debug_state;

void softpc_debug_bind(softpc_debug_state *state);
lib_status softpc_machine_debug(softpc_machine *machine, softpc_debug_state *state,
    const x86_debug_request *request,
    x86_debug_response *result);

#endif
