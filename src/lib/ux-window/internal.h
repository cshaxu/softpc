#ifndef UX_WINDOW_INTERNAL_H
#define UX_WINDOW_INTERNAL_H

#include "lib/ux-window/window.h"
#include "lib/ux-base/internal/mailbox.h"

struct ux_window {
    ux_component_mailboxes mailboxes;
    void *input_context;
    ux_input_sink input_sink;
    ux_hotkey_matcher hotkey_matcher;
    atomic_int stopping;
    char initial_title[UX_WINDOW_TITLE_CAPACITY];
    void *native_state;
};

lib_status ux_window_native_start(ux_window *window);
void ux_window_native_stop(ux_window *window);

#endif
