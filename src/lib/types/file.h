#ifndef LIB_TYPES_FILE_H
#define LIB_TYPES_FILE_H

#include "lib/types/types_interface.h"

typedef struct lib_native_file lib_native_file;

/* Raw C/OS file calls. Storage chooses which operation it needs and owns all
 * file policy, validation, ownership, and exact-transfer semantics. */
lib_status lib_native_file_open_readonly(const char *path,
    lib_native_file **out_file);
lib_status lib_native_file_open_readwrite(const char *path,
    lib_native_file **out_file);
lib_status lib_native_file_open_truncate(const char *path,
    lib_native_file **out_file);
lib_status lib_native_file_open_append(const char *path,
    lib_native_file **out_file);
lib_status lib_native_file_read(lib_native_file *file, void *bytes,
    lib_size byte_count, lib_size *out_byte_count);
lib_status lib_native_file_write(lib_native_file *file, const void *bytes,
    lib_size byte_count, lib_size *out_byte_count);
lib_status lib_native_file_flush(lib_native_file *file);
lib_status lib_native_file_seek_absolute(lib_native_file *file, lib_i64 offset);
lib_status lib_native_file_byte_count(lib_native_file *file,
    lib_i64 *out_byte_count);
lib_status lib_native_file_close(lib_native_file **file);

#endif
