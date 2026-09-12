#include "lib/types/file.h"
#include "lib/types/win32/scalar.h"
#include "lib/types/win32/sync.h"
#include "lib/types/types_interface.h"

#include "lib/types/win32/file.h"

#include "lib/storage/file.h"

struct storage_file_platform {
    lib_c_file *stream;
};

static lib_status storage_file_platform_open_writer(const char *path,
    const char *mode, storage_file_platform **out_file)
{
    lib_c_file *stream;
    storage_file_platform *file;

    if (path == LIB_NULL || mode == LIB_NULL || out_file == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_file = LIB_NULL;
    stream = lib_c_fopen(path, mode);
    if (stream == LIB_NULL) return LIB_STATUS_IO_ERROR;
    file = lib_allocate_zero(1u, sizeof(*file));
    if (file == LIB_NULL) {
        (void)lib_c_fclose(stream);
        return LIB_STATUS_NO_MEMORY;
    }
    file->stream = stream;
    *out_file = file;
    return LIB_STATUS_OK;
}

static lib_status storage_file_platform_open(const char *path, lib_bool readwrite,
    storage_file_platform **out_file)
{
    lib_win32_handle handle;
    int descriptor;
    storage_file_platform *file;
    lib_win32_dword access_flags = readwrite != LIB_FALSE ?
        LIB_WIN32_GENERIC_READ | LIB_WIN32_GENERIC_WRITE : LIB_WIN32_GENERIC_READ;
    lib_win32_dword share = readwrite != LIB_FALSE ? 0u : LIB_WIN32_FILE_SHARE_READ;

    handle = lib_win32_create_file_a(path, access_flags, share, LIB_NULL, LIB_WIN32_OPEN_EXISTING,
        LIB_WIN32_FILE_ATTRIBUTE_NORMAL, LIB_NULL);
    if (handle == LIB_WIN32_INVALID_HANDLE_VALUE) return LIB_STATUS_IO_ERROR;
    descriptor = lib_win32_open_osfhandle((lib_iptr)handle,
        readwrite != LIB_FALSE ? LIB_WIN32_O_RDWR | LIB_WIN32_O_BINARY : LIB_WIN32_O_RDONLY | LIB_WIN32_O_BINARY);
    if (descriptor == -1) {
        (void)lib_win32_close_handle(handle);
        return LIB_STATUS_IO_ERROR;
    }
    file = lib_allocate_zero(1u, sizeof(*file));
    if (file == LIB_NULL) {
        (void)lib_win32_close(descriptor);
        return LIB_STATUS_NO_MEMORY;
    }
    file->stream = lib_win32_fdopen(descriptor,
        readwrite != LIB_FALSE ? "rb+" : "rb");
    if (file->stream == LIB_NULL) {
        (void)lib_win32_close(descriptor);
        lib_release(file);
        return LIB_STATUS_IO_ERROR;
    }
    *out_file = file;
    return LIB_STATUS_OK;
}

lib_status storage_file_platform_open_readonly(const char *path,
    storage_file_platform **out_file)
{ return storage_file_platform_open(path, LIB_FALSE, out_file); }

lib_status storage_file_platform_open_readwrite(const char *path,
    storage_file_platform **out_file)
{ return storage_file_platform_open(path, LIB_TRUE, out_file); }

lib_status storage_file_platform_open_truncate(const char *path,
    storage_file_platform **out_file)
{ return storage_file_platform_open_writer(path, "wb", out_file); }

lib_status storage_file_platform_open_append(const char *path,
    storage_file_platform **out_file)
{ return storage_file_platform_open_writer(path, "ab", out_file); }

lib_status storage_file_platform_read(storage_file_platform *file, void *bytes,
    lib_size byte_count, lib_size *out_byte_count)
{
    if (file == LIB_NULL || out_byte_count == LIB_NULL ||
        (bytes == LIB_NULL && byte_count != 0u))
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_byte_count = lib_c_fread(bytes, 1u, byte_count, file->stream);
    return lib_c_ferror(file->stream) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status storage_file_platform_write(storage_file_platform *file, const void *bytes,
    lib_size byte_count, lib_size *out_byte_count)
{
    if (file == LIB_NULL || out_byte_count == LIB_NULL ||
        (bytes == LIB_NULL && byte_count != 0u))
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_byte_count = lib_c_fwrite(bytes, 1u, byte_count, file->stream);
    return lib_c_ferror(file->stream) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status storage_file_platform_flush(storage_file_platform *file)
{
    if (file == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    return lib_c_fflush(file->stream) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status storage_file_platform_close(storage_file_platform **file)
{
    storage_file_platform *value;
    int result;

    if (file == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    value = *file;
    *file = LIB_NULL;
    if (value == LIB_NULL) return LIB_STATUS_OK;
    result = lib_c_fclose(value->stream);
    lib_release(value);
    return result == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status storage_file_platform_seek_absolute(storage_file_platform *file, lib_i64 offset)
{ return lib_win32_fseeki64(file->stream, offset, LIB_SEEK_SET) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR; }

lib_status storage_file_platform_byte_count(storage_file_platform *file, lib_i64 *out_byte_count)
{
    lib_i64 offset = lib_win32_ftelli64(file->stream);
    lib_i64 length;

    if (offset < 0 || lib_win32_fseeki64(file->stream, 0, LIB_SEEK_END) != 0)
        return LIB_STATUS_IO_ERROR;
    length = lib_win32_ftelli64(file->stream);
    if (length < 0 || lib_win32_fseeki64(file->stream, offset, LIB_SEEK_SET) != 0)
        return LIB_STATUS_IO_ERROR;
    *out_byte_count = length;
    return LIB_STATUS_OK;
}
