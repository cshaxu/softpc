#ifndef WIN32_PRESENTATION_INPUT_H
#define WIN32_PRESENTATION_INPUT_H

#include <stdint.h>
#include "event.h"

#ifdef _WIN32
#include <windows.h>

/* This component normalizes Win32 input only. It emits product-neutral input
 * events and neither knows nor maps any guest keyboard/controller protocol. */

typedef struct win32_presentation_keyboard_normalizer {
    WORD pending_high_surrogate;
    WORD recovered_virtual_key;
    WORD suppressed_virtual_key;
} win32_presentation_keyboard_normalizer;

/* These functions only normalize host packets.  A project binding maps each
   physical record to its guest input protocol and owns its input queue. */
int win32_presentation_keyboard_submit_transition(void *context,
    win32_presentation_event_sink sink, WORD scan, WORD virtual_key,
    DWORD control_state, int pressed);
int win32_presentation_keyboard_submit_utf16(
    win32_presentation_keyboard_normalizer *state, void *context,
    win32_presentation_event_sink sink, WORD code_unit);
/* Emit the standard physical Ctrl+Alt+Del make/break sequence. */
int win32_presentation_keyboard_submit_ctrl_alt_del(void *context,
    win32_presentation_event_sink sink);
/* A project may consume a Ctrl+Alt host chord after its modifier makes have
   arrived.  Emit releases before the binding pauses or substitutes input. */
int win32_presentation_keyboard_release_ctrl_alt(void *context,
    win32_presentation_event_sink sink);
/* Emit Alt+Enter after neutralizing the consumed Ctrl+Alt host chord. */
int win32_presentation_keyboard_submit_alt_enter(void *context,
    win32_presentation_event_sink sink);
void win32_presentation_keyboard_note_recovered_key(
    win32_presentation_keyboard_normalizer *state, WORD virtual_key);
void win32_presentation_keyboard_release_recovered_key(
    win32_presentation_keyboard_normalizer *state, WORD virtual_key);
int win32_presentation_keyboard_consume_duplicate_character(
    win32_presentation_keyboard_normalizer *state, WORD code_unit);
#endif

#endif
