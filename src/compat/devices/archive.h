#ifndef SOFTPC_DEVICE_ARCHIVE_H
#define SOFTPC_DEVICE_ARCHIVE_H

/* Opaque private archive. Queue links, callback addresses and legacy
   controller structs stay inside the device implementation. */
typedef struct softpc_device_archive softpc_device_archive;

softpc_device_archive *softpc_device_archive_create(void);
void softpc_device_archive_dispose(softpc_device_archive *archive);
int softpc_device_archive_capture(softpc_device_archive *archive);
int softpc_device_archive_restore(softpc_device_archive *archive);

#endif
