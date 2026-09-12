#include "lib/ui-base/mailbox_wake.h"
#include "lib/types/native_sync.h"

#ifdef _WIN32
struct ui_mailbox_wake {
    lib_native_event *native;
};

ui_mailbox_wake *ui_mailbox_wake_create(void)
{
    ui_mailbox_wake *wake = (ui_mailbox_wake *)lib_allocate_zero(1u,
        sizeof(*wake));

    if (wake == LIB_NULL || lib_native_event_create(LIB_FALSE,
            &wake->native) != LIB_STATUS_OK) {
        lib_release(wake);
        return LIB_NULL;
    }
    return wake;
}

void ui_mailbox_wake_destroy(ui_mailbox_wake *wake)
{
    if (wake == LIB_NULL) return;
    lib_native_event_destroy(wake->native);
    lib_release(wake);
}

void ui_mailbox_wake_signal(ui_mailbox_wake *wake)
{
    if (wake != LIB_NULL) lib_native_event_signal(wake->native);
}

ui_mailbox_wake_wait_result ui_mailbox_wake_wait(
    const ui_mailbox_wake *wake, lib_u32 timeout_milliseconds)
{
    lib_bool signaled = LIB_FALSE;

    return wake == LIB_NULL || lib_native_event_wait(wake->native,
            timeout_milliseconds, &signaled) != LIB_STATUS_OK ?
        UI_MAILBOX_WAKE_WAIT_FAULT : signaled != LIB_FALSE ?
        UI_MAILBOX_WAKE_WAIT_WAKE : UI_MAILBOX_WAKE_WAIT_TIMED_OUT;
}

ui_mailbox_wake_wait_result ui_mailbox_wake_wait_messages(
    const ui_mailbox_wake *wake, lib_u32 timeout_milliseconds)
{
    lib_bool wake_signaled = LIB_FALSE;
    lib_bool message_available = LIB_FALSE;

    if (wake == LIB_NULL) return UI_MAILBOX_WAKE_WAIT_FAULT;
    if (lib_native_event_wait_messages(wake->native, timeout_milliseconds,
            &wake_signaled, &message_available) != LIB_STATUS_OK)
        return UI_MAILBOX_WAKE_WAIT_FAULT;
    return wake_signaled != LIB_FALSE ? UI_MAILBOX_WAKE_WAIT_WAKE :
        message_available != LIB_FALSE ? UI_MAILBOX_WAKE_WAIT_MESSAGE :
        UI_MAILBOX_WAKE_WAIT_TIMED_OUT;
}
#endif
