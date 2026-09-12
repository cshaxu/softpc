#include "lib/types/types_interface.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "lib/types/file_private.h"

static lib_status lib_native_file_open(const char *path, lib_bool readwrite,
    lib_native_file **out_file)
{
    struct flock lock = { 0 };
    lib_native_file *file = lib_allocate_zero(1u, sizeof(*file));

    if (file == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    file->stream = fopen(path,
        readwrite != LIB_FALSE ? "rb+" : "rb");
    if (file->stream == LIB_NULL) {
        lib_release(file);
        return LIB_STATUS_IO_ERROR;
    }
    lock.l_type = readwrite != LIB_FALSE ? F_WRLCK : F_RDLCK;
    lock.l_whence = SEEK_SET;
    if (fcntl(fileno(file->stream), F_SETLK, &lock) != 0) {
        (void)fclose(file->stream);
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
{ return fseeko(file->stream, (off_t)offset, SEEK_SET) == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR; }

lib_status lib_native_file_tell(lib_native_file *file, lib_i64 *out_offset)
{
    off_t offset = ftello(file->stream);
    if (offset < 0) return LIB_STATUS_IO_ERROR;
    *out_offset = (lib_i64)offset;
    return LIB_STATUS_OK;
}

lib_status lib_native_file_byte_count(lib_native_file *file, lib_i64 *out_byte_count)
{
    off_t offset = ftello(file->stream);
    off_t length;

    if (offset < 0 || fseeko(file->stream, 0, SEEK_END) != 0)
        return LIB_STATUS_IO_ERROR;
    length = ftello(file->stream);
    if (length < 0 || fseeko(file->stream, offset, SEEK_SET) != 0)
        return LIB_STATUS_IO_ERROR;
    *out_byte_count = (lib_i64)length;
    return LIB_STATUS_OK;
}
