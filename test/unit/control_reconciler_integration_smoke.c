#include "control.h"
#include "reconciler.h"
#include "keyboard.h"

#include <assert.h>
#include <string.h>

/* control.c's guest adapter is not exercised by this queue/reconciler proof.
 * These stubs keep the test at the application control boundary. */
int app_keyboard_deliver_input(void *context, const ux_event *event)
{ (void)context; (void)event; return 1; }
int app_keyboard_submit_ctrl_alt_del(void *context, ux_event_sink sink)
{ (void)context; (void)sink; return 1; }
int app_keyboard_submit_alt_enter(void *context, ux_event_sink sink)
{ (void)context; (void)sink; return 1; }

static void take(app_control_queue *queue, app_control_event *event)
{ assert(app_control_queue_take(queue, event, 0u)); }

int main(void)
{
    app_control_queue *queue = NULL;
    app_control_event event;
    app_reconciler reconciler;
    lib_console_line start = { 5u, "start" };
    lib_console_line pause = { 5u, "pause" };
    ux_input_event raw_key = { 0 };
    ux_input_event retired = { 0 };

    assert(app_control_queue_create(&queue));
    raw_key.type = UX_EVENT_KEY;
    raw_key.source_identity = 41u;
    raw_key.data.key.virtual_key = 'A';
    raw_key.data.key.pressed = 1u;

    /* Application events admitted after each completed monitor -> raw ->
       monitor handoff remain one FIFO. Native records left in the host input
       buffer before either activation are deliberately flushed by the broker;
       this test does not claim to preserve those pre-cutover records. */
    assert(app_control_queue_push_monitor_line(queue, &start));
    assert(app_control_queue_push_broker_completed(queue, 1, 7u));
    assert(app_control_queue_push_ux_for_run(queue, &raw_key, 7u));
    assert(app_control_queue_push_broker_completed(queue, 0, 7u));
    assert(app_control_queue_push_monitor_line(queue, &pause));
    take(queue, &event); assert(event.kind == APP_CONTROL_MONITOR_LINE);
    take(queue, &event); assert(event.kind == APP_CONTROL_BROKER_COMPLETED &&
        event.value.broker_vm_console_current);
    take(queue, &event); assert(event.kind == APP_CONTROL_UX_INPUT &&
        app_control_accept_ux_event(&event, 7u, SOFTPC_RUNTIME_RUNNING));
    take(queue, &event); assert(event.kind == APP_CONTROL_BROKER_COMPLETED &&
        !event.value.broker_vm_console_current);
    take(queue, &event); assert(event.kind == APP_CONTROL_MONITOR_LINE);

    /* Window -> Console input belongs to the activated run. A raw event from
       the former Console can never be delivered after Window owns a new run. */
    assert(app_control_queue_push_ux_for_run(queue, &raw_key, 6u));
    take(queue, &event);
    assert(!app_control_accept_ux_event(&event, 7u, SOFTPC_RUNTIME_RUNNING));

    /* Retirement is useful while input is active, but a late retirement after
       stop is rejected before app_control_handle_ux can touch guest input. */
    retired.type = UX_EVENT_SOURCE_RETIRED;
    retired.source_identity = raw_key.source_identity;
    assert(app_control_queue_push_ux_for_run(queue, &retired, 7u));
    take(queue, &event); assert(app_control_accept_ux_event(&event, 7u,
        SOFTPC_RUNTIME_RUNNING));
    assert(!app_control_accept_ux_event(&event, 7u, SOFTPC_RUNTIME_STOPPED));

    /* A frozen Window must not affect a paused VM with a late ordinary key
       make. Cleanup records still enter so a guest cannot retain a key or
       mouse button across the paused boundary. */
    assert(app_control_queue_push_ux_for_run(queue, &raw_key, 7u));
    take(queue, &event);
    assert(!app_control_accept_ux_event(&event, 7u, SOFTPC_RUNTIME_PAUSED));
    raw_key.data.key.pressed = 0u;
    assert(app_control_queue_push_ux_for_run(queue, &raw_key, 7u));
    take(queue, &event); assert(app_control_accept_ux_event(&event, 7u,
        SOFTPC_RUNTIME_PAUSED));

    /* A UX component's capacity failure is a product control fact, not a
       swallowed leaf-local status. The monitor consumer reports this kind. */
    assert(app_control_queue_push_ux_delivery_failed(queue, 41u,
        LIB_STATUS_LIMIT_EXCEEDED, 7u));
    take(queue, &event);
    assert(event.kind == APP_CONTROL_UX_DELIVERY_FAILED);
    assert(event.value.delivery_failure.source_identity == 41u);
    assert(event.value.delivery_failure.status == LIB_STATUS_LIMIT_EXCEEDED);

    /* The reconciler supplies the one-way native handoff plan: running text
       creates and binds raw Console; pause first returns Current Console to
       monitor and only then retires the VM Console component. */
    app_reconciler_initialize(&reconciler, SOFTPC_PRESENTATION_CONSOLE, 1);
    app_reconciler_note_intent(&reconciler, APP_RECONCILER_INTENT_START);
    assert(app_reconciler_take_action(&reconciler) ==
        APP_RECONCILER_ACTION_RUNTIME_START);
    app_reconciler_note_runtime(&reconciler, SOFTPC_RUNTIME_RUNNING);
    app_reconciler_note_frame(&reconciler, 0);
    assert(app_reconciler_take_action(&reconciler) ==
        APP_RECONCILER_ACTION_CREATE_VM_CONSOLE);
    app_reconciler_note_vm_console(&reconciler, 1);
    assert(app_reconciler_take_action(&reconciler) ==
        APP_RECONCILER_ACTION_BIND_VM_CONSOLE);
    app_reconciler_note_current_console(&reconciler,
        APP_RECONCILER_CONSOLE_VM);
    app_reconciler_note_intent(&reconciler, APP_RECONCILER_INTENT_PAUSE);
    assert(app_reconciler_take_action(&reconciler) ==
        APP_RECONCILER_ACTION_RUNTIME_PAUSE);
    app_reconciler_note_runtime(&reconciler, SOFTPC_RUNTIME_PAUSED);
    assert(app_reconciler_take_action(&reconciler) ==
        APP_RECONCILER_ACTION_BIND_MONITOR);
    app_reconciler_note_current_console(&reconciler,
        APP_RECONCILER_CONSOLE_MONITOR);
    assert(app_reconciler_take_action(&reconciler) ==
        APP_RECONCILER_ACTION_DESTROY_VM_CONSOLE);

    app_control_queue_destroy(queue);
    return 0;
}
