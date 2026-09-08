#ifndef UX_BASE_HOTKEY_H
#define UX_BASE_HOTKEY_H

#include "lib/ux-base/event.h"

#define UX_HOTKEY_CAPACITY 16u
#define UX_HOTKEY_PENDING_CAPACITY 4u
#define UX_HOTKEY_SUPPRESSED_CAPACITY (UX_HOTKEY_PENDING_CAPACITY + 1u)

enum {
    UX_HOTKEY_MODIFIER_CONTROL = 0x01u,
    UX_HOTKEY_MODIFIER_ALT = 0x02u,
    UX_HOTKEY_MODIFIER_SHIFT = 0x04u,
    UX_HOTKEY_KEY_CONTROL = 0x11u,
    UX_HOTKEY_KEY_ALT = 0x12u,
    UX_HOTKEY_KEY_SHIFT = 0x10u
};

typedef struct ux_hotkey_registration {
    lib_u32 key;
    lib_u8 modifiers;
    char identifier[UX_HOTKEY_IDENTIFIER_CAPACITY];
} ux_hotkey_registration;

typedef struct ux_hotkey_registry {
    ux_hotkey_registration entries[UX_HOTKEY_CAPACITY];
    lib_u32 count;
} ux_hotkey_registry;

typedef struct ux_hotkey_suppressed_key {
    lib_u32 virtual_key;
    lib_u16 scan_code;
} ux_hotkey_suppressed_key;

typedef struct ux_hotkey_matcher {
    ux_hotkey_registry registry;
    ux_input_event pending[UX_HOTKEY_PENDING_CAPACITY];
    lib_u32 pending_count;
    /* A matched chord suppresses every make and every later break belonging
     * to that chord.  Each physical pending make is retained: virtual key
     * alone is not an identity because left/right modifiers share it. */
    ux_hotkey_suppressed_key suppressed_keys[UX_HOTKEY_SUPPRESSED_CAPACITY];
    lib_u32 suppressed_count;
} ux_hotkey_matcher;

void ux_hotkey_registry_initialize(ux_hotkey_registry *registry);
lib_status ux_hotkey_registry_register(ux_hotkey_registry *registry,
    lib_u32 key, lib_u8 modifiers, const char *identifier);
void ux_hotkey_matcher_initialize(ux_hotkey_matcher *matcher,
    const ux_hotkey_registry *registry);
/* Emits ordinary events and matched UX_EVENT_HOTKEY values through `sink`.
 * A false return means the sink rejected an event; no background retry path
 * exists, so caller owns its component-local failure policy. */
int ux_hotkey_matcher_submit(ux_hotkey_matcher *matcher,
    const ux_input_event *event, ux_input_sink sink, void *context);
void ux_hotkey_matcher_discard(ux_hotkey_matcher *matcher);

#endif
