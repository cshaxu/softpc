#ifndef LIB_STORAGE_FILE_H
#define LIB_STORAGE_FILE_H

#include "lib/types/file.h"

typedef struct lib_storage_file {
    lib_native_file *native;
} lib_storage_file;

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
