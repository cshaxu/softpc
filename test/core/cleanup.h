#ifndef SOFTPC_TEST_CLEANUP_H
#define SOFTPC_TEST_CLEANUP_H

/* Product tests may remove an image only after their machine/runtime has been
 * destroyed. Windows filters can retain a closed handle briefly. */
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
static int softpc_test_remove_image(const char *path)
{
    DWORD deadline = GetTickCount() + 1000u;
    do {
        if (remove(path) == 0) return 1;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}
#else
static int softpc_test_remove_image(const char *path)
{
    return remove(path) == 0;
}
#endif

#endif
