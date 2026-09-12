#include "lib/storage/file_interface.h"
#include "test_cleanup.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    static const unsigned char payload[] = { 'A', 0u, 'B', '\n' };
    unsigned char actual[sizeof(payload)] = { 0u };
    const char *path = "softpc-storage-writer-binary-smoke.bin";
    lib_storage_file_writer *writer = LIB_NULL;
    FILE *file;

    assert(lib_storage_file_writer_open(path, LIB_STORAGE_FILE_WRITER_TRUNCATE,
        &writer) == LIB_STATUS_OK);
    assert(lib_storage_file_writer_write(writer, payload, sizeof(payload)) ==
        LIB_STATUS_OK);
    assert(lib_storage_file_writer_close(writer) == LIB_STATUS_OK);
    file = fopen(path, "rb");
    assert(file != NULL);
    assert(fread(actual, 1u, sizeof(actual), file) == sizeof(actual));
    assert(fgetc(file) == EOF);
    assert(fclose(file) == 0);
    assert(lib_memory_compare(actual, payload, sizeof(payload)) == 0);
    assert(softpc_test_remove_image(path));
    return 0;
}
