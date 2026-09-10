#include "lib/ux-base/win32/input.h"
#include "lib/ux-base/hotkey_interface.h"

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

static int capture_key(void *context, const ux_event *event)
{
    softpc_keyboard_capture *capture = (softpc_keyboard_capture *)context;
    if (event == NULL || event->type != UX_EVENT_KEY ||
        capture->count == sizeof(capture->keys)) return 0;
    capture->keys[capture->count] = (uint8_t)event->data.key.scan_code;
    capture->releases[capture->count] = (uint8_t)!event->data.key.pressed;
    capture->modifiers[capture->count] = event->data.key.modifiers;
    capture->identities[capture->count] = event->data.key.key;
    capture->flags[capture->count++] = event->data.key.flags;
    return 1;
}

typedef struct softpc_hotkey_capture {
    ux_hotkey_matcher matcher;
    ux_event events[8];
    unsigned int count;
} softpc_hotkey_capture;

static int capture_hotkey(void *context, const ux_event *event)
{
    softpc_hotkey_capture *capture = (softpc_hotkey_capture *)context;
    if (capture == NULL || event == NULL || capture->count == 8u) return 0;
    capture->events[capture->count++] = *event;
    return 1;
}

static int normalize_and_match(void *context, const ux_event *event)
{
    softpc_hotkey_capture *capture = (softpc_hotkey_capture *)context;
    return capture != NULL && ux_hotkey_matcher_submit(&capture->matcher,
        event, capture_hotkey, capture);
}

static void assert_registered_raw_chord(lib_u32 trigger, const char *identifier)
{
    ux_hotkey_registry registry;
    softpc_hotkey_capture capture = { 0 };
    const lib_u8 control_alt = UX_HOTKEY_MODIFIER_CONTROL |
        UX_HOTKEY_MODIFIER_ALT;

    ux_hotkey_registry_initialize(&registry);
    assert(ux_hotkey_registry_register(&registry, trigger, control_alt,
        identifier) == LIB_STATUS_OK);
    ux_hotkey_matcher_initialize(&capture.matcher, &registry);
    assert(ux_win32_keyboard_submit_transition(&capture, normalize_and_match,
        0x1du, VK_CONTROL, 0u, UX_HOTKEY_MODIFIER_CONTROL, 1));
    assert(ux_win32_keyboard_submit_transition(&capture, normalize_and_match,
        0x38u, VK_MENU, 0u, control_alt, 1));
    assert(ux_win32_keyboard_submit_transition(&capture, normalize_and_match,
        (WORD)MapVirtualKeyW((UINT)trigger, MAPVK_VK_TO_VSC), (WORD)trigger,
        0u, control_alt, 1));
    assert(capture.count == 1u && capture.events[0].type == UX_EVENT_HOTKEY);
    assert(strcmp(capture.events[0].data.hotkey.identifier, identifier) == 0);
    /* Every make and break in the matched raw chord is private to UX. */
    assert(ux_win32_keyboard_submit_transition(&capture, normalize_and_match,
        (WORD)MapVirtualKeyW((UINT)trigger, MAPVK_VK_TO_VSC), (WORD)trigger,
        0u, control_alt, 0));
    assert(ux_win32_keyboard_submit_transition(&capture, normalize_and_match,
        0x38u, VK_MENU, 0u, UX_HOTKEY_MODIFIER_CONTROL, 0));
    assert(ux_win32_keyboard_submit_transition(&capture, normalize_and_match,
        0x1du, VK_CONTROL, 0u, 0u, 0));
    assert(capture.count == 1u);
}

int main(void)
{
    softpc_keyboard_capture capture = { 0 };
    ux_win32_keyboard_normalizer normalizer = { 0 };

    /* The shared component preserves the host physical scan; each project maps it. */
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0x1eu, 'A', 0u, 0u, 1));
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0x1eu, 'A', 0u, 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x1eu && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x1eu && capture.releases[1] == 1u);
    assert(capture.identities[0] == 'A' && capture.identities[1] == 'A');

    /* Esc is an ordinary original key-table entry (key 110), not a host
       stop command. */
    capture.count = 0u;
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0x01u, VK_ESCAPE, 0u, 0u, 1));
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0x01u, VK_ESCAPE, 0u, 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x01u && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x01u && capture.releases[1] == 1u);

    /* A raw Console record from RDP can retain VK_RETURN while reporting no
       physical scan.  Both UX leaves use this shared recovery path, so the
       guest still receives the normal Enter make/break pair. */
    capture.count = 0u;
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0u, VK_RETURN, 0u, 0u, 1));
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0u, VK_RETURN, 0u, 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x1cu && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x1cu && capture.releases[1] == 1u);
    assert(capture.identities[0] == UX_KEY_ENTER &&
        capture.identities[1] == UX_KEY_ENTER);

    /* Extended state is a neutral UX flag, not a copied Win32 control bit. */
    capture.count = 0u;
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0xe04du, VK_RIGHT, ENHANCED_KEY, 0u, 1));
    assert(capture.count == 1u && capture.identities[0] == UX_KEY_RIGHT);
    assert(capture.flags[0] == UX_KEY_FLAG_EXTENDED);

    /* Raw Console input provides its own per-record modifier state.  These
       registrations must not depend on process-global GetKeyState(), which
       RDP does not reliably update for INPUT_RECORD delivery. */
    assert_registered_raw_chord('P', "pause-toggle");
    assert_registered_raw_chord('D', "send-ctrl-alt-del");
    assert_registered_raw_chord('F', "send-alt-enter");

    /* Window supplies a mask at its own native boundary; the common
       normalizer preserves that value rather than replacing it globally. */
    capture.count = 0u;
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0x19u, 'P', 0u, UX_HOTKEY_MODIFIER_CONTROL |
        UX_HOTKEY_MODIFIER_ALT, 1));
    assert(capture.count == 1u);
    assert(capture.modifiers[0] == (UX_HOTKEY_MODIFIER_CONTROL |
        UX_HOTKEY_MODIFIER_ALT));

    /* A scan-less RDP key followed by its WM_CHAR must not inject twice. */
    ux_win32_keyboard_note_recovered_key(&normalizer, 'A');
    assert(ux_win32_keyboard_consume_duplicate_character(&normalizer,
        L'a'));
    assert(!ux_win32_keyboard_consume_duplicate_character(&normalizer,
        L'a'));

    /* UTF-16 input uses the active host layout to synthesize make/break;
       it never places text directly in guest memory. */
    capture.count = 0u;
    assert(ux_win32_keyboard_submit_utf16(&normalizer, &capture,
        capture_key, L'a'));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x1eu && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x1eu && capture.releases[1] == 1u);
    return 0;
}
#else
int main(void) { return 0; }
#endif
