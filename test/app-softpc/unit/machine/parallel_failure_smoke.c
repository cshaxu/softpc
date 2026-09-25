#include "lib/types/types_interface.h"

#include <assert.h>
#include <stdio.h>

static lib_size write_limit;
static int flush_result;
static unsigned write_count;

static lib_size fake_fwrite(const void *bytes, lib_size size, lib_size count,
    FILE *output)
{
    (void)bytes;
    assert(size == 1u && output == (FILE *)1);
    ++write_count;
    return write_limit < count ? write_limit : count;
}

static int fake_fflush(FILE *output)
{
    assert(output == (FILE *)1);
    return flush_result;
}

#define fwrite fake_fwrite
#define fflush fake_fflush
#include "compat/parallel.c"
#undef fflush
#undef fwrite

int main(void)
{
    HOST_LPT *lpt = &host_lpt[0];
    int index;

    lpt->buffer = lib_allocate(KBUFFER_SIZE);
    assert(lpt->buffer != NULL);
    for (index = 0; index < HIGH_WATER; ++index)
        lpt->buffer[index] = (byte)index;
    lpt->output = (FILE *)1;
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
    assert(write_count == 2u && lpt->bytes_in_buffer == 1);
    assert(lpt->buffer[0] == 0xa5u);

    lpt->bytes_in_buffer = 1;
    write_limit = 1u;
    flush_result = EOF;
    assert(!host_print_doc(0));
    assert(lpt->bytes_in_buffer == 0);
    lpt->output = NULL;
    lpt->active = FALSE;
    lib_release(lpt->buffer);
    lpt->buffer = NULL;
    return 0;
}
