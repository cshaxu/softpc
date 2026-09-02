#include "win32_keyboard.h"

#include <assert.h>

#ifdef _WIN32
typedef struct softpc_keyboard_capture {
    uint8_t keys[16];
    uint8_t releases[16];
    unsigned int count;
} softpc_keyboard_capture;

static int capture_key(void *context, uint8_t key_number, uint8_t released)
{
    softpc_keyboard_capture *capture = (softpc_keyboard_capture *)context;
    if (capture->count == sizeof(capture->keys)) return 0;
    capture->keys[capture->count] = key_number;
    capture->releases[capture->count++] = released;
    return 1;
}

int main(void)
{
    softpc_keyboard_capture capture = { { 0 }, { 0 }, 0u };
    softpc_win32_keyboard_normalizer normalizer = { 0 };

    /* The original nt_keycd table, not this outer adapter, assigns 31 to A. */
    assert(softpc_win32_keyboard_submit_transition(&capture, capture_key,
        0x1eu, 'A', 0u, 1));
    assert(softpc_win32_keyboard_submit_transition(&capture, capture_key,
        0x1eu, 'A', 0u, 0));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 31u && capture.releases[0] == 0u);
    assert(capture.keys[1] == 31u && capture.releases[1] == 1u);

    /* A scan-less RDP key followed by its WM_CHAR must not inject twice. */
    softpc_win32_keyboard_note_recovered_key(&normalizer, 'A');
    assert(softpc_win32_keyboard_consume_duplicate_character(&normalizer,
        L'a'));
    assert(!softpc_win32_keyboard_consume_duplicate_character(&normalizer,
        L'a'));

    /* UTF-16 input uses the active host layout to synthesize make/break;
       it never places text directly in guest memory. */
    capture.count = 0u;
    assert(softpc_win32_keyboard_submit_utf16(&normalizer, &capture,
        capture_key, L'a'));
    assert(capture.count == 2u);
    assert(capture.keys[0] == 31u && capture.releases[0] == 0u);
    assert(capture.keys[1] == 31u && capture.releases[1] == 1u);
    return 0;
}
#else
int main(void) { return 0; }
#endif
