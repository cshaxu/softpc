#include "lib/ux-window/window.h"
#include "lib/ux-base/internal.h"

struct ux_window {
    ux_frame_mailbox frames;
    ux_hotkey_matcher *hotkeys;
    ux_input_sink input_sink;
    void *input_context;
    atomic_flag lock;
    lib_bool started;
    lib_bool mouse_enabled;
    lib_bool mouse_release_requested;
    char title[UX_WINDOW_TITLE_CAPACITY];
};

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
    status = ux_hotkey_matcher_create(&window->hotkeys, options->hotkeys,
        options->hotkey_count);
    if (status != LIB_STATUS_OK) {
        free(window);
        return status;
    }
    window->input_sink = options->input_sink;
    window->input_context = options->input_context;
    memcpy(window->title, options->initial_title,
        (lib_size)(title_end - options->initial_title) + 1u);
    *out_window = window;
    return LIB_STATUS_OK;
}

lib_status ux_window_start(ux_window *window)
{
    if (window == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    ux_window_lock(window);
    if (window->started != LIB_FALSE) {
        ux_window_unlock(window);
        return LIB_STATUS_INVALID_STATE;
    }
    /* The platform worker is added by the Win32/Linux implementation.  This
     * core transition deliberately makes no product or host decision. */
    window->started = LIB_TRUE;
    ux_window_unlock(window);
    return LIB_STATUS_OK;
}

void ux_window_destroy(ux_window *window)
{
    if (window == LIB_NULL) return;
    (void)ux_hotkey_matcher_retire(window->hotkeys, window, window->input_sink,
        window->input_context);
    ux_hotkey_matcher_destroy(window->hotkeys);
    free(window);
}

lib_status ux_window_publish_frame(ux_window *window, const ux_frame *frame)
{
    return window == LIB_NULL ? LIB_STATUS_INVALID_ARGUMENT :
        ux_frame_mailbox_publish(&window->frames, frame);
}

lib_status ux_window_set_title(ux_window *window, const char *title)
{
    const char *end;

    if (window == LIB_NULL || title == LIB_NULL ||
        (end = memchr(title, '\0', UX_WINDOW_TITLE_CAPACITY)) == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    ux_window_lock(window);
    memcpy(window->title, title, (lib_size)(end - title) + 1u);
    ux_window_unlock(window);
    return LIB_STATUS_OK;
}

lib_status ux_window_set_mouse_enabled(ux_window *window, lib_bool enabled)
{
    if (window == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    ux_window_lock(window);
    window->mouse_enabled = enabled != LIB_FALSE;
    ux_window_unlock(window);
    return LIB_STATUS_OK;
}

lib_status ux_window_release_mouse(ux_window *window)
{
    if (window == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    ux_window_lock(window);
    window->mouse_release_requested = LIB_TRUE;
    ux_window_unlock(window);
    return LIB_STATUS_OK;
}

const void *ux_window_input_source(const ux_window *window)
{
    return window;
}
