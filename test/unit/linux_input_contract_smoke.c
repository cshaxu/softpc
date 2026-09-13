#include "lib/ui-base/input_interface.h"
#include "lib/ui-base/linux/input.h"
#include <assert.h>

static ui_input_event received;
static int capture(void *context, const ui_input_event *event)
{ (void)context; received = *event; return 1; }
int main(void)
{
    ui_keyboard_normalizer state = { 0 };
    assert(ui_keyboard_submit_transition(NULL, capture, 0u, UI_LINUI_KEY_UP,
        0u, 0u, 1));
    assert(received.type == UI_EVENT_KEY && received.data.key.key == UI_KEY_UP &&
        received.data.key.scan_code == 0u);
    assert(ui_keyboard_submit_utf16(&state, NULL, NULL, capture, 'a'));
    assert(received.type == UI_EVENT_TEXT && received.data.text.scalar == 'a');
    assert(ui_keyboard_submit_utf16(&state, NULL, NULL, capture, 0xd83du));
    assert(ui_keyboard_submit_utf16(&state, NULL, NULL, capture, 0xde00u));
    assert(received.type == UI_EVENT_TEXT && received.data.text.scalar == 0x1f600u);
    assert(!ui_keyboard_submit_transition(NULL, capture, 0u, 0xffffu, 0u, 0u, 1));
    return 0;
}
