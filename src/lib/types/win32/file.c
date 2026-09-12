#include "lib/types/types_interface.h"

#include <fcntl.h>
#include <io.h>
#include <windows.h>

#include "lib/types/file_private.h"

static lib_status lib_native_file_open(const char *path, lib_bool readwrite,
    lib_native_file **out_file)
{
    HANDLE handle;
    int descriptor;
    lib_native_file *file;
    DWORD native_access = readwrite != LIB_FALSE ?
        GENERIC_READ | GENERIC_WRITE : GENERIC_READ;
    DWORD share = readwrite != LIB_FALSE ? 0u : FILE_SHARE_READ;

    handle = CreateFileA(path, native_access, share, LIB_NULL, OPEN_EXISTING,
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

lib_status lib_native_file_open_readonly(const char *path,
    lib_native_file **out_file)
{ return lib_native_file_open(path, LIB_FALSE, out_file); }

lib_status lib_native_file_open_readwrite(const char *path,
    lib_native_file **out_file)
{ return lib_native_file_open(path, LIB_TRUE, out_file); }

lib_status lib_native_file_seek_absolute(lib_native_file *file, lib_i64 offset)
{ return _fseeki64(file->stream, offset, SEEK_SET) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR; }

lib_status lib_native_file_tell(lib_native_file *file, lib_i64 *out_offset)
{
    lib_i64 offset = _ftelli64(file->stream);
    if (offset < 0) return LIB_STATUS_IO_ERROR;
    *out_offset = offset;
    return LIB_STATUS_OK;
}

lib_status lib_native_file_byte_count(lib_native_file *file, lib_i64 *out_byte_count)
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
