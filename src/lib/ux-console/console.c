#include "lib/ux-console/internal.h"

static void ux_console_event(void *context, const lib_console_event *event)
{
    ux_console *console = context;
    ux_input_event input;

    if (console == LIB_NULL || event == LIB_NULL) return;
    if (event->kind == LIB_CONSOLE_EVENT_RAW_KEY) {
        if (ux_input_make_key(&input, console, event->value.raw_key.scan_code,
                event->value.raw_key.key,
                event->value.raw_key.modifiers, event->value.raw_key.pressed) ==
            LIB_STATUS_OK) {
            (void)ux_hotkey_matcher_submit(console->hotkeys, &input,
                console->input_sink, console->input_context);
        }
    } else if (event->kind == LIB_CONSOLE_EVENT_RAW_MOUSE) {
        if (ux_input_make_mouse(&input, console,
                event->value.raw_mouse.delta_x, event->value.raw_mouse.delta_y,
                0, 0, 0, 0, event->value.raw_mouse.buttons, LIB_TRUE) == LIB_STATUS_OK)
            (void)ux_hotkey_matcher_submit(console->hotkeys, &input,
                console->input_sink, console->input_context);
    }
}

lib_status ux_console_create(ux_console **out_console,
    const ux_console_options *options)
{
    ux_console *console;
    lib_status status;

    if (out_console == LIB_NULL || options == LIB_NULL || options->input_sink == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_console = LIB_NULL;
    console = calloc(1u, sizeof(*console));
    if (console == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    ux_frame_mailbox_initialize(&console->frames);
    ux_control_mailbox_initialize(&console->controls);
    status = ux_hotkey_matcher_create(&console->hotkeys, options->hotkeys,
        options->hotkey_count);
    if (status == LIB_STATUS_OK)
        status = lib_console_create(&console->logical_console);
    if (status == LIB_STATUS_OK)
        status = lib_console_set_event_sink(console->logical_console,
            ux_console_event, console);
    if (status != LIB_STATUS_OK) {
        lib_console_destroy(console->logical_console);
        ux_hotkey_matcher_destroy(console->hotkeys);
        free(console);
        return status;
    }
    console->input_sink = options->input_sink;
    console->input_context = options->input_context;
    *out_console = console;
    return LIB_STATUS_OK;
}

lib_status ux_console_start(ux_console *console)
{
    lib_status status;
    if (console == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    if (console->started != LIB_FALSE) return LIB_STATUS_INVALID_STATE;
    status = ux_console_native_start(console);
    if (status != LIB_STATUS_OK) return status;
    console->started = LIB_TRUE;
    return LIB_STATUS_OK;
}

void ux_console_destroy(ux_console *console)
{
    if (console == LIB_NULL) return;
    ux_console_native_stop(console);
    if (console->started != LIB_FALSE)
        (void)ux_hotkey_matcher_retire(console->hotkeys, console,
            console->input_sink, console->input_context);
    (void)lib_console_set_event_sink(console->logical_console, LIB_NULL, LIB_NULL);
    lib_console_destroy(console->logical_console);
    ux_hotkey_matcher_destroy(console->hotkeys);
    free(console);
}

lib_status ux_console_publish_frame(ux_console *console, const ux_frame *frame)
{
    lib_status status;
    if (console == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = ux_frame_mailbox_publish(&console->frames, frame);
    if (status == LIB_STATUS_OK) ux_console_native_signal(console);
    return status;
}

lib_status ux_console_present_frame(ux_console *console, const ux_frame *frame)
{
    lib_console_text_frame text = { 0 };

    if (console == LIB_NULL || !ux_frame_is_valid(frame)) return LIB_STATUS_INVALID_ARGUMENT;
    if (frame->graphics != 0u) return LIB_STATUS_OK; /* preserve last text frame */
    text.columns = frame->text_columns;
    text.rows = frame->text_rows;
    text.cursor_column = frame->cursor_column;
    text.cursor_row = frame->cursor_row;
    text.cursor_top = frame->cursor_top;
    text.cursor_bottom = frame->cursor_bottom;
    text.font_height = frame->font_height;
    text.cursor_visible = frame->cursor_visible != 0u && frame->cursor_phase != 0u;
    /* Both frame formats are fixed 80-column surfaces.  Copying only
     * columns*rows would compact a narrow text mode and make every row after
     * the first render from the wrong source offset. */
    memcpy(text.text, frame->text, sizeof(text.text));
    memcpy(text.attributes, frame->attributes, sizeof(text.attributes));
    memcpy(text.palette, frame->text_palette, sizeof(text.palette));
    return lib_console_present_text_frame(console->logical_console, &text);
}

lib_console *ux_console_get_console(ux_console *console)
{
    return console == LIB_NULL ? LIB_NULL : console->logical_console;
}

const void *ux_console_input_source(const ux_console *console)
{
    return console;
}

lib_status ux_console_take_control(ux_console *console,
    ux_control_message *out_message, lib_bool *out_has_message)
{
    if (console == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    return ux_control_mailbox_take(&console->controls, out_message,
        out_has_message);
}

lib_status ux_console_push_control(ux_console *console,
    const ux_control_message *message)
{
    lib_status status;
    if (console == LIB_NULL || message == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    status = ux_control_mailbox_push(&console->controls, message);
    if (status == LIB_STATUS_OK) ux_console_native_signal(console);
    return status;
}
