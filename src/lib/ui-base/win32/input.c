#include "lib/types/win32/scalar.h"
#include "lib/ui-base/win32/input.h"
#include "lib/ui-base/hotkey_interface.h"
#include "lib/types/win32/input.h"

static lib_u16 ui_win32_keyboard_resolve_scan(lib_u16 virtual_key)
{
    return (lib_u16)lib_win32_map_virtual_key((lib_win32_uint)virtual_key, LIB_WIN32_MAPVK_VK_TO_VSC);
}

/* Layout interpretation belongs to the UI keyboard adapter.  types exposes
 * only raw key-state and scan-code queries. */
static lib_bool ui_win32_keyboard_map_scalar(lib_u32 scalar,
    lib_u16 *out_virtual_key, lib_u8 *out_modifiers)
{
    lib_win32_key_state mapped;

    if (out_virtual_key == LIB_NULL || out_modifiers == LIB_NULL || scalar > 0xffu)
        return LIB_FALSE;
    mapped = lib_win32_key_scan((lib_win32_char)scalar);
    if (mapped == -1) return LIB_FALSE;
    *out_virtual_key = (lib_u16)(mapped & 0xff);
    *out_modifiers = 0u;
    if (((mapped >> 8) & LIB_WIN32_KEY_SCAN_SHIFT) != 0u)
        *out_modifiers |= UI_WIN32_INPUT_MODIFIER_SHIFT;
    if (((mapped >> 8) & LIB_WIN32_KEY_SCAN_CONTROL) != 0u)
        *out_modifiers |= UI_WIN32_INPUT_MODIFIER_CONTROL;
    if (((mapped >> 8) & LIB_WIN32_KEY_SCAN_ALT) != 0u)
        *out_modifiers |= UI_WIN32_INPUT_MODIFIER_ALT;
    return LIB_TRUE;
}

/* Native values stop at this adapter boundary.  This component decides how
 * they become neutral UI key identities. */
static lib_u32 ui_win32_keyboard_key(lib_u16 virtual_key)
{
    if ((virtual_key >= LIB_WIN32_KEY_0 && virtual_key <= LIB_WIN32_KEY_9) ||
        (virtual_key >= LIB_WIN32_KEY_A && virtual_key <= LIB_WIN32_KEY_Z))
        return virtual_key;
    if (virtual_key >= LIB_WIN32_KEY_NUMPAD_0 && virtual_key <= LIB_WIN32_KEY_NUMPAD_9)
        return UI_KEY_KEYPAD_0 + virtual_key - LIB_WIN32_KEY_NUMPAD_0;
    if (virtual_key >= LIB_WIN32_KEY_F1 && virtual_key <= LIB_WIN32_KEY_F12)
        return UI_KEY_F1 + virtual_key - LIB_WIN32_KEY_F1;
    if (virtual_key >= LIB_WIN32_KEY_F13 && virtual_key <= LIB_WIN32_KEY_F24)
        return UI_KEY_F13 + virtual_key - LIB_WIN32_KEY_F13;
    switch (virtual_key) {
    case LIB_WIN32_KEY_SPACE: return ' ';
    case LIB_WIN32_KEY_OEM_1: return ';'; case LIB_WIN32_KEY_OEM_PLUS: return '=';
    case LIB_WIN32_KEY_OEM_COMMA: return ','; case LIB_WIN32_KEY_OEM_MINUS: return '-';
    case LIB_WIN32_KEY_OEM_PERIOD: return '.'; case LIB_WIN32_KEY_OEM_2: return '/';
    case LIB_WIN32_KEY_OEM_3: return '`'; case LIB_WIN32_KEY_OEM_4: return '[';
    case LIB_WIN32_KEY_OEM_5: return '\\'; case LIB_WIN32_KEY_OEM_6: return ']';
    case LIB_WIN32_KEY_OEM_7: return '\'';
    case LIB_WIN32_KEY_BACK: return UI_KEY_BACKSPACE; case LIB_WIN32_KEY_TAB: return UI_KEY_TAB;
    case LIB_WIN32_KEY_RETURN: return UI_KEY_ENTER; case LIB_WIN32_KEY_ESCAPE: return UI_KEY_ESCAPE;
    case LIB_WIN32_KEY_SHIFT: case LIB_WIN32_KEY_LEFT_SHIFT: case LIB_WIN32_KEY_RIGHT_SHIFT: return UI_KEY_SHIFT;
    case LIB_WIN32_KEY_CONTROL: case LIB_WIN32_KEY_LEFT_CONTROL: case LIB_WIN32_KEY_RIGHT_CONTROL: return UI_KEY_CONTROL;
    case LIB_WIN32_KEY_ALT: case LIB_WIN32_KEY_LEFT_ALT: case LIB_WIN32_KEY_RIGHT_ALT: return UI_KEY_ALT;
    case LIB_WIN32_KEY_CAPS_LOCK: return UI_KEY_CAPS_LOCK; case LIB_WIN32_KEY_NUM_LOCK: return UI_KEY_NUM_LOCK;
    case LIB_WIN32_KEY_SCROLL_LOCK: return UI_KEY_SCROLL_LOCK; case LIB_WIN32_KEY_PAUSE: return UI_KEY_PAUSE;
    case LIB_WIN32_KEY_SNAPSHOT: return UI_KEY_PRINT_SCREEN; case LIB_WIN32_KEY_HOME: return UI_KEY_HOME;
    case LIB_WIN32_KEY_END: return UI_KEY_END; case LIB_WIN32_KEY_PAGE_UP: return UI_KEY_PAGE_UP;
    case LIB_WIN32_KEY_PAGE_DOWN: return UI_KEY_PAGE_DOWN; case LIB_WIN32_KEY_LEFT: return UI_KEY_LEFT;
    case LIB_WIN32_KEY_UP: return UI_KEY_UP; case LIB_WIN32_KEY_RIGHT: return UI_KEY_RIGHT;
    case LIB_WIN32_KEY_DOWN: return UI_KEY_DOWN; case LIB_WIN32_KEY_INSERT: return UI_KEY_INSERT;
    case LIB_WIN32_KEY_DELETE: return UI_KEY_DELETE; case LIB_WIN32_KEY_LEFT_WINDOWS: return UI_KEY_LEFT_WINDOWS;
    case LIB_WIN32_KEY_RIGHT_WINDOWS: return UI_KEY_RIGHT_WINDOWS; case LIB_WIN32_KEY_APPS: return UI_KEY_MENU;
    case LIB_WIN32_KEY_MULTIPLY: return UI_KEY_KEYPAD_MULTIPLY;
    case LIB_WIN32_KEY_ADD: return UI_KEY_KEYPAD_ADD; case LIB_WIN32_KEY_SUBTRACT: return UI_KEY_KEYPAD_SUBTRACT;
    case LIB_WIN32_KEY_DECIMAL: return UI_KEY_KEYPAD_DECIMAL; case LIB_WIN32_KEY_DIVIDE: return UI_KEY_KEYPAD_DIVIDE;
    default: return 0u;
    }
}

static int ui_win32_keyboard_emit(void *context, ui_event_sink sink,
    lib_u16 scan, lib_u16 virtual_key, lib_u8 record_flags,
    lib_u8 hotkey_modifiers, int pressed)
{
    ui_event event;
    lib_u32 key = ui_win32_keyboard_key(virtual_key);

    if (sink == LIB_NULL || key == 0u || scan == 0u) return 0;
    lib_memory_set(&event, 0, sizeof(event));
    event.type = UI_EVENT_KEY;
    event.data.key.pressed = pressed != 0;
    event.data.key.key = key;
    event.data.key.scan_code = scan;
    event.data.key.flags = (record_flags & UI_WIN32_INPUT_FLAG_EXTENDED) != 0u ?
        UI_KEY_FLAG_EXTENDED : 0u;
    event.data.key.modifiers = hotkey_modifiers;
    return sink(context, &event);
}

int ui_win32_keyboard_submit_transition(void *context, ui_event_sink sink,
    lib_u16 scan, lib_u16 virtual_key, lib_u8 record_flags,
    lib_u8 hotkey_modifiers, int pressed)
{
    if (scan == 0u) scan = ui_win32_keyboard_resolve_scan(virtual_key);
    return ui_win32_keyboard_emit(context, sink, scan, virtual_key,
        record_flags, hotkey_modifiers, pressed);
}

void ui_win32_keyboard_note_recovered_key(
    ui_win32_keyboard_normalizer *state, lib_u16 virtual_key)
{
    if (state != LIB_NULL) state->recovered_virtual_key =
        ui_win32_keyboard_resolve_scan(virtual_key) == 0u ? 0u : virtual_key;
}

void ui_win32_keyboard_release_recovered_key(
    ui_win32_keyboard_normalizer *state, lib_u16 virtual_key)
{
    if (state != LIB_NULL && state->recovered_virtual_key == virtual_key)
        state->recovered_virtual_key = 0u;
}

int ui_win32_keyboard_consume_duplicate_character(
    ui_win32_keyboard_normalizer *state, lib_u16 code_unit)
{
    lib_u16 virtual_key;
    lib_u8 modifiers;
    int duplicate;

    if (state == LIB_NULL || state->recovered_virtual_key == 0u || code_unit == 0u ||
        (code_unit >= 0xd800u && code_unit <= 0xdfffu)) return 0;
    duplicate = ui_win32_keyboard_map_scalar(code_unit, &virtual_key, &modifiers) &&
        virtual_key == state->recovered_virtual_key;
    state->recovered_virtual_key = 0u;
    return duplicate;
}

static int ui_win32_keyboard_submit_character(void *context,
    ui_event_sink sink, lib_u32 scalar)
{
    lib_u16 virtual_key;
    lib_u16 scan;
    lib_u8 modifiers;
    lib_u8 hotkey_modifiers = 0u;

    if (scalar == 0u || scalar > 0xffffu ||
        (scalar >= 0xd800u && scalar <= 0xdfffu)) return 0;
    if (!ui_win32_keyboard_map_scalar(scalar, &virtual_key, &modifiers)) {
        ui_event event;
        lib_memory_set(&event, 0, sizeof(event));
        event.type = UI_EVENT_TEXT;
        event.data.text.scalar = scalar;
        return sink != LIB_NULL && sink(context, &event);
    }
    scan = ui_win32_keyboard_resolve_scan(virtual_key);
    if (scan == 0u) return 0;
    if ((modifiers & UI_WIN32_INPUT_MODIFIER_CONTROL) != 0u) {
        hotkey_modifiers |= UI_HOTKEY_MODIFIER_CONTROL;
        if (!ui_win32_keyboard_emit(context, sink, 0x1du, LIB_WIN32_KEY_CONTROL, 0u,
                hotkey_modifiers, 1)) return 0;
    }
    if ((modifiers & UI_WIN32_INPUT_MODIFIER_ALT) != 0u) {
        hotkey_modifiers |= UI_HOTKEY_MODIFIER_ALT;
        if (!ui_win32_keyboard_emit(context, sink, 0x38u, LIB_WIN32_KEY_ALT, 0u,
                hotkey_modifiers, 1)) return 0;
    }
    if ((modifiers & UI_WIN32_INPUT_MODIFIER_SHIFT) != 0u) {
        hotkey_modifiers |= UI_HOTKEY_MODIFIER_SHIFT;
        if (!ui_win32_keyboard_emit(context, sink, 0x2au, LIB_WIN32_KEY_SHIFT, 0u,
                hotkey_modifiers, 1)) return 0;
    }
    if (!ui_win32_keyboard_emit(context, sink, scan, virtual_key, 0u,
            hotkey_modifiers, 1) ||
        !ui_win32_keyboard_emit(context, sink, scan, virtual_key, 0u,
            hotkey_modifiers, 0)) return 0;
    if ((modifiers & UI_WIN32_INPUT_MODIFIER_SHIFT) != 0u) {
        hotkey_modifiers &= (lib_u8)~UI_HOTKEY_MODIFIER_SHIFT;
        if (!ui_win32_keyboard_emit(context, sink, 0x2au, LIB_WIN32_KEY_SHIFT, 0u,
                hotkey_modifiers, 0)) return 0;
    }
    if ((modifiers & UI_WIN32_INPUT_MODIFIER_ALT) != 0u) {
        hotkey_modifiers &= (lib_u8)~UI_HOTKEY_MODIFIER_ALT;
        if (!ui_win32_keyboard_emit(context, sink, 0x38u, LIB_WIN32_KEY_ALT, 0u,
                hotkey_modifiers, 0)) return 0;
    }
    if ((modifiers & UI_WIN32_INPUT_MODIFIER_CONTROL) != 0u) {
        hotkey_modifiers &= (lib_u8)~UI_HOTKEY_MODIFIER_CONTROL;
        if (!ui_win32_keyboard_emit(context, sink, 0x1du, LIB_WIN32_KEY_CONTROL, 0u,
                hotkey_modifiers, 0)) return 0;
    }
    return 1;
}

int ui_win32_keyboard_submit_utf16(ui_win32_keyboard_normalizer *state,
    void *context, ui_event_sink sink, lib_u16 code_unit)
{
    lib_u32 scalar;

    if (state == LIB_NULL) return 0;
    if (code_unit >= 0xd800u && code_unit <= 0xdbffu) {
        if (state->pending_high_surrogate != 0u) return 0;
        state->pending_high_surrogate = code_unit;
        return 1;
    }
    if (code_unit >= 0xdc00u && code_unit <= 0xdfffu) {
        if (state->pending_high_surrogate == 0u) return 0;
        scalar = 0x10000u + (((lib_u32)state->pending_high_surrogate -
            0xd800u) << 10u) + ((lib_u32)code_unit - 0xdc00u);
        state->pending_high_surrogate = 0u;
        return ui_win32_keyboard_submit_character(context, sink, scalar);
    }
    if (state->pending_high_surrogate != 0u) {
        state->pending_high_surrogate = 0u;
        return 0;
    }
    return ui_win32_keyboard_submit_character(context, sink, code_unit);
}

lib_u8 ui_win32_keyboard_flags_from_lparam(lib_u64 message_lparam)
{
    return (message_lparam & 0x01000000u) != 0u ?
        UI_WIN32_INPUT_FLAG_EXTENDED : 0u;
}
