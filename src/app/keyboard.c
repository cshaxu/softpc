#include "keyboard.h"

#ifdef _WIN32
#include "runtime.h"
#include <windows.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD key_event);

int app_keyboard_deliver_input(void *context,
    const ux_event *event)
{
    return context != NULL && event != NULL && app_runtime_enqueue_input_event(
        (app_runtime *)context, event);
}

int app_keyboard_inject_machine_event(softpc_machine *machine,
    const ux_event *event)
{
    KEY_EVENT_RECORD copy;
    BYTE key_number;

    if (machine == NULL || event == NULL ||
        event->type != UX_EVENT_KEY) return 0;
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
    ux_action_registry *registry)
{
    const uint8_t modifiers = UX_MODIFIER_CONTROL | UX_MODIFIER_ALT;

    if (registry == NULL) return 0;
    ux_actions_initialize(registry);
    return ux_actions_register(registry, 'P', modifiers, 1u) == LIB_STATUS_OK &&
        ux_actions_register(registry, 'D', modifiers, 2u) == LIB_STATUS_OK &&
        ux_actions_register(registry, 'F', modifiers, 3u) == LIB_STATUS_OK &&
        ux_actions_register(registry, 'M', modifiers, 4u) == LIB_STATUS_OK;
}

static int app_keyboard_emit(void *context, ux_event_sink sink, WORD scan,
    WORD virtual_key, int pressed)
{
    ux_event event = { 0 };
    if (sink == NULL) return 0;
    event.type = UX_EVENT_KEY;
    event.data.key.scan_code = scan;
    event.data.key.virtual_key = virtual_key;
    event.data.key.pressed = pressed != 0;
    return sink(context, &event);
}

int app_keyboard_release_ctrl_alt(void *context, ux_event_sink sink)
{
    return app_keyboard_emit(context, sink, 0x1du, VK_CONTROL, 0) &&
        app_keyboard_emit(context, sink, 0x38u, VK_MENU, 0);
}

int app_keyboard_submit_ctrl_alt_del(void *context, ux_event_sink sink)
{
    return app_keyboard_emit(context, sink, 0x1du, VK_CONTROL, 1) &&
        app_keyboard_emit(context, sink, 0x38u, VK_MENU, 1) &&
        app_keyboard_emit(context, sink, 0x0153u, VK_DELETE, 1) &&
        app_keyboard_emit(context, sink, 0x0153u, VK_DELETE, 0) &&
        app_keyboard_emit(context, sink, 0x38u, VK_MENU, 0) &&
        app_keyboard_emit(context, sink, 0x1du, VK_CONTROL, 0);
}

int app_keyboard_submit_alt_enter(void *context, ux_event_sink sink)
{
    return app_keyboard_release_ctrl_alt(context, sink) &&
        app_keyboard_emit(context, sink, 0x38u, VK_MENU, 1) &&
        app_keyboard_emit(context, sink, 0x1cu, VK_RETURN, 1) &&
        app_keyboard_emit(context, sink, 0x1cu, VK_RETURN, 0) &&
        app_keyboard_emit(context, sink, 0x38u, VK_MENU, 0);
}
#endif
