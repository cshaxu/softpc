#include "lib/ux-base/frame.h"
#include "lib/ux-base/hotkey.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct ux_capture {
    ux_input_event events[4];
    unsigned int count;
} ux_capture;

static int ux_capture_event(void *opaque, const ux_input_event *event)
{
    ux_capture *capture = (ux_capture *)opaque;
    if (capture == NULL || event == NULL || capture->count == 4u) return 0;
    capture->events[capture->count++] = *event;
    return 1;
}

int main(void)
{
    ux_frame *frame = calloc(1u, sizeof(*frame));
    ux_hotkey_registry registry;
    ux_hotkey_matcher matcher;
    ux_capture capture = { 0 };
    ux_input_event event = { 0 };

    assert(frame != NULL);
    frame->valid = 1u;
    frame->graphics = 0u;
    frame->text_columns = 80u;
    frame->text_rows = 25u;
    frame->text[0] = 'X';
    assert(ux_frame_is_valid(frame));

    ux_hotkey_registry_initialize(&registry);
    assert(ux_hotkey_registry_register(&registry, 'P',
        UX_HOTKEY_MODIFIER_CONTROL | UX_HOTKEY_MODIFIER_ALT,
        "pause-toggle") == LIB_STATUS_OK);
    ux_hotkey_matcher_initialize(&matcher, &registry);
    event.type = UX_EVENT_KEY;
    event.data.key.virtual_key = UX_HOTKEY_KEY_CONTROL;
    event.data.key.pressed = 1u;
    event.data.key.hotkey_modifiers = UX_HOTKEY_MODIFIER_CONTROL;
    assert(ux_hotkey_matcher_submit(&matcher, &event, ux_capture_event,
        &capture));
    event.data.key.virtual_key = UX_HOTKEY_KEY_ALT;
    event.data.key.hotkey_modifiers = UX_HOTKEY_MODIFIER_CONTROL |
        UX_HOTKEY_MODIFIER_ALT;
    assert(ux_hotkey_matcher_submit(&matcher, &event, ux_capture_event,
        &capture));
    event.type = UX_EVENT_KEY;
    event.data.key.virtual_key = 'P';
    event.data.key.pressed = 1u;
    event.data.key.hotkey_modifiers = UX_HOTKEY_MODIFIER_CONTROL |
        UX_HOTKEY_MODIFIER_ALT;
    assert(ux_hotkey_matcher_submit(&matcher, &event, ux_capture_event,
        &capture));
    assert(capture.count == 1u && capture.events[0].type == UX_EVENT_HOTKEY);
    assert(strcmp(capture.events[0].data.hotkey.identifier,
        "pause-toggle") == 0);
    event.data.key.pressed = 0u;
    assert(ux_hotkey_matcher_submit(&matcher, &event, ux_capture_event,
        &capture));
    event.data.key.virtual_key = UX_HOTKEY_KEY_ALT;
    assert(ux_hotkey_matcher_submit(&matcher, &event, ux_capture_event,
        &capture));
    event.data.key.virtual_key = UX_HOTKEY_KEY_CONTROL;
    event.data.key.hotkey_modifiers = 0u;
    assert(ux_hotkey_matcher_submit(&matcher, &event, ux_capture_event,
        &capture));
    assert(capture.count == 1u);
    free(frame);
    return 0;
}
