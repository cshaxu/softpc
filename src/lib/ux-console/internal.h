#ifndef UX_CONSOLE_INTERNAL_H
#define UX_CONSOLE_INTERNAL_H

#include "lib/ux-console/console.h"
#include "lib/ux-base/internal.h"

typedef struct ux_console_native ux_console_native;

struct ux_console {
    ux_frame_mailbox frames;
    ux_hotkey_matcher *hotkeys;
    ux_input_sink input_sink;
    void *input_context;
    lib_console *logical_console;
    ux_console_native *native;
    lib_bool started;
};

lib_status ux_console_native_start(ux_console *console);
void ux_console_native_stop(ux_console *console);
void ux_console_native_signal(ux_console *console);
lib_status ux_console_present_frame(ux_console *console, const ux_frame *frame);

#endif
