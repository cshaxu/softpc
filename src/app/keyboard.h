#ifndef APP_KEYBOARD_H
#define APP_KEYBOARD_H

#include "lib/kvm-base/event_interface.h"
#include "lib/kvm-base/hotkey_interface.h"

/* Product hotkey registration and neutral key-sequence policy. */
int app_keyboard_deliver_input(void *context,
    const kvm_input_event *event);
int app_keyboard_hotkeys(kvm_hotkey_registry *registry);
int app_keyboard_release_ctrl_alt(void *context, kvm_input_sink sink);
int app_keyboard_submit_ctrl_alt_del(void *context, kvm_input_sink sink);
int app_keyboard_submit_alt_enter(void *context, kvm_input_sink sink);

#endif
