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

void ux_control_mailbox_initialize(ux_control_mailbox *mailbox)
{
    if (mailbox == LIB_NULL) return;
    memset(mailbox, 0, sizeof(*mailbox));
    mailbox->lock = (atomic_flag)ATOMIC_FLAG_INIT;
    atomic_flag_clear_explicit(&mailbox->lock, memory_order_release);
}

lib_status ux_control_mailbox_push(ux_control_mailbox *mailbox,
    const ux_control_message *message)
{
    lib_u32 index;
    if (mailbox == LIB_NULL || message == LIB_NULL ||
        message->kind > UX_CONTROL_RELEASE_MOUSE) return LIB_STATUS_INVALID_ARGUMENT;
    ux_frame_mailbox_lock(&mailbox->lock);
    /* Reserve the final slot for a terminal stop, so destruction can always
     * preserve FIFO termination after ordinary controls have filled the queue. */
    if (mailbox->count >= UX_CONTROL_MAILBOX_CAPACITY ||
        (message->kind != UX_CONTROL_STOP &&
         mailbox->count + 1u >= UX_CONTROL_MAILBOX_CAPACITY)) {
        ux_frame_mailbox_unlock(&mailbox->lock);
        return LIB_STATUS_LIMIT_EXCEEDED;
    }
    index = (mailbox->head + mailbox->count) % UX_CONTROL_MAILBOX_CAPACITY;
    mailbox->entries[index] = *message;
    ++mailbox->count;
    ux_frame_mailbox_unlock(&mailbox->lock);
    return LIB_STATUS_OK;
}

lib_status ux_control_mailbox_take(ux_control_mailbox *mailbox,
    ux_control_message *out_message, lib_bool *out_has_message)
{
    if (mailbox == LIB_NULL || out_message == LIB_NULL ||
        out_has_message == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    ux_frame_mailbox_lock(&mailbox->lock);
    if (mailbox->count == 0u) {
        *out_has_message = LIB_FALSE;
    } else {
        *out_message = mailbox->entries[mailbox->head];
        mailbox->head = (mailbox->head + 1u) % UX_CONTROL_MAILBOX_CAPACITY;
        --mailbox->count;
        *out_has_message = LIB_TRUE;
    }
    ux_frame_mailbox_unlock(&mailbox->lock);
    return LIB_STATUS_OK;
}
