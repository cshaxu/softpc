#include "lib/ui-base/win32/actions.h"
#include "lib/types/native_input.h"

lib_u8 ui_win32_modifiers_from_key_state(void)
{
    return lib_native_input_current_modifiers();
}
