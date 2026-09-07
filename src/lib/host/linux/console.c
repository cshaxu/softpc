#include "lib/host/internal/console_native.h"

#ifndef _WIN32
struct host_console_native { int unavailable; };
lib_status host_console_native_create(host_console_native **out_native)
{
    if (out_native != LIB_NULL) *out_native = LIB_NULL;
    return LIB_STATUS_UNSUPPORTED;
}
void host_console_native_destroy(host_console_native *native_console)
{ (void)native_console; }
lib_status host_console_native_activate(host_console_native *native_console,
    lib_console *console, host_console_mode mode, lib_u32 generation)
{ (void)native_console; (void)console; (void)mode; (void)generation; return LIB_STATUS_UNSUPPORTED; }
void host_console_native_deactivate(host_console_native *native_console)
{ (void)native_console; }
lib_status host_console_native_write(void *context, const char *text, lib_size length)
{ (void)context; (void)text; (void)length; return LIB_STATUS_NOT_CURRENT; }
#endif
