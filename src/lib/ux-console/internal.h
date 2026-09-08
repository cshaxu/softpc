#ifndef UX_CONSOLE_INTERNAL_H
#define UX_CONSOLE_INTERNAL_H

#include "lib/ux-console/console.h"
#include "lib/ux-base/internal/mailbox.h"

struct ux_console {
    ux_component_mailboxes mailboxes;
    lib_console *logical_console;
    void *input_context;
    ux_input_sink input_sink;
    ux_hotkey_matcher hotkey_matcher;
    atomic_int stopping;
    void *native_state;
};

lib_status ux_console_native_start(ux_console *console);
void ux_console_native_stop(ux_console *console);

#endif
