#include "lib/ux-console/internal.h"

#ifndef _WIN32
lib_status ux_console_native_start(ux_console *console)
{ (void)console; return LIB_STATUS_UNSUPPORTED; }
void ux_console_native_stop(ux_console *console)
{ (void)console; }
void ux_console_native_signal(ux_console *console)
{ (void)console; }
#endif
