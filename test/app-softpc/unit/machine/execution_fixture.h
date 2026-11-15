#ifndef SOFTPC_TEST_EXECUTION_FIXTURE_H
#define SOFTPC_TEST_EXECUTION_FIXTURE_H

#include "machine/debug.h"
#include <assert.h>

typedef struct softpc_test_execution {
    softpc_machine *machine;
    softpc_debug_state debug;
} softpc_test_execution;

static void softpc_test_execution_event(void *context)
{
    softpc_test_execution *execution = context;
    x86_debug_response result;
    const x86_debug_request request = { .operation = X86_DEBUG_GET_EXECUTION_RESULT };
    assert(softpc_machine_debug(execution->machine, &execution->debug,
        &request, &result) == LIB_STATUS_OK);
    if (result.enabled) softpc_machine_request_stop(execution->machine);
}

static inline softpc_machine_result softpc_test_execute(softpc_machine *machine,
    x86_debug_execution_plan_kind kind, lib_u32 value)
{
    softpc_test_execution execution = { .machine = machine };
    x86_debug_request request = { .operation = X86_DEBUG_SET_EXECUTION_PLAN,
        .execution_kind = kind, .instruction_count = value, .address = value };
    x86_debug_response response;
    softpc_machine_result result;
    assert(softpc_machine_debug(machine, &execution.debug, &request, &response) == LIB_STATUS_OK);
    softpc_debug_bind(&execution.debug);
    softpc_machine_set_executor_callback(machine, softpc_test_execution_event, &execution);
    result = softpc_machine_run(machine);
    softpc_machine_set_executor_callback(machine, NULL, NULL);
    softpc_debug_bind(NULL);
    if (result == SOFTPC_MACHINE_OK) {
        request.operation = X86_DEBUG_GET_EXECUTION_RESULT;
        assert(softpc_machine_debug(machine, &execution.debug, &request, &response) == LIB_STATUS_OK);
        assert(response.enabled);
        if (kind == X86_DEBUG_EXECUTION_TRACE) assert(response.value == value);
    }
    return result;
}

#endif
