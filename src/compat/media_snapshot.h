#ifndef SOFTPC_MEDIA_SNAPSHOT_H
#define SOFTPC_MEDIA_SNAPSHOT_H

#include "snapshot_stream.h"
#include "lib/storage/medium_interface.h"

/* Borrowed only during an executor-owned media operation. */
typedef struct softpc_media_view {
    lib_storage_medium *medium;
    const char *path;
    lib_storage_medium_mode mode;
    lib_u32 cylinder;
} softpc_media_view;

enum { SOFTPC_MEDIA_ARCHIVE_PATH_MAX = 1024u };

void softpc_floppy_media_view(unsigned slot, softpc_media_view *view);
void softpc_hdd_media_view(unsigned slot, softpc_media_view *view);
lib_status softpc_floppy_media_restore(unsigned slot,
    const char *path, lib_storage_medium_mode mode,
    lib_storage_medium **replacement, lib_u32 cylinder);
lib_status softpc_hdd_media_restore(unsigned slot,
    const char *path, lib_storage_medium_mode mode,
    lib_storage_medium **replacement);

typedef struct softpc_media_archive softpc_media_archive;
void softpc_media_archive_dispose(softpc_media_archive **archive);
lib_status softpc_media_archive_capture(softpc_media_archive **archive);
lib_status softpc_media_archive_write(const softpc_media_archive *archive,
    softpc_snapshot_bytes_write write, void *context);
lib_status softpc_media_archive_read(softpc_media_archive **archive,
    softpc_snapshot_bytes_read read, void *context);
/* Floppy restores its copied path/mode. Fixed disk restores its copied path,
   but maps its saved mode through the current product hard-disk policy. */
lib_status softpc_media_archive_prepare(softpc_media_archive *archive,
    lib_storage_medium_mode hard_disk_mode);
lib_status softpc_media_archive_restore(softpc_media_archive *archive);

#endif
