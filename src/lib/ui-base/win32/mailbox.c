#include "lib/ui-base/mailbox_wake.h"

#include "lib/types/win32.h"

struct ui_mailbox_wake { HANDLE handle; };

ui_mailbox_wake *ui_mailbox_wake_create(void)
{
    ui_mailbox_wake *wake = lib_allocate_zero(1u, sizeof(*wake));
    if (wake == LIB_NULL) return LIB_NULL;
    wake->handle = CreateEventA(LIB_NULL, FALSE, FALSE, LIB_NULL);
    if (wake->handle == NULL) { lib_release(wake); return LIB_NULL; }
    return wake;
}
void ui_mailbox_wake_destroy(ui_mailbox_wake *wake)
{ if (wake != LIB_NULL) { if (wake->handle != NULL) (void)CloseHandle(wake->handle); lib_release(wake); } }
void ui_mailbox_wake_signal(ui_mailbox_wake *wake)
{ if (wake != LIB_NULL && wake->handle != NULL) (void)SetEvent(wake->handle); }

ui_mailbox_wake_wait_result ui_mailbox_wake_wait(
    const ui_mailbox_wake *wake, lib_u32 timeout_milliseconds)
{
    DWORD result;
    if (wake == LIB_NULL || wake->handle == NULL) return UI_MAILBOX_WAKE_WAIT_FAULT;
    result = WaitForSingleObject(wake->handle, (DWORD)timeout_milliseconds);
    return result == WAIT_OBJECT_0 ? UI_MAILBOX_WAKE_WAIT_WAKE :
        result == WAIT_TIMEOUT ? UI_MAILBOX_WAKE_WAIT_TIMED_OUT : UI_MAILBOX_WAKE_WAIT_FAULT;
}

ui_mailbox_wake_wait_result ui_mailbox_wake_wait_messages(
    const ui_mailbox_wake *wake, lib_u32 timeout_milliseconds)
{
    HANDLE handle;
    DWORD result;
    if (wake == LIB_NULL || wake->handle == NULL) return UI_MAILBOX_WAKE_WAIT_FAULT;
    handle = wake->handle;
    result = MsgWaitForMultipleObjects(1u, &handle, FALSE,
        (DWORD)timeout_milliseconds, QS_ALLINPUT);
    return result == WAIT_OBJECT_0 ? UI_MAILBOX_WAKE_WAIT_WAKE :
        result == WAIT_OBJECT_0 + 1u ? UI_MAILBOX_WAKE_WAIT_MESSAGE :
        result == WAIT_TIMEOUT ? UI_MAILBOX_WAKE_WAIT_TIMED_OUT : UI_MAILBOX_WAKE_WAIT_FAULT;
}
