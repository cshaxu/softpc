#include "common/session/control.h"

#include <string.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>

#include <stdlib.h>

#define COMMON_SESSION_EVENT_QUEUE_INITIAL_CAPACITY 64u
#define COMMON_SESSION_EVENT_PRESSED_CAPACITY 256u

typedef struct common_session_pressed_key {
    uint64_t source;
    kvm_input_event event;
} common_session_pressed_key;

struct common_session_queue {
    CRITICAL_SECTION lock;
    HANDLE available;
    common_session_event *events;
    unsigned int first;
    unsigned int count;
    unsigned int capacity;
    common_session_pressed_key pressed[COMMON_SESSION_EVENT_PRESSED_CAPACITY];
    unsigned int pressed_count;
    int fatal_delivery_pending;
    uint64_t fatal_delivery_source;
    uint32_t fatal_delivery_generation;
    lib_status fatal_delivery_status;
    int fatal_queue_delivery_pending;
    uint32_t fatal_queue_delivery_generation;
    lib_status fatal_queue_delivery_status;
};

static int common_session_queue_push(common_session_queue *queue,
    const common_session_event *event);

/* Allocation failure must not turn a key transition into a silent drop.  This
 * fixed metadata slot is the queue's final, allocation-free fault record. */
static void common_session_queue_latch_delivery_failure(common_session_queue *queue,
    uint64_t source_identity, lib_status status, uint32_t run_generation)
{
    if (queue == NULL) return;
    EnterCriticalSection(&queue->lock);
    if (!queue->fatal_delivery_pending) {
        queue->fatal_delivery_pending = 1;
        queue->fatal_delivery_source = source_identity;
        queue->fatal_delivery_generation = run_generation;
        queue->fatal_delivery_status = status;
    }
    (void)SetEvent(queue->available);
    LeaveCriticalSection(&queue->lock);
}

/* All control records are facts required by the sole reconciler.  If the
 * dynamically growing FIFO cannot retain one, expose a terminal failure from
 * fixed storage rather than pretending the fact never happened. */
static void common_session_queue_latch_queue_delivery_failure(
    common_session_queue *queue, lib_status status, uint32_t run_generation)
{
    if (queue == NULL) return;
    EnterCriticalSection(&queue->lock);
    if (!queue->fatal_queue_delivery_pending) {
        queue->fatal_queue_delivery_pending = 1;
        queue->fatal_queue_delivery_generation = run_generation;
        queue->fatal_queue_delivery_status = status;
    }
    (void)SetEvent(queue->available);
    LeaveCriticalSection(&queue->lock);
}

static int common_session_queue_push_required(common_session_queue *queue,
    const common_session_event *event)
{
    if (common_session_queue_push(queue, event)) return 1;
    common_session_queue_latch_queue_delivery_failure(queue, LIB_STATUS_NO_MEMORY,
        event == NULL ? 0u : event->run_generation);
    return 0;
}

static int common_session_queue_push(common_session_queue *queue,
    const common_session_event *event)
{
    common_session_event *expanded;
    unsigned int index;
    if (queue == NULL || event == NULL) return 0;
    EnterCriticalSection(&queue->lock);
    if (queue->count == queue->capacity) {
        unsigned int next_capacity = queue->capacity * 2u;
        if (next_capacity <= queue->capacity ||
            next_capacity > UINT_MAX / sizeof(*expanded)) {
            LeaveCriticalSection(&queue->lock);
            return 0;
        }
        expanded = calloc(next_capacity, sizeof(*expanded));
        if (expanded == NULL) {
            LeaveCriticalSection(&queue->lock);
            return 0;
        }
        for (index = 0u; index < queue->count; ++index)
            expanded[index] = queue->events[(queue->first + index) % queue->capacity];
        free(queue->events);
        queue->events = expanded;
        queue->first = 0u;
        queue->capacity = next_capacity;
    }
    queue->events[(queue->first + queue->count) % queue->capacity] =
        *event;
    ++queue->count;
    (void)SetEvent(queue->available);
    LeaveCriticalSection(&queue->lock);
    return 1;
}

int common_session_queue_create(common_session_queue **out_queue)
{
    common_session_queue *queue;
    if (out_queue == NULL) return 0;
    *out_queue = NULL;
    queue = calloc(1u, sizeof(*queue));
    if (queue == NULL) return 0;
    InitializeCriticalSection(&queue->lock);
    queue->capacity = COMMON_SESSION_EVENT_QUEUE_INITIAL_CAPACITY;
    queue->events = calloc(queue->capacity, sizeof(*queue->events));
    queue->available = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (queue->events == NULL || queue->available == NULL) {
        free(queue->events);
        if (queue->available != NULL) CloseHandle(queue->available);
        DeleteCriticalSection(&queue->lock);
        free(queue);
        return 0;
    }
    *out_queue = queue;
    return 1;
}

void common_session_queue_destroy(common_session_queue *queue)
{
    if (queue == NULL) return;
    CloseHandle(queue->available);
    free(queue->events);
    DeleteCriticalSection(&queue->lock);
    free(queue);
}

int common_session_queue_push_ux(common_session_queue *queue,
    const kvm_input_event *event)
{
    return common_session_queue_push_kvm_for_run(queue, event, 0u);
}

int common_session_queue_push_kvm_for_run(common_session_queue *queue,
    const kvm_input_event *event, uint32_t run_generation)
{
    common_session_event copied = { 0 };
    if (event == NULL) return 0;
    copied.kind = COMMON_SESSION_EVENT_KVM_INPUT;
    copied.run_generation = run_generation;
    copied.value.kvm = *event;
    if (common_session_queue_push(queue, &copied)) return 1;
    common_session_queue_latch_delivery_failure(queue, event->source_identity,
        LIB_STATUS_NO_MEMORY, run_generation);
    return 0;
}

int common_session_queue_push_monitor_line(common_session_queue *queue,
    const lib_console_line *line, int rejected)
{
    common_session_event copied = { 0 };
    if (line == NULL) return 0;
    copied.kind = COMMON_SESSION_EVENT_MONITOR_LINE;
    copied.value.line = *line;
    copied.monitor_line_rejected = rejected;
    return common_session_queue_push_required(queue, &copied);
}

int common_session_queue_push_console_failed(common_session_queue *queue)
{
    common_session_event event = { 0 };
    event.kind = COMMON_SESSION_EVENT_CONSOLE_FAILED;
    return common_session_queue_push_required(queue, &event);
}

int common_session_queue_push_runtime_completed(common_session_queue *queue,
    common_session_machine_state state, uint32_t run_generation)
{
    common_session_event event = { COMMON_SESSION_EVENT_RUNTIME_COMPLETED, run_generation };
    event.value.runtime_state = state;
    return common_session_queue_push_required(queue, &event);
}

int common_session_queue_push_frame_completed(common_session_queue *queue,
    uint32_t sequence, int graphics, uint32_t run_generation)
{
    common_session_event event = { COMMON_SESSION_EVENT_FRAME_COMPLETED, run_generation };
    event.value.frame.sequence = sequence;
    event.value.frame.graphics = graphics != 0;
    return common_session_queue_push_required(queue, &event);
}

int common_session_queue_push_component_completed(common_session_queue *queue,
    common_session_component_kind component, int exists, uint32_t run_generation)
{
    common_session_event event = { COMMON_SESSION_EVENT_COMPONENT_COMPLETED, run_generation };
    event.value.component.component = component;
    event.value.component.exists = exists != 0;
    return common_session_queue_push_required(queue, &event);
}

int common_session_queue_push_broker_completed(common_session_queue *queue,
    int vm_console_current, uint32_t run_generation)
{
    common_session_event event = { COMMON_SESSION_EVENT_BROKER_COMPLETED, run_generation };
    event.value.broker_vm_console_current = vm_console_current != 0;
    return common_session_queue_push_required(queue, &event);
}

int common_session_queue_push_kvm_delivery_failed(common_session_queue *queue,
    uint64_t source_identity, lib_status status, uint32_t run_generation)
{
    common_session_event event = { COMMON_SESSION_EVENT_KVM_DELIVERY_FAILED, run_generation };
    event.value.delivery_failure.source_identity = source_identity;
    event.value.delivery_failure.status = status;
    return common_session_queue_push_required(queue, &event);
}

int common_session_queue_take(common_session_queue *queue,
    common_session_event *out_event, unsigned long timeout_ms)
{
    if (queue == NULL || out_event == NULL ||
        WaitForSingleObject(queue->available, timeout_ms) != WAIT_OBJECT_0)
        return 0;
    EnterCriticalSection(&queue->lock);
    if (queue->count == 0u && queue->fatal_delivery_pending) {
        memset(out_event, 0, sizeof(*out_event));
        out_event->kind = COMMON_SESSION_EVENT_KVM_DELIVERY_FAILED;
        out_event->run_generation = queue->fatal_delivery_generation;
        out_event->value.delivery_failure.source_identity = queue->fatal_delivery_source;
        out_event->value.delivery_failure.status = queue->fatal_delivery_status;
        queue->fatal_delivery_pending = 0;
        if (!queue->fatal_queue_delivery_pending)
            (void)ResetEvent(queue->available);
        LeaveCriticalSection(&queue->lock);
        return 1;
    }
    if (queue->count == 0u && queue->fatal_queue_delivery_pending) {
        memset(out_event, 0, sizeof(*out_event));
        out_event->kind = COMMON_SESSION_EVENT_QUEUE_DELIVERY_FAILED;
        out_event->run_generation = queue->fatal_queue_delivery_generation;
        out_event->value.queue_delivery_status = queue->fatal_queue_delivery_status;
        queue->fatal_queue_delivery_pending = 0;
        (void)ResetEvent(queue->available);
        LeaveCriticalSection(&queue->lock);
        return 1;
    }
    if (queue->count == 0u) {
        (void)ResetEvent(queue->available);
        LeaveCriticalSection(&queue->lock);
        return 0;
    }
    *out_event = queue->events[queue->first];
    queue->first = (queue->first + 1u) % queue->capacity;
    --queue->count;
    if (queue->count == 0u && !queue->fatal_delivery_pending &&
        !queue->fatal_queue_delivery_pending)
        (void)ResetEvent(queue->available);
    LeaveCriticalSection(&queue->lock);
    return 1;
}

int common_session_accept_kvm_event(const common_session_event *event,
    uint32_t current_run_generation, common_session_machine_state runtime_state)
{
    const kvm_input_event *input;

    if (event == NULL || event->kind != COMMON_SESSION_EVENT_KVM_INPUT) return 0;
    input = &event->value.kvm;
    /* Source identity is globally monotonic.  Retirement is not guest input:
       it must always reach the ledger, even when the component belonged to a
       retired run, so a later allocation cannot inherit its held keys. */
    if (input->type == KVM_EVENT_SOURCE_RETIRED) return 1;
    if (event->run_generation != 0u && event->run_generation !=
        current_run_generation) return 0;
    if (runtime_state == COMMON_SESSION_MACHINE_RUNNING) return 1;
    if (runtime_state != COMMON_SESSION_MACHINE_PAUSED) return 0;
    return input->type == KVM_EVENT_WINDOW_CLOSE ||
        input->type == KVM_EVENT_SOURCE_RETIRED ||
        input->type == KVM_EVENT_HOTKEY ||
        (input->type == KVM_EVENT_KEY && input->data.key.pressed == 0u) ||
        (input->type == KVM_EVENT_MOUSE && input->data.mouse.buttons == 0u);
}

static void common_session_forget_pressed(common_session_queue *queue,
    const kvm_input_event *event)
{
    unsigned int index;
    for (index = 0u; index < queue->pressed_count; ++index) {
        common_session_pressed_key *pressed = &queue->pressed[index];
        if (pressed->source == event->source_identity &&
            pressed->event.data.key.scan_code == event->data.key.scan_code &&
            pressed->event.data.key.key == event->data.key.key) {
            pressed[0] = queue->pressed[--queue->pressed_count];
            return;
        }
    }
}

static void common_session_remember_pressed(common_session_queue *queue,
    const kvm_input_event *event)
{
    unsigned int index;
    common_session_forget_pressed(queue, event);
    if (queue->pressed_count == COMMON_SESSION_EVENT_PRESSED_CAPACITY) return;
    index = queue->pressed_count++;
    queue->pressed[index].source = event->source_identity;
    queue->pressed[index].event = *event;
}

static int common_session_release_source(common_session_queue *queue,
    uint64_t source, common_session_machine_state runtime_state,
    common_session_input_sink sink, void *sink_context)
{
    unsigned int index = 0u;
    while (index < queue->pressed_count) {
        common_session_pressed_key *pressed = &queue->pressed[index];
        if (pressed->source != source) {
            ++index;
            continue;
        }
        pressed->event.data.key.pressed = 0u;
        /* Source retirement is ledger cleanup.  It can reach the control
           queue after pause, but must not turn into a late guest release in
           the paused or stopped VM. */
        if (runtime_state == COMMON_SESSION_MACHINE_RUNNING &&
            !sink(sink_context, &pressed->event)) return 0;
        queue->pressed[index] = queue->pressed[--queue->pressed_count];
    }
    return 1;
}

int common_session_dispatch_input(common_session_queue *queue, const kvm_input_event *event,
    common_session_machine_state runtime_state, common_session_input_sink sink,
    void *sink_context)
{
    if (queue == NULL || event == NULL || sink == NULL) return 0;
    if (event->type == KVM_EVENT_KEY) {
        if (event->data.key.pressed != 0u) common_session_remember_pressed(queue, event);
        else common_session_forget_pressed(queue, event);
        return runtime_state != COMMON_SESSION_MACHINE_RUNNING ||
            sink(sink_context, event);
    }
    if (event->type == KVM_EVENT_MOUSE)
        return runtime_state != COMMON_SESSION_MACHINE_RUNNING ||
            sink(sink_context, event);
    if (event->type == KVM_EVENT_SOURCE_RETIRED)
        return common_session_release_source(queue, event->source_identity,
            runtime_state, sink, sink_context);
    if (event->type == KVM_EVENT_WINDOW_CLOSE)
        return 1;
    if (event->type != KVM_EVENT_HOTKEY) return 1;
    /* Product adapters consume registered hotkeys before this generic input
       ledger.  No identifier or guest protocol belongs in common/session. */
    return 1;
}
#endif
