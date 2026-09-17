#ifndef SOFTPC_CCPU_ARCHIVE_H
#define SOFTPC_CCPU_ARCHIVE_H

#include "snapshot.h"

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
    int valid;
} softpc_ccpu_archive;

void softpc_ccpu_archive_dispose(softpc_ccpu_archive *archive);
int softpc_ccpu_archive_capture(softpc_ccpu_archive *archive);
int softpc_ccpu_archive_restore(const softpc_ccpu_archive *archive);

#endif
