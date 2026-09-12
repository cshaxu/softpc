#include "keyboard.h"

#ifdef _WIN32
#include "runtime.h"
#include <windows.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD key_event);

/* This is SoftPC's private guest-protocol adapter. Shared UI values never
 * expose these Win32 constants; they reach KeyMsgToKeyCode only here. */
static WORD app_keyboard_to_virtual_key(lib_u32 key)
{
    if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z'))
        return (WORD)key;
    if (key >= UI_KEY_KEYPAD_0 && key <= UI_KEY_KEYPAD_9)
        return (WORD)(VK_NUMPAD0 + key - UI_KEY_KEYPAD_0);
    if (key >= UI_KEY_F1 && key <= UI_KEY_F12)
        return (WORD)(VK_F1 + key - UI_KEY_F1);
    if (key >= UI_KEY_F13 && key <= UI_KEY_F24)
        return (WORD)(VK_F13 + key - UI_KEY_F13);
    switch (key) {
    case ' ': return VK_SPACE; case ';': return VK_OEM_1; case '=': return VK_OEM_PLUS;
    case ',': return VK_OEM_COMMA; case '-': return VK_OEM_MINUS;
    case '.': return VK_OEM_PERIOD; case '/': return VK_OEM_2;
    case '`': return VK_OEM_3; case '[': return VK_OEM_4;
    case '\\': return VK_OEM_5; case ']': return VK_OEM_6; case '\'': return VK_OEM_7;
    case UI_KEY_BACKSPACE: return VK_BACK; case UI_KEY_TAB: return VK_TAB;
    case UI_KEY_ENTER: return VK_RETURN; case UI_KEY_ESCAPE: return VK_ESCAPE;
    case UI_KEY_SHIFT: return VK_SHIFT; case UI_KEY_CONTROL: return VK_CONTROL;
    case UI_KEY_ALT: return VK_MENU; case UI_KEY_CAPS_LOCK: return VK_CAPITAL;
    case UI_KEY_NUM_LOCK: return VK_NUMLOCK; case UI_KEY_SCROLL_LOCK: return VK_SCROLL;
    case UI_KEY_PAUSE: return VK_PAUSE; case UI_KEY_PRINT_SCREEN: return VK_SNAPSHOT;
    case UI_KEY_HOME: return VK_HOME; case UI_KEY_END: return VK_END;
    case UI_KEY_PAGE_UP: return VK_PRIOR; case UI_KEY_PAGE_DOWN: return VK_NEXT;
    case UI_KEY_LEFT: return VK_LEFT; case UI_KEY_UP: return VK_UP;
    case UI_KEY_RIGHT: return VK_RIGHT; case UI_KEY_DOWN: return VK_DOWN;
    case UI_KEY_INSERT: return VK_INSERT; case UI_KEY_DELETE: return VK_DELETE;
    case UI_KEY_LEFT_WINDOWS: return VK_LWIN; case UI_KEY_RIGHT_WINDOWS: return VK_RWIN;
    case UI_KEY_MENU: return VK_APPS; case UI_KEY_KEYPAD_MULTIPLY: return VK_MULTIPLY;
    case UI_KEY_KEYPAD_ADD: return VK_ADD; case UI_KEY_KEYPAD_SUBTRACT: return VK_SUBTRACT;
    case UI_KEY_KEYPAD_DECIMAL: return VK_DECIMAL; case UI_KEY_KEYPAD_DIVIDE: return VK_DIVIDE;
    default: return 0u;
    }
}

int app_keyboard_deliver_input(void *context,
    const ui_input_event *event)
{
    return context != NULL && event != NULL && app_runtime_enqueue_input_event(
        (app_runtime *)context, event);
}

int app_keyboard_inject_machine_event(softpc_machine *machine,
    const ui_input_event *event)
{
    KEY_EVENT_RECORD copy;
    BYTE key_number;

    if (machine == NULL || event == NULL ||
        event->type != UI_EVENT_KEY) return 0;
    ZeroMemory(&copy, sizeof(copy));
    copy.bKeyDown = event->data.key.pressed != 0;
    copy.wVirtualKeyCode = app_keyboard_to_virtual_key(event->data.key.key);
    if (copy.wVirtualKeyCode == 0u) return 0;
    copy.wVirtualScanCode = (WORD)(event->data.key.scan_code & 0xffu);
    if ((event->data.key.flags & UI_KEY_FLAG_EXTENDED) != 0u)
        copy.dwControlKeyState |= ENHANCED_KEY;
    key_number = KeyMsgToKeyCode(&copy);
    return key_number != 0u && softpc_machine_key_number(machine, key_number,
        (uint8_t)!copy.bKeyDown) == SOFTPC_MACHINE_OK;
}

int app_keyboard_hotkeys(ui_hotkey_registry *registry)
{
    static const ui_hotkey_registry hotkeys = {
        { { 'P', UI_HOTKEY_MODIFIER_CONTROL | UI_HOTKEY_MODIFIER_ALT,
              "pause-toggle" },
          { 'D', UI_HOTKEY_MODIFIER_CONTROL | UI_HOTKEY_MODIFIER_ALT,
              "send-ctrl-alt-del" },
          { 'F', UI_HOTKEY_MODIFIER_CONTROL | UI_HOTKEY_MODIFIER_ALT,
              "send-alt-enter" },
          { 'M', UI_HOTKEY_MODIFIER_CONTROL | UI_HOTKEY_MODIFIER_ALT,
              "release-window-mouse" } }, 4u };

    if (registry == NULL) return 0;
    *registry = hotkeys;
    return 1;
}

static int app_keyboard_emit(void *context, ui_input_sink sink, WORD scan,
    lib_u32 key, int pressed)
{
    ui_input_event event = { 0 };
    if (sink == NULL) return 0;
    event.type = UI_EVENT_KEY;
    event.data.key.scan_code = scan;
    event.data.key.key = key;
    event.data.key.flags = (scan & 0x0100u) != 0u ? UI_KEY_FLAG_EXTENDED : 0u;
    event.data.key.pressed = pressed != 0;
    return sink(context, &event);
}

int app_keyboard_release_ctrl_alt(void *context, ui_input_sink sink)
{
    return app_keyboard_emit(context, sink, 0x1du, UI_KEY_CONTROL, 0) &&
        app_keyboard_emit(context, sink, 0x38u, UI_KEY_ALT, 0);
}

int app_keyboard_submit_ctrl_alt_del(void *context, ui_input_sink sink)
{
    return app_keyboard_emit(context, sink, 0x1du, UI_KEY_CONTROL, 1) &&
        app_keyboard_emit(context, sink, 0x38u, UI_KEY_ALT, 1) &&
        app_keyboard_emit(context, sink, 0x0153u, UI_KEY_DELETE, 1) &&
        app_keyboard_emit(context, sink, 0x0153u, UI_KEY_DELETE, 0) &&
        app_keyboard_emit(context, sink, 0x38u, UI_KEY_ALT, 0) &&
        app_keyboard_emit(context, sink, 0x1du, UI_KEY_CONTROL, 0);
}

int app_keyboard_submit_alt_enter(void *context, ui_input_sink sink)
{
    return app_keyboard_release_ctrl_alt(context, sink) &&
        app_keyboard_emit(context, sink, 0x38u, UI_KEY_ALT, 1) &&
        app_keyboard_emit(context, sink, 0x1cu, UI_KEY_ENTER, 1) &&
        app_keyboard_emit(context, sink, 0x1cu, UI_KEY_ENTER, 0) &&
        app_keyboard_emit(context, sink, 0x38u, UI_KEY_ALT, 0);
}
#endif
