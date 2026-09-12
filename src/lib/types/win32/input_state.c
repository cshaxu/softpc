#include "lib/types/win32/input.h"
#include "lib/types/types_interface.h"

#ifdef _WIN32
#include <windows.h>

lib_u8 lib_native_input_current_modifiers(void)
{
    lib_u8 modifiers = 0u;

    if ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
        modifiers |= 0x01u;
    if ((GetKeyState(VK_MENU) & 0x8000) != 0)
        modifiers |= 0x02u;
    if ((GetKeyState(VK_SHIFT) & 0x8000) != 0)
        modifiers |= 0x04u;
    return modifiers;
}

lib_u16 lib_native_input_scan_code(lib_u16 native_key)
{
    lib_u32 scan = (lib_u32)MapVirtualKeyExW((UINT)native_key,
        MAPVK_VK_TO_VSC_EX, GetKeyboardLayout(0u));

    return (lib_u16)(((scan & 0xff00u) == 0xe000u ? 0x0100u : 0u) |
        (scan & 0x00ffu));
}

lib_bool lib_native_input_map_scalar(lib_u32 scalar, lib_u16 *out_native_key,
    lib_u8 *out_modifiers)
{
    SHORT mapped;

    if (out_native_key == LIB_NULL || out_modifiers == LIB_NULL || scalar == 0u ||
        scalar > 0xffffu || (scalar >= 0xd800u && scalar <= 0xdfffu))
        return LIB_FALSE;
    mapped = VkKeyScanExW((WCHAR)scalar, GetKeyboardLayout(0u));
    if (mapped == -1) return LIB_FALSE;
    *out_native_key = (lib_u16)((lib_u16)mapped & 0xffu);
    *out_modifiers = 0u;
    if (((lib_u16)mapped & 0x0100u) != 0u)
        *out_modifiers |= 0x04u;
    if (((lib_u16)mapped & 0x0200u) != 0u)
        *out_modifiers |= 0x01u;
    if (((lib_u16)mapped & 0x0400u) != 0u)
        *out_modifiers |= 0x02u;
    return LIB_TRUE;
}

#endif
