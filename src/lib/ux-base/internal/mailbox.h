#ifndef UX_BASE_INTERNAL_MAILBOX_H
#define UX_BASE_INTERNAL_MAILBOX_H

#include "lib/ux-base/frame.h"
#include "lib/ux-base/mailbox_native.h"

#define UX_COMPONENT_CONTROL_CAPACITY 32u
#define UX_COMPONENT_WINDOW_TITLE_CAPACITY 128u

typedef enum ux_component_control_kind {
    UX_COMPONENT_CONTROL_STOP,
    UX_COMPONENT_CONTROL_SET_WINDOW_TITLE,
    UX_COMPONENT_CONTROL_SET_WINDOW_MOUSE_ENABLED,
    UX_COMPONENT_CONTROL_RELEASE_WINDOW_MOUSE
} ux_component_control_kind;

typedef struct ux_component_control {
    ux_component_control_kind kind;
    union {
        char title[UX_COMPONENT_WINDOW_TITLE_CAPACITY];
        lib_bool window_mouse_enabled;
    } value;
} ux_component_control;

/* Each UX leaf owns exactly one of these. It contains two independent
 * mailboxes: a latest-wins copied frame and a FIFO control queue. The native
 * wake object is merely their shared wait primitive, never a third mailbox. */
typedef struct ux_component_mailboxes {
    atomic_flag frame_lock;
    atomic_flag control_lock;
    ux_frame frame;
    lib_u32 frame_generation;
    ux_component_control controls[UX_COMPONENT_CONTROL_CAPACITY];
    lib_u32 control_head;
    lib_u32 control_count;
    ux_mailbox_native *wake;
} ux_component_mailboxes;

lib_status ux_component_mailboxes_create(ux_component_mailboxes *mailboxes);
void ux_component_mailboxes_destroy(ux_component_mailboxes *mailboxes);
lib_status ux_component_mailboxes_publish_frame(ux_component_mailboxes *mailboxes,
    const ux_frame *frame);
lib_status ux_component_mailboxes_enqueue_control(
    ux_component_mailboxes *mailboxes, const ux_component_control *control);
lib_bool ux_component_mailboxes_take_control(ux_component_mailboxes *mailboxes,
    ux_component_control *out_control);
lib_bool ux_component_mailboxes_capture_frame(ux_component_mailboxes *mailboxes,
    lib_u32 *in_out_generation, ux_frame *out_frame);
ux_mailbox_native *ux_component_mailboxes_wake(
    const ux_component_mailboxes *mailboxes);

#endif
