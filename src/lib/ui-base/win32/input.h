#ifndef UI_WIN32_INPUT_H
#define UI_WIN32_INPUT_H

#include "lib/ui-base/event_interface.h"

/* Win32 virtual-key vocabulary belongs to this normalizer. These copied
 * values never escape ui-base as the cross-platform input contract. */
enum {
    UI_WIN32_KEY_BACK = 0x08u, UI_WIN32_KEY_TAB = 0x09u,
    UI_WIN32_KEY_RETURN = 0x0du, UI_WIN32_KEY_SHIFT = 0x10u,
    UI_WIN32_KEY_CONTROL = 0x11u, UI_WIN32_KEY_ALT = 0x12u,
    UI_WIN32_KEY_PAUSE = 0x13u, UI_WIN32_KEY_CAPS_LOCK = 0x14u,
    UI_WIN32_KEY_ESCAPE = 0x1bu, UI_WIN32_KEY_SPACE = 0x20u,
    UI_WIN32_KEY_PAGE_UP = 0x21u, UI_WIN32_KEY_PAGE_DOWN = 0x22u,
    UI_WIN32_KEY_END = 0x23u, UI_WIN32_KEY_HOME = 0x24u,
    UI_WIN32_KEY_LEFT = 0x25u, UI_WIN32_KEY_UP = 0x26u,
    UI_WIN32_KEY_RIGHT = 0x27u, UI_WIN32_KEY_DOWN = 0x28u,
    UI_WIN32_KEY_SNAPSHOT = 0x2cu, UI_WIN32_KEY_INSERT = 0x2du,
    UI_WIN32_KEY_DELETE = 0x2eu, UI_WIN32_KEY_0 = 0x30u,
    UI_WIN32_KEY_9 = 0x39u, UI_WIN32_KEY_A = 0x41u,
    UI_WIN32_KEY_Z = 0x5au, UI_WIN32_KEY_LEFT_WINDOWS = 0x5bu,
    UI_WIN32_KEY_RIGHT_WINDOWS = 0x5cu, UI_WIN32_KEY_APPS = 0x5du,
    UI_WIN32_KEY_NUMPAD_0 = 0x60u, UI_WIN32_KEY_NUMPAD_9 = 0x69u,
    UI_WIN32_KEY_MULTIPLY = 0x6au, UI_WIN32_KEY_ADD = 0x6bu,
    UI_WIN32_KEY_SUBTRACT = 0x6du, UI_WIN32_KEY_DECIMAL = 0x6eu,
    UI_WIN32_KEY_DIVIDE = 0x6fu, UI_WIN32_KEY_F1 = 0x70u,
    UI_WIN32_KEY_F12 = 0x7bu, UI_WIN32_KEY_F13 = 0x7cu,
    UI_WIN32_KEY_F24 = 0x87u, UI_WIN32_KEY_NUM_LOCK = 0x90u,
    UI_WIN32_KEY_SCROLL_LOCK = 0x91u, UI_WIN32_KEY_LEFT_SHIFT = 0xa0u,
    UI_WIN32_KEY_RIGHT_SHIFT = 0xa1u, UI_WIN32_KEY_LEFT_CONTROL = 0xa2u,
    UI_WIN32_KEY_RIGHT_CONTROL = 0xa3u, UI_WIN32_KEY_LEFT_ALT = 0xa4u,
    UI_WIN32_KEY_RIGHT_ALT = 0xa5u, UI_WIN32_KEY_OEM_1 = 0xbau,
    UI_WIN32_KEY_OEM_PLUS = 0xbbu, UI_WIN32_KEY_OEM_COMMA = 0xbcu,
    UI_WIN32_KEY_OEM_MINUS = 0xbdu, UI_WIN32_KEY_OEM_PERIOD = 0xbeu,
    UI_WIN32_KEY_OEM_2 = 0xbfu, UI_WIN32_KEY_OEM_3 = 0xc0u,
    UI_WIN32_KEY_OEM_4 = 0xdbu, UI_WIN32_KEY_OEM_5 = 0xdcu,
    UI_WIN32_KEY_OEM_6 = 0xddu, UI_WIN32_KEY_OEM_7 = 0xdeu
};

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
    ui_event_sink sink, lib_u16 scan, lib_u16 native_key,
    lib_u8 native_flags, lib_u8 hotkey_modifiers, int pressed);
int ui_win32_keyboard_submit_utf16(
    ui_win32_keyboard_normalizer *state, void *context,
    ui_event_sink sink, lib_u16 code_unit);
void ui_win32_keyboard_note_recovered_key(
    ui_win32_keyboard_normalizer *state, lib_u16 native_key);
void ui_win32_keyboard_release_recovered_key(
    ui_win32_keyboard_normalizer *state, lib_u16 native_key);
int ui_win32_keyboard_consume_duplicate_character(
    ui_win32_keyboard_normalizer *state, lib_u16 code_unit);
lib_u8 ui_win32_keyboard_flags_from_lparam(lib_u64 native_lparam);

#endif
