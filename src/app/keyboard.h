#ifndef APP_KEYBOARD_H
#define APP_KEYBOARD_H

#include "lib/ux/actions.h"
#include "lib/ux/event.h"
#include "machine.h"

#ifdef _WIN32
/* SoftPC's thin binding of normalized Win32 input.  The shared component
 * owns host packet recovery; this adapter alone invokes the original nt_keycd
 * table and queues its resulting SoftPC key number. */
int app_keyboard_deliver_input(void *context,
    const ux_event *event);
int app_keyboard_inject_machine_event(softpc_machine *machine,
    const ux_event *event);
int app_keyboard_register_actions(
    ux_action_registry *registry);
int app_keyboard_release_ctrl_alt(void *context, ux_event_sink sink);
int app_keyboard_submit_ctrl_alt_del(void *context, ux_event_sink sink);
int app_keyboard_submit_alt_enter(void *context, ux_event_sink sink);
#endif

#endif
