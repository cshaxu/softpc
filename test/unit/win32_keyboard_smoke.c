#include "../../src/lib/platform/win32/input.h"

#include <assert.h>

#ifdef _WIN32
typedef struct softpc_keyboard_capture {
    uint8_t keys[16];
    uint8_t releases[16];
    unsigned int count;
} softpc_keyboard_capture;

static int capture_key(void *context, const KEY_EVENT_RECORD *event)
{
    softpc_keyboard_capture *capture = (softpc_keyboard_capture *)context;
    if (event == NULL || capture->count == sizeof(capture->keys)) return 0;
    capture->keys[capture->count] = (uint8_t)event->wVirtualScanCode;
    capture->releases[capture->count++] = (uint8_t)!event->bKeyDown;
    return 1;
}

int main(void)
{
    softpc_keyboard_capture capture = { { 0 }, { 0 }, 0u };
    win32_presentation_keyboard_normalizer normalizer = { 0 };

    /* The shared component preserves the host physical scan; each project maps it. */
    assert(win32_presentation_keyboard_submit_transition(&capture, capture_key,
        0x1eu, 'A', 0u, 1));
    assert(win32_presentation_keyboard_submit_transition(&capture, capture_key,
        0x1eu, 'A', 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x1eu && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x1eu && capture.releases[1] == 1u);

    /* Ctrl+Alt+D is a frontend shortcut, but it must remain six ordinary
       transitions through the original key table rather than a VM reset
       side channel. */
    capture.count = 0u;
    assert(win32_presentation_keyboard_submit_ctrl_alt_del(&capture, capture_key));
    assert(capture.count == 6u);
    assert(capture.keys[0] == 0x1du && capture.keys[1] == 0x38u &&
        capture.keys[2] == 0x53u && capture.keys[3] == 0x53u &&
        capture.keys[4] == 0x38u && capture.keys[5] == 0x1du &&
        capture.releases[0] == 0u && capture.releases[1] == 0u &&
        capture.releases[2] == 0u && capture.releases[3] == 1u &&
        capture.releases[4] == 1u && capture.releases[5] == 1u);

    capture.count = 0u;
    assert(win32_presentation_keyboard_release_ctrl_alt(&capture, capture_key));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x1du && capture.releases[0] == 1u);
    assert(capture.keys[1] == 0x38u && capture.releases[1] == 1u);

    capture.count = 0u;
    assert(win32_presentation_keyboard_submit_alt_enter(&capture, capture_key));
    /* Ctrl+Alt+F has already produced host Ctrl/Alt makes.  The replacement
       must clear them before creating a distinct guest Alt+Enter, never send
       guest Ctrl+Alt+Enter. */
    assert(capture.count == 6u);
    assert(capture.keys[0] == 0x1du && capture.releases[0] == 1u);
    assert(capture.keys[1] == 0x38u && capture.releases[1] == 1u);
    assert(capture.keys[2] == 0x38u && capture.releases[2] == 0u);
    assert(capture.keys[3] == 0x1cu && capture.releases[3] == 0u);
    assert(capture.keys[4] == 0x1cu && capture.releases[4] == 1u);
    assert(capture.keys[5] == 0x38u && capture.releases[5] == 1u);

    /* Esc is an ordinary original key-table entry (key 110), not a host
       stop command. */
    capture.count = 0u;
    assert(win32_presentation_keyboard_submit_transition(&capture, capture_key,
        0x01u, VK_ESCAPE, 0u, 1));
    assert(win32_presentation_keyboard_submit_transition(&capture, capture_key,
        0x01u, VK_ESCAPE, 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x01u && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x01u && capture.releases[1] == 1u);

    /* A scan-less RDP key followed by its WM_CHAR must not inject twice. */
    win32_presentation_keyboard_note_recovered_key(&normalizer, 'A');
    assert(win32_presentation_keyboard_consume_duplicate_character(&normalizer,
        L'a'));
    assert(!win32_presentation_keyboard_consume_duplicate_character(&normalizer,
        L'a'));

    /* UTF-16 input uses the active host layout to synthesize make/break;
       it never places text directly in guest memory. */
    capture.count = 0u;
    assert(win32_presentation_keyboard_submit_utf16(&normalizer, &capture,
        capture_key, L'a'));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 0x1eu && capture.releases[0] == 0u);
    assert(capture.keys[1] == 0x1eu && capture.releases[1] == 1u);
    return 0;
}
#else
int main(void) { return 0; }
#endif
