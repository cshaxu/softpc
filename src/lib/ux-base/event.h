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
    /* A raw component is permanently retiring. The application must clear
     * any guest pressed-state associated with this source before forgetting it. */
    UX_EVENT_SOURCE_RETIRED
} ux_event_type;

#define UX_HOTKEY_IDENTIFIER_CAPACITY 64u

enum {
    UX_MOUSE_BUTTON_LEFT = 0x01u,
    UX_MOUSE_BUTTON_RIGHT = 0x02u,
    UX_MOUSE_BUTTON_MIDDLE = 0x04u
};

typedef struct ux_input_event {
    /* Borrowed opaque component handle. It is for lifetime tracing only;
     * product action policy never branches on input source. */
    const void *source;
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

/* Legacy spellings preserve the existing input producer ABI while the split
 * components and SoftPC FIFO migrate to the explicit input-event name. */
typedef ux_input_event ux_event;

typedef int (*ux_input_sink)(void *context, const ux_input_event *event);
typedef ux_input_sink ux_event_sink;

static inline void ux_input_event_set_source(ux_input_event *event,
    const void *source)
{
    if (event != LIB_NULL) event->source = source;
}

#endif
