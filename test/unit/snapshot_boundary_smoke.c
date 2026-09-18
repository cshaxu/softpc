#include <assert.h>

#define base_clock_monotonic_counter snapshot_test_counter
#define softpc_platform_set_clock_running snapshot_test_timer
#include "../../src/vm/snapshot.c"

static lib_u64 now, frequency = 1000u;
static lib_status clock_status;
static int timer_ok = 1, stops, starts;

lib_status snapshot_test_counter(lib_u64 *units, lib_u64 *units_per_second)
{
    *units = now;
    *units_per_second = frequency;
    return clock_status;
}

int snapshot_test_timer(int running)
{
    if (running) ++starts; else ++stops;
    return timer_ok;
}

int main(void)
{
    softpc_snapshot snapshot = {0};
    const softpc_ccpu_entry halted = {1, 1u}, fetch = {0, 0u};
    assert(softpc_snapshot_begin(NULL) == LIB_STATUS_INVALID_ARGUMENT);
    assert(softpc_snapshot_finish(NULL) == LIB_STATUS_INVALID_ARGUMENT);
    assert(!softpc_snapshot_checkpoint(&snapshot, 1u, &fetch));
    assert(softpc_snapshot_begin(&snapshot) == LIB_STATUS_OK);
    assert(softpc_snapshot_begin(&snapshot) == LIB_STATUS_INVALID_STATE);
    now = 999u;
    assert(!softpc_snapshot_checkpoint(&snapshot, 2u, &halted));
    assert(stops == 0 && snapshot.phase == SOFTPC_SNAPSHOT_WAITING);
    assert(softpc_snapshot_checkpoint(&snapshot, 1u, &halted));
    assert(snapshot.phase == SOFTPC_SNAPSHOT_READY && snapshot.status == LIB_STATUS_OK);
    assert(stops == 1);
    now = 10000u; /* Capture I/O time is not the boundary-seeking deadline. */
    assert(!softpc_snapshot_checkpoint(&snapshot, 1u, &fetch));
    timer_ok = 0;
    assert(softpc_snapshot_finish(&snapshot) == LIB_STATUS_IO_ERROR);
    assert(snapshot.phase == SOFTPC_SNAPSHOT_READY);
    timer_ok = 1;
    assert(softpc_snapshot_finish(&snapshot) == LIB_STATUS_OK);
    assert(starts == 2 && snapshot.phase == SOFTPC_SNAPSHOT_IDLE);
    assert(softpc_snapshot_finish(&snapshot) == LIB_STATUS_OK && starts == 2);

    assert(softpc_snapshot_begin(&snapshot) == LIB_STATUS_OK);
    now += 1000u; /* At the deadline even an outer entry is too late. */
    assert(softpc_snapshot_checkpoint(&snapshot, 1u, &fetch));
    assert(snapshot.status == LIB_STATUS_LIMIT_EXCEEDED && stops == 1);
    assert(snapshot.phase == SOFTPC_SNAPSHOT_FAILED);
    assert(!softpc_snapshot_checkpoint(&snapshot, 1u, &fetch));
    assert(softpc_snapshot_finish(&snapshot) == LIB_STATUS_OK && starts == 2);

    assert(softpc_snapshot_begin(&snapshot) == LIB_STATUS_OK);
    now += 1000u;
    assert(softpc_snapshot_checkpoint(&snapshot, 2u, &halted));
    assert(snapshot.status == LIB_STATUS_LIMIT_EXCEEDED && stops == 1);
    assert(softpc_snapshot_finish(&snapshot) == LIB_STATUS_OK);

    clock_status = LIB_STATUS_IO_ERROR;
    assert(softpc_snapshot_begin(&snapshot) == LIB_STATUS_IO_ERROR);
    assert(snapshot.phase == SOFTPC_SNAPSHOT_IDLE);
    clock_status = LIB_STATUS_OK;
    frequency = 0u;
    assert(softpc_snapshot_begin(&snapshot) == LIB_STATUS_IO_ERROR);
    frequency = 1000u;
    assert(softpc_snapshot_begin(&snapshot) == LIB_STATUS_OK);
    clock_status = LIB_STATUS_IO_ERROR;
    assert(softpc_snapshot_checkpoint(&snapshot, 1u, &fetch));
    assert(snapshot.phase == SOFTPC_SNAPSHOT_FAILED && stops == 1);
    assert(softpc_snapshot_finish(&snapshot) == LIB_STATUS_OK);
    clock_status = LIB_STATUS_OK;
    assert(softpc_snapshot_begin(&snapshot) == LIB_STATUS_OK);
    ++frequency;
    assert(softpc_snapshot_checkpoint(&snapshot, 1u, &fetch));
    assert(snapshot.status == LIB_STATUS_IO_ERROR);
    assert(softpc_snapshot_finish(&snapshot) == LIB_STATUS_OK);

    assert(softpc_snapshot_begin(&snapshot) == LIB_STATUS_OK);
    timer_ok = 0;
    assert(softpc_snapshot_checkpoint(&snapshot, 1u, &fetch));
    assert(snapshot.phase == SOFTPC_SNAPSHOT_FAILED && snapshot.status == LIB_STATUS_IO_ERROR);
    assert(stops == 2);
    assert(softpc_snapshot_finish(&snapshot) == LIB_STATUS_OK && starts == 2);
    return 0;
}
