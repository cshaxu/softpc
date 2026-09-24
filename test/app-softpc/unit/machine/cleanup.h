#include "../../time.h"
#ifndef SOFTPC_TEST_CLEANUP_H
#define SOFTPC_TEST_CLEANUP_H

/* Product tests may remove an image only after their machine/runtime has been
 * destroyed. Windows filters can retain a closed handle briefly. */
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
static int softpc_test_remove_image(const char *path)
{
    lib_u64 deadline = softpc_test_clock_milliseconds() + 1000u;
    do {
        if (remove(path) == 0) return 1;
        softpc_test_sleep_milliseconds(10u);
    } while (softpc_test_clock_milliseconds() < deadline);
    return 0;
}
#else
static int softpc_test_remove_image(const char *path)
{
    return remove(path) == 0;
}
#endif

#endif
