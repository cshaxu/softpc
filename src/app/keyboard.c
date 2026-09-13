#include "keyboard.h"

#ifdef _WIN32
#include "runtime.h"
#include <windows.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD key_event);

/* This is SoftPC's private guest-protocol adapter. Shared KVM values never
 * expose these Win32 constants; they reach KeyMsgToKeyCode only here. */
static WORD app_keyboard_to_virtual_key(lib_u32 key)
{
    if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z'))
        return (WORD)key;
    if (key >= KVM_KEY_KEYPAD_0 && key <= KVM_KEY_KEYPAD_9)
        return (WORD)(VK_NUMPAD0 + key - KVM_KEY_KEYPAD_0);
    if (key >= KVM_KEY_F1 && key <= KVM_KEY_F12)
        return (WORD)(VK_F1 + key - KVM_KEY_F1);
    if (key >= KVM_KEY_F13 && key <= KVM_KEY_F24)
        return (WORD)(VK_F13 + key - KVM_KEY_F13);
    switch (key) {
    case ' ': return VK_SPACE; case ';': return VK_OEM_1; case '=': return VK_OEM_PLUS;
    case ',': return VK_OEM_COMMA; case '-': return VK_OEM_MINUS;
    case '.': return VK_OEM_PERIOD; case '/': return VK_OEM_2;
    case '`': return VK_OEM_3; case '[': return VK_OEM_4;
    case '\\': return VK_OEM_5; case ']': return VK_OEM_6; case '\'': return VK_OEM_7;
    case KVM_KEY_BACKSPACE: return VK_BACK; case KVM_KEY_TAB: return VK_TAB;
    case KVM_KEY_ENTER: return VK_RETURN; case KVM_KEY_ESCAPE: return VK_ESCAPE;
    case KVM_KEY_SHIFT: return VK_SHIFT; case KVM_KEY_CONTROL: return VK_CONTROL;
    case KVM_KEY_ALT: return VK_MENU; case KVM_KEY_CAPS_LOCK: return VK_CAPITAL;
    case KVM_KEY_NUM_LOCK: return VK_NUMLOCK; case KVM_KEY_SCROLL_LOCK: return VK_SCROLL;
    case KVM_KEY_PAUSE: return VK_PAUSE; case KVM_KEY_PRINT_SCREEN: return VK_SNAPSHOT;
    case KVM_KEY_HOME: return VK_HOME; case KVM_KEY_END: return VK_END;
    case KVM_KEY_PAGE_UP: return VK_PRIOR; case KVM_KEY_PAGE_DOWN: return VK_NEXT;
    case KVM_KEY_LEFT: return VK_LEFT; case KVM_KEY_UP: return VK_UP;
    case KVM_KEY_RIGHT: return VK_RIGHT; case KVM_KEY_DOWN: return VK_DOWN;
    case KVM_KEY_INSERT: return VK_INSERT; case KVM_KEY_DELETE: return VK_DELETE;
    case KVM_KEY_LEFT_WINDOWS: return VK_LWIN; case KVM_KEY_RIGHT_WINDOWS: return VK_RWIN;
    case KVM_KEY_MENU: return VK_APPS; case KVM_KEY_KEYPAD_MULTIPLY: return VK_MULTIPLY;
    case KVM_KEY_KEYPAD_ADD: return VK_ADD; case KVM_KEY_KEYPAD_SUBTRACT: return VK_SUBTRACT;
    case KVM_KEY_KEYPAD_DECIMAL: return VK_DECIMAL; case KVM_KEY_KEYPAD_DIVIDE: return VK_DIVIDE;
    default: return 0u;
    }
}

int app_keyboard_deliver_input(void *context,
    const kvm_input_event *event)
{
    return context != NULL && event != NULL && app_runtime_enqueue_input_event(
        (app_runtime *)context, event);
}

int app_keyboard_inject_machine_event(softpc_machine *machine,
    const kvm_input_event *event)
{
    KEY_EVENT_RECORD copy;
    BYTE key_number;

    if (machine == NULL || event == NULL ||
        event->type != KVM_EVENT_KEY) return 0;
    ZeroMemory(&copy, sizeof(copy));
    copy.bKeyDown = event->data.key.pressed != 0;
    copy.wVirtualKeyCode = app_keyboard_to_virtual_key(event->data.key.key);
    if (copy.wVirtualKeyCode == 0u) return 0;
    copy.wVirtualScanCode = (WORD)(event->data.key.scan_code & 0xffu);
    if ((event->data.key.flags & KVM_KEY_FLAG_EXTENDED) != 0u)
        copy.dwControlKeyState |= ENHANCED_KEY;
    key_number = KeyMsgToKeyCode(&copy);
    return key_number != 0u && softpc_machine_key_number(machine, key_number,
        (uint8_t)!copy.bKeyDown) == SOFTPC_MACHINE_OK;
}

int app_keyboard_hotkeys(kvm_hotkey_registry *registry)
{
    static const kvm_hotkey_registry hotkeys = {
        { { 'P', KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT,
              "pause-toggle" },
          { 'D', KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT,
              "send-ctrl-alt-del" },
          { 'F', KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT,
              "send-alt-enter" },
          { 'M', KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT,
              "release-window-mouse" } }, 4u };

    if (registry == NULL) return 0;
    *registry = hotkeys;
    return 1;
}

static int app_keyboard_emit(void *context, kvm_input_sink sink, WORD scan,
    lib_u32 key, int pressed)
{
    kvm_input_event event = { 0 };
    if (sink == NULL) return 0;
    event.type = KVM_EVENT_KEY;
    event.data.key.scan_code = scan;
    event.data.key.key = key;
    event.data.key.flags = (scan & 0x0100u) != 0u ? KVM_KEY_FLAG_EXTENDED : 0u;
    event.data.key.pressed = pressed != 0;
    return sink(context, &event);
}

int app_keyboard_release_ctrl_alt(void *context, kvm_input_sink sink)
{
    return app_keyboard_emit(context, sink, 0x1du, KVM_KEY_CONTROL, 0) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 0);
}

int app_keyboard_submit_ctrl_alt_del(void *context, kvm_input_sink sink)
{
    return app_keyboard_emit(context, sink, 0x1du, KVM_KEY_CONTROL, 1) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 1) &&
        app_keyboard_emit(context, sink, 0x0153u, KVM_KEY_DELETE, 1) &&
        app_keyboard_emit(context, sink, 0x0153u, KVM_KEY_DELETE, 0) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 0) &&
        app_keyboard_emit(context, sink, 0x1du, KVM_KEY_CONTROL, 0);
}

int app_keyboard_submit_alt_enter(void *context, kvm_input_sink sink)
{
    return app_keyboard_release_ctrl_alt(context, sink) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 1) &&
        app_keyboard_emit(context, sink, 0x1cu, KVM_KEY_ENTER, 1) &&
        app_keyboard_emit(context, sink, 0x1cu, KVM_KEY_ENTER, 0) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 0);
}
#endif
