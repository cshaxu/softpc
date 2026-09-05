#ifndef WIN32_PRESENTATION_EVENT_H
#define WIN32_PRESENTATION_EVENT_H

#include <stdint.h>

/* Product-neutral host input emitted by the presentation library.  Virtual
 * keys and scan codes describe a host physical transition; text is the
 * Unicode scalar that could not be represented by such a transition. */
typedef enum win32_presentation_event_type {
    WIN32_PRESENTATION_EVENT_KEY,
    WIN32_PRESENTATION_EVENT_TEXT,
    WIN32_PRESENTATION_EVENT_MOUSE
} win32_presentation_event_type;

typedef struct win32_presentation_event {
    win32_presentation_event_type type;
    union {
        struct {
            uint16_t scan_code;
            uint16_t virtual_key;
            uint32_t modifiers;
            uint8_t pressed;
        } key;
        struct {
            uint32_t scalar;
        } text;
        struct {
            int32_t delta_x;
            int32_t delta_y;
            uint8_t left_down;
            uint8_t right_down;
        } mouse;
    } data;
} win32_presentation_event;

typedef int (*win32_presentation_event_sink)(void *context,
    const win32_presentation_event *event);

#endif
