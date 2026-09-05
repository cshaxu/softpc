#ifndef APP_KEYBOARD_H
#define APP_KEYBOARD_H

#include "../lib/platform/win32/input.h"
#include "../lib/platform/win32/actions.h"
#include "machine.h"

#ifdef _WIN32
/* SoftPC's thin binding of normalized Win32 input.  The shared component
 * owns host packet recovery; this adapter alone invokes the original nt_keycd
 * table and queues its resulting SoftPC key number. */
int app_keyboard_deliver_input(void *context,
    const win32_presentation_event *event);
int app_keyboard_inject_machine_event(softpc_machine *machine,
    const win32_presentation_event *event);
int app_keyboard_register_actions(
    win32_presentation_action_registry *registry);
#endif

#endif
