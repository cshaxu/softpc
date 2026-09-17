#ifndef SOFTPC_SNAPSHOT_H
#define SOFTPC_SNAPSHOT_H

#include "lib/types/types_interface.h"
#include "compat/ccpu/lifecycle.h"

/* Executor-owned operation state, not a second machine lifecycle. */
typedef enum softpc_snapshot_phase {
    SOFTPC_SNAPSHOT_IDLE,
    SOFTPC_SNAPSHOT_WAITING,
    SOFTPC_SNAPSHOT_READY,
    SOFTPC_SNAPSHOT_FAILED
} softpc_snapshot_phase;

typedef struct softpc_snapshot {
    softpc_snapshot_phase phase;
    lib_status status;
    lib_u64 started, frequency;
    softpc_ccpu_entry entry;
} softpc_snapshot;

lib_status softpc_snapshot_begin(softpc_snapshot *snapshot);
/* True exactly once on reaching READY/FAILED. Never unwinds the CPU. */
lib_bool softpc_snapshot_checkpoint(softpc_snapshot *snapshot,
    unsigned long depth, const softpc_ccpu_entry *entry);
/* On clock restart failure retains READY: the caller must not resume CPU. */
lib_status softpc_snapshot_finish(softpc_snapshot *snapshot);

#endif
