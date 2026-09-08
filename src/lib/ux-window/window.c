#include "lib/ux-window/internal.h"

static lib_status ux_window_enqueue(ux_window *window,
    ux_component_control control)
{
    return window == LIB_NULL ? LIB_STATUS_INVALID_ARGUMENT :
        ux_component_mailboxes_enqueue_control(&window->mailboxes, &control);
}

lib_status ux_window_create(ux_window **out_window,
    const ux_window_options *options)
{
    ux_window *window;
    lib_status status;

    if (out_window == LIB_NULL || options == LIB_NULL ||
        options->input_sink == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_window = LIB_NULL;
    window = calloc(1u, sizeof(*window));
    if (window == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    window->input_context = options->input_context;
    window->input_sink = options->input_sink;
    window->initial_title[sizeof(window->initial_title) - 1u] = '\0';
    memcpy(window->initial_title, options->initial_title,
        sizeof(window->initial_title) - 1u);
    ux_hotkey_matcher_initialize(&window->hotkey_matcher, &options->hotkeys);
    atomic_init(&window->stopping, 0);
    status = ux_component_mailboxes_create(&window->mailboxes);
    if (status == LIB_STATUS_OK) status = ux_window_native_start(window);
    if (status != LIB_STATUS_OK) {
        ux_component_mailboxes_destroy(&window->mailboxes);
        free(window);
        return status;
    }
    *out_window = window;
    return LIB_STATUS_OK;
}

void ux_window_destroy(ux_window *window)
{
    if (window == LIB_NULL) return;
    (void)ux_window_request_stop(window);
    ux_window_native_stop(window);
    ux_component_mailboxes_destroy(&window->mailboxes);
    free(window);
}

lib_status ux_window_publish_frame(ux_window *window, const ux_frame *frame)
{
    return window == LIB_NULL ? LIB_STATUS_INVALID_ARGUMENT :
        ux_component_mailboxes_publish_frame(&window->mailboxes, frame);
}

lib_status ux_window_set_title(ux_window *window, const char *title)
{
    ux_component_control control = { 0 };
    if (title == LIB_NULL || memchr(title, '\0',
            UX_COMPONENT_WINDOW_TITLE_CAPACITY) == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    control.kind = UX_COMPONENT_CONTROL_SET_WINDOW_TITLE;
    memcpy(control.value.title, title, strlen(title) + 1u);
    return ux_window_enqueue(window, control);
}

lib_status ux_window_set_mouse_enabled(ux_window *window, lib_bool enabled)
{
    ux_component_control control = { UX_COMPONENT_CONTROL_SET_WINDOW_MOUSE_ENABLED,
        { 0 } };
    control.value.window_mouse_enabled = enabled != LIB_FALSE;
    return ux_window_enqueue(window, control);
}

lib_status ux_window_release_mouse(ux_window *window)
{
    ux_component_control control = { UX_COMPONENT_CONTROL_RELEASE_WINDOW_MOUSE,
        { 0 } };
    return ux_window_enqueue(window, control);
}

lib_status ux_window_request_stop(ux_window *window)
{
    ux_component_control control = { UX_COMPONENT_CONTROL_STOP, { 0 } };
    return ux_window_enqueue(window, control);
}
