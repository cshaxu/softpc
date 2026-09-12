#ifndef UI_INPUT_INTERFACE_H
#define UI_INPUT_INTERFACE_H

/* Leaf-support contract for copied platform records, not an application ABI.
 * Raw key values are decoded by the selected component-local implementation.
 * Callers own one zero-initialized normalizer per input source. */

#include "lib/ui-base/event_interface.h"

/* Copied adapter flags, never a raw platform control-state word. */
enum {
    UI_INPUT_MODIFIER_CONTROL = 0x01u,
    UI_INPUT_MODIFIER_ALT = 0x02u,
    UI_INPUT_MODIFIER_SHIFT = 0x04u,
    UI_INPUT_FLAG_EXTENDED = 0x01u
};

/* Common normalization owns state and delivery. Platform decoding owns layout
 * and scan recovery. A platform without a physical text mapping emits TEXT. */

typedef struct ui_keyboard_normalizer {
    lib_u16 pending_high_surrogate;
    lib_u16 recovered_virtual_key;
} ui_keyboard_normalizer;

/* These functions only normalize host packets.  A project binding maps each
   physical record to its own input protocol and owns its input queue. */
int ui_keyboard_submit_transition(void *context,
    ui_input_sink sink, lib_u16 scan, lib_u16 virtual_key,
    lib_u8 record_flags, lib_u8 hotkey_modifiers, int pressed);
int ui_keyboard_submit_utf16(
    ui_keyboard_normalizer *state, void *context,
    ui_input_sink sink, lib_u16 code_unit);
void ui_keyboard_note_recovered_key(
    ui_keyboard_normalizer *state, lib_u16 virtual_key);
void ui_keyboard_release_recovered_key(
    ui_keyboard_normalizer *state, lib_u16 virtual_key);
int ui_keyboard_consume_duplicate_character(
    ui_keyboard_normalizer *state, lib_u16 code_unit);

#endif
