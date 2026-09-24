#ifndef SOFTPC_SNAPSHOT_H
#define SOFTPC_SNAPSHOT_H

#include "lib/types/types_interface.h"
#include "compat/ccpu/lifecycle.h"
#include "compat/ccpu/archive.h"
#include "compat/media_snapshot.h"

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
} softpc_snapshot;

/* VM-private state image for S4 roundtrip proof.  Later sections encode these
   copied values; this type is deliberately not a public Common payload. */
typedef struct softpc_snapshot_image {
    softpc_ccpu_archive ccpu;
    softpc_media_archive *media;
    softpc_ccpu_entry entry;
} softpc_snapshot_image;

lib_status softpc_snapshot_begin(softpc_snapshot *snapshot);
/* True exactly once on reaching READY/FAILED. Never unwinds the CPU. */
lib_bool softpc_snapshot_checkpoint(softpc_snapshot *snapshot,
    unsigned long depth, const softpc_ccpu_entry *entry);
/* On clock restart failure retains READY: the caller must not resume CPU. */
lib_status softpc_snapshot_finish(softpc_snapshot *snapshot);
void softpc_snapshot_image_dispose(softpc_snapshot_image *image);
lib_status softpc_snapshot_image_capture(softpc_snapshot_image *image,
    const softpc_ccpu_entry *entry);
lib_status softpc_snapshot_image_restore(softpc_snapshot_image *image,
    softpc_ccpu_entry *entry);
/* VM-private canonical image container. It composes the independently owned
   core and device streams; paths/files remain outside this boundary. */
lib_status softpc_snapshot_image_write(const softpc_snapshot_image *image,
    softpc_snapshot_bytes_write write, void *context);
lib_status softpc_snapshot_image_read(softpc_snapshot_image *image,
    lib_u32 expected_memory_bytes, softpc_snapshot_bytes_read read,
    void *context);

#endif
