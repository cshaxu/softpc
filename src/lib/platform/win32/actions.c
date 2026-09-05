#include "actions.h"

#ifdef _WIN32
#include <string.h>

void win32_presentation_actions_init(
    win32_presentation_action_registry *registry)
{
    if (registry != NULL) memset(registry, 0, sizeof(*registry));
}

int win32_presentation_actions_register(
    win32_presentation_action_registry *registry, WORD virtual_key,
    uint8_t modifiers, win32_presentation_action action)
{
    win32_presentation_action_chord *entry;

    if (registry == NULL || action == WIN32_PRESENTATION_ACTION_NONE ||
        registry->count >= WIN32_PRESENTATION_ACTION_CAPACITY) return 0;
    entry = &registry->entries[registry->count++];
    entry->virtual_key = virtual_key;
    entry->modifiers = modifiers;
    entry->action = action;
    return 1;
}

win32_presentation_action win32_presentation_actions_match(
    const win32_presentation_action_registry *registry, WORD virtual_key,
    uint8_t modifiers)
{
    uint32_t index;

    if (registry == NULL) return WIN32_PRESENTATION_ACTION_NONE;
    for (index = 0u; index < registry->count; ++index) {
        const win32_presentation_action_chord *entry =
            &registry->entries[index];
        if (entry->virtual_key == virtual_key &&
            entry->modifiers == modifiers) return entry->action;
    }
    return WIN32_PRESENTATION_ACTION_NONE;
}

uint8_t win32_presentation_actions_modifiers_from_win32(void)
{
    uint8_t modifiers = 0u;

    if (GetKeyState(VK_CONTROL) < 0)
        modifiers |= WIN32_PRESENTATION_MODIFIER_CONTROL;
    if (GetKeyState(VK_MENU) < 0)
        modifiers |= WIN32_PRESENTATION_MODIFIER_ALT;
    if (GetKeyState(VK_SHIFT) < 0)
        modifiers |= WIN32_PRESENTATION_MODIFIER_SHIFT;
    return modifiers;
}

uint8_t win32_presentation_actions_modifiers_from_control_state(
    DWORD control_state)
{
    uint8_t modifiers = 0u;

    if ((control_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0u)
        modifiers |= WIN32_PRESENTATION_MODIFIER_CONTROL;
    if ((control_state & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0u)
        modifiers |= WIN32_PRESENTATION_MODIFIER_ALT;
    if ((control_state & SHIFT_PRESSED) != 0u)
        modifiers |= WIN32_PRESENTATION_MODIFIER_SHIFT;
    return modifiers;
}
#endif
