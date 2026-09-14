#ifndef SOFTPC_HOST_MACHINE_DEBUG_H
#define SOFTPC_HOST_MACHINE_DEBUG_H

#include "host/machine.h"
#include "common/machine/machine_interface.h"

/* Product driver owns this state; only its executor accesses it. */
typedef struct softpc_debug_state {
    common_machine_debug_execution_plan_kind kind;
    lib_u64 target_count;
    lib_u32 address, executed, stopped_address;
    lib_bool stop_pending, result_ready, skip_first;
} softpc_debug_state;

void softpc_debug_bind(softpc_debug_state *state);
lib_status softpc_machine_debug(softpc_machine *machine, softpc_debug_state *state,
    const common_machine_debug_request *request,
    common_machine_debug_result *result);

#endif
