#include "lib/ui-base/win32/actions.h"
#include "lib/types/win32/input.h"
#include "lib/ui-base/hotkey_interface.h"

lib_u8 ui_win32_modifiers_from_key_state(void)
{
    lib_u8 modifiers = 0u;

    if ((lib_win32_get_key_state(VK_CONTROL) & 0x8000) != 0)
        modifiers |= UI_HOTKEY_MODIFIER_CONTROL;
    if ((lib_win32_get_key_state(VK_MENU) & 0x8000) != 0)
        modifiers |= UI_HOTKEY_MODIFIER_ALT;
    if ((lib_win32_get_key_state(VK_SHIFT) & 0x8000) != 0)
        modifiers |= UI_HOTKEY_MODIFIER_SHIFT;
    return modifiers;
}
