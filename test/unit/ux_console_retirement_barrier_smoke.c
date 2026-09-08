#include "lib/base/internal/console.h"
#include "lib/ux-console/console.h"

#include <assert.h>

#ifdef _WIN32
#include <windows.h>

typedef struct retirement_probe {
    HANDLE input_entered;
    HANDLE release_input;
    HANDLE input_done;
    HANDLE retired;
    HANDLE destroyed;
    ux_input_event events[2];
    LONG event_count;
} retirement_probe;

typedef struct delivery_context {
    lib_console *console;
    HANDLE done;
} delivery_context;

typedef struct destroy_context {
    ux_console *console;
    HANDLE done;
} destroy_context;

static int retirement_input(void *opaque, const ux_input_event *event)
{
    retirement_probe *probe = (retirement_probe *)opaque;
    LONG index;
    assert(probe != NULL && event != NULL);
    index = InterlockedIncrement(&probe->event_count) - 1;
    assert(index >= 0 && index < 2);
    probe->events[index] = *event;
    if (event->type == UX_EVENT_KEY) {
        SetEvent(probe->input_entered);
        assert(WaitForSingleObject(probe->release_input, INFINITE) == WAIT_OBJECT_0);
        SetEvent(probe->input_done);
    } else {
        assert(event->type == UX_EVENT_SOURCE_RETIRED);
        SetEvent(probe->retired);
    }
    return 1;
}

static void retirement_failure(void *opaque, lib_u64 source_identity,
    lib_status status)
{
    (void)opaque; (void)source_identity; (void)status;
    assert(!"unexpected UX delivery failure");
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
    ux_console_destroy(context->console);
    SetEvent(context->done);
    return 0u;
}

int main(void)
{
    retirement_probe probe = { 0 };
    ux_console_options options = { 0 };
    ux_console *console = NULL;
    delivery_context delivery = { 0 };
    destroy_context destroy = { 0 };
    HANDLE delivery_thread;
    HANDLE destroy_thread;

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
    assert(ux_console_create(&console, &options) == LIB_STATUS_OK);
    delivery.console = ux_console_get_console(console);
    assert(lib_console_bind_generation(delivery.console, 1u) == LIB_STATUS_OK);
    delivery.done = CreateEventA(NULL, TRUE, FALSE, NULL);
    destroy.console = console;
    destroy.done = probe.destroyed;
    assert(delivery.done != NULL);
    delivery_thread = CreateThread(NULL, 0u, retirement_deliver, &delivery, 0u,
        NULL);
    assert(delivery_thread != NULL);
    assert(WaitForSingleObject(probe.input_entered, INFINITE) == WAIT_OBJECT_0);
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
    assert(probe.events[0].type == UX_EVENT_KEY);
    assert(probe.events[1].type == UX_EVENT_SOURCE_RETIRED);
    CloseHandle(delivery_thread);
    CloseHandle(destroy_thread);
    CloseHandle(delivery.done);
    CloseHandle(probe.input_entered);
    CloseHandle(probe.release_input);
    CloseHandle(probe.input_done);
    CloseHandle(probe.retired);
    CloseHandle(probe.destroyed);
    return 0;
}
#else
int main(void) { return 0; }
#endif
