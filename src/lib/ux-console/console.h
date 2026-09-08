#ifndef UX_CONSOLE_H
#define UX_CONSOLE_H

#include "lib/base/console.h"
#include "lib/ux-base/hotkey.h"
#include "lib/ux-base/frame.h"

typedef struct ux_console ux_console;

typedef struct ux_console_options {
    void *input_context;
    ux_input_sink input_sink;
    ux_hotkey_registry hotkeys;
} ux_console_options;

lib_status ux_console_create(ux_console **out_console,
    const ux_console_options *options);
void ux_console_destroy(ux_console *console);
/* Borrowed logical Console object. SoftPC passes it to host for Current
 * Console registration before it permits raw VM input. */
lib_console *ux_console_get_console(const ux_console *console);
lib_status ux_console_publish_frame(ux_console *console,
    const ux_frame *frame);
lib_status ux_console_request_stop(ux_console *console);

#endif
