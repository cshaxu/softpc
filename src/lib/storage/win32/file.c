#include "lib/types/types_interface.h"

#include "lib/types/win32.h"
#include "lib/types/win32_runtime.h"

#include "lib/storage/file.h"

struct storage_file_platform {
    FILE *stream;
};

static lib_status storage_file_platform_open_writer(const char *path,
    const char *mode, storage_file_platform **out_file)
{
    FILE *stream;
    storage_file_platform *file;

    if (path == LIB_NULL || mode == LIB_NULL || out_file == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_file = LIB_NULL;
    stream = fopen(path, mode);
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

static lib_status storage_file_platform_open(const char *path, lib_bool readwrite,
    storage_file_platform **out_file)
{
    HANDLE handle;
    int descriptor;
    storage_file_platform *file;
    DWORD access_flags = readwrite != LIB_FALSE ?
        GENERIC_READ | GENERIC_WRITE : GENERIC_READ;
    DWORD share = readwrite != LIB_FALSE ? 0u : FILE_SHARE_READ;

    handle = CreateFileA(path, access_flags, share, LIB_NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, LIB_NULL);
    if (handle == INVALID_HANDLE_VALUE) return LIB_STATUS_IO_ERROR;
    descriptor = _open_osfhandle((intptr_t)handle,
        readwrite != LIB_FALSE ? _O_RDWR | _O_BINARY : _O_RDONLY | _O_BINARY);
    if (descriptor == -1) {
        (void)CloseHandle(handle);
        return LIB_STATUS_IO_ERROR;
    }
    file = lib_allocate_zero(1u, sizeof(*file));
    if (file == LIB_NULL) {
        (void)_close(descriptor);
        return LIB_STATUS_NO_MEMORY;
    }
    file->stream = _fdopen(descriptor,
        readwrite != LIB_FALSE ? "rb+" : "rb");
    if (file->stream == LIB_NULL) {
        (void)_close(descriptor);
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
    *out_byte_count = fread(bytes, 1u, byte_count, file->stream);
    return ferror(file->stream) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status storage_file_platform_write(storage_file_platform *file, const void *bytes,
    lib_size byte_count, lib_size *out_byte_count)
{
    if (file == LIB_NULL || out_byte_count == LIB_NULL ||
        (bytes == LIB_NULL && byte_count != 0u))
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_byte_count = fwrite(bytes, 1u, byte_count, file->stream);
    return ferror(file->stream) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status storage_file_platform_flush(storage_file_platform *file)
{
    if (file == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    return fflush(file->stream) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status storage_file_platform_close(storage_file_platform **file)
{
    storage_file_platform *value;
    int result;

    if (file == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    value = *file;
    *file = LIB_NULL;
    if (value == LIB_NULL) return LIB_STATUS_OK;
    result = fclose(value->stream);
    lib_release(value);
    return result == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

lib_status storage_file_platform_seek_absolute(storage_file_platform *file, lib_i64 offset)
{ return _fseeki64(file->stream, offset, SEEK_SET) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR; }

lib_status storage_file_platform_byte_count(storage_file_platform *file, lib_i64 *out_byte_count)
{
    lib_i64 offset = _ftelli64(file->stream);
    lib_i64 length;

    if (offset < 0 || _fseeki64(file->stream, 0, SEEK_END) != 0)
        return LIB_STATUS_IO_ERROR;
    length = _ftelli64(file->stream);
    if (length < 0 || _fseeki64(file->stream, offset, SEEK_SET) != 0)
        return LIB_STATUS_IO_ERROR;
    *out_byte_count = length;
    return LIB_STATUS_OK;
}
