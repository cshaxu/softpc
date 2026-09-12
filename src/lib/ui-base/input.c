#include "lib/ui-base/input.h"
#include "lib/ui-base/hotkey_interface.h"

static int ui_keyboard_emit(void *context, ui_input_sink sink,
    lib_u16 scan, lib_u32 key, lib_u8 record_flags,
    lib_u8 hotkey_modifiers, int pressed)
{
    ui_input_event event;

    if (sink == LIB_NULL || key == 0u) return 0;
    lib_memory_set(&event, 0, sizeof(event));
    event.type = UI_EVENT_KEY;
    event.data.key.pressed = pressed != 0;
    event.data.key.key = key;
    event.data.key.scan_code = scan;
    event.data.key.flags = (record_flags & UI_INPUT_FLAG_EXTENDED) != 0u ?
        UI_KEY_FLAG_EXTENDED : 0u;
    event.data.key.modifiers = hotkey_modifiers;
    return sink(context, &event);
}

int ui_keyboard_submit_transition(void *context, ui_input_sink sink,
    lib_u16 scan, lib_u16 virtual_key, lib_u8 record_flags,
    lib_u8 hotkey_modifiers, int pressed)
{
    lib_u32 key;
    if (!ui_keyboard_platform_transition(scan, virtual_key, &scan, &key)) return 0;
    return ui_keyboard_emit(context, sink, scan, key,
        record_flags, hotkey_modifiers, pressed);
}

void ui_keyboard_note_recovered_key(
    ui_keyboard_normalizer *state, lib_u16 virtual_key)
{
    lib_u16 scan;
    lib_u32 key;
    if (state != LIB_NULL) state->recovered_virtual_key =
        ui_keyboard_platform_transition(0u, virtual_key, &scan, &key) ? virtual_key : 0u;
}

void ui_keyboard_release_recovered_key(
    ui_keyboard_normalizer *state, lib_u16 virtual_key)
{
    if (state != LIB_NULL && state->recovered_virtual_key == virtual_key)
        state->recovered_virtual_key = 0u;
}

int ui_keyboard_consume_duplicate_character(
    ui_keyboard_normalizer *state, lib_u16 code_unit)
{
    lib_u16 virtual_key;
    lib_u8 modifiers;
    int duplicate;

    if (state == LIB_NULL || state->recovered_virtual_key == 0u || code_unit == 0u ||
        (code_unit >= 0xd800u && code_unit <= 0xdfffu)) return 0;
    duplicate = ui_keyboard_platform_map_scalar(code_unit, &virtual_key, &modifiers) &&
        virtual_key == state->recovered_virtual_key;
    state->recovered_virtual_key = 0u;
    return duplicate;
}

static int ui_keyboard_submit_character(void *context,
    ui_input_sink sink, lib_u32 scalar)
{
    lib_u16 virtual_key;
    lib_u32 key;
    lib_u16 scan;
    lib_u8 modifiers;
    lib_u8 hotkey_modifiers = 0u;

    if (scalar == 0u || scalar > 0x10ffffu ||
        (scalar >= 0xd800u && scalar <= 0xdfffu)) return 0;
    if (!ui_keyboard_platform_map_scalar(scalar, &virtual_key, &modifiers)) {
        ui_input_event event;
        lib_memory_set(&event, 0, sizeof(event));
        event.type = UI_EVENT_TEXT;
        event.data.text.scalar = scalar;
        return sink != LIB_NULL && sink(context, &event);
    }
    if (!ui_keyboard_platform_transition(0u, virtual_key, &scan, &key)) return 0;
    if ((modifiers & UI_INPUT_MODIFIER_CONTROL) != 0u) {
        hotkey_modifiers |= UI_HOTKEY_MODIFIER_CONTROL;
        if (!ui_keyboard_emit(context, sink, 0x1du, UI_KEY_CONTROL, 0u,
                hotkey_modifiers, 1)) return 0;
    }
    if ((modifiers & UI_INPUT_MODIFIER_ALT) != 0u) {
        hotkey_modifiers |= UI_HOTKEY_MODIFIER_ALT;
        if (!ui_keyboard_emit(context, sink, 0x38u, UI_KEY_ALT, 0u,
                hotkey_modifiers, 1)) return 0;
    }
    if ((modifiers & UI_INPUT_MODIFIER_SHIFT) != 0u) {
        hotkey_modifiers |= UI_HOTKEY_MODIFIER_SHIFT;
        if (!ui_keyboard_emit(context, sink, 0x2au, UI_KEY_SHIFT, 0u,
                hotkey_modifiers, 1)) return 0;
    }
    if (!ui_keyboard_emit(context, sink, scan, key, 0u,
            hotkey_modifiers, 1) ||
        !ui_keyboard_emit(context, sink, scan, key, 0u,
            hotkey_modifiers, 0)) return 0;
    if ((modifiers & UI_INPUT_MODIFIER_SHIFT) != 0u) {
        hotkey_modifiers &= (lib_u8)~UI_HOTKEY_MODIFIER_SHIFT;
        if (!ui_keyboard_emit(context, sink, 0x2au, UI_KEY_SHIFT, 0u,
                hotkey_modifiers, 0)) return 0;
    }
    if ((modifiers & UI_INPUT_MODIFIER_ALT) != 0u) {
        hotkey_modifiers &= (lib_u8)~UI_HOTKEY_MODIFIER_ALT;
        if (!ui_keyboard_emit(context, sink, 0x38u, UI_KEY_ALT, 0u,
                hotkey_modifiers, 0)) return 0;
    }
    if ((modifiers & UI_INPUT_MODIFIER_CONTROL) != 0u) {
        hotkey_modifiers &= (lib_u8)~UI_HOTKEY_MODIFIER_CONTROL;
        if (!ui_keyboard_emit(context, sink, 0x1du, UI_KEY_CONTROL, 0u,
                hotkey_modifiers, 0)) return 0;
    }
    return 1;
}

int ui_keyboard_submit_utf16(ui_keyboard_normalizer *state,
    void *context, ui_input_sink sink, lib_u16 code_unit)
{
    lib_u32 scalar;

    if (state == LIB_NULL) return 0;
    if (code_unit >= 0xd800u && code_unit <= 0xdbffu) {
        if (state->pending_high_surrogate != 0u) {
            state->pending_high_surrogate = 0u;
            return 0;
        }
        state->pending_high_surrogate = code_unit;
        return 1;
    }
    if (code_unit >= 0xdc00u && code_unit <= 0xdfffu) {
        if (state->pending_high_surrogate == 0u) return 0;
        scalar = 0x10000u + (((lib_u32)state->pending_high_surrogate -
            0xd800u) << 10u) + ((lib_u32)code_unit - 0xdc00u);
        state->pending_high_surrogate = 0u;
        return ui_keyboard_submit_character(context, sink, scalar);
    }
    if (state->pending_high_surrogate != 0u) {
        state->pending_high_surrogate = 0u;
        return 0;
    }
    return ui_keyboard_submit_character(context, sink, code_unit);
}
