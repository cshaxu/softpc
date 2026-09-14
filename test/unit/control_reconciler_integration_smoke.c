#include "control.h"
#include "keyboard.h"

#include <assert.h>
#include <string.h>

/* These stubs keep the test at the application control boundary while making
 * the paused guest-injection barrier observable. */
static unsigned int delivered_guest_input;
static unsigned int delivered_cad;
static unsigned int delivered_caf;

int app_keyboard_deliver_input(void *context, const kvm_input_event *event)
{ (void)context; (void)event; ++delivered_guest_input; return 1; }
int app_keyboard_submit_ctrl_alt_del(void *context, kvm_input_sink sink)
{ (void)context; (void)sink; ++delivered_cad; return 1; }
int app_keyboard_submit_alt_enter(void *context, kvm_input_sink sink)
{ (void)context; (void)sink; ++delivered_caf; return 1; }

static void take(app_control_queue *queue, app_control_event *event)
{ assert(app_control_queue_take(queue, event, 0u)); }

int main(void)
{
    app_control_queue *queue = NULL;
    app_control_event event;
    lib_console_line start = { 5u, "start" };
    lib_console_line pause = { 5u, "pause" };
    kvm_input_event raw_key = { 0 };
    kvm_input_event raw_mouse = { 0 };
    kvm_input_event retired = { 0 };
    kvm_input_event hotkey = { 0 };

    assert(app_control_queue_create(&queue));
    raw_key.type = KVM_EVENT_KEY;
    raw_key.source_identity = 41u;
    raw_key.data.key.key = 'A';
    raw_key.data.key.pressed = 1u;

    /* Application events admitted after each completed monitor -> raw ->
       monitor handoff remain one FIFO. Native records left in the host input
       buffer before either activation are deliberately flushed by the broker;
       this test does not claim to preserve those pre-cutover records. */
    assert(app_control_queue_push_monitor_line(queue, &start, 0));
    assert(app_control_queue_push_broker_completed(queue, 1, 7u));
    assert(app_control_queue_push_kvm_for_run(queue, &raw_key, 7u));
    assert(app_control_queue_push_broker_completed(queue, 0, 7u));
    assert(app_control_queue_push_monitor_line(queue, &pause, 0));
    take(queue, &event); assert(event.kind == APP_CONTROL_MONITOR_LINE);
    take(queue, &event); assert(event.kind == APP_CONTROL_BROKER_COMPLETED &&
        event.value.broker_vm_console_current);
    take(queue, &event); assert(event.kind == APP_CONTROL_KVM_INPUT &&
        app_control_accept_kvm_event(&event, 7u, SOFTPC_RUNTIME_RUNNING));
    take(queue, &event); assert(event.kind == APP_CONTROL_BROKER_COMPLETED &&
        !event.value.broker_vm_console_current);
    take(queue, &event); assert(event.kind == APP_CONTROL_MONITOR_LINE);

    /* Window -> Console input belongs to the activated run. A raw event from
       the former Console can never be delivered after Window owns a new run. */
    assert(app_control_queue_push_kvm_for_run(queue, &raw_key, 6u));
    take(queue, &event);
    assert(!app_control_accept_kvm_event(&event, 7u, SOFTPC_RUNTIME_RUNNING));

    /* Retirement is ledger cleanup, never guest input.  It remains admitted
       after a run changes so the unique source cannot leave held keys behind;
       app_control_handle_ux still emits no guest release while stopped. */
    retired.type = KVM_EVENT_SOURCE_RETIRED;
    retired.source_identity = raw_key.source_identity;
    assert(app_control_queue_push_kvm_for_run(queue, &retired, 7u));
    take(queue, &event); assert(app_control_accept_kvm_event(&event, 7u,
        SOFTPC_RUNTIME_RUNNING));
    assert(app_control_accept_kvm_event(&event, 8u, SOFTPC_RUNTIME_STOPPED));
    assert(app_control_handle_ux(queue, (app_runtime *)1, &event.value.kvm,
        SOFTPC_RUNTIME_STOPPED));

    /* A frozen Window must not affect a paused VM with any late ordinary
       input. Releases remain admitted only for the app's pressed-key ledger;
       they must not cross the separate VM ingress boundary. */
    assert(app_control_queue_push_kvm_for_run(queue, &raw_key, 7u));
    take(queue, &event);
    assert(!app_control_accept_kvm_event(&event, 7u, SOFTPC_RUNTIME_PAUSED));
    raw_key.data.key.pressed = 0u;
    assert(app_control_queue_push_kvm_for_run(queue, &raw_key, 7u));
    take(queue, &event); assert(app_control_accept_kvm_event(&event, 7u,
        SOFTPC_RUNTIME_PAUSED));
    assert(app_control_handle_ux(queue, (app_runtime *)1, &event.value.kvm,
        SOFTPC_RUNTIME_PAUSED));
    raw_mouse.type = KVM_EVENT_MOUSE;
    raw_mouse.data.mouse.buttons = 0u;
    assert(app_control_handle_ux(queue, (app_runtime *)1, &raw_mouse,
        SOFTPC_RUNTIME_PAUSED));
    assert(delivered_guest_input == 0u);

    /* Paused still admits a current-run registered hotkey for product
       handling. A stale run remains rejected. Guest-producing hotkeys are
       consumed at the control/guest boundary while paused. */
    hotkey.type = KVM_EVENT_HOTKEY;
    strcpy(hotkey.data.hotkey.identifier, "pause-toggle");
    assert(app_control_queue_push_kvm_for_run(queue, &hotkey, 7u));
    take(queue, &event);
    assert(app_control_accept_kvm_event(&event, 7u, SOFTPC_RUNTIME_PAUSED));
    assert(app_control_queue_push_kvm_for_run(queue, &hotkey, 6u));
    take(queue, &event);
    assert(!app_control_accept_kvm_event(&event, 7u, SOFTPC_RUNTIME_PAUSED));
    strcpy(hotkey.data.hotkey.identifier, "send-ctrl-alt-del");
    assert(app_control_handle_ux(queue, (app_runtime *)1, &hotkey,
        SOFTPC_RUNTIME_PAUSED));
    strcpy(hotkey.data.hotkey.identifier, "send-alt-enter");
    assert(app_control_handle_ux(queue, (app_runtime *)1, &hotkey,
        SOFTPC_RUNTIME_PAUSED));
    assert(delivered_guest_input == 0u && delivered_cad == 0u &&
        delivered_caf == 0u);

    /* A KVM component's capacity failure is a product control fact, not a
       swallowed leaf-local status. The monitor consumer reports this kind. */
    assert(app_control_queue_push_kvm_delivery_failed(queue, 41u,
        LIB_STATUS_LIMIT_EXCEEDED, 7u));
    take(queue, &event);
    assert(event.kind == APP_CONTROL_KVM_DELIVERY_FAILED);
    assert(event.value.delivery_failure.source_identity == 41u);
    assert(event.value.delivery_failure.status == LIB_STATUS_LIMIT_EXCEEDED);

    /* UI owns native monitor delivery now; the app queue still records its
       copied terminal failure as one control fact. */
    assert(app_control_queue_push_console_failed(queue));
    take(queue, &event);
    assert(event.kind == APP_CONTROL_CONSOLE_FAILED);
    assert(!app_control_queue_take(queue, &event, 0u));
    app_control_queue_destroy(queue);
    return 0;
}
