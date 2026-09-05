#include "keyboard.h"

#ifdef _WIN32
#include "runtime.h"

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD key_event);

int app_keyboard_deliver_input(void *context,
    const win32_presentation_event *event)
{
    return context != NULL && event != NULL && app_runtime_enqueue_input_event(
        (app_runtime *)context, event);
}

int app_keyboard_inject_machine_event(softpc_machine *machine,
    const win32_presentation_event *event)
{
    KEY_EVENT_RECORD copy;
    BYTE key_number;

    if (machine == NULL || event == NULL ||
        event->type != WIN32_PRESENTATION_EVENT_KEY) return 0;
    ZeroMemory(&copy, sizeof(copy));
    copy.bKeyDown = event->data.key.pressed != 0;
    copy.wVirtualKeyCode = event->data.key.virtual_key;
    copy.wVirtualScanCode = (WORD)(event->data.key.scan_code & 0xffu);
    copy.dwControlKeyState = event->data.key.modifiers;
    if ((event->data.key.scan_code & 0x0100u) != 0u)
        copy.dwControlKeyState |= ENHANCED_KEY;
    key_number = KeyMsgToKeyCode(&copy);
    return key_number != 0u && softpc_machine_key_number(machine, key_number,
        (uint8_t)!copy.bKeyDown) == SOFTPC_MACHINE_OK;
}

int app_keyboard_register_actions(
    win32_presentation_action_registry *registry)
{
    const uint8_t modifiers = WIN32_PRESENTATION_MODIFIER_CONTROL |
        WIN32_PRESENTATION_MODIFIER_ALT;

    if (registry == NULL) return 0;
    win32_presentation_actions_init(registry);
    return win32_presentation_actions_register(registry, 'P', modifiers,
        WIN32_PRESENTATION_ACTION_PAUSE_TOGGLE) &&
        win32_presentation_actions_register(registry, 'D', modifiers,
            WIN32_PRESENTATION_ACTION_SEND_CTRL_ALT_DEL) &&
        win32_presentation_actions_register(registry, 'F', modifiers,
            WIN32_PRESENTATION_ACTION_SEND_ALT_ENTER) &&
        win32_presentation_actions_register(registry, 'M', modifiers,
            WIN32_PRESENTATION_ACTION_RELEASE_MOUSE);
}
#endif
