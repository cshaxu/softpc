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
    lib_u16 character_key;
} ui_keyboard_normalizer;

/* These functions only normalize host packets.  A project binding maps each
   physical record to its own input protocol and owns its input queue. */
int ui_keyboard_submit_transition(void *context,
    ui_input_sink sink, lib_u16 scan, lib_u16 virtual_key,
    lib_u8 record_flags, lib_u8 hotkey_modifiers, int pressed);
int ui_keyboard_submit_utf16(
    ui_keyboard_normalizer *state, void *context,
    ui_input_sink sink, lib_u16 code_unit);
/* Native adapters copy either separate transition/character messages or one
 * combined record. In a CHARACTER packet, nonzero scan identifies a character
 * translated from a physical message, not independent text. This entry alone
 * selects physical vs text and deduplicates. */
typedef enum ui_keyboard_record_kind {
    UI_KEYBOARD_TRANSITION,
    UI_KEYBOARD_CHARACTER,
    UI_KEYBOARD_COMBINED
} ui_keyboard_record_kind;

typedef struct ui_keyboard_record {
    ui_keyboard_record_kind kind;
    lib_u16 scan;
    lib_u16 key;
    lib_u16 utf16;
    lib_u8 flags;
    lib_u8 modifiers;
    lib_bool pressed;
} ui_keyboard_record;

int ui_keyboard_submit_record(ui_keyboard_normalizer *state, void *context,
    ui_input_sink sink, const ui_keyboard_record *record);

#endif
