#ifndef SOFTPC_VM_WIN32_KEYBOARD_H
#define SOFTPC_VM_WIN32_KEYBOARD_H

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>

typedef int (*softpc_win32_keyboard_sink)(void *context, uint8_t key_number,
    uint8_t released);

typedef struct softpc_win32_keyboard_normalizer {
    WORD pending_high_surrogate;
    WORD recovered_virtual_key;
    WORD suppressed_virtual_key;
} softpc_win32_keyboard_normalizer;

/* These functions only normalize host packets.  The original nt_keycd table
   still assigns SoftPC key numbers; the runtime remains the sole input queue
   owner and the original 8042 remains the guest controller owner. */
int softpc_win32_keyboard_submit_transition(void *context,
    softpc_win32_keyboard_sink sink, WORD scan, WORD virtual_key,
    DWORD control_state, int pressed);
int softpc_win32_keyboard_submit_utf16(softpc_win32_keyboard_normalizer *state,
    void *context, softpc_win32_keyboard_sink sink, WORD code_unit);
/* Inject the standard physical Ctrl+Alt+Del make/break sequence through the
   same nt_keycd/8042 path as ordinary host keyboard packets. */
int softpc_win32_keyboard_submit_ctrl_alt_del(void *context,
    softpc_win32_keyboard_sink sink);
/* Inject Alt+Enter as four ordinary physical transitions through the same
   nt_keycd/8042 path. This is guest input, never a host window command. */
int softpc_win32_keyboard_submit_alt_enter(void *context,
    softpc_win32_keyboard_sink sink);
void softpc_win32_keyboard_note_recovered_key(
    softpc_win32_keyboard_normalizer *state, WORD virtual_key);
void softpc_win32_keyboard_release_recovered_key(
    softpc_win32_keyboard_normalizer *state, WORD virtual_key);
int softpc_win32_keyboard_consume_duplicate_character(
    softpc_win32_keyboard_normalizer *state, WORD code_unit);
#endif

#endif
