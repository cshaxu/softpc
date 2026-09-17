#ifndef SOFTPC_CCPU_ARCHIVE_H
#define SOFTPC_CCPU_ARCHIVE_H

#include "../snapshot_stream.h"
#include "snapshot.h"

typedef struct softpc_device_archive softpc_device_archive;

/* Internal canonical-stream callbacks. The eventual VM image composes this
 * core section with the separately owned device section; neither callback
 * carries a file, path, handle, pointer into guest state or product policy. */
/* Heap ownership is local to the standalone adapter.  The backing pointers
   never cross into the snapshot payload or the Common machine boundary. */
typedef struct softpc_ccpu_archive {
    softpc_ccpu_register_state registers;
    softpc_ccpu_execution_state execution;
    softpc_ccpu_debug_state debug;
    softpc_ccpu_tlb_state tlb;
    softpc_ccpu_fpu_state fpu;
    softpc_ccpu_sas_state sas;
    uint8_t *memory;
    uint8_t *page_types;
    uint8_t *tlb_page_index;
    softpc_device_archive *devices;
    int valid;
} softpc_ccpu_archive;

void softpc_ccpu_archive_dispose(softpc_ccpu_archive *archive);
int softpc_ccpu_archive_capture(softpc_ccpu_archive *archive);
int softpc_ccpu_archive_restore(const softpc_ccpu_archive *archive);
lib_status softpc_ccpu_archive_write_core(const softpc_ccpu_archive *archive,
    softpc_snapshot_bytes_write write, void *context);
lib_status softpc_ccpu_archive_read_core(softpc_ccpu_archive *archive,
    softpc_snapshot_bytes_read read, void *context);

#endif
