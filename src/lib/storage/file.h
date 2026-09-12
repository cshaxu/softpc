#ifndef LIB_STORAGE_FILE_H
#define LIB_STORAGE_FILE_H

#include "lib/types/types_interface.h"

typedef struct storage_file_platform storage_file_platform;

typedef struct lib_storage_file {
    storage_file_platform *platform;
} lib_storage_file;

/* The selected storage platform source owns the native handle and exposes
 * this single uniform internal contract to storage/file.c. */
lib_status storage_file_platform_open_readonly(const char *path,
    storage_file_platform **out_file);
lib_status storage_file_platform_open_readwrite(const char *path,
    storage_file_platform **out_file);
lib_status storage_file_platform_open_truncate(const char *path,
    storage_file_platform **out_file);
lib_status storage_file_platform_open_append(const char *path,
    storage_file_platform **out_file);
lib_status storage_file_platform_read(storage_file_platform *file, void *bytes,
    lib_size byte_count, lib_size *out_byte_count);
lib_status storage_file_platform_write(storage_file_platform *file,
    const void *bytes, lib_size byte_count, lib_size *out_byte_count);
lib_status storage_file_platform_flush(storage_file_platform *file);
lib_status storage_file_platform_seek_absolute(storage_file_platform *file,
    lib_i64 offset);
lib_status storage_file_platform_byte_count(storage_file_platform *file,
    lib_i64 *out_byte_count);
lib_status storage_file_platform_close(storage_file_platform **file);

lib_status lib_storage_file_open_readonly(const char *path,
    lib_storage_file **out_file);
lib_status lib_storage_file_open_readwrite(const char *path,
    lib_storage_file **out_file);
lib_status lib_storage_file_open_truncate(const char *path,
    lib_storage_file **out_file);
lib_status lib_storage_file_open_append(const char *path,
    lib_storage_file **out_file);
lib_status lib_storage_file_read_exact(lib_storage_file *file, void *bytes,
    lib_size byte_count);
lib_status lib_storage_file_write_exact(lib_storage_file *file,
    const void *bytes, lib_size byte_count);
lib_status lib_storage_file_flush(lib_storage_file *file);
lib_status lib_storage_file_seek_absolute(lib_storage_file *file,
    lib_i64 offset);
lib_status lib_storage_file_byte_count(lib_storage_file *file,
    lib_i64 *out_byte_count);
lib_status lib_storage_file_close(lib_storage_file **file);

#endif
