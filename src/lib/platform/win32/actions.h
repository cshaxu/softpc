#ifndef WIN32_PRESENTATION_ACTIONS_H
#define WIN32_PRESENTATION_ACTIONS_H

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>

typedef enum win32_presentation_action {
    WIN32_PRESENTATION_ACTION_NONE = 0,
    WIN32_PRESENTATION_ACTION_PAUSE_TOGGLE,
    WIN32_PRESENTATION_ACTION_SEND_CTRL_ALT_DEL,
    WIN32_PRESENTATION_ACTION_SEND_ALT_ENTER,
    WIN32_PRESENTATION_ACTION_RELEASE_MOUSE
} win32_presentation_action;

enum {
    WIN32_PRESENTATION_MODIFIER_CONTROL = 0x01u,
    WIN32_PRESENTATION_MODIFIER_ALT = 0x02u,
    WIN32_PRESENTATION_MODIFIER_SHIFT = 0x04u
};

typedef struct win32_presentation_action_chord {
    WORD virtual_key;
    uint8_t modifiers;
    win32_presentation_action action;
} win32_presentation_action_chord;

#define WIN32_PRESENTATION_ACTION_CAPACITY 16u
typedef struct win32_presentation_action_registry {
    win32_presentation_action_chord entries[
        WIN32_PRESENTATION_ACTION_CAPACITY];
    uint32_t count;
} win32_presentation_action_registry;

void win32_presentation_actions_init(
    win32_presentation_action_registry *registry);
int win32_presentation_actions_register(
    win32_presentation_action_registry *registry, WORD virtual_key,
    uint8_t modifiers, win32_presentation_action action);
win32_presentation_action win32_presentation_actions_match(
    const win32_presentation_action_registry *registry, WORD virtual_key,
    uint8_t modifiers);
uint8_t win32_presentation_actions_modifiers_from_win32(void);
uint8_t win32_presentation_actions_modifiers_from_control_state(
    DWORD control_state);
#endif

#endif
