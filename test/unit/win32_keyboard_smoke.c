#include "lib/ui-base/input_interface.h"
#include "lib/ui-base/hotkey_interface.h"
#include "lib/types/win32/input.h"

#include <assert.h>
#include <string.h>

#ifdef _WIN32
typedef struct softpc_keyboard_capture {
    uint8_t keys[16];
    uint8_t releases[16];
    uint8_t modifiers[16];
    lib_u32 identities[16];
    lib_u32 flags[16];
    unsigned int count;
} softpc_keyboard_capture;

static int capture_key(void *context, const ui_input_event *event)
{
    softpc_keyboard_capture *capture = (softpc_keyboard_capture *)context;
    if (event == NULL || event->type != UI_EVENT_KEY ||
        capture->count == sizeof(capture->keys)) return 0;
    capture->keys[capture->count] = (uint8_t)event->data.key.scan_code;
    capture->releases[capture->count] = (uint8_t)!event->data.key.pressed;
    capture->modifiers[capture->count] = event->data.key.modifiers;
    capture->identities[capture->count] = event->data.key.key;
    capture->flags[capture->count++] = event->data.key.flags;
    return 1;
}

typedef struct softpc_hotkey_capture {
    ui_hotkey_matcher matcher;
    ui_input_event events[8];
    unsigned int count;
} softpc_hotkey_capture;

static int capture_hotkey(void *context, const ui_input_event *event)
{
    softpc_hotkey_capture *capture = (softpc_hotkey_capture *)context;
    if (capture == NULL || event == NULL || capture->count == 8u) return 0;
    capture->events[capture->count++] = *event;
    return 1;
}

static int normalize_and_match(void *context, const ui_input_event *event)
{
    softpc_hotkey_capture *capture = (softpc_hotkey_capture *)context;
    return capture != NULL && ui_hotkey_matcher_submit(&capture->matcher,
        event, capture_hotkey, capture, LIB_TRUE);
}

static void assert_registered_raw_chord(lib_u32 trigger, const char *identifier)
{
    ui_hotkey_registry registry;
    softpc_hotkey_capture capture = { 0 };
    const lib_u8 control_alt = UI_HOTKEY_MODIFIER_CONTROL |
        UI_HOTKEY_MODIFIER_ALT;

    ui_hotkey_registry_initialize(&registry);
    assert(ui_hotkey_registry_register(&registry, trigger, control_alt,
        identifier) == LIB_STATUS_OK);
    ui_hotkey_matcher_initialize(&capture.matcher, &registry);
    assert(ui_keyboard_submit_transition(&capture, normalize_and_match,
        0x1du, LIB_WIN32_KEY_CONTROL, 0u, UI_HOTKEY_MODIFIER_CONTROL, 1));
    assert(ui_keyboard_submit_transition(&capture, normalize_and_match,
        0x38u, LIB_WIN32_KEY_ALT, 0u, control_alt, 1));
    assert(ui_keyboard_submit_transition(&capture, normalize_and_match,
        (lib_u16)lib_win32_map_virtual_key((UINT)trigger, MAPVK_VK_TO_VSC), (lib_u16)trigger,
        0u, control_alt, 1));
    assert(capture.count == 1u && capture.events[0].type == UI_EVENT_HOTKEY);
    assert(strcmp(capture.events[0].data.hotkey.identifier, identifier) == 0);
    /* Every make and break in the matched raw chord is private to UI. */
    assert(ui_keyboard_submit_transition(&capture, normalize_and_match,
        (lib_u16)lib_win32_map_virtual_key((UINT)trigger, MAPVK_VK_TO_VSC), (lib_u16)trigger,
        0u, control_alt, 0));
    assert(ui_keyboard_submit_transition(&capture, normalize_and_match,
        0x38u, LIB_WIN32_KEY_ALT, 0u, UI_HOTKEY_MODIFIER_CONTROL, 0));
    assert(ui_keyboard_submit_transition(&capture, normalize_and_match,
        0x1du, LIB_WIN32_KEY_CONTROL, 0u, 0u, 0));
    assert(capture.count == 1u);
    ui_hotkey_matcher_discard(&capture.matcher);
}

int main(void)
{
    softpc_keyboard_capture capture = { 0 };
    ui_keyboard_normalizer normalizer = { 0 };
    softpc_hotkey_capture text = { 0 };
    assert(ui_keyboard_submit_utf16(&normalizer, NULL, &text, capture_hotkey, 0xd83du, 1u));
    assert(text.count == 0u);
    assert(ui_keyboard_submit_utf16(&normalizer, NULL, &text, capture_hotkey, 0xde00u, 1u));
    assert(text.count == 1u && text.events[0].type == UI_EVENT_TEXT &&
        text.events[0].data.text.scalar == 0x1f600u);
    assert(!ui_keyboard_submit_utf16(&normalizer, NULL, &text, capture_hotkey, 0xdc00u, 1u));
    assert(ui_keyboard_submit_utf16(&normalizer, NULL, &text, capture_hotkey, 0xd800u, 1u));
    assert(ui_keyboard_submit_utf16(&normalizer, NULL, &text, capture_hotkey, 0xd800u, 1u));
    assert(normalizer.pending_high_surrogate == 0xd800u);
    assert(ui_keyboard_submit_utf16(&normalizer, NULL, &text, capture_hotkey, 0xd800u, 1u));
    assert(ui_keyboard_submit_utf16(&normalizer, NULL, &text, capture_hotkey, 'a', 1u));
    assert(ui_keyboard_submit_utf16(&normalizer, NULL, &text, capture_hotkey, 0x4e00u, 1u));
    text.count = 8u;
    assert(!ui_keyboard_submit_utf16(&normalizer, NULL, &text, capture_hotkey, 0x4e00u, 1u));

    /* The shared component preserves the host physical scan; each project maps it. */
    assert(ui_keyboard_submit_transition(&capture, capture_key,
        0x1eu, 'A', 0u, 0u, 1));
    assert(ui_keyboard_submit_transition(&capture, capture_key,
        0x1eu, 'A', 0u, 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x1eu && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x1eu && capture.releases[1] == 1u);
    assert(capture.identities[0] == 'A' && capture.identities[1] == 'A');

    /* Esc is an ordinary original key-table entry (key 110), not a host
       stop command. */
    capture.count = 0u;
    assert(ui_keyboard_submit_transition(&capture, capture_key,
        0x01u, LIB_WIN32_KEY_ESCAPE, 0u, 0u, 1));
    assert(ui_keyboard_submit_transition(&capture, capture_key,
        0x01u, LIB_WIN32_KEY_ESCAPE, 0u, 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x01u && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x01u && capture.releases[1] == 1u);

    /* A raw Console record from RDP can retain VK_RETURN while reporting no
       physical scan.  Both UI leaves use this shared recovery path, so the
       guest still receives the normal Enter make/break pair. */
    capture.count = 0u;
    assert(ui_keyboard_submit_transition(&capture, capture_key,
        0u, LIB_WIN32_KEY_RETURN, 0u, 0u, 1));
    assert(ui_keyboard_submit_transition(&capture, capture_key,
        0u, LIB_WIN32_KEY_RETURN, 0u, 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x1cu && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x1cu && capture.releases[1] == 1u);
    assert(capture.identities[0] == UI_KEY_ENTER &&
        capture.identities[1] == UI_KEY_ENTER);

    /* Extended state is a neutral UI flag, not a copied Win32 control bit. */
    capture.count = 0u;
    assert(ui_keyboard_submit_transition(&capture, capture_key,
        0xe04du, LIB_WIN32_KEY_RIGHT, UI_INPUT_FLAG_EXTENDED, 0u, 1));
    assert(capture.count == 1u && capture.identities[0] == UI_KEY_RIGHT);
    assert(capture.flags[0] == UI_KEY_FLAG_EXTENDED);
    assert(capture.keys[0] == 0x4du);
    assert(ui_keyboard_submit_transition(&capture, capture_key,
        0x4du, LIB_WIN32_KEY_RIGHT, UI_INPUT_FLAG_EXTENDED, 0u, 1));
    assert(capture.keys[1] == capture.keys[0] && capture.flags[1] == capture.flags[0]);

    /* Raw Console input provides its own per-record modifier state.  These
       registrations must not depend on process-global GetKeyState(), which
       RDP does not reliably update for INPUT_RECORD delivery. */
    assert_registered_raw_chord('P', "pause-toggle");
    assert_registered_raw_chord('D', "send-ctrl-alt-del");
    assert_registered_raw_chord('F', "send-alt-enter");

    /* Window supplies a mask at its own native boundary; the common
       normalizer preserves that value rather than replacing it globally. */
    capture.count = 0u;
    assert(ui_keyboard_submit_transition(&capture, capture_key,
        0x19u, 'P', 0u, UI_HOTKEY_MODIFIER_CONTROL |
        UI_HOTKEY_MODIFIER_ALT, 1));
    assert(capture.count == 1u);
    assert(capture.modifiers[0] == (UI_HOTKEY_MODIFIER_CONTROL |
        UI_HOTKEY_MODIFIER_ALT));

    /* Only unmapped transitions ask the native adapter for character translation. */
    capture.count = 0;
    ui_keyboard_record record = { UI_KEYBOARD_TRANSITION, 0, 'A', 0, 0, 0, 1 };
    assert(ui_keyboard_submit_record(&normalizer, NULL, &capture, capture_key, &record) ==
        UI_KEYBOARD_ACCEPTED);
    record.pressed = 0;
    assert(ui_keyboard_submit_record(&normalizer, NULL, &capture, capture_key, &record) ==
        UI_KEYBOARD_ACCEPTED);
    assert(capture.count == 2);
    record.key = VK_PACKET; record.pressed = 1;
    assert(ui_keyboard_submit_record(&normalizer, NULL, &capture, capture_key, &record) ==
        UI_KEYBOARD_UNMAPPED);
    assert(capture.count == 2);

    /* UTF-16 input uses the active host layout to synthesize make/break;
       it never places text directly in guest memory. */
    capture.count = 0u;
    assert(ui_keyboard_submit_utf16(&normalizer, NULL, &capture,
        capture_key, L'a', 1u));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x1eu && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x1eu && capture.releases[1] == 1u);
    return 0;
}
#else
int main(void) { return 0; }
#endif
