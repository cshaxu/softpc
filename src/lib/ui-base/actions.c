#include "lib/ui-base/win32/actions.h"
#include "lib/types/win32/input.h"

lib_u8 ui_win32_modifiers_from_key_state(void)
{
    return lib_native_input_current_modifiers();
}
