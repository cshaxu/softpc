#ifndef UX_WINDOW_H
#define UX_WINDOW_H

#include "lib/ux-base/hotkey.h"
#include "lib/ux-base/frame.h"

#define UX_WINDOW_TITLE_CAPACITY 128u

typedef struct ux_window ux_window;

typedef struct ux_window_options {
    void *input_context;
    ux_input_sink input_sink;
    ux_hotkey_registry hotkeys;
    char initial_title[UX_WINDOW_TITLE_CAPACITY];
} ux_window_options;

lib_status ux_window_create(ux_window **out_window,
    const ux_window_options *options);
void ux_window_destroy(ux_window *window);
lib_status ux_window_publish_frame(ux_window *window, const ux_frame *frame);
lib_status ux_window_set_title(ux_window *window, const char *title);
lib_status ux_window_set_mouse_enabled(ux_window *window, lib_bool enabled);
lib_status ux_window_release_mouse(ux_window *window);
lib_status ux_window_request_stop(ux_window *window);

#endif
