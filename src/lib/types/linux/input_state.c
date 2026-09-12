#include "lib/types/types_interface.h"

lib_u8 lib_native_input_current_modifiers(void)
{
    return 0u;
}

lib_u8 lib_native_input_flags(lib_u64 native_control_state)
{
    (void)native_control_state;
    return 0u;
}

lib_u16 lib_native_input_scan_code(lib_u16 native_key)
{
    (void)native_key;
    return 0u;
}

lib_bool lib_native_input_map_scalar(lib_u32 scalar, lib_u16 *out_native_key,
    lib_u8 *out_modifiers)
{
    (void)scalar;
    if (out_native_key != LIB_NULL) *out_native_key = 0u;
    if (out_modifiers != LIB_NULL) *out_modifiers = 0u;
    return LIB_FALSE;
}
