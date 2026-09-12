#ifndef LIB_TYPES_NATIVE_INPUT_H
#define LIB_TYPES_NATIVE_INPUT_H

#include "lib/types/types_interface.h"

/* Raw keyboard/layout queries. UI-base owns all interpretation of the
 * returned copied values. */
lib_u8 lib_native_input_current_modifiers(void);
lib_u16 lib_native_input_scan_code(lib_u16 native_key);
lib_bool lib_native_input_map_scalar(lib_u32 scalar, lib_u16 *out_native_key,
    lib_u8 *out_modifiers);

#endif
