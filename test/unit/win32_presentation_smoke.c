#include "lib/ui-base/frame_interface.h"
#include "lib/ui-base/hotkey_interface.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct ui_capture {
    ui_input_event events[16];
    unsigned int count;
} ui_capture;

static int ui_capture_event(void *opaque, const ui_input_event *event)
{
    ui_capture *capture = (ui_capture *)opaque;
    if (capture == NULL || event == NULL || capture->count == 16u) return 0;
    capture->events[capture->count++] = *event;
    return 1;
}

int main(void)
{
    ui_frame *frame = calloc(1u, sizeof(*frame));
    ui_hotkey_registry registry;
    ui_hotkey_matcher matcher;
    ui_capture capture = { 0 };
    ui_input_event event = { 0 };

    assert(frame != NULL);
    frame->valid = 1u;
    frame->graphics = 0u;
    frame->text_columns = 80u;
    frame->text_rows = 25u;
    frame->text[0] = 'X';
    assert(ui_frame_is_valid(frame));

    ui_hotkey_registry_initialize(&registry);
    assert(ui_hotkey_registry_register(&registry, 'P',
        UI_HOTKEY_MODIFIER_CONTROL | UI_HOTKEY_MODIFIER_ALT,
        "pause-toggle") == LIB_STATUS_OK);
    ui_hotkey_matcher_initialize(&matcher, &registry);
    event.type = UI_EVENT_KEY;
    event.data.key.key = UI_HOTKEY_KEY_CONTROL;
    event.data.key.scan_code = 0x1du;
    event.data.key.pressed = 1u;
    event.data.key.modifiers = UI_HOTKEY_MODIFIER_CONTROL;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event,
        &capture));
    event.data.key.key = UI_HOTKEY_KEY_ALT;
    event.data.key.scan_code = 0x38u;
    event.data.key.modifiers = UI_HOTKEY_MODIFIER_CONTROL |
        UI_HOTKEY_MODIFIER_ALT;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event,
        &capture));
    event.type = UI_EVENT_KEY;
    event.data.key.key = 'P';
    event.data.key.scan_code = 0x19u;
    event.data.key.pressed = 1u;
    event.data.key.modifiers = UI_HOTKEY_MODIFIER_CONTROL |
        UI_HOTKEY_MODIFIER_ALT;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event,
        &capture));
    assert(capture.count == 1u && capture.events[0].type == UI_EVENT_HOTKEY);
    assert(strcmp(capture.events[0].data.hotkey.identifier,
        "pause-toggle") == 0);
    /* Auto-repeat, then a second press while Ctrl/Alt stay held. Neither
       operation may forget the outstanding modifier breaks. */
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    event.data.key.pressed = 0u;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    event.data.key.pressed = 1u;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    assert(capture.count == 3u);
    assert(capture.events[1].type == UI_EVENT_HOTKEY);
    assert(capture.events[2].type == UI_EVENT_HOTKEY);
    event.data.key.pressed = 0u;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event,
        &capture));
    event.data.key.key = UI_HOTKEY_KEY_ALT;
    event.data.key.scan_code = 0x38u;
    event.data.key.pressed = 1u;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    assert(matcher.pending_count == 0u); /* Held modifier repeat is consumed. */
    event.data.key.pressed = 0u;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event,
        &capture));
    event.data.key.key = UI_HOTKEY_KEY_CONTROL;
    event.data.key.scan_code = 0x1du;
    event.data.key.modifiers = 0u;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event,
        &capture));
    assert(capture.count == 3u && matcher.suppressed_count == 0u);
    /* Both physical Ctrl keys use VK_CONTROL, but a matched chord must
       suppress both breaks rather than leaking the second to the guest. */
    capture.count = 0u;
    ui_hotkey_matcher_initialize(&matcher, &registry);
    event.type = UI_EVENT_KEY;
    event.data.key.key = UI_HOTKEY_KEY_CONTROL;
    event.data.key.scan_code = 0x1du;
    event.data.key.pressed = 1u;
    event.data.key.modifiers = UI_HOTKEY_MODIFIER_CONTROL;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    event.data.key.scan_code = 0x11du;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    event.data.key.key = UI_HOTKEY_KEY_ALT;
    event.data.key.scan_code = 0x38u;
    event.data.key.modifiers = UI_HOTKEY_MODIFIER_CONTROL |
        UI_HOTKEY_MODIFIER_ALT;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    event.data.key.key = 'P';
    event.data.key.scan_code = 0x19u;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    assert(capture.count == 1u && capture.events[0].type == UI_EVENT_HOTKEY);
    event.data.key.pressed = 0u;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    event.data.key.key = UI_HOTKEY_KEY_ALT;
    event.data.key.scan_code = 0x38u;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    event.data.key.key = UI_HOTKEY_KEY_CONTROL;
    event.data.key.scan_code = 0x1du;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    event.data.key.scan_code = 0x11du;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    assert(capture.count == 1u);
    /* An uncompleted registered prefix is never swallowed: the original
       modifier and the mismatching key replay in their source order. */
    capture.count = 0u;
    ui_hotkey_matcher_initialize(&matcher, &registry);
    event.type = UI_EVENT_KEY;
    event.data.key.key = UI_HOTKEY_KEY_CONTROL;
    event.data.key.pressed = 1u;
    event.data.key.modifiers = UI_HOTKEY_MODIFIER_CONTROL;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    event.data.key.key = 'X';
    event.data.key.modifiers = UI_HOTKEY_MODIFIER_CONTROL;
    assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    assert(capture.count == 2u);
    assert(capture.events[0].data.key.key == UI_HOTKEY_KEY_CONTROL);
    assert(capture.events[1].data.key.key == 'X');
    /* Saturation rejects the next chord; it never overwrites a held key. */
    capture.count = 0u;
    ui_hotkey_matcher_initialize(&matcher, &registry);
    event.data.key.key = 'P';
    event.data.key.modifiers = UI_HOTKEY_MODIFIER_CONTROL | UI_HOTKEY_MODIFIER_ALT;
    for (unsigned int i = 0; i < UI_HOTKEY_SUPPRESSED_CAPACITY; ++i) {
        event.data.key.scan_code = (lib_u16)(i + 1u);
        assert(ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    }
    event.data.key.scan_code = UI_HOTKEY_SUPPRESSED_CAPACITY + 1u;
    assert(!ui_hotkey_matcher_submit(&matcher, &event, ui_capture_event, &capture));
    assert(matcher.suppressed_count == UI_HOTKEY_SUPPRESSED_CAPACITY);
    assert(capture.count == UI_HOTKEY_SUPPRESSED_CAPACITY);
    free(frame);
    return 0;
}
