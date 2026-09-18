#include "snapshot.h"
#include "compat/platform.h"
#include "lib/base/clock_interface.h"

lib_status softpc_snapshot_begin(softpc_snapshot *snapshot)
{
    lib_status status;
    lib_u64 now, frequency;
    if (snapshot == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    if (snapshot->phase != SOFTPC_SNAPSHOT_IDLE) return LIB_STATUS_INVALID_STATE;
    status = base_clock_monotonic_counter(&now, &frequency);
    if (status != LIB_STATUS_OK) return status;
    if (frequency == 0u) return LIB_STATUS_IO_ERROR;
    snapshot->started = now;
    snapshot->frequency = frequency;
    snapshot->phase = SOFTPC_SNAPSHOT_WAITING;
    return LIB_STATUS_OK;
}

lib_bool softpc_snapshot_checkpoint(softpc_snapshot *snapshot,
    unsigned long depth, const softpc_ccpu_entry *entry)
{
    lib_u64 now, frequency;
    if (snapshot->phase != SOFTPC_SNAPSHOT_WAITING) return LIB_FALSE;
    snapshot->status = base_clock_monotonic_counter(&now, &frequency);
    if (snapshot->status == LIB_STATUS_OK) {
        if (frequency != snapshot->frequency)
            snapshot->status = LIB_STATUS_IO_ERROR;
        else if (now - snapshot->started >= frequency)
            snapshot->status = LIB_STATUS_LIMIT_EXCEEDED;
        else if (depth != 1u)
            return LIB_FALSE;
        else if (!softpc_platform_set_clock_running(0))
            snapshot->status = LIB_STATUS_IO_ERROR;
        else {
            snapshot->phase = SOFTPC_SNAPSHOT_READY;
            return LIB_TRUE;
        }
    }
    snapshot->phase = SOFTPC_SNAPSHOT_FAILED;
    return LIB_TRUE;
}

lib_status softpc_snapshot_finish(softpc_snapshot *snapshot)
{
    if (snapshot == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    if (snapshot->phase == SOFTPC_SNAPSHOT_READY &&
        !softpc_platform_set_clock_running(1)) return LIB_STATUS_IO_ERROR;
    *snapshot = (softpc_snapshot){0};
    return LIB_STATUS_OK;
}
