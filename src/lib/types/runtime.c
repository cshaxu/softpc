#include "lib/types/types_interface.h"

#include <stdlib.h>
#include <string.h>

void *lib_memory_set(void *destination, int value, lib_size byte_count)
{
    return memset(destination, value, byte_count);
}

void *lib_memory_copy(void *destination, const void *source, lib_size byte_count)
{
    return memcpy(destination, source, byte_count);
}

int lib_memory_compare(const void *left, const void *right, lib_size byte_count)
{
    return memcmp(left, right, byte_count);
}

const void *lib_memory_find(const void *bytes, int value, lib_size byte_count)
{
    return memchr(bytes, value, byte_count);
}

lib_size lib_text_length(const char *text)
{
    return strlen(text);
}

void *lib_allocate(lib_size byte_count)
{
    return malloc(byte_count);
}

void *lib_allocate_zero(lib_size count, lib_size byte_count)
{
    return calloc(count, byte_count);
}

void lib_release(void *memory)
{
    free(memory);
}
