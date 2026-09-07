#include "lib/ux-window/internal.h"

#ifndef _WIN32
lib_status ux_window_native_start(ux_window *window)
{ (void)window; return LIB_STATUS_UNSUPPORTED; }
void ux_window_native_stop(ux_window *window)
{ (void)window; }
void ux_window_native_signal(ux_window *window)
{ (void)window; }
#endif
