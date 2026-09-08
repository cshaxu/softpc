#ifndef UX_WINDOW_H
#define UX_WINDOW_H

#include "lib/ux-base/component.h"

#define UX_WINDOW_TITLE_CAPACITY 128u

typedef struct ux_window ux_window;

typedef ux_component_options ux_window_options;

lib_status ux_window_create(ux_window **out_window,
    const ux_window_options *options);
lib_status ux_window_publish_frame(ux_window *window, const ux_frame *frame);
void ux_window_destroy(ux_window *window);
lib_status ux_window_set_title(ux_window *window, const char *title);
lib_status ux_window_enable_mouse(ux_window *window);
/* Disabling is terminal for current capture: it queues the disabled flag and
 * an explicit release in FIFO order. */
lib_status ux_window_disable_mouse(ux_window *window);
lib_status ux_window_release_mouse(ux_window *window);

#endif
