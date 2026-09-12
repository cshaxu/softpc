#ifndef LIB_TYPES_CLOCK_H
#define LIB_TYPES_CLOCK_H

#include "lib/types/types_interface.h"

/* Raw monotonic counter facts. Host owns the public clock contract and unit
 * conversion policy. */
#ifdef _WIN32
#include "lib/types/win32.h"
static inline lib_status lib_clock_counter(lib_u64 *out_units,
    lib_u64 *out_units_per_second)
{
    LARGE_INTEGER counter;
    LARGE_INTEGER frequency;

    if (out_units == LIB_NULL || out_units_per_second == LIB_NULL ||
        !QueryPerformanceCounter(&counter) ||
        !QueryPerformanceFrequency(&frequency) || counter.QuadPart < 0 ||
        frequency.QuadPart <= 0) return LIB_STATUS_IO_ERROR;
    *out_units = (lib_u64)counter.QuadPart;
    *out_units_per_second = (lib_u64)frequency.QuadPart;
    return LIB_STATUS_OK;
}
#else
#include "lib/types/posix.h"
static inline lib_status lib_clock_counter(lib_u64 *out_units,
    lib_u64 *out_units_per_second)
{
    struct timespec value;

    if (out_units == LIB_NULL || out_units_per_second == LIB_NULL ||
        clock_gettime(CLOCK_MONOTONIC, &value) != 0 || value.tv_sec < 0 ||
        value.tv_nsec < 0) return LIB_STATUS_IO_ERROR;
    *out_units = (lib_u64)value.tv_sec * 1000000000u + (lib_u64)value.tv_nsec;
    *out_units_per_second = 1000000000u;
    return LIB_STATUS_OK;
}
#endif

#endif
