#include "lib/kvm-base/mailbox_interface.h"
#include <assert.h>

static lib_bool reject_mutex;
static lib_status create_mutex(base_sync_mutex **out)
{
    if (reject_mutex) { *out = NULL; return LIB_STATUS_NO_MEMORY; }
    return base_sync_mutex_create(out);
}
/* Count actual wake requests at the platform boundary, including failures. */
static unsigned creates, destroys, signals, notifications;
static lib_status allocation = LIB_STATUS_NO_MEMORY;
static int token;
lib_status kvm_mailbox_wake_create(kvm_mailbox_wake **out)
{
    ++creates;
    *out = allocation == LIB_STATUS_OK ? (kvm_mailbox_wake *)&token : NULL;
    return allocation;
}
void kvm_mailbox_wake_destroy(kvm_mailbox_wake *wake)
{ if (wake != NULL) { assert(wake == (kvm_mailbox_wake *)&token); ++destroys; } }
lib_status kvm_mailbox_wake_signal(kvm_mailbox_wake *wake)
{ assert(wake == (kvm_mailbox_wake *)&token); ++signals; return LIB_STATUS_OK; }
static lib_status notify(void *context)
{ assert(context == &token); ++notifications; return LIB_STATUS_OK; }
#define base_sync_mutex_create create_mutex
#include "lib/kvm-base/mailbox.c"
#undef base_sync_mutex_create

int main(void)
{
    static kvm_component_mailboxes mailbox;
    reject_mutex = LIB_TRUE;
    assert(kvm_component_mailboxes_create(&mailbox) == LIB_STATUS_NO_MEMORY);
    assert(mailbox.frame_lock == NULL && mailbox.wake == NULL && creates == 0);
    kvm_component_mailboxes_destroy(&mailbox);
    reject_mutex = LIB_FALSE;
    assert(kvm_component_mailboxes_create(&mailbox) == LIB_STATUS_OK);
    assert(creates == 0u && mailbox.notify == NULL && mailbox.wake == NULL);
    assert(kvm_component_mailboxes_notify(&mailbox) == LIB_STATUS_INVALID_STATE);
    kvm_component_mailboxes_destroy(&mailbox);
    assert(destroys == 0u);

    /* Window succeeds even when default-wake allocation would fail. */
    assert(kvm_component_mailboxes_create(&mailbox) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_select_notify(&mailbox, notify, &token) == LIB_STATUS_OK);
    assert(creates == 0u && mailbox.wake == NULL);
    assert(kvm_component_mailboxes_select_notify(&mailbox, NULL, NULL) == LIB_STATUS_INVALID_STATE);
    assert(kvm_component_mailboxes_select_notify(&mailbox, notify, NULL) == LIB_STATUS_INVALID_STATE);
    assert(kvm_component_mailboxes_notify(&mailbox) == LIB_STATUS_OK);
    assert(notifications == 1u && creates == 0u && signals == 0u);
    kvm_component_mailboxes_destroy(&mailbox);
    assert(destroys == 0u);

    /* Failed Console selection owns nothing and can be safely disposed. */
    assert(kvm_component_mailboxes_create(&mailbox) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_select_notify(&mailbox, NULL, NULL) == LIB_STATUS_NO_MEMORY);
    assert(creates == 1u && mailbox.notify == NULL && mailbox.wake == NULL);
    kvm_component_mailboxes_destroy(&mailbox);
    assert(destroys == 0u);
    allocation = LIB_STATUS_OK;
    assert(kvm_component_mailboxes_create(&mailbox) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_select_notify(&mailbox, NULL, NULL) == LIB_STATUS_OK);
    assert(creates == 2u && mailbox.wake != NULL);
    assert(kvm_component_mailboxes_select_notify(&mailbox, NULL, NULL) == LIB_STATUS_INVALID_STATE);
    assert(kvm_component_mailboxes_select_notify(&mailbox, notify, &token) == LIB_STATUS_INVALID_STATE);
    assert(kvm_component_mailboxes_notify(&mailbox) == LIB_STATUS_OK);
    assert(signals == 1u && notifications == 1u && creates == 2u);
    kvm_component_mailboxes_destroy(&mailbox);
    assert(destroys == 1u);
    return 0;
}
