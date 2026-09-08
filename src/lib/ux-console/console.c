#include "lib/ux-console/console.h"
#include "lib/ux-base/internal.h"

struct ux_console {
    ux_frame_mailbox frames;
    ux_hotkey_matcher *hotkeys;
    ux_input_sink input_sink;
    void *input_context;
    lib_console *logical_console;
    lib_bool started;
};

static void ux_console_event(void *context, const lib_console_event *event)
{
    ux_console *console = context;
    ux_input_event input;

    if (console == LIB_NULL || event == LIB_NULL) return;
    if (event->kind == LIB_CONSOLE_EVENT_RAW_KEY) {
        if (ux_input_make_key(&input, console, 0u, event->value.raw_key.key,
                event->value.raw_key.modifiers, event->value.raw_key.pressed) ==
            LIB_STATUS_OK) {
            (void)ux_hotkey_matcher_submit(console->hotkeys, &input,
                console->input_sink, console->input_context);
        }
    } else if (event->kind == LIB_CONSOLE_EVENT_RAW_MOUSE) {
        memset(&input, 0, sizeof(input));
        input.source_handle = console;
        input.kind = UX_INPUT_MOUSE;
        input.value.mouse.delta_x = event->value.raw_mouse.delta_x;
        input.value.mouse.delta_y = event->value.raw_mouse.delta_y;
        input.value.mouse.buttons = event->value.raw_mouse.buttons;
        input.value.mouse.relative = LIB_TRUE;
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
    if (console == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    if (console->started != LIB_FALSE) return LIB_STATUS_INVALID_STATE;
    /* The platform renderer is independent of host registration.  S4 core
     * establishes its logical object and event sink before that worker starts. */
    console->started = LIB_TRUE;
    return LIB_STATUS_OK;
}

void ux_console_destroy(ux_console *console)
{
    if (console == LIB_NULL) return;
    (void)ux_hotkey_matcher_retire(console->hotkeys, console,
        console->input_sink, console->input_context);
    (void)lib_console_set_event_sink(console->logical_console, LIB_NULL, LIB_NULL);
    lib_console_destroy(console->logical_console);
    ux_hotkey_matcher_destroy(console->hotkeys);
    free(console);
}

lib_status ux_console_publish_frame(ux_console *console, const ux_frame *frame)
{
    return console == LIB_NULL ? LIB_STATUS_INVALID_ARGUMENT :
        ux_frame_mailbox_publish(&console->frames, frame);
}

lib_console *ux_console_get_console(ux_console *console)
{
    return console == LIB_NULL ? LIB_NULL : console->logical_console;
}

const void *ux_console_input_source(const ux_console *console)
{
    return console;
}
