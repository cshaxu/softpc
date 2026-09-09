#ifndef UX_EVENT_H
#define UX_EVENT_H

#include "lib/base/base.h"

/* Product-neutral host input emitted by the presentation library.  Virtual
 * keys and scan codes describe a host physical transition; text is the
 * Unicode scalar that could not be represented by such a transition. */
typedef enum ux_event_type {
    UX_EVENT_KEY,
    UX_EVENT_TEXT,
    UX_EVENT_MOUSE,
    UX_EVENT_HOTKEY,
    UX_EVENT_WINDOW_CLOSE,
    /* A raw component is permanently retiring. This is its final asynchronous
     * lifetime fact; the application clears pressed state by source_identity
     * and must not dereference the borrowed source handle. */
    UX_EVENT_SOURCE_RETIRED
} ux_event_type;

#define UX_HOTKEY_IDENTIFIER_CAPACITY 64u

enum {
    /* Neutral terminal key identities.  These describe input only; registered
     * hotkey interpretation remains an application concern. */
    UX_KEY_F1 = 0x00010001u,
    UX_KEY_F2 = 0x00010002u,
    UX_KEY_F3 = 0x00010003u,
    UX_KEY_F4 = 0x00010004u,
    UX_KEY_F5 = 0x00010005u,
    UX_KEY_F6 = 0x00010006u,
    UX_KEY_F7 = 0x00010007u,
    UX_KEY_F8 = 0x00010008u,
    UX_KEY_F9 = 0x00010009u,
    UX_KEY_F10 = 0x0001000au,
    UX_KEY_F11 = 0x0001000bu,
    UX_KEY_F12 = 0x0001000cu,
    UX_KEY_ENTER = 0x00020001u,
    UX_KEY_BACKSPACE = 0x00020002u,
    UX_KEY_UP = 0x00020003u,
    UX_KEY_DOWN = 0x00020004u,
    UX_KEY_LEFT = 0x00020005u,
    UX_KEY_RIGHT = 0x00020006u,
    UX_KEY_HOME = 0x00020007u,
    UX_KEY_END = 0x00020008u,
    UX_KEY_PAGE_UP = 0x00020009u,
    UX_KEY_PAGE_DOWN = 0x0002000au,
    UX_KEY_INSERT = 0x0002000bu,
    UX_KEY_DELETE = 0x0002000cu,
    UX_MOUSE_BUTTON_LEFT = 0x01u,
    UX_MOUSE_BUTTON_RIGHT = 0x02u,
    UX_MOUSE_BUTTON_MIDDLE = 0x04u
};

typedef struct ux_input_event {
    /* Borrowed opaque component handle. It is for lifetime tracing only;
     * product action policy never branches on input source. */
    const void *source;
    /* Monotonic instance identity.  Unlike the borrowed address above this
     * value remains unambiguous after the component has been retired and its
     * storage can be reused.  Consumers use it only for lifetime hygiene. */
    lib_u64 source_identity;
    ux_event_type type;
    union {
        struct {
            lib_u16 scan_code;
            /* A host-native or neutral key identity. Zero means absent. */
            lib_u32 virtual_key;
            /* Native guest-injection state (for example ENHANCED_KEY). */
            lib_u32 modifiers;
            /* Platform-neutral Ctrl/Alt/Shift mask used only by matcher. */
            lib_u8 hotkey_modifiers;
            lib_u8 pressed;
        } key;
        struct {
            lib_u32 scalar;
        } text;
        struct {
            lib_i32 delta_x;
            lib_i32 delta_y;
            lib_i32 absolute_x;
            lib_i32 absolute_y;
            lib_i32 wheel_x;
            lib_i32 wheel_y;
            lib_u32 buttons;
            lib_u8 relative;
        } mouse;
        struct {
            char identifier[UX_HOTKEY_IDENTIFIER_CAPACITY];
        } hotkey;
    } data;
} ux_input_event;

/* Legacy spellings preserve the existing input producer ABI while split
 * components and the application FIFO migrate to the explicit input-event
 * name. */
typedef ux_input_event ux_event;

typedef int (*ux_input_sink)(void *context, const ux_input_event *event);
typedef ux_input_sink ux_event_sink;

static inline void ux_input_event_set_source(ux_input_event *event,
    const void *source, lib_u64 source_identity)
{
    if (event != LIB_NULL) {
        event->source = source;
        event->source_identity = source_identity;
    }
}

#endif
