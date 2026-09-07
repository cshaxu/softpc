#ifndef UX_WINDOW_INTERNAL_H
#define UX_WINDOW_INTERNAL_H

#include "lib/ux-window/window.h"
#include "lib/ux-base/internal.h"

typedef struct ux_window_native ux_window_native;

struct ux_window {
    ux_frame_mailbox frames;
    ux_control_mailbox controls;
    ux_hotkey_matcher *hotkeys;
    ux_input_sink input_sink;
    void *input_context;
    atomic_flag lock;
    ux_window_native *native;
    lib_bool started;
    lib_bool starting;
};

lib_status ux_window_submit_input(ux_window *window,
    const ux_input_event *event);
lib_status ux_window_request_close(ux_window *window);
/* Paint may observe copied presentation state at any time.  It must never
 * consume a control request: only the native worker takes those requests. */
lib_status ux_window_capture_paint_state(const ux_window *window,
    ux_frame *out_frame, lib_u32 *out_generation);
lib_status ux_window_take_control(ux_window *window,
    ux_control_message *out_message, lib_bool *out_has_message);
lib_status ux_window_push_control(ux_window *window,
    const ux_control_message *message);
lib_status ux_window_native_start(ux_window *window);
void ux_window_native_stop(ux_window *window);
void ux_window_native_signal(ux_window *window);

#endif
