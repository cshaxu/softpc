#include "lib/ux-window/internal.h"

static void ux_window_lock(ux_window *window)
{
    while (atomic_flag_test_and_set_explicit(&window->lock, memory_order_acquire)) {}
}

static void ux_window_unlock(ux_window *window)
{
    atomic_flag_clear_explicit(&window->lock, memory_order_release);
}

lib_status ux_window_create(ux_window **out_window,
    const ux_window_options *options)
{
    ux_window *window;
    const char *title_end;
    lib_status status;

    if (out_window == LIB_NULL || options == LIB_NULL || options->input_sink == LIB_NULL ||
        (title_end = memchr(options->initial_title, '\0',
            sizeof(options->initial_title))) == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_window = LIB_NULL;
    window = calloc(1u, sizeof(*window));
    if (window == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    window->lock = (atomic_flag)ATOMIC_FLAG_INIT;
    atomic_flag_clear_explicit(&window->lock, memory_order_release);
    ux_frame_mailbox_initialize(&window->frames);
    ux_control_mailbox_initialize(&window->controls);
    status = ux_hotkey_matcher_create(&window->hotkeys, options->hotkeys,
        options->hotkey_count);
    if (status != LIB_STATUS_OK) {
        free(window);
        return status;
    }
    window->input_sink = options->input_sink;
    window->input_context = options->input_context;
    {
        ux_control_message initial_title = { UX_CONTROL_SET_TITLE, { { 0 } } };
        memcpy(initial_title.value.title, options->initial_title,
            (lib_size)(title_end - options->initial_title) + 1u);
        status = ux_control_mailbox_push(&window->controls, &initial_title);
        if (status != LIB_STATUS_OK) {
            ux_hotkey_matcher_destroy(window->hotkeys);
            free(window);
            return status;
        }
    }
    *out_window = window;
    return LIB_STATUS_OK;
}

lib_status ux_window_start(ux_window *window)
{
    lib_status status;
    if (window == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    ux_window_lock(window);
    if (window->started != LIB_FALSE || window->starting != LIB_FALSE) {
        ux_window_unlock(window);
        return LIB_STATUS_INVALID_STATE;
    }
    window->starting = LIB_TRUE;
    ux_window_unlock(window);
    status = ux_window_native_start(window);
    ux_window_lock(window);
    window->starting = LIB_FALSE;
    if (status != LIB_STATUS_OK) {
        ux_window_unlock(window);
        return status;
    }
    window->started = LIB_TRUE;
    ux_window_unlock(window);
    return LIB_STATUS_OK;
}

void ux_window_destroy(ux_window *window)
{
    if (window == LIB_NULL) return;
    ux_window_native_stop(window);
    if (window->started != LIB_FALSE)
        (void)ux_hotkey_matcher_retire(window->hotkeys, window,
            window->input_sink, window->input_context);
    ux_hotkey_matcher_destroy(window->hotkeys);
    free(window);
}

lib_status ux_window_publish_frame(ux_window *window, const ux_frame *frame)
{
    lib_status status;
    if (window == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = ux_frame_mailbox_publish(&window->frames, frame);
    if (status == LIB_STATUS_OK) ux_window_native_signal(window);
    return status;
}

lib_status ux_window_set_title(ux_window *window, const char *title)
{
    const char *end;
    ux_control_message message = { UX_CONTROL_SET_TITLE, { { 0 } } };

    if (window == LIB_NULL || title == LIB_NULL ||
        (end = memchr(title, '\0', UX_WINDOW_TITLE_CAPACITY)) == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    memcpy(message.value.title, title, (lib_size)(end - title) + 1u);
    return ux_window_push_control(window, &message);
}

lib_status ux_window_set_mouse_enabled(ux_window *window, lib_bool enabled)
{
    ux_control_message message = { UX_CONTROL_SET_MOUSE_ENABLED,
        { { 0 } } };
    message.value.mouse_enabled = enabled != LIB_FALSE;
    return ux_window_push_control(window, &message);
}

lib_status ux_window_release_mouse(ux_window *window)
{
    const ux_control_message message = { UX_CONTROL_RELEASE_MOUSE, { { 0 } } };
    return ux_window_push_control(window, &message);
}

const void *ux_window_input_source(const ux_window *window)
{
    return window;
}

lib_status ux_window_submit_input(ux_window *window, const ux_input_event *event)
{
    if (window == LIB_NULL || event == LIB_NULL || event->source_handle != window)
        return LIB_STATUS_INVALID_ARGUMENT;
    return ux_hotkey_matcher_submit(window->hotkeys, event, window->input_sink,
        window->input_context);
}

lib_status ux_window_request_close(ux_window *window)
{
    ux_input_event event;
    lib_status status = ux_input_make_window_close(&event, window);
    return status == LIB_STATUS_OK ? window->input_sink(window->input_context, &event) : status;
}

lib_status ux_window_capture_paint_state(const ux_window *window,
    ux_frame *out_frame, lib_u32 *out_generation)
{
    if (window == LIB_NULL || out_frame == LIB_NULL || out_generation == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    return ux_frame_mailbox_take(&window->frames, out_frame, out_generation);
}

lib_status ux_window_take_control(ux_window *window,
    ux_control_message *out_message, lib_bool *out_has_message)
{
    if (window == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    return ux_control_mailbox_take(&window->controls, out_message,
        out_has_message);
}

lib_status ux_window_push_control(ux_window *window,
    const ux_control_message *message)
{
    lib_status status;
    if (window == LIB_NULL || message == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    status = ux_control_mailbox_push(&window->controls, message);
    if (status == LIB_STATUS_OK) ux_window_native_signal(window);
    return status;
}
