/* Compile the real component bodies against controlled external queries.
 * No UI interaction, sleeping, or timing-dependent assertion is needed. */
#include "lib/types/win32/clock.h"
#include "lib/types/win32/input.h"
#include "lib/types/types_interface.h"

static int counter_ok = 1, frequency_ok = 1;
static LONGLONG counter_value = 123, frequency_value = 1000;
static unsigned query_count;
static unsigned pressed;

static BOOL fake_counter(lib_win32_counter *out)
{ ++query_count; out->QuadPart = counter_value; return counter_ok; }
static BOOL fake_frequency(lib_win32_counter *out)
{ ++query_count; out->QuadPart = frequency_value; return frequency_ok; }
static lib_win32_key_state fake_key_state(int key)
{
    unsigned bit = key == VK_CONTROL ? 1u : key == VK_MENU ? 2u : key == VK_SHIFT ? 4u : 0u;
    return (lib_win32_key_state)((pressed & bit) != 0u ? 0x8000u : 0u);
}

#undef lib_win32_query_performance_counter
#undef lib_win32_query_performance_frequency
#undef lib_win32_get_key_state
#define lib_win32_query_performance_counter fake_counter
#define lib_win32_query_performance_frequency fake_frequency
#define lib_win32_get_key_state fake_key_state
#include "lib/host/win32/clock.c"
#include "lib/ui-base/win32/actions.c"

#define CHECK(expression) do { if (!(expression)) return __LINE__; } while (0)
int main(void)
{
    lib_u64 units = 999u, frequency = 888u;
    CHECK(host_clock_platform_counter(LIB_NULL, &frequency) == LIB_STATUS_IO_ERROR);
    CHECK(host_clock_platform_counter(&units, LIB_NULL) == LIB_STATUS_IO_ERROR);
    CHECK(query_count == 0u);
    counter_ok = 0;
    CHECK(host_clock_platform_counter(&units, &frequency) == LIB_STATUS_IO_ERROR);
    CHECK(units == 999u && frequency == 888u && query_count == 1u);
    counter_ok = 1; frequency_ok = 0;
    CHECK(host_clock_platform_counter(&units, &frequency) == LIB_STATUS_IO_ERROR);
    frequency_ok = 1; frequency_value = 0;
    CHECK(host_clock_platform_counter(&units, &frequency) == LIB_STATUS_IO_ERROR);
    frequency_value = -1;
    CHECK(host_clock_platform_counter(&units, &frequency) == LIB_STATUS_IO_ERROR);
    frequency_value = 1000; counter_value = -1;
    CHECK(host_clock_platform_counter(&units, &frequency) == LIB_STATUS_IO_ERROR);
    CHECK(units == 999u && frequency == 888u);
    counter_value = 123;
    CHECK(host_clock_platform_counter(&units, &frequency) == LIB_STATUS_OK);
    CHECK(units == 123u && frequency == 1000u);
    for (pressed = 0u; pressed != 8u; ++pressed) {
        lib_u8 expected = 0u;
        if ((pressed & 1u) != 0u) expected |= UI_HOTKEY_MODIFIER_CONTROL;
        if ((pressed & 2u) != 0u) expected |= UI_HOTKEY_MODIFIER_ALT;
        if ((pressed & 4u) != 0u) expected |= UI_HOTKEY_MODIFIER_SHIFT;
        CHECK(ui_win32_modifiers_from_key_state() == expected);
    }
    return 0;
}
