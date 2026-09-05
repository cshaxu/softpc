#include "keyboard.h"

#ifdef _WIN32
#include "runtime.h"

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD key_event);

int app_keyboard_enqueue_win32_event(void *context,
    const KEY_EVENT_RECORD *event)
{
    KEY_EVENT_RECORD copy;
    BYTE key_number;

    if (context == NULL || event == NULL) return 0;
    copy = *event;
    key_number = KeyMsgToKeyCode(&copy);
    return key_number != 0u && app_runtime_enqueue_key(
        (app_runtime *)context, key_number, (uint8_t)!copy.bKeyDown);
}
#endif
