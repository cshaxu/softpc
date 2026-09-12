#ifndef LIB_TYPES_FILE_NATIVE_H
#define LIB_TYPES_FILE_NATIVE_H

#include "lib/types/types_interface.h"

struct lib_file;

lib_status lib_file_native_open(const char *path, lib_file_access access,
    struct lib_file **out_file);
lib_status lib_file_native_seek_absolute(struct lib_file *file, lib_i64 offset);
lib_status lib_file_native_tell(struct lib_file *file, lib_i64 *out_offset);
lib_status lib_file_native_byte_count(struct lib_file *file,
    lib_i64 *out_byte_count);

#endif
