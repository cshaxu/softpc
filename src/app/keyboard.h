#ifndef APP_KEYBOARD_H
#define APP_KEYBOARD_H

#include "../lib/platform/win32/input.h"

#ifdef _WIN32
/* SoftPC's thin binding of normalized Win32 input.  The shared component
 * owns host packet recovery; this adapter alone invokes the original nt_keycd
 * table and queues its resulting SoftPC key number. */
int app_keyboard_enqueue_win32_event(void *context,
    const KEY_EVENT_RECORD *event);
#endif

#endif
