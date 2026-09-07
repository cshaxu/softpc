#include "lib/ux-base/input.h"

#include <assert.h>

typedef struct captured_events {
    ux_input_event items[8u];
    lib_u32 count;
} captured_events;

static lib_status capture_event(void *context, const ux_input_event *event)
{
    captured_events *captured = context;
    assert(captured != NULL && event != NULL && captured->count < 8u);
    captured->items[captured->count++] = *event;
    return LIB_STATUS_OK;
}

int main(void)
{
    ux_hotkey_registration registration = { 'P', UX_MODIFIER_CONTROL |
        UX_MODIFIER_ALT, "pause-toggle" };
    ux_hotkey_matcher *matcher = NULL;
    ux_hotkey_matcher *other_matcher = NULL;
    ux_input_event event;
    captured_events captured = { 0 };
    int source;
    int other_source;

    assert(ux_hotkey_matcher_create(&matcher, &registration, 1u) == LIB_STATUS_OK);
    assert(ux_input_make_key(&event, &source, 0u, UX_KEY_CONTROL,
        UX_MODIFIER_CONTROL, LIB_TRUE) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_submit(matcher, &event, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(captured.count == 0u);
    assert(ux_input_make_key(&event, &source, 0u, UX_KEY_ALT,
        UX_MODIFIER_CONTROL | UX_MODIFIER_ALT, LIB_TRUE) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_submit(matcher, &event, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(ux_input_make_key(&event, &source, 0u, 'P',
        UX_MODIFIER_CONTROL | UX_MODIFIER_ALT, LIB_TRUE) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_submit(matcher, &event, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(captured.count == 1u && captured.items[0].kind == UX_INPUT_HOTKEY);
    assert(strcmp(captured.items[0].value.hotkey.identifier, "pause-toggle") == 0);
    assert(captured.items[0].source_handle == &source);
    assert(ux_input_make_key(&event, &source, 0u, 'P',
        UX_MODIFIER_CONTROL | UX_MODIFIER_ALT, LIB_FALSE) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_submit(matcher, &event, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(ux_input_make_key(&event, &source, 0u, UX_KEY_ALT,
        UX_MODIFIER_CONTROL, LIB_FALSE) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_submit(matcher, &event, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(ux_input_make_key(&event, &source, 0u, UX_KEY_CONTROL, 0u,
        LIB_FALSE) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_submit(matcher, &event, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(captured.count == 1u);

    assert(ux_input_make_key(&event, &source, 0u, UX_KEY_CONTROL,
        UX_MODIFIER_CONTROL, LIB_TRUE) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_submit(matcher, &event, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(ux_input_make_key(&event, &source, 0u, 'X', UX_MODIFIER_CONTROL,
        LIB_TRUE) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_submit(matcher, &event, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(captured.count == 3u);
    assert(captured.items[1].value.key.virtual_key == UX_KEY_CONTROL);
    assert(captured.items[2].value.key.virtual_key == 'X');
    assert(ux_hotkey_matcher_retire(matcher, &source, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(captured.count == 4u && captured.items[3].kind == UX_INPUT_RESET);
    ux_hotkey_matcher_destroy(matcher);

    captured.count = 0u;
    assert(ux_hotkey_matcher_create(&matcher, &registration, 1u) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_create(&other_matcher, &registration, 1u) ==
        LIB_STATUS_OK);
    /* Source-local state is not a shared modifier bitmap: Ctrl from one
     * component and P from another must stay ordinary input. */
    assert(ux_input_make_key(&event, &source, 0u, UX_KEY_CONTROL,
        UX_MODIFIER_CONTROL, LIB_TRUE) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_submit(matcher, &event, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(ux_input_make_key(&event, &other_source, 0u, 'P',
        UX_MODIFIER_CONTROL | UX_MODIFIER_ALT, LIB_TRUE) == LIB_STATUS_OK);
    assert(ux_hotkey_matcher_submit(other_matcher, &event, capture_event,
        &captured) == LIB_STATUS_OK);
    assert(captured.count == 1u && captured.items[0].kind == UX_INPUT_KEY);
    assert(captured.items[0].source_handle == &other_source);
    assert(ux_hotkey_matcher_retire(matcher, &source, capture_event, &captured) ==
        LIB_STATUS_OK);
    assert(captured.count == 3u && captured.items[1].source_handle == &source &&
        captured.items[2].kind == UX_INPUT_RESET);
    ux_hotkey_matcher_destroy(other_matcher);
    ux_hotkey_matcher_destroy(matcher);
    return 0;
}
