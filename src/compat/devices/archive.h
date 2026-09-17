#ifndef SOFTPC_DEVICE_ARCHIVE_H
#define SOFTPC_DEVICE_ARCHIVE_H

#include "../snapshot_stream.h"

/* Opaque private archive. Queue links, callback addresses and legacy
   controller structs stay inside the device implementation. */
typedef struct softpc_device_archive softpc_device_archive;

softpc_device_archive *softpc_device_archive_create(void);
void softpc_device_archive_dispose(softpc_device_archive *archive);
int softpc_device_archive_capture(softpc_device_archive *archive);
int softpc_device_archive_restore(softpc_device_archive *archive);
lib_status softpc_device_archive_write(const softpc_device_archive *archive,
    softpc_snapshot_bytes_write write, void *context);
lib_status softpc_device_archive_read(softpc_device_archive **out_archive,
    softpc_snapshot_bytes_read read, void *context);

#endif
