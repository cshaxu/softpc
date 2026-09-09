#include "keyboard.h"

#ifdef _WIN32
#include "runtime.h"
#include <windows.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD key_event);

/* This is SoftPC's private guest-protocol adapter. Shared UX values never
 * expose these Win32 constants; they reach KeyMsgToKeyCode only here. */
static WORD app_keyboard_to_virtual_key(lib_u32 key)
{
    if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z'))
        return (WORD)key;
    if (key >= UX_KEY_KEYPAD_0 && key <= UX_KEY_KEYPAD_9)
        return (WORD)(VK_NUMPAD0 + key - UX_KEY_KEYPAD_0);
    if (key >= UX_KEY_F1 && key <= UX_KEY_F12)
        return (WORD)(VK_F1 + key - UX_KEY_F1);
    if (key >= UX_KEY_F13 && key <= UX_KEY_F24)
        return (WORD)(VK_F13 + key - UX_KEY_F13);
    switch (key) {
    case ' ': return VK_SPACE; case ';': return VK_OEM_1; case '=': return VK_OEM_PLUS;
    case ',': return VK_OEM_COMMA; case '-': return VK_OEM_MINUS;
    case '.': return VK_OEM_PERIOD; case '/': return VK_OEM_2;
    case '`': return VK_OEM_3; case '[': return VK_OEM_4;
    case '\\': return VK_OEM_5; case ']': return VK_OEM_6; case '\'': return VK_OEM_7;
    case UX_KEY_BACKSPACE: return VK_BACK; case UX_KEY_TAB: return VK_TAB;
    case UX_KEY_ENTER: return VK_RETURN; case UX_KEY_ESCAPE: return VK_ESCAPE;
    case UX_KEY_SHIFT: return VK_SHIFT; case UX_KEY_CONTROL: return VK_CONTROL;
    case UX_KEY_ALT: return VK_MENU; case UX_KEY_CAPS_LOCK: return VK_CAPITAL;
    case UX_KEY_NUM_LOCK: return VK_NUMLOCK; case UX_KEY_SCROLL_LOCK: return VK_SCROLL;
    case UX_KEY_PAUSE: return VK_PAUSE; case UX_KEY_PRINT_SCREEN: return VK_SNAPSHOT;
    case UX_KEY_HOME: return VK_HOME; case UX_KEY_END: return VK_END;
    case UX_KEY_PAGE_UP: return VK_PRIOR; case UX_KEY_PAGE_DOWN: return VK_NEXT;
    case UX_KEY_LEFT: return VK_LEFT; case UX_KEY_UP: return VK_UP;
    case UX_KEY_RIGHT: return VK_RIGHT; case UX_KEY_DOWN: return VK_DOWN;
    case UX_KEY_INSERT: return VK_INSERT; case UX_KEY_DELETE: return VK_DELETE;
    case UX_KEY_LEFT_WINDOWS: return VK_LWIN; case UX_KEY_RIGHT_WINDOWS: return VK_RWIN;
    case UX_KEY_MENU: return VK_APPS; case UX_KEY_KEYPAD_MULTIPLY: return VK_MULTIPLY;
    case UX_KEY_KEYPAD_ADD: return VK_ADD; case UX_KEY_KEYPAD_SUBTRACT: return VK_SUBTRACT;
    case UX_KEY_KEYPAD_DECIMAL: return VK_DECIMAL; case UX_KEY_KEYPAD_DIVIDE: return VK_DIVIDE;
    default: return 0u;
    }
}

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
    copy.wVirtualKeyCode = app_keyboard_to_virtual_key(event->data.key.key);
    if (copy.wVirtualKeyCode == 0u) return 0;
    copy.wVirtualScanCode = (WORD)(event->data.key.scan_code & 0xffu);
    if ((event->data.key.flags & UX_KEY_FLAG_EXTENDED) != 0u)
        copy.dwControlKeyState |= ENHANCED_KEY;
    key_number = KeyMsgToKeyCode(&copy);
    return key_number != 0u && softpc_machine_key_number(machine, key_number,
        (uint8_t)!copy.bKeyDown) == SOFTPC_MACHINE_OK;
}

int app_keyboard_hotkeys(ux_hotkey_registry *registry)
{
    static const ux_hotkey_registry hotkeys = {
        { { 'P', UX_HOTKEY_MODIFIER_CONTROL | UX_HOTKEY_MODIFIER_ALT,
              "pause-toggle" },
          { 'D', UX_HOTKEY_MODIFIER_CONTROL | UX_HOTKEY_MODIFIER_ALT,
              "send-ctrl-alt-del" },
          { 'F', UX_HOTKEY_MODIFIER_CONTROL | UX_HOTKEY_MODIFIER_ALT,
              "send-alt-enter" },
          { 'M', UX_HOTKEY_MODIFIER_CONTROL | UX_HOTKEY_MODIFIER_ALT,
              "release-window-mouse" } }, 4u };

    if (registry == NULL) return 0;
    *registry = hotkeys;
    return 1;
}

static int app_keyboard_emit(void *context, ux_event_sink sink, WORD scan,
    lib_u32 key, int pressed)
{
    ux_event event = { 0 };
    if (sink == NULL) return 0;
    event.type = UX_EVENT_KEY;
    event.data.key.scan_code = scan;
    event.data.key.key = key;
    event.data.key.flags = (scan & 0x0100u) != 0u ? UX_KEY_FLAG_EXTENDED : 0u;
    event.data.key.pressed = pressed != 0;
    return sink(context, &event);
}

int app_keyboard_release_ctrl_alt(void *context, ux_event_sink sink)
{
    return app_keyboard_emit(context, sink, 0x1du, UX_KEY_CONTROL, 0) &&
        app_keyboard_emit(context, sink, 0x38u, UX_KEY_ALT, 0);
}

int app_keyboard_submit_ctrl_alt_del(void *context, ux_event_sink sink)
{
    return app_keyboard_emit(context, sink, 0x1du, UX_KEY_CONTROL, 1) &&
        app_keyboard_emit(context, sink, 0x38u, UX_KEY_ALT, 1) &&
        app_keyboard_emit(context, sink, 0x0153u, UX_KEY_DELETE, 1) &&
        app_keyboard_emit(context, sink, 0x0153u, UX_KEY_DELETE, 0) &&
        app_keyboard_emit(context, sink, 0x38u, UX_KEY_ALT, 0) &&
        app_keyboard_emit(context, sink, 0x1du, UX_KEY_CONTROL, 0);
}

int app_keyboard_submit_alt_enter(void *context, ux_event_sink sink)
{
    return app_keyboard_release_ctrl_alt(context, sink) &&
        app_keyboard_emit(context, sink, 0x38u, UX_KEY_ALT, 1) &&
        app_keyboard_emit(context, sink, 0x1cu, UX_KEY_ENTER, 1) &&
        app_keyboard_emit(context, sink, 0x1cu, UX_KEY_ENTER, 0) &&
        app_keyboard_emit(context, sink, 0x38u, UX_KEY_ALT, 0);
}
#endif
