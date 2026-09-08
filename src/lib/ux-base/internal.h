#ifndef UX_BASE_INTERNAL_H
#define UX_BASE_INTERNAL_H

#include "lib/ux-base/frame.h"

typedef struct ux_frame_mailbox {
    atomic_flag lock;
    lib_u32 generation;
    ux_frame frame;
} ux_frame_mailbox;

void ux_frame_mailbox_initialize(ux_frame_mailbox *mailbox);
lib_status ux_frame_mailbox_publish(ux_frame_mailbox *mailbox,
    const ux_frame *frame);
lib_status ux_frame_mailbox_take(const ux_frame_mailbox *mailbox,
    ux_frame *out_frame, lib_u32 *out_generation);

#endif
