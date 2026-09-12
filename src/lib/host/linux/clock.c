#include "lib/host/clock.h"
#include "lib/types/native_clock.h"

lib_status host_clock_platform_counter(lib_u64 *out_units,
    lib_u64 *out_units_per_second)
{
    return lib_native_monotonic_counter(out_units, out_units_per_second);
}
