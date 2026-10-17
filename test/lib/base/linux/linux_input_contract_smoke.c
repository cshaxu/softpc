#include "lib/types/test.h"
#include "lib/base/input_adapter_interface.h"
#include "lib/base/linux/input.h"

static lib_key_input received;
int main(void)
{
    base_keyboard_decoder state = { 0 };
    lib_test_assert(base_keyboard_decode(&(base_keyboard_decoder){ 0 }, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0u, BASE_LINUX_KEY_UP, 0u, 0u, 0u, 1, 1u }, &received));
    lib_test_assert(received.key == LIB_KEY_UP &&
        received.scan_code == 0u);
    lib_test_assert(base_keyboard_decode(&state,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16='a', .repeat_count=1u }, &received));
    lib_test_assert(received.scalar == 'a');
    lib_test_assert(base_keyboard_decode(&state,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=0xd83du, .repeat_count=1u }, &received));
    lib_test_assert(base_keyboard_decode(&state,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=0xde00u, .repeat_count=1u }, &received));
    lib_test_assert(received.scalar == 0x1f600u);
    lib_test_assert(base_keyboard_decode(&(base_keyboard_decoder){ 0 }, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0u, 0xffffu, 0u, 0u, 0u, 1, 1u }, &received) == BASE_KEYBOARD_UNMAPPED);
    return 0;
}
