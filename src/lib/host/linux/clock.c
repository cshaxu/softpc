#include "lib/host/clock.h"
#include "lib/types/clock.h"

lib_status host_clock_platform_counter(lib_u64 *out_units,
    lib_u64 *out_units_per_second)
{
    return lib_clock_counter(out_units, out_units_per_second);
}
