#include <assert.h>
#include <string.h>

#include <windows.h>

#include "nt_uis.h"
#include "../../src/compat/input.h"

static void check_original_table(unsigned count, DWORD flags, DWORD expected)
{
    DWORD fingerprint = 2166136261u;
    unsigned scan, modifiers, released;
    for (scan = 0; scan < count; ++scan) {
        KEY_EVENT_RECORD event;
        BYTE key;
        memset(&event, 0, sizeof(event));
        event.wVirtualScanCode = (WORD)scan;
        event.dwControlKeyState = flags;
        key = KeyMsgToKeyCode(&event);
        fingerprint = (fingerprint ^ key) * 16777619u;
        if (flags == 0u)
            assert(softpc_host_scan1_to_key(scan) == key);
        for (modifiers = 0; modifiers < 8; ++modifiers) {
            event.dwControlKeyState = flags |
                ((modifiers & 1) ? SHIFT_PRESSED : 0) |
                ((modifiers & 2) ? LEFT_CTRL_PRESSED : 0) |
                ((modifiers & 4) ? LEFT_ALT_PRESSED : 0);
            for (released = 0; released < 2; ++released) {
                event.bKeyDown = !released;
                assert(KeyMsgToKeyCode(&event) == key);
            }
        }
    }
    assert(fingerprint == expected);
}

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
    /* All original table entries, without a second mapping table or an
       external source dependency. Fingerprints are FNV-1a over key numbers. */
    check_original_table(133u, 0u, 0x63e314e3u);
    check_original_table(84u, ENHANCED_KEY, 0x26270444u);
    assert(softpc_host_scan1_to_key(133u) == 0);
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
    assert(app_keycode(133u, 0u) == 0u);         /* first regular OOB */
    assert(app_keycode(84u, ENHANCED_KEY) == 0u); /* first enhanced OOB */
    return 0;
}
