#include "lib/console/console.h"
#include "lib/ui-console/console_interface.h"

#include <assert.h>

#ifdef _WIN32
typedef struct mouse_scale_probe {
    ui_input_event events[3];
    lib_u32 count;
} mouse_scale_probe;

static int mouse_scale_sink(void *opaque, const ui_input_event *event)
{
    mouse_scale_probe *probe = (mouse_scale_probe *)opaque;
    assert(probe != LIB_NULL && event != LIB_NULL);
    if (event->type == UI_EVENT_SOURCE_RETIRED) return 1;
    assert(event->type == UI_EVENT_MOUSE);
    assert(probe->count < 3u);
    probe->events[probe->count++] = *event;
    return 1;
}

static void mouse_scale_failure(void *opaque, lib_u64 source_identity,
    lib_status status)
{
    (void)opaque;
    (void)source_identity;
    (void)status;
    assert(!"unexpected UI input delivery failure");
}

static void deliver_mouse(lib_console *console, lib_i32 column, lib_i32 row)
{
    lib_console_event event = { 0 };
    event.kind = LIB_CONSOLE_EVENT_RAW_MOUSE;
    event.binding_generation = 1u;
    event.value.raw_mouse.delta_x = column;
    event.value.raw_mouse.delta_y = row;
    assert(lib_console_deliver_event(console, &event) == LIB_STATUS_OK);
}

int main(void)
{
    mouse_scale_probe probe = { 0 };
    ui_console_options options = { 0 };
    ui_console *component = LIB_NULL;
    lib_console *console;

    options.input_context = &probe;
    options.input_sink = mouse_scale_sink;
    options.failure_context = &probe;
    options.failure_sink = mouse_scale_failure;
    assert(ui_console_create(&component, &options) == LIB_STATUS_OK);
    console = ui_console_get_console(component);
    assert(console != LIB_NULL);
    assert(lib_console_bind_generation(console, 1u) == LIB_STATUS_OK);

    /* The first native position establishes a relative-motion baseline. */
    deliver_mouse(console, 40, 12);
    deliver_mouse(console, 41, 12);
    deliver_mouse(console, 41, 13);

    assert(probe.count == 3u);
    assert(probe.events[0].data.mouse.relative == 1u);
    assert(probe.events[0].data.mouse.delta_x == 0);
    assert(probe.events[0].data.mouse.delta_y == 0);
    assert(probe.events[1].data.mouse.delta_x == 8);
    assert(probe.events[1].data.mouse.delta_y == 0);
    assert(probe.events[2].data.mouse.delta_x == 0);
    assert(probe.events[2].data.mouse.delta_y == 8);
    ui_console_destroy(component);
    return 0;
}
#else
int main(void) { return 0; }
#endif
