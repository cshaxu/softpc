#include "lib/types/types_interface.h"

#include "lib/storage/file.h"
#include "lib/storage/file_interface.h"

struct lib_storage_file_writer { lib_storage_file *file; };

static lib_status lib_storage_file_open(const char *path,
    lib_status (*open_native)(const char *, lib_native_file **),
    lib_storage_file **out_file)
{
    lib_storage_file *file;

    if (path == LIB_NULL || open_native == LIB_NULL || out_file == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_file = LIB_NULL;
    file = (lib_storage_file *)lib_allocate_zero(1u, sizeof(*file));
    if (file == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    if (open_native(path, &file->native) != LIB_STATUS_OK) {
        lib_release(file);
        return LIB_STATUS_IO_ERROR;
    }
    *out_file = file;
    return LIB_STATUS_OK;
}

lib_status lib_storage_file_open_readonly(const char *path,
    lib_storage_file **out_file)
{ return lib_storage_file_open(path, lib_native_file_open_readonly, out_file); }

lib_status lib_storage_file_open_readwrite(const char *path,
    lib_storage_file **out_file)
{ return lib_storage_file_open(path, lib_native_file_open_readwrite, out_file); }

lib_status lib_storage_file_open_truncate(const char *path,
    lib_storage_file **out_file)
{ return lib_storage_file_open(path, lib_native_file_open_truncate, out_file); }

lib_status lib_storage_file_open_append(const char *path,
    lib_storage_file **out_file)
{ return lib_storage_file_open(path, lib_native_file_open_append, out_file); }

lib_status lib_storage_file_read_exact(lib_storage_file *file, void *bytes,
    lib_size byte_count)
{
    lib_size transferred;

    if (file == LIB_NULL || (bytes == LIB_NULL && byte_count != 0u))
        return LIB_STATUS_INVALID_ARGUMENT;
    return lib_native_file_read(file->native, bytes, byte_count, &transferred) !=
            LIB_STATUS_OK || transferred != byte_count ? LIB_STATUS_IO_ERROR :
            LIB_STATUS_OK;
}

lib_status lib_storage_file_write_exact(lib_storage_file *file,
    const void *bytes, lib_size byte_count)
{
    lib_size transferred;

    if (file == LIB_NULL || (bytes == LIB_NULL && byte_count != 0u))
        return LIB_STATUS_INVALID_ARGUMENT;
    return lib_native_file_write(file->native, bytes, byte_count, &transferred) !=
            LIB_STATUS_OK || transferred != byte_count ? LIB_STATUS_IO_ERROR :
            LIB_STATUS_OK;
}

lib_status lib_storage_file_flush(lib_storage_file *file)
{ return file == LIB_NULL ? LIB_STATUS_INVALID_ARGUMENT :
    lib_native_file_flush(file->native); }

lib_status lib_storage_file_seek_absolute(lib_storage_file *file,
    lib_i64 offset)
{ return file == LIB_NULL || offset < 0 ? LIB_STATUS_INVALID_ARGUMENT :
    lib_native_file_seek_absolute(file->native, offset); }

lib_status lib_storage_file_byte_count(lib_storage_file *file,
    lib_i64 *out_byte_count)
{ return file == LIB_NULL || out_byte_count == LIB_NULL ? LIB_STATUS_INVALID_ARGUMENT :
    lib_native_file_byte_count(file->native, out_byte_count); }

lib_status lib_storage_file_close(lib_storage_file **file)
{
    lib_storage_file *value;
    lib_status status;

    if (file == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    value = *file;
    *file = LIB_NULL;
    if (value == LIB_NULL) return LIB_STATUS_OK;
    status = lib_native_file_close(&value->native);
    lib_release(value);
    return status;
}

lib_status lib_storage_file_read_owned(const char *path, lib_size maximum,
    void **out_bytes, lib_size *out_byte_count)
{
    lib_storage_file *file = LIB_NULL;
    lib_i64 length;
    void *bytes = LIB_NULL;

    if (path == LIB_NULL || out_bytes == LIB_NULL || out_byte_count == LIB_NULL) {
        return LIB_STATUS_INVALID_ARGUMENT;
    }
    *out_bytes = LIB_NULL;
    *out_byte_count = 0u;
    if (lib_storage_file_open_readonly(path, &file) != LIB_STATUS_OK ||
        lib_storage_file_byte_count(file, &length) != LIB_STATUS_OK || length < 0 ||
        (lib_u64)length > maximum ||
        (bytes = lib_allocate((lib_size)length == 0u ? 1u : (lib_size)length)) == LIB_NULL ||
        lib_storage_file_read_exact(file, bytes, (lib_size)length) != LIB_STATUS_OK) {
        (void)lib_storage_file_close(&file);
        lib_release(bytes);
        return LIB_STATUS_IO_ERROR;
    }
    if (lib_storage_file_close(&file) != LIB_STATUS_OK) {
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
    if ((mode == LIB_STORAGE_FILE_WRITER_TRUNCATE ?
            lib_storage_file_open_truncate(path, &writer->file) :
            lib_storage_file_open_append(path, &writer->file)) != LIB_STATUS_OK) {
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
    return lib_storage_file_write_exact(writer->file, bytes, byte_count);
}

lib_status lib_storage_file_writer_close(lib_storage_file_writer *writer)
{
    if (writer == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    {
        lib_status status = lib_storage_file_close(&writer->file);
        lib_release(writer);
        return status;
    }
}
