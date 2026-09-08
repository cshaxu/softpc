#ifndef UX_BASE_INPUT_H
#define UX_BASE_INPUT_H

#include "lib/base/base.h"

#define UX_HOTKEY_IDENTIFIER_CAPACITY 48u
#define UX_HOTKEY_CAPACITY 16u

enum {
    UX_MODIFIER_CONTROL = 0x01u,
    UX_MODIFIER_ALT = 0x02u,
    UX_MODIFIER_SHIFT = 0x04u,
    UX_KEY_CONTROL = 0x11u,
    UX_KEY_ALT = 0x12u,
    UX_KEY_SHIFT = 0x10u
};

typedef enum ux_input_kind {
    UX_INPUT_KEY,
    UX_INPUT_TEXT,
    UX_INPUT_MOUSE,
    UX_INPUT_HOTKEY,
    UX_INPUT_WINDOW_CLOSE_REQUESTED,
    UX_INPUT_RESET
} ux_input_kind;

typedef struct ux_input_event {
    /* Opaque component handle retained only for tracing/lifetime validation.
     * SoftPC must not assign product meaning from this value. */
    const void *source_handle;
    ux_input_kind kind;
    union {
        struct {
            lib_u16 scan_code;
            lib_u32 virtual_key;
            lib_u32 modifiers;
            lib_bool pressed;
        } key;
        struct { lib_u32 scalar; } text;
        struct {
            lib_i32 delta_x;
            lib_i32 delta_y;
            lib_i32 absolute_x;
            lib_i32 absolute_y;
            lib_i32 wheel_x;
            lib_i32 wheel_y;
            lib_u32 buttons;
            lib_bool relative;
        } mouse;
        struct { char identifier[UX_HOTKEY_IDENTIFIER_CAPACITY]; } hotkey;
    } value;
} ux_input_event;

typedef lib_status (*ux_input_sink)(void *context,
    const ux_input_event *event);

typedef struct ux_hotkey_registration {
    lib_u32 virtual_key;
    lib_u32 modifiers;
    char identifier[UX_HOTKEY_IDENTIFIER_CAPACITY];
} ux_hotkey_registration;

typedef struct ux_hotkey_matcher ux_hotkey_matcher;

lib_status ux_input_make_key(ux_input_event *out_event,
    const void *source_handle, lib_u16 scan_code, lib_u32 virtual_key,
    lib_u32 modifiers, lib_bool pressed);
lib_status ux_input_make_hotkey(ux_input_event *out_event,
    const void *source_handle, const char *identifier);
lib_status ux_input_make_window_close(ux_input_event *out_event,
    const void *source_handle);
lib_status ux_input_make_reset(ux_input_event *out_event,
    const void *source_handle);

lib_status ux_hotkey_matcher_create(ux_hotkey_matcher **out_matcher,
    const ux_hotkey_registration *registrations, lib_u32 registration_count);
void ux_hotkey_matcher_destroy(ux_hotkey_matcher *matcher);
lib_status ux_hotkey_matcher_submit(ux_hotkey_matcher *matcher,
    const ux_input_event *event, ux_input_sink sink, void *sink_context);
lib_status ux_hotkey_matcher_retire(ux_hotkey_matcher *matcher,
    const void *source_handle, ux_input_sink sink, void *sink_context);

#endif
