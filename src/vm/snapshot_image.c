#include "snapshot.h"

void softpc_snapshot_image_dispose(softpc_snapshot_image *image)
{
    if (image == NULL) return;
    softpc_ccpu_archive_dispose(&image->ccpu);
    image->entry = (softpc_ccpu_entry){0};
}

lib_status softpc_snapshot_image_capture(softpc_snapshot_image *image,
    const softpc_ccpu_entry *entry)
{
    if (image == NULL || entry == NULL ||
        (entry->halted != 0 && entry->halted != 1) || entry->trap > 1ul ||
        (!entry->halted && entry->trap != 0ul))
        return LIB_STATUS_INVALID_ARGUMENT;
    if (!softpc_ccpu_archive_capture(&image->ccpu)) return LIB_STATUS_IO_ERROR;
    image->entry = *entry;
    return LIB_STATUS_OK;
}

lib_status softpc_snapshot_image_restore(const softpc_snapshot_image *image,
    softpc_ccpu_entry *entry)
{
    if (image == NULL || entry == NULL || !image->ccpu.valid)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (!softpc_ccpu_archive_restore(&image->ccpu)) return LIB_STATUS_IO_ERROR;
    *entry = image->entry;
    return LIB_STATUS_OK;
}
