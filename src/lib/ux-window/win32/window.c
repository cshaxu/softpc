#include "lib/ux-window/internal.h"

#ifdef _WIN32
/* The native message-loop implementation is intentionally the only future
 * owner of HWND/WndProc.  Core already exposes no product callback path. */
lib_status ux_window_native_start(ux_window *window)
{ (void)window; return LIB_STATUS_UNSUPPORTED; }
void ux_window_native_stop(ux_window *window)
{ (void)window; }
void ux_window_native_signal(ux_window *window)
{ (void)window; }
#endif
