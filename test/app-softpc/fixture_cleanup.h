#ifndef SOFTPC_TEST_FIXTURE_CLEANUP_H
#define SOFTPC_TEST_FIXTURE_CLEANUP_H

/* App tests may remove an image only after their machine/runtime has been
 * destroyed. Windows filters can retain a closed handle briefly. */
#include "lib/types/test.h"
#include "lib/types/file.h"

#ifdef _WIN32
#include "lib/types/win32/test.h"
static lib_i32 softpc_test_remove_image(const char *path)
{
    lib_win32_dword deadline = lib_win32_get_tick_count() + 1000u;
    do {
        if (lib_c_remove(path) == 0) return 1;
        lib_win32_sleep(10u);
    } while ((lib_win32_long)(lib_win32_get_tick_count() - deadline) < 0);
    return 0;
}
#else
static lib_i32 softpc_test_remove_image(const char *path)
{
    return lib_c_remove(path) == 0;
}
#endif

#endif
