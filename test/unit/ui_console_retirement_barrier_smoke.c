#include "lib/console/binding_interface.h"
#include "lib/ui-console/console_interface.h"

#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#include "lib/ui-console/console.h"

static LONG fail_wake;
static ui_mailbox_wake_wait_result retirement_wait(
    const ui_mailbox_wake *wake, lib_u32 timeout)
{
    ui_mailbox_wake_wait_result result = ui_mailbox_wake_wait(wake, timeout);
    return InterlockedCompareExchange(&fail_wake, 0, 0) ?
        UI_MAILBOX_WAKE_WAIT_FAULT : result;
}
/* Compile the production worker; only its wait result is controllable. */
#define ui_mailbox_wake_wait retirement_wait
#include "lib/ui-console/win32/component.c"
#undef ui_mailbox_wake_wait

typedef struct retirement_probe {
    HANDLE input_entered;
    HANDLE release_input;
    HANDLE input_done;
    HANDLE retired;
    HANDLE destroyed;
    ui_input_event events[2];
    LONG event_count;
    LONG failures;
} retirement_probe;

typedef struct delivery_context {
    lib_console *console;
    HANDLE done;
} delivery_context;

typedef struct destroy_context {
    ui_console *console;
    HANDLE done;
} destroy_context;

static int retirement_input(void *opaque, const ui_input_event *event)
{
    retirement_probe *probe = (retirement_probe *)opaque;
    LONG index;
    assert(probe != NULL && event != NULL);
    index = InterlockedIncrement(&probe->event_count) - 1;
    assert(index >= 0 && index < 2);
    probe->events[index] = *event;
    if (event->type == UI_EVENT_KEY) {
        SetEvent(probe->input_entered);
        assert(WaitForSingleObject(probe->release_input, INFINITE) == WAIT_OBJECT_0);
        SetEvent(probe->input_done);
    } else {
        assert(event->type == UI_EVENT_SOURCE_RETIRED);
        SetEvent(probe->retired);
    }
    return 1;
}

static void retirement_failure(void *opaque, lib_u64 source_identity,
    lib_status status)
{
    retirement_probe *probe = opaque;
    assert(fail_wake && source_identity != 0u && status == LIB_STATUS_IO_ERROR);
    InterlockedIncrement(&probe->failures);
}

static DWORD WINAPI retirement_deliver(void *opaque)
{
    delivery_context *context = (delivery_context *)opaque;
    lib_console_event event = { 0 };
    event.kind = LIB_CONSOLE_EVENT_RAW_KEY;
    event.binding_generation = 1u;
    event.value.raw_key.key = 'A';
    event.value.raw_key.pressed = LIB_TRUE;
    assert(lib_console_deliver_event(context->console, &event) == LIB_STATUS_OK);
    SetEvent(context->done);
    return 0u;
}

static DWORD WINAPI retirement_destroy(void *opaque)
{
    destroy_context *context = (destroy_context *)opaque;
    ui_console_destroy(context->console);
    SetEvent(context->done);
    return 0u;
}

static void check_retirement(int fault)
{
    retirement_probe probe = { 0 };
    ui_console_options options = { 0 };
    ui_console *console = NULL;
    delivery_context delivery = { 0 };
    destroy_context destroy = { 0 };
    HANDLE delivery_thread;
    HANDLE destroy_thread;
    lib_console_event late_event = { 0 };

    probe.input_entered = CreateEventA(NULL, TRUE, FALSE, NULL);
    probe.release_input = CreateEventA(NULL, TRUE, FALSE, NULL);
    probe.input_done = CreateEventA(NULL, TRUE, FALSE, NULL);
    probe.retired = CreateEventA(NULL, TRUE, FALSE, NULL);
    probe.destroyed = CreateEventA(NULL, TRUE, FALSE, NULL);
    assert(probe.input_entered && probe.release_input && probe.input_done &&
        probe.retired && probe.destroyed);
    options.input_context = &probe;
    options.input_sink = retirement_input;
    options.failure_context = &probe;
    options.failure_sink = retirement_failure;
    assert(ui_console_create(&console, &options) == LIB_STATUS_OK);
    delivery.console = lib_console_retain(ui_console_get_console(console));
    assert(lib_console_bind_generation(delivery.console, 1u) == LIB_STATUS_OK);
    delivery.done = CreateEventA(NULL, TRUE, FALSE, NULL);
    destroy.console = console;
    destroy.done = probe.destroyed;
    assert(delivery.done != NULL);
    delivery_thread = CreateThread(NULL, 0u, retirement_deliver, &delivery, 0u,
        NULL);
    assert(delivery_thread != NULL);
    assert(WaitForSingleObject(probe.input_entered, INFINITE) == WAIT_OBJECT_0);
    if (fault) {
        InterlockedExchange(&fail_wake, 1);
        ui_mailbox_wake_signal(ui_component_mailboxes_wake(&console->base.mailboxes));
    }
    destroy_thread = CreateThread(NULL, 0u, retirement_destroy, &destroy, 0u,
        NULL);
    assert(destroy_thread != NULL);
    /* The component cannot retire or free itself while a copied key callback
       is still active. No timer is involved in this proof. */
    assert(WaitForSingleObject(probe.destroyed, 0u) == WAIT_TIMEOUT);
    SetEvent(probe.release_input);
    assert(WaitForSingleObject(delivery.done, INFINITE) == WAIT_OBJECT_0);
    assert(WaitForSingleObject(probe.retired, INFINITE) == WAIT_OBJECT_0);
    assert(WaitForSingleObject(probe.destroyed, INFINITE) == WAIT_OBJECT_0);
    assert(probe.event_count == 2);
    assert(probe.events[0].type == UI_EVENT_KEY);
    assert(probe.events[1].type == UI_EVENT_SOURCE_RETIRED);
    assert(probe.failures == fault);
    assert(WaitForSingleObject(delivery_thread, INFINITE) == WAIT_OBJECT_0);
    assert(WaitForSingleObject(destroy_thread, INFINITE) == WAIT_OBJECT_0);
    late_event.kind = LIB_CONSOLE_EVENT_RAW_KEY;
    late_event.binding_generation = 1u;
    late_event.value.raw_key.key = 'B';
    late_event.value.raw_key.pressed = LIB_TRUE;
    assert(lib_console_deliver_event(delivery.console, &late_event) == LIB_STATUS_INVALID_STATE);
    assert(probe.event_count == 2 && probe.failures == fault);
    lib_console_release(delivery.console);
    CloseHandle(delivery_thread);
    CloseHandle(destroy_thread);
    CloseHandle(delivery.done);
    CloseHandle(probe.input_entered);
    CloseHandle(probe.release_input);
    CloseHandle(probe.input_done);
    CloseHandle(probe.retired);
    CloseHandle(probe.destroyed);
}

int main(void)
{
    check_retirement(0);
    check_retirement(1);
    return 0;
}
#else
int main(void) { return 0; }
#endif
