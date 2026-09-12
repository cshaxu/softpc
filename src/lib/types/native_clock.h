#ifndef LIB_TYPES_NATIVE_CLOCK_H
#define LIB_TYPES_NATIVE_CLOCK_H

#include "lib/types/types_interface.h"

/* Raw monotonic counter facts. Host owns the public clock contract and unit
 * conversion policy. */
lib_status lib_native_monotonic_counter(lib_u64 *out_units,
    lib_u64 *out_units_per_second);

#endif
