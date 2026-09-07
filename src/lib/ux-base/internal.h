#ifndef UX_BASE_INTERNAL_H
#define UX_BASE_INTERNAL_H

#include "lib/ux-base/frame.h"

typedef struct ux_frame_mailbox {
    atomic_flag lock;
    lib_u32 generation;
    ux_frame frame;
} ux_frame_mailbox;

#define UX_CONTROL_MAILBOX_CAPACITY 32u

typedef enum ux_control_kind {
    UX_CONTROL_STOP,
    UX_CONTROL_SET_TITLE,
    UX_CONTROL_SET_MOUSE_ENABLED,
    UX_CONTROL_RELEASE_MOUSE
} ux_control_kind;

typedef struct ux_control_message {
    ux_control_kind kind;
    union {
        char title[UX_WINDOW_TITLE_CAPACITY];
        lib_bool mouse_enabled;
    } value;
} ux_control_message;

typedef struct ux_control_mailbox {
    atomic_flag lock;
    lib_u32 head;
    lib_u32 count;
    ux_control_message entries[UX_CONTROL_MAILBOX_CAPACITY];
} ux_control_mailbox;

void ux_frame_mailbox_initialize(ux_frame_mailbox *mailbox);
lib_status ux_frame_mailbox_publish(ux_frame_mailbox *mailbox,
    const ux_frame *frame);
lib_status ux_frame_mailbox_take(const ux_frame_mailbox *mailbox,
    ux_frame *out_frame, lib_u32 *out_generation);
void ux_control_mailbox_initialize(ux_control_mailbox *mailbox);
lib_status ux_control_mailbox_push(ux_control_mailbox *mailbox,
    const ux_control_message *message);
lib_status ux_control_mailbox_take(ux_control_mailbox *mailbox,
    ux_control_message *out_message, lib_bool *out_has_message);

#endif
