#include "lib/ux/win32/input.h"

#include <assert.h>

#ifdef _WIN32
typedef struct softpc_keyboard_capture {
    uint8_t keys[16];
    uint8_t releases[16];
    unsigned int count;
} softpc_keyboard_capture;

static int capture_key(void *context, const ux_event *event)
{
    softpc_keyboard_capture *capture = (softpc_keyboard_capture *)context;
    if (event == NULL || event->type != UX_EVENT_KEY ||
        capture->count == sizeof(capture->keys)) return 0;
    capture->keys[capture->count] = (uint8_t)event->data.key.scan_code;
    capture->releases[capture->count++] = (uint8_t)!event->data.key.pressed;
    return 1;
}

int main(void)
{
    softpc_keyboard_capture capture = { { 0 }, { 0 }, 0u };
    ux_win32_keyboard_normalizer normalizer = { 0 };

    /* The shared component preserves the host physical scan; each project maps it. */
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0x1eu, 'A', 0u, 1));
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0x1eu, 'A', 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x1eu && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x1eu && capture.releases[1] == 1u);

    /* Esc is an ordinary original key-table entry (key 110), not a host
       stop command. */
    capture.count = 0u;
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0x01u, VK_ESCAPE, 0u, 1));
    assert(ux_win32_keyboard_submit_transition(&capture, capture_key,
        0x01u, VK_ESCAPE, 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x01u && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x01u && capture.releases[1] == 1u);

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
