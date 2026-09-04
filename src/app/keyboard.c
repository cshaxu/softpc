#include "keyboard.h"

#ifdef _WIN32
#include "runtime.h"

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD key_event);

static WORD softpc_win32_keyboard_decode_scan(WORD raw_scan)
{
    return (raw_scan & 0xff00u) == 0xe000u ?
        (WORD)(0x0100u | (raw_scan & 0x00ffu)) : (WORD)(raw_scan & 0x00ffu);
}

static WORD softpc_win32_keyboard_resolve_scan(WORD virtual_key)
{
    return softpc_win32_keyboard_decode_scan((WORD)MapVirtualKeyExW(
        virtual_key, MAPVK_VK_TO_VSC_EX, GetKeyboardLayout(0u)));
}

static int softpc_win32_keyboard_emit(void *context,
    softpc_win32_keyboard_sink sink, WORD scan, WORD virtual_key,
    DWORD control_state, int pressed)
{
    KEY_EVENT_RECORD event;
    BYTE key_number;

    if (sink == NULL || virtual_key == 0u || scan == 0u) return 0;
    ZeroMemory(&event, sizeof(event));
    event.bKeyDown = pressed != 0;
    event.wVirtualKeyCode = virtual_key;
    event.wVirtualScanCode = (WORD)(scan & 0xffu);
    event.dwControlKeyState = control_state;
    if ((scan & 0x0100u) != 0u) event.dwControlKeyState |= ENHANCED_KEY;
    key_number = KeyMsgToKeyCode(&event);
    return key_number != 0u && sink(context, key_number, (uint8_t)!pressed);
}

int softpc_win32_keyboard_submit_transition(void *context,
    softpc_win32_keyboard_sink sink, WORD scan, WORD virtual_key,
    DWORD control_state, int pressed)
{
    if (scan == 0u) scan = softpc_win32_keyboard_resolve_scan(virtual_key);
    return softpc_win32_keyboard_emit(context, sink, scan, virtual_key,
        control_state, pressed);
}

int softpc_win32_keyboard_submit_ctrl_alt_del(void *context,
    softpc_win32_keyboard_sink sink)
{
    /* Del is an extended Set-1 key.  Do not treat this as a monitor command:
       the guest BIOS/OS observes the same six transitions as physical PC
       hardware. */
    return softpc_win32_keyboard_emit(context, sink, 0x1du, VK_CONTROL, 0u, 1) &&
        softpc_win32_keyboard_emit(context, sink, 0x38u, VK_MENU, 0u, 1) &&
        softpc_win32_keyboard_emit(context, sink, 0x0153u, VK_DELETE, 0u, 1) &&
        softpc_win32_keyboard_emit(context, sink, 0x0153u, VK_DELETE, 0u, 0) &&
        softpc_win32_keyboard_emit(context, sink, 0x38u, VK_MENU, 0u, 0) &&
        softpc_win32_keyboard_emit(context, sink, 0x1du, VK_CONTROL, 0u, 0);
}

int softpc_win32_keyboard_release_ctrl_alt(void *context,
    softpc_win32_keyboard_sink sink)
{
    /* The host shortcut's Ctrl/Alt makes have already travelled through the
       ordinary path.  `host_key_up` deliberately ignores a later duplicate
       release, so it is safe to normalize both traditional consoles and RDP
       packets that coalesce modifier state into the chord record. */
    return softpc_win32_keyboard_emit(context, sink, 0x1du, VK_CONTROL, 0u, 0) &&
        softpc_win32_keyboard_emit(context, sink, 0x38u, VK_MENU, 0u, 0);
}

int softpc_win32_keyboard_submit_alt_enter(void *context,
    softpc_win32_keyboard_sink sink)
{
    /* Ctrl+Alt+F is a host chord, not guest Ctrl+Alt+Enter.  First clear the
       already-forwarded host modifiers, then generate a fresh physical guest
       Alt+Enter.  The host's eventual Ctrl/Alt key-ups are harmless duplicate
       releases in the original keyba implementation. */
    return softpc_win32_keyboard_release_ctrl_alt(context, sink) &&
        softpc_win32_keyboard_emit(context, sink, 0x38u, VK_MENU, 0u, 1) &&
        softpc_win32_keyboard_emit(context, sink, 0x1cu, VK_RETURN, 0u, 1) &&
        softpc_win32_keyboard_emit(context, sink, 0x1cu, VK_RETURN, 0u, 0) &&
        softpc_win32_keyboard_emit(context, sink, 0x38u, VK_MENU, 0u, 0);
}

void softpc_win32_keyboard_note_recovered_key(
    softpc_win32_keyboard_normalizer *state, WORD virtual_key)
{
    if (state != NULL) state->recovered_virtual_key =
        softpc_win32_keyboard_resolve_scan(virtual_key) == 0u ? 0u : virtual_key;
}

void softpc_win32_keyboard_release_recovered_key(
    softpc_win32_keyboard_normalizer *state, WORD virtual_key)
{
    if (state != NULL && state->recovered_virtual_key == virtual_key)
        state->recovered_virtual_key = 0u;
}

int softpc_win32_keyboard_consume_duplicate_character(
    softpc_win32_keyboard_normalizer *state, WORD code_unit)
{
    SHORT mapped;
    int duplicate;

    if (state == NULL || state->recovered_virtual_key == 0u || code_unit == 0u ||
        (code_unit >= 0xd800u && code_unit <= 0xdfffu)) return 0;
    mapped = VkKeyScanExW((WCHAR)code_unit, GetKeyboardLayout(0u));
    duplicate = mapped != -1 && (WORD)(mapped & 0xffu) ==
        state->recovered_virtual_key;
    state->recovered_virtual_key = 0u;
    return duplicate;
}

static int softpc_win32_keyboard_submit_character(void *context,
    softpc_win32_keyboard_sink sink, uint32_t scalar)
{
    SHORT mapped;
    WORD virtual_key;
    WORD scan;
    uint8_t modifiers;

    if (scalar == 0u || scalar > 0xffffu ||
        (scalar >= 0xd800u && scalar <= 0xdfffu)) return 0;
    mapped = VkKeyScanExW((WCHAR)scalar, GetKeyboardLayout(0u));
    if (mapped == -1) return 0;
    virtual_key = (WORD)(mapped & 0xffu);
    scan = softpc_win32_keyboard_resolve_scan(virtual_key);
    if (scan == 0u) return 0;
    modifiers = (uint8_t)((mapped >> 8u) & 0xffu);
    if ((modifiers & 2u) != 0u && !softpc_win32_keyboard_emit(context, sink,
            0x1du, VK_CONTROL, 0u, 1)) return 0;
    if ((modifiers & 4u) != 0u && !softpc_win32_keyboard_emit(context, sink,
            0x38u, VK_MENU, 0u, 1)) return 0;
    if ((modifiers & 1u) != 0u && !softpc_win32_keyboard_emit(context, sink,
            0x2au, VK_SHIFT, 0u, 1)) return 0;
    if (!softpc_win32_keyboard_emit(context, sink, scan, virtual_key, 0u, 1) ||
        !softpc_win32_keyboard_emit(context, sink, scan, virtual_key, 0u, 0))
        return 0;
    if ((modifiers & 1u) != 0u && !softpc_win32_keyboard_emit(context, sink,
            0x2au, VK_SHIFT, 0u, 0)) return 0;
    if ((modifiers & 4u) != 0u && !softpc_win32_keyboard_emit(context, sink,
            0x38u, VK_MENU, 0u, 0)) return 0;
    if ((modifiers & 2u) != 0u && !softpc_win32_keyboard_emit(context, sink,
            0x1du, VK_CONTROL, 0u, 0)) return 0;
    return 1;
}

int softpc_win32_keyboard_submit_utf16(softpc_win32_keyboard_normalizer *state,
    void *context, softpc_win32_keyboard_sink sink, WORD code_unit)
{
    uint32_t scalar;

    if (state == NULL) return 0;
    if (code_unit >= 0xd800u && code_unit <= 0xdbffu) {
        if (state->pending_high_surrogate != 0u) return 0;
        state->pending_high_surrogate = code_unit;
        return 1;
    }
    if (code_unit >= 0xdc00u && code_unit <= 0xdfffu) {
        if (state->pending_high_surrogate == 0u) return 0;
        scalar = 0x10000u + (((uint32_t)state->pending_high_surrogate -
            0xd800u) << 10u) + ((uint32_t)code_unit - 0xdc00u);
        state->pending_high_surrogate = 0u;
        return softpc_win32_keyboard_submit_character(context, sink, scalar);
    }
    if (state->pending_high_surrogate != 0u) {
        state->pending_high_surrogate = 0u;
        return 0;
    }
    return softpc_win32_keyboard_submit_character(context, sink, code_unit);
}
#endif
