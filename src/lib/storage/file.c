#include "lib/types/types_interface.h"

#include "lib/storage/file_interface.h"

struct lib_storage_file_writer { lib_file *file; };

lib_status lib_storage_file_read_owned(const char *path, lib_size maximum,
    void **out_bytes, lib_size *out_byte_count)
{
    lib_file *file = LIB_NULL;
    lib_i64 length;
    void *bytes = LIB_NULL;

    if (path == LIB_NULL || out_bytes == LIB_NULL || out_byte_count == LIB_NULL) {
        return LIB_STATUS_INVALID_ARGUMENT;
    }
    *out_bytes = LIB_NULL;
    *out_byte_count = 0u;
    if (lib_file_open(path, LIB_FILE_ACCESS_READONLY, &file) != LIB_STATUS_OK ||
        lib_file_byte_count(file, &length) != LIB_STATUS_OK || length < 0 ||
        (lib_u64)length > maximum ||
        (bytes = lib_allocate((lib_size)length == 0u ? 1u : (lib_size)length)) == LIB_NULL ||
        lib_file_read_exact(file, bytes, (lib_size)length) != LIB_STATUS_OK) {
        (void)lib_file_close(&file);
        lib_release(bytes);
        return LIB_STATUS_IO_ERROR;
    }
    if (lib_file_close(&file) != LIB_STATUS_OK) {
        lib_release(bytes);
        return LIB_STATUS_IO_ERROR;
    }
    *out_bytes = bytes;
    *out_byte_count = (lib_size)length;
    return LIB_STATUS_OK;
}

lib_status lib_storage_file_writer_open(const char *path,
    lib_storage_file_writer_mode mode,
    lib_storage_file_writer **out_writer)
{
    lib_storage_file_writer *writer;

    if (out_writer == LIB_NULL || path == LIB_NULL ||
        mode < LIB_STORAGE_FILE_WRITER_TRUNCATE ||
        mode > LIB_STORAGE_FILE_WRITER_APPEND) return LIB_STATUS_INVALID_ARGUMENT;
    *out_writer = LIB_NULL;
    writer = lib_allocate(sizeof(*writer));
    if (writer == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    if (lib_file_open_writer(path,
            mode == LIB_STORAGE_FILE_WRITER_TRUNCATE ? LIB_FILE_WRITE_TRUNCATE :
            LIB_FILE_WRITE_APPEND, &writer->file) != LIB_STATUS_OK) {
        lib_release(writer);
        return LIB_STATUS_IO_ERROR;
    }
    *out_writer = writer;
    return LIB_STATUS_OK;
}

lib_status lib_storage_file_writer_write(lib_storage_file_writer *writer,
    const void *bytes, lib_size byte_count)
{
    if (writer == LIB_NULL || (bytes == LIB_NULL && byte_count != 0u))
        return LIB_STATUS_INVALID_ARGUMENT;
    return lib_file_write_exact(writer->file, bytes, byte_count);
}

lib_status lib_storage_file_writer_close(lib_storage_file_writer *writer)
{
    if (writer == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    {
        lib_status status = lib_file_close(&writer->file);
        lib_release(writer);
        return status;
    }
}
