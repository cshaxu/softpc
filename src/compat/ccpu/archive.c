#include "archive.h"
#include "../devices/archive.h"

#include <stdlib.h>
#include <string.h>

static int softpc_ccpu_archive_allocate(softpc_ccpu_archive *archive,
    uint32_t memory_bytes, uint32_t page_type_bytes)
{
    uint8_t *memory;
    uint8_t *page_types;
    uint8_t *tlb_page_index;

    if (archive->memory != NULL && archive->sas.memory_bytes == memory_bytes &&
        archive->sas.page_type_bytes == page_type_bytes)
        return 1;
    memory = malloc(memory_bytes);
    page_types = malloc(page_type_bytes);
    tlb_page_index = malloc(SOFTPC_CCPU_FAST_TLB_PAGE_COUNT);
    if (memory == NULL || page_types == NULL || tlb_page_index == NULL) {
        free(memory);
        free(page_types);
        free(tlb_page_index);
        return 0;
    }
    free(archive->memory);
    free(archive->page_types);
    free(archive->tlb_page_index);
    softpc_device_archive_dispose(archive->devices);
    archive->devices = NULL;
    archive->memory = memory;
    archive->page_types = page_types;
    archive->tlb_page_index = tlb_page_index;
    return 1;
}

void softpc_ccpu_archive_dispose(softpc_ccpu_archive *archive)
{
    if (archive == NULL) return;
    free(archive->memory);
    free(archive->page_types);
    free(archive->tlb_page_index);
    softpc_device_archive_dispose(archive->devices);
    *archive = (softpc_ccpu_archive){0};
}

int softpc_ccpu_archive_capture(softpc_ccpu_archive *archive)
{
    softpc_ccpu_sas_state sas;

    if (archive == NULL) return 0;
    archive->valid = 0;
    memset(&sas, 0, sizeof(sas));
    softpc_ccpu_snapshot_capture_sas(&sas, NULL, 0u, NULL, 0u);
    if (sas.memory_bytes == 0u || sas.page_type_bytes == 0u ||
        !softpc_ccpu_archive_allocate(archive, sas.memory_bytes,
            sas.page_type_bytes))
        return 0;
    softpc_ccpu_snapshot_capture_sas(&archive->sas, archive->memory,
        sas.memory_bytes, archive->page_types, sas.page_type_bytes);
    if (archive->sas.memory_bytes != sas.memory_bytes ||
        archive->sas.page_type_bytes != sas.page_type_bytes)
        return 0;
    softpc_ccpu_snapshot_capture_registers(&archive->registers);
    softpc_ccpu_snapshot_capture_execution(&archive->execution);
    softpc_ccpu_snapshot_capture_debug(&archive->debug);
    softpc_ccpu_snapshot_capture_tlb(&archive->tlb, archive->tlb_page_index,
        SOFTPC_CCPU_FAST_TLB_PAGE_COUNT);
    softpc_ccpu_snapshot_capture_fpu(&archive->fpu);
    if (archive->devices == NULL)
        archive->devices = softpc_device_archive_create();
    if (archive->devices == NULL ||
        !softpc_device_archive_capture(archive->devices)) return 0;
    archive->valid = 1;
    return 1;
}

int softpc_ccpu_archive_restore(const softpc_ccpu_archive *archive)
{
    if (archive == NULL || !archive->valid || archive->memory == NULL ||
        archive->page_types == NULL || archive->tlb_page_index == NULL ||
        !softpc_ccpu_snapshot_restore_sas(&archive->sas, archive->memory,
            archive->sas.memory_bytes, archive->page_types,
            archive->sas.page_type_bytes))
        return 0;
    softpc_ccpu_snapshot_restore_registers(&archive->registers);
    softpc_ccpu_snapshot_restore_execution(&archive->execution);
    if (!softpc_ccpu_snapshot_restore_debug(&archive->debug) ||
        !softpc_ccpu_snapshot_restore_tlb(&archive->tlb,
            archive->tlb_page_index, SOFTPC_CCPU_FAST_TLB_PAGE_COUNT))
        return 0;
    softpc_ccpu_snapshot_restore_fpu(&archive->fpu);
    return softpc_device_archive_restore(archive->devices);
}
