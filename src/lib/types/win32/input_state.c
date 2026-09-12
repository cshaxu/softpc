#include "lib/types/types_interface.h"
#include "lib/types/types_interface.h"

#ifdef _WIN32
#include <windows.h>

lib_u8 lib_native_input_current_modifiers(void)
{
    lib_u8 modifiers = 0u;

    if ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
        modifiers |= LIB_NATIVE_INPUT_MODIFIER_CONTROL;
    if ((GetKeyState(VK_MENU) & 0x8000) != 0)
        modifiers |= LIB_NATIVE_INPUT_MODIFIER_ALT;
    if ((GetKeyState(VK_SHIFT) & 0x8000) != 0)
        modifiers |= LIB_NATIVE_INPUT_MODIFIER_SHIFT;
    return modifiers;
}

#endif
