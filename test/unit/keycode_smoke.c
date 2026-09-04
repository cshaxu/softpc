#include <assert.h>
#include <string.h>

#include <windows.h>

#include "nt_uis.h"

static BYTE app_keycode(WORD scan_code, DWORD control_state)
{
    KEY_EVENT_RECORD event;

    memset(&event, 0, sizeof(event));
    event.wVirtualScanCode = scan_code;
    event.dwControlKeyState = control_state;
    return KeyMsgToKeyCode(&event);
}

int main(void)
{
    /* The standalone frontends feed these original nt_keycd tables, not a
       locally maintained Win32-to-PC map.  Keep ordinary and E0-extended
       keys distinct: their Scan-1 values overlap but their SoftPC key
       numbers do not. */
    assert(app_keycode(0x1eu, 0u) == 31u);       /* A */
    assert(app_keycode(0x4du, 0u) == 102u);      /* keypad Right */
    assert(app_keycode(0x4du, ENHANCED_KEY) == 89u); /* Right arrow */
    assert(app_keycode(0x1du, 0u) == 58u);       /* left Ctrl */
    assert(app_keycode(0x1du, ENHANCED_KEY) == 64u); /* right Ctrl */
    assert(app_keycode(0x54u, 0u) == 0u);        /* unmapped */
    return 0;
}
