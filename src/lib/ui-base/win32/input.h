#ifndef UI_WIN32_INPUT_H
#define UI_WIN32_INPUT_H

#include "lib/ui-base/event_interface.h"

/* Adapter-local flags, not the raw VkKeyScan high-byte representation. */
enum {
    UI_WIN32_INPUT_MODIFIER_CONTROL = 0x01u,
    UI_WIN32_INPUT_MODIFIER_ALT = 0x02u,
    UI_WIN32_INPUT_MODIFIER_SHIFT = 0x04u,
    UI_WIN32_INPUT_FLAG_EXTENDED = 0x01u
};

/* This component normalizes Win32 input only. It emits product-neutral input
 * events and neither knows nor maps any product input protocol. */

typedef struct ui_win32_keyboard_normalizer {
    lib_u16 pending_high_surrogate;
    lib_u16 recovered_virtual_key;
    lib_u16 suppressed_virtual_key;
} ui_win32_keyboard_normalizer;

/* These functions only normalize host packets.  A project binding maps each
   physical record to its own input protocol and owns its input queue. */
int ui_win32_keyboard_submit_transition(void *context,
    ui_event_sink sink, lib_u16 scan, lib_u16 virtual_key,
    lib_u8 record_flags, lib_u8 hotkey_modifiers, int pressed);
int ui_win32_keyboard_submit_utf16(
    ui_win32_keyboard_normalizer *state, void *context,
    ui_event_sink sink, lib_u16 code_unit);
void ui_win32_keyboard_note_recovered_key(
    ui_win32_keyboard_normalizer *state, lib_u16 virtual_key);
void ui_win32_keyboard_release_recovered_key(
    ui_win32_keyboard_normalizer *state, lib_u16 virtual_key);
int ui_win32_keyboard_consume_duplicate_character(
    ui_win32_keyboard_normalizer *state, lib_u16 code_unit);
lib_u8 ui_win32_keyboard_flags_from_lparam(lib_u64 message_lparam);

#endif
