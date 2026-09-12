#ifndef LIB_TYPES_INPUT_H
#define LIB_TYPES_INPUT_H

#include "lib/types/types_interface.h"

/* Raw keyboard/layout queries. UI-base owns all interpretation of the
 * returned copied values. */
#ifdef _WIN32
#include "lib/types/win32.h"
static inline lib_u8 lib_input_current_modifiers(void)
{
    lib_u8 modifiers = 0u;

    if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) modifiers |= 0x01u;
    if ((GetKeyState(VK_MENU) & 0x8000) != 0) modifiers |= 0x02u;
    if ((GetKeyState(VK_SHIFT) & 0x8000) != 0) modifiers |= 0x04u;
    return modifiers;
}

static inline lib_u16 lib_input_scan_code(lib_u16 virtual_key)
{ return (lib_u16)MapVirtualKeyA((UINT)virtual_key, MAPVK_VK_TO_VSC); }

#else
static inline lib_u8 lib_input_current_modifiers(void)
{ return 0u; }

static inline lib_u16 lib_input_scan_code(lib_u16 virtual_key)
{ (void)virtual_key; return 0u; }

#endif

#endif
