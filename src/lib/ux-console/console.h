#ifndef UX_CONSOLE_H
#define UX_CONSOLE_H

#include "lib/base/console.h"
#include "lib/ux-base/frame.h"
#include "lib/ux-base/input.h"

typedef struct ux_console ux_console;

typedef struct ux_console_options {
    const ux_hotkey_registration *hotkeys;
    lib_u32 hotkey_count;
    ux_input_sink input_sink;
    void *input_context;
} ux_console_options;

lib_status ux_console_create(ux_console **out_console,
    const ux_console_options *options);
lib_status ux_console_start(ux_console *console);
void ux_console_destroy(ux_console *console);
lib_status ux_console_publish_frame(ux_console *console, const ux_frame *frame);
lib_console *ux_console_get_console(ux_console *console);
const void *ux_console_input_source(const ux_console *console);

#endif
