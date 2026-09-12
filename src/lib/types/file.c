#include "lib/types/types_interface.h"

#include "lib/types/file.h"
#include "lib/types/file_native.h"

lib_status lib_file_open(const char *path, lib_file_access access,
    lib_file **out_file)
{
    if (path == LIB_NULL || out_file == LIB_NULL ||
        (access != LIB_FILE_ACCESS_READONLY &&
         access != LIB_FILE_ACCESS_READWRITE)) return LIB_STATUS_INVALID_ARGUMENT;
    *out_file = LIB_NULL;
    return lib_file_native_open(path, access, out_file);
}

lib_status lib_file_open_writer(const char *path, lib_file_write_mode mode,
    lib_file **out_file)
{
    FILE *stream;
    lib_file *file;

    if (path == LIB_NULL || out_file == LIB_NULL ||
        (mode != LIB_FILE_WRITE_TRUNCATE && mode != LIB_FILE_WRITE_APPEND))
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_file = LIB_NULL;
    stream = fopen(path, mode == LIB_FILE_WRITE_TRUNCATE ? "wb" : "ab");
    if (stream == LIB_NULL) return LIB_STATUS_IO_ERROR;
    file = lib_allocate_zero(1u, sizeof(*file));
    if (file == LIB_NULL) {
        (void)fclose(stream);
        return LIB_STATUS_NO_MEMORY;
    }
    file->stream = stream;
    *out_file = file;
    return LIB_STATUS_OK;
}

lib_status lib_file_read_exact(lib_file *file, void *bytes, lib_size byte_count)
{
    if (file == LIB_NULL || (bytes == LIB_NULL && byte_count != 0u))
        return LIB_STATUS_INVALID_ARGUMENT;
    return byte_count == 0u || fread(bytes, 1u, byte_count, file->stream) == byte_count ?
        LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status lib_file_write_exact(lib_file *file, const void *bytes,
    lib_size byte_count)
{
    if (file == LIB_NULL || (bytes == LIB_NULL && byte_count != 0u))
        return LIB_STATUS_INVALID_ARGUMENT;
    return byte_count == 0u || fwrite(bytes, 1u, byte_count, file->stream) == byte_count ?
        LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status lib_file_flush(lib_file *file)
{
    if (file == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    return fflush(file->stream) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status lib_file_seek_absolute(lib_file *file, lib_i64 offset)
{
    if (file == LIB_NULL || offset < 0) return LIB_STATUS_INVALID_ARGUMENT;
    return lib_file_native_seek_absolute(file, offset);
}

lib_status lib_file_byte_count(lib_file *file, lib_i64 *out_byte_count)
{
    if (file == LIB_NULL || out_byte_count == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    return lib_file_native_byte_count(file, out_byte_count);
}

lib_status lib_file_close(lib_file **file)
{
    lib_file *value;
    int result;

    if (file == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    value = *file;
    *file = LIB_NULL;
    if (value == LIB_NULL) return LIB_STATUS_OK;
    result = fclose(value->stream);
    lib_release(value);
    return result == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}
