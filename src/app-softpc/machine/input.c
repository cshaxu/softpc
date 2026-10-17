#include "lib/types/types_interface.h"
#include "input.h"
#include <windows.h>

#include "../softpc.new/host/inc/nt_uis.h"

/* This is SoftPC's private guest-protocol adapter. Shared KVM values never
 * expose these Win32 constants; they reach KeyMsgToKeyCode only here. */
static WORD vm_keyboard_to_virtual_key(lib_u32 key)
{
    if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z'))
        return (WORD)key;
    if (key >= BASE_KEY_KEYPAD_0 && key <= BASE_KEY_KEYPAD_9)
        return (WORD)(VK_NUMPAD0 + key - BASE_KEY_KEYPAD_0);
    if (key >= BASE_KEY_F1 && key <= BASE_KEY_F12)
        return (WORD)(VK_F1 + key - BASE_KEY_F1);
    if (key >= BASE_KEY_F13 && key <= BASE_KEY_F24)
        return (WORD)(VK_F13 + key - BASE_KEY_F13);
    switch (key) {
    case ' ': return VK_SPACE; case ';': return VK_OEM_1; case '=': return VK_OEM_PLUS;
    case ',': return VK_OEM_COMMA; case '-': return VK_OEM_MINUS;
    case '.': return VK_OEM_PERIOD; case '/': return VK_OEM_2;
    case '`': return VK_OEM_3; case '[': return VK_OEM_4;
    case '\\': return VK_OEM_5; case ']': return VK_OEM_6; case '\'': return VK_OEM_7;
    case BASE_KEY_BACKSPACE: return VK_BACK; case BASE_KEY_TAB: return VK_TAB;
    case BASE_KEY_ENTER: return VK_RETURN; case BASE_KEY_ESCAPE: return VK_ESCAPE;
    case BASE_KEY_SHIFT: return VK_SHIFT; case BASE_KEY_CONTROL: return VK_CONTROL;
    case BASE_KEY_ALT: return VK_MENU; case BASE_KEY_CAPS_LOCK: return VK_CAPITAL;
    case BASE_KEY_NUM_LOCK: return VK_NUMLOCK; case BASE_KEY_SCROLL_LOCK: return VK_SCROLL;
    case BASE_KEY_PAUSE: return VK_PAUSE; case BASE_KEY_PRINT_SCREEN: return VK_SNAPSHOT;
    case BASE_KEY_HOME: return VK_HOME; case BASE_KEY_END: return VK_END;
    case BASE_KEY_PAGE_UP: return VK_PRIOR; case BASE_KEY_PAGE_DOWN: return VK_NEXT;
    case BASE_KEY_LEFT: return VK_LEFT; case BASE_KEY_UP: return VK_UP;
    case BASE_KEY_RIGHT: return VK_RIGHT; case BASE_KEY_DOWN: return VK_DOWN;
    case BASE_KEY_INSERT: return VK_INSERT; case BASE_KEY_DELETE: return VK_DELETE;
    case BASE_KEY_LEFT_WINDOWS: return VK_LWIN; case BASE_KEY_RIGHT_WINDOWS: return VK_RWIN;
    case BASE_KEY_MENU: return VK_APPS; case BASE_KEY_KEYPAD_MULTIPLY: return VK_MULTIPLY;
    case BASE_KEY_KEYPAD_ADD: return VK_ADD; case BASE_KEY_KEYPAD_SUBTRACT: return VK_SUBTRACT;
    case BASE_KEY_KEYPAD_DECIMAL: return VK_DECIMAL; case BASE_KEY_KEYPAD_DIVIDE: return VK_DIVIDE;
    default: return 0u;
    }
}

int vm_keyboard_inject_machine_event(softpc_machine *machine,
    const kvm_input_event *event)
{
    KEY_EVENT_RECORD copy;
    BYTE key_number;

    if (machine == NULL || event == NULL ||
        event->type != KVM_EVENT_KEY) return 0;
    ZeroMemory(&copy, sizeof(copy));
    copy.bKeyDown = event->data.key.pressed != 0;
    copy.wVirtualKeyCode = vm_keyboard_to_virtual_key(event->data.key.key);
    if (copy.wVirtualKeyCode == 0u) return 0;
    copy.wVirtualScanCode = (WORD)(event->data.key.scan_code & 0xffu);
    if ((event->data.key.flags & BASE_KEY_FLAG_EXTENDED) != 0u)
        copy.dwControlKeyState |= ENHANCED_KEY;
    key_number = KeyMsgToKeyCode(&copy);
    return key_number != 0u && softpc_machine_key_number(machine, key_number,
        (lib_u8)!copy.bKeyDown) == SOFTPC_MACHINE_OK;
}
