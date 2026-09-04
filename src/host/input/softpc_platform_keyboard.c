#include "insignia.h"
#include "host_def.h"
#include "keyboard.h"
#include "softpc_host_input.h"

extern void AT_kbd_init(void);
extern void AT_kbd_post(void);
extern void host_key_down(int key);
extern void host_key_up(int key);

static void softpc_keyboard_host_void(void)
{
}

static void softpc_keyboard_host_lights()
{
}

KEYBDFUNCS softpc_keyboard_host_functions = {
    softpc_keyboard_host_void,
    softpc_keyboard_host_void,
    softpc_keyboard_host_void,
    softpc_keyboard_host_void,
    softpc_keyboard_host_lights,
    softpc_keyboard_host_lights};

SHORT host_error(error_number, options, extra_text)
int error_number;
int options;
char *extra_text;
{
    UNUSED(error_number);
    UNUSED(options);
    UNUSED(extra_text);
    return 0;
}

void softpc_platform_keyboard_reset(void)
{
    extern void keyboard_init(void);
    extern void keyboard_post(void);

    keyboard_init();
    keyboard_post();
    AT_kbd_init();
    AT_kbd_post();
}

int softpc_platform_keyboard_key(int key, int released)
{
    if (key <= 0)
        return 0;
    if (released)
        host_key_up(key);
    else
        host_key_down(key);
    return 1;
}

int softpc_platform_keyboard_scancode(IU8 scan_code)
{
    int key = softpc_host_scan1_to_key((unsigned int)(scan_code & 0x7fu));
    if (key == 0)
        return 0;
    if ((scan_code & 0x80u) != 0u)
        host_key_up(key);
    else
        host_key_down(key);
    return 1;
}
