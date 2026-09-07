#include "lib/ux-base/internal.h"

static void ux_frame_mailbox_lock(atomic_flag *lock)
{
    while (atomic_flag_test_and_set_explicit(lock, memory_order_acquire)) {}
}

static void ux_frame_mailbox_unlock(atomic_flag *lock)
{
    atomic_flag_clear_explicit(lock, memory_order_release);
}

void ux_frame_mailbox_initialize(ux_frame_mailbox *mailbox)
{
    if (mailbox == LIB_NULL) return;
    memset(mailbox, 0, sizeof(*mailbox));
    mailbox->lock = (atomic_flag)ATOMIC_FLAG_INIT;
    atomic_flag_clear_explicit(&mailbox->lock, memory_order_release);
}

lib_status ux_frame_mailbox_publish(ux_frame_mailbox *mailbox,
    const ux_frame *frame)
{
    if (mailbox == LIB_NULL || !ux_frame_is_valid(frame))
        return LIB_STATUS_INVALID_ARGUMENT;
    ux_frame_mailbox_lock(&mailbox->lock);
    mailbox->frame = *frame;
    mailbox->frame.sequence = ++mailbox->generation;
    ux_frame_mailbox_unlock(&mailbox->lock);
    return LIB_STATUS_OK;
}

lib_status ux_frame_mailbox_take(const ux_frame_mailbox *mailbox,
    ux_frame *out_frame, lib_u32 *out_generation)
{
    ux_frame_mailbox *mutable_mailbox = (ux_frame_mailbox *)mailbox;

    if (mailbox == LIB_NULL || out_frame == LIB_NULL || out_generation == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    ux_frame_mailbox_lock(&mutable_mailbox->lock);
    *out_frame = mailbox->frame;
    *out_generation = mailbox->generation;
    ux_frame_mailbox_unlock(&mutable_mailbox->lock);
    return LIB_STATUS_OK;
}
