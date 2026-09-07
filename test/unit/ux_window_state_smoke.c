#include "lib/ux-window/internal.h"

#include <assert.h>
#include <string.h>

typedef struct ux_window_capture {
    ux_input_event event;
    lib_u32 count;
} ux_window_capture;

static lib_status capture_input(void *context, const ux_input_event *event)
{
    ux_window_capture *capture = context;
    if (capture == LIB_NULL || event == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    capture->event = *event;
    ++capture->count;
    return LIB_STATUS_OK;
}

int main(void)
{
    ux_window_options options = { 0 };
    ux_window *window = LIB_NULL;
    ux_frame frame = { 0 };
    ux_frame copied = { 0 };
    lib_u32 generation = 0u;
    ux_control_message control;
    lib_bool has_control = LIB_FALSE;
    ux_window_capture capture = { 0 };
    ux_input_event text = { 0 };

    options.input_sink = capture_input;
    options.input_context = &capture;
    strcpy(options.initial_title, "SoftPC");
    assert(ux_window_create(&window, &options) == LIB_STATUS_OK);
    frame.valid = 1u;
    frame.graphics = 0u;
    frame.text_columns = 80u;
    frame.text_rows = 25u;
    assert(ux_window_publish_frame(window, &frame) == LIB_STATUS_OK);
    assert(ux_window_set_title(window, "SoftPC running") == LIB_STATUS_OK);
    assert(ux_window_set_mouse_enabled(window, LIB_TRUE) == LIB_STATUS_OK);
    assert(ux_window_release_mouse(window) == LIB_STATUS_OK);

    /* Paint only reads frame. The Window worker receives controls in order. */
    assert(ux_window_capture_paint_state(window, &copied, &generation) ==
        LIB_STATUS_OK);
    assert(copied.valid != 0u && generation != 0u);
    assert(ux_window_take_control(window, &control, &has_control) ==
        LIB_STATUS_OK && has_control != LIB_FALSE);
    assert(control.kind == UX_CONTROL_SET_TITLE &&
        strcmp(control.value.title, "SoftPC") == 0);
    assert(ux_window_take_control(window, &control, &has_control) ==
        LIB_STATUS_OK && has_control != LIB_FALSE);
    assert(control.kind == UX_CONTROL_SET_TITLE &&
        strcmp(control.value.title, "SoftPC running") == 0);
    assert(ux_window_take_control(window, &control, &has_control) ==
        LIB_STATUS_OK && has_control != LIB_FALSE);
    assert(control.kind == UX_CONTROL_SET_MOUSE_ENABLED &&
        control.value.mouse_enabled != LIB_FALSE);
    assert(ux_window_take_control(window, &control, &has_control) ==
        LIB_STATUS_OK && has_control != LIB_FALSE);
    assert(control.kind == UX_CONTROL_RELEASE_MOUSE);
    assert(ux_window_take_control(window, &control, &has_control) ==
        LIB_STATUS_OK && has_control == LIB_FALSE);

    assert(ux_input_make_text(&text, ux_window_input_source(window), 'a') ==
        LIB_STATUS_OK);
    assert(ux_window_submit_input(window, &text) == LIB_STATUS_OK);
    assert(capture.count == 1u && capture.event.kind == UX_INPUT_TEXT &&
        capture.event.value.text.scalar == 'a');

    ux_window_destroy(window);
    return 0;
}
