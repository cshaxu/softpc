#include "lib/ux-console/console.h"

#include <assert.h>
#include <string.h>

typedef struct captured_input {
    ux_input_event event;
    lib_u32 count;
} captured_input;

static lib_status capture_input(void *context, const ux_input_event *event)
{
    captured_input *captured = context;
    if (captured == LIB_NULL || event == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    captured->event = *event;
    ++captured->count;
    return LIB_STATUS_OK;
}

int main(void)
{
    captured_input captured = { 0 };
    ux_console_options options = { 0 };
    ux_console *console = LIB_NULL;
    lib_console_event raw = { 0 };
    ux_frame frame = { 0 };

    options.input_sink = capture_input;
    options.input_context = &captured;
    assert(ux_console_create(&console, &options) == LIB_STATUS_OK);
    assert(ux_console_get_console(console) != LIB_NULL);
    assert(ux_console_start(console) == LIB_STATUS_OK);
    frame.valid = 1u;
    frame.graphics = 0u;
    frame.text_columns = 80u;
    frame.text_rows = 25u;
    memset(frame.text, ' ', sizeof(frame.text));
    assert(ux_console_publish_frame(console, &frame) == LIB_STATUS_OK);

    raw.kind = LIB_CONSOLE_EVENT_RAW_KEY;
    raw.value.raw_key.key = 'A';
    raw.value.raw_key.pressed = LIB_TRUE;
    assert(lib_console_deliver_event(ux_console_get_console(console), &raw) ==
        LIB_STATUS_OK);
    assert(captured.count == 1u);
    assert(captured.event.kind == UX_INPUT_KEY);
    assert(captured.event.source_handle == ux_console_input_source(console));
    assert(captured.event.value.key.virtual_key == 'A');

    ux_console_destroy(console);
    return 0;
}
