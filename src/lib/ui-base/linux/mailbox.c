#define _POSIX_C_SOURCE 200809L

#include "lib/ui-base/mailbox_wake.h"

#include "lib/types/linux/sync.h"
#include "lib/types/linux/clock.h"

struct ui_mailbox_wake {
    pthread_mutex_t lock;
    pthread_cond_t changed;
    lib_bool signaled;
};

ui_mailbox_wake *ui_mailbox_wake_create(void)
{
    ui_mailbox_wake *wake = lib_allocate_zero(1u, sizeof(*wake));
    if (wake == LIB_NULL) return LIB_NULL;
    if (pthread_mutex_init(&wake->lock, LIB_NULL) != 0 ||
        pthread_cond_init(&wake->changed, LIB_NULL) != 0) {
        (void)pthread_mutex_destroy(&wake->lock); lib_release(wake); return LIB_NULL;
    }
    return wake;
}
void ui_mailbox_wake_destroy(ui_mailbox_wake *wake)
{ if (wake != LIB_NULL) { (void)pthread_cond_destroy(&wake->changed); (void)pthread_mutex_destroy(&wake->lock); lib_release(wake); } }
void ui_mailbox_wake_signal(ui_mailbox_wake *wake)
{ if (wake != LIB_NULL) { (void)pthread_mutex_lock(&wake->lock); wake->signaled = LIB_TRUE; (void)pthread_cond_signal(&wake->changed); (void)pthread_mutex_unlock(&wake->lock); } }
ui_mailbox_wake_wait_result ui_mailbox_wake_wait(
    const ui_mailbox_wake *wake, lib_u32 timeout_milliseconds)
{
    struct timespec delay = { (time_t)(timeout_milliseconds / 1000u),
        (long)(timeout_milliseconds % 1000u) * 1000000L };
    ui_mailbox_wake *value = (ui_mailbox_wake *)wake;
    if (value == LIB_NULL) return UI_MAILBOX_WAKE_WAIT_FAULT;
    (void)pthread_mutex_lock(&value->lock);
    if (value->signaled != LIB_FALSE) { value->signaled = LIB_FALSE; (void)pthread_mutex_unlock(&value->lock); return UI_MAILBOX_WAKE_WAIT_WAKE; }
    (void)pthread_mutex_unlock(&value->lock);
    if (timeout_milliseconds != 0u) (void)nanosleep(&delay, LIB_NULL);
    return UI_MAILBOX_WAKE_WAIT_TIMED_OUT;
}
ui_mailbox_wake_wait_result ui_mailbox_wake_wait_messages(
    const ui_mailbox_wake *wake, lib_u32 timeout_milliseconds)
{ return ui_mailbox_wake_wait(wake, timeout_milliseconds); }
