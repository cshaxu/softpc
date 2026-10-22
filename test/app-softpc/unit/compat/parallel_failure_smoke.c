#include "lib/types/types_interface.h"
#include "lib/storage/file_interface.h"

#ifdef NDEBUG
#error Parallel failure verification requires live assertions
#endif
#include <assert.h>
#include <stdio.h>

static lib_size write_limit;
static int flush_result;
static unsigned write_count;
static unsigned flush_count;
static lib_bool write_error;

static lib_status fake_write(lib_storage_file_writer *output,
    const void *bytes, lib_size count, lib_size *written)
{
    (void)bytes;
    assert(output == (lib_storage_file_writer *)1);
    ++write_count;
    *written = write_limit < count ? write_limit : count;
    return *written == count && !write_error ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

static lib_status fake_flush(lib_storage_file_writer *output)
{
    assert(output == (lib_storage_file_writer *)1);
    ++flush_count;
    return flush_result == 0 ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

#define lib_storage_file_writer_write_some fake_write
#define lib_storage_file_writer_flush fake_flush
#include "compat/parallel.c"
#undef lib_storage_file_writer_flush
#undef lib_storage_file_writer_write_some

int main(void)
{
    HOST_LPT *lpt = &host_lpt[0];
    int index;

    lpt->buffer = lib_allocate(KBUFFER_SIZE);
    assert(lpt->buffer != NULL);
    for (index = 0; index < HIGH_WATER; ++index)
        lpt->buffer[index] = (byte)index;
    lpt->output = (lib_storage_file_writer *)1;
    lpt->active = TRUE;
    lpt->bytes_in_buffer = HIGH_WATER;
    lpt->flush_threshold = HIGH_WATER;
    write_limit = HIGH_WATER - 20u;
    flush_result = 0;
    write_count = 0u;

    assert(!host_print_byte(0, 0xa5u));
    assert(write_count == 1u && lpt->bytes_in_buffer == 20);
    assert(lpt->buffer[0] == (byte)(HIGH_WATER - 20));

    write_limit = HIGH_WATER;
    assert(host_print_byte(0, 0xa5u));
    assert(write_count == 1u && lpt->bytes_in_buffer == 21);
    for (index = 0; index < 20; ++index)
        assert(lpt->buffer[index] == (byte)(HIGH_WATER - 20 + index));
    assert(lpt->buffer[20] == 0xa5u);
    assert(host_print_doc(0));
    assert(write_count == 2u && lpt->bytes_in_buffer == 0);

    lpt->bytes_in_buffer = 1;
    write_limit = 1u;
    flush_result = EOF;
    assert(!host_print_doc(0));
    assert(lpt->bytes_in_buffer == 0);
    /* An accepted prefix remains accepted even if the stream reports error.
       Full acceptance must still reach the existing flush, never be replayed. */
    lpt->bytes_in_buffer = 1;
    flush_result = 0;
    flush_count = 0;
    write_error = LIB_TRUE;
    assert(!host_print_doc(0));
    assert(lpt->bytes_in_buffer == 0 && flush_count == 1u);
    lpt->output = NULL;
    lpt->active = FALSE;
    lib_release(lpt->buffer);
    lpt->buffer = NULL;
    return 0;
}
