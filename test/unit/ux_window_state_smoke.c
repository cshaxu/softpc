#include "lib/ux-window/internal.h"

#include <assert.h>
#include <string.h>

static lib_status discard_input(void *context, const ux_input_event *event)
{
    (void)context;
    (void)event;
    return LIB_STATUS_OK;
}

int main(void)
{
    ux_window_options options = { 0 };
    ux_window *window = LIB_NULL;
    ux_frame frame = { 0 };
    ux_frame copied = { 0 };
    lib_u32 generation = 0u;
    char title[UX_WINDOW_TITLE_CAPACITY];
    lib_bool mouse_enabled = LIB_FALSE;
    lib_bool release_mouse = LIB_FALSE;

    options.input_sink = discard_input;
    strcpy(options.initial_title, "SoftPC");
    assert(ux_window_create(&window, &options) == LIB_STATUS_OK);
    frame.valid = 1u;
    frame.graphics = 0u;
    frame.text_columns = 80u;
    frame.text_rows = 25u;
    assert(ux_window_publish_frame(window, &frame) == LIB_STATUS_OK);
    assert(ux_window_set_mouse_enabled(window, LIB_TRUE) == LIB_STATUS_OK);
    assert(ux_window_release_mouse(window) == LIB_STATUS_OK);

    /* WM_PAINT may copy a frame but cannot consume a worker-only command. */
    assert(ux_window_capture_paint_state(window, &copied, &generation) ==
        LIB_STATUS_OK);
    assert(copied.valid != 0u && generation != 0u);
    assert(ux_window_take_control_state(window, title, &mouse_enabled,
        &release_mouse) == LIB_STATUS_OK);
    assert(strcmp(title, "SoftPC") == 0);
    assert(mouse_enabled != LIB_FALSE && release_mouse != LIB_FALSE);
    assert(ux_window_take_control_state(window, title, &mouse_enabled,
        &release_mouse) == LIB_STATUS_OK);
    assert(release_mouse == LIB_FALSE);

    ux_window_destroy(window);
    return 0;
}
