#ifndef SOFTPC_TEST_TIME_H
#define SOFTPC_TEST_TIME_H

#include "lib/base/clock_interface.h"
#include "lib/base/sync_interface.h"

static lib_u64 softpc_test_clock_milliseconds(void)
{
    lib_u64 milliseconds = 0u;
    return base_clock_milliseconds(&milliseconds) == LIB_STATUS_OK ?
        milliseconds : 0u;
}

static void softpc_test_sleep_milliseconds(lib_u32 milliseconds)
{
    base_sync_sleep_milliseconds(milliseconds);
}

#endif
