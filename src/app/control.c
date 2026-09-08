#include "control.h"
#include "keyboard.h"

#include <string.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>

#include <stdlib.h>

#define APP_CONTROL_QUEUE_INITIAL_CAPACITY 64u
#define APP_CONTROL_PRESSED_CAPACITY 256u

typedef struct app_control_pressed_key {
    uint64_t source;
    ux_input_event event;
} app_control_pressed_key;

struct app_control_queue {
    CRITICAL_SECTION lock;
    HANDLE available;
    app_control_event *events;
    unsigned int first;
    unsigned int count;
    unsigned int capacity;
    app_control_pressed_key pressed[APP_CONTROL_PRESSED_CAPACITY];
    unsigned int pressed_count;
};

static int app_control_queue_push(app_control_queue *queue,
    const app_control_event *event)
{
    app_control_event *expanded;
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

int app_control_queue_create(app_control_queue **out_queue)
{
    app_control_queue *queue;
    if (out_queue == NULL) return 0;
    *out_queue = NULL;
    queue = calloc(1u, sizeof(*queue));
    if (queue == NULL) return 0;
    InitializeCriticalSection(&queue->lock);
    queue->capacity = APP_CONTROL_QUEUE_INITIAL_CAPACITY;
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

void app_control_queue_destroy(app_control_queue *queue)
{
    if (queue == NULL) return;
    CloseHandle(queue->available);
    free(queue->events);
    DeleteCriticalSection(&queue->lock);
    free(queue);
}

int app_control_queue_push_ux(app_control_queue *queue,
    const ux_input_event *event)
{
    return app_control_queue_push_ux_for_run(queue, event, 0u);
}

int app_control_queue_push_ux_for_run(app_control_queue *queue,
    const ux_input_event *event, uint32_t run_generation)
{
    app_control_event copied = { 0 };
    if (event == NULL) return 0;
    copied.kind = APP_CONTROL_UX_INPUT;
    copied.run_generation = run_generation;
    copied.value.ux = *event;
    return app_control_queue_push(queue, &copied);
}

int app_control_queue_push_monitor_line(app_control_queue *queue,
    const lib_console_line *line)
{
    app_control_event copied = { 0 };
    if (line == NULL) return 0;
    copied.kind = APP_CONTROL_MONITOR_LINE;
    copied.value.line = *line;
    return app_control_queue_push(queue, &copied);
}

int app_control_queue_push_runtime_completed(app_control_queue *queue,
    app_runtime_state state, uint32_t run_generation)
{
    app_control_event event = { APP_CONTROL_RUNTIME_COMPLETED, run_generation };
    event.value.runtime_state = state;
    return app_control_queue_push(queue, &event);
}

int app_control_queue_push_frame_completed(app_control_queue *queue,
    uint32_t sequence, int graphics, uint32_t run_generation)
{
    app_control_event event = { APP_CONTROL_FRAME_COMPLETED, run_generation };
    event.value.frame.sequence = sequence;
    event.value.frame.graphics = graphics != 0;
    return app_control_queue_push(queue, &event);
}

int app_control_queue_push_component_completed(app_control_queue *queue,
    app_control_component_kind component, int exists, uint32_t run_generation)
{
    app_control_event event = { APP_CONTROL_COMPONENT_COMPLETED, run_generation };
    event.value.component.component = component;
    event.value.component.exists = exists != 0;
    return app_control_queue_push(queue, &event);
}

int app_control_queue_push_broker_completed(app_control_queue *queue,
    int vm_console_current, uint32_t run_generation)
{
    app_control_event event = { APP_CONTROL_BROKER_COMPLETED, run_generation };
    event.value.broker_vm_console_current = vm_console_current != 0;
    return app_control_queue_push(queue, &event);
}

int app_control_queue_take(app_control_queue *queue,
    app_control_event *out_event, unsigned long timeout_ms)
{
    if (queue == NULL || out_event == NULL ||
        WaitForSingleObject(queue->available, timeout_ms) != WAIT_OBJECT_0)
        return 0;
    EnterCriticalSection(&queue->lock);
    if (queue->count == 0u) {
        (void)ResetEvent(queue->available);
        LeaveCriticalSection(&queue->lock);
        return 0;
    }
    *out_event = queue->events[queue->first];
    queue->first = (queue->first + 1u) % queue->capacity;
    --queue->count;
    if (queue->count == 0u) (void)ResetEvent(queue->available);
    LeaveCriticalSection(&queue->lock);
    return 1;
}

static void app_control_forget_pressed(app_control_queue *queue,
    const ux_input_event *event)
{
    unsigned int index;
    for (index = 0u; index < queue->pressed_count; ++index) {
        app_control_pressed_key *pressed = &queue->pressed[index];
        if (pressed->source == event->source_identity &&
            pressed->event.data.key.scan_code == event->data.key.scan_code &&
            pressed->event.data.key.virtual_key == event->data.key.virtual_key) {
            pressed[0] = queue->pressed[--queue->pressed_count];
            return;
        }
    }
}

static void app_control_remember_pressed(app_control_queue *queue,
    const ux_input_event *event)
{
    unsigned int index;
    app_control_forget_pressed(queue, event);
    if (queue->pressed_count == APP_CONTROL_PRESSED_CAPACITY) return;
    index = queue->pressed_count++;
    queue->pressed[index].source = event->source_identity;
    queue->pressed[index].event = *event;
}

static int app_control_release_source(app_control_queue *queue,
    app_runtime *runtime, uint64_t source)
{
    unsigned int index = 0u;
    while (index < queue->pressed_count) {
        app_control_pressed_key *pressed = &queue->pressed[index];
        if (pressed->source != source) {
            ++index;
            continue;
        }
        pressed->event.data.key.pressed = 0u;
        if (!app_keyboard_deliver_input(runtime, &pressed->event)) return 0;
        queue->pressed[index] = queue->pressed[--queue->pressed_count];
    }
    return 1;
}

int app_control_handle_ux(app_control_queue *queue, app_runtime *runtime,
    const ux_input_event *event)
{
    if (queue == NULL || runtime == NULL || event == NULL) return 0;
    if (event->type == UX_EVENT_KEY) {
        if (event->data.key.pressed != 0u) app_control_remember_pressed(queue, event);
        else app_control_forget_pressed(queue, event);
        return app_keyboard_deliver_input(runtime, event);
    }
    if (event->type == UX_EVENT_MOUSE)
        return app_keyboard_deliver_input(runtime, event);
    if (event->type == UX_EVENT_SOURCE_RETIRED)
        return app_control_release_source(queue, runtime, event->source_identity);
    if (event->type == UX_EVENT_WINDOW_CLOSE)
        return app_runtime_request_window_close(runtime);
    if (event->type != UX_EVENT_HOTKEY) return 1;
    if (strcmp(event->data.hotkey.identifier, "pause-toggle") == 0)
        return app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED ?
            app_runtime_resume(runtime) : app_runtime_pause(runtime);
    if (strcmp(event->data.hotkey.identifier, "send-ctrl-alt-del") == 0)
        return app_keyboard_submit_ctrl_alt_del(runtime, app_keyboard_deliver_input);
    if (strcmp(event->data.hotkey.identifier, "send-alt-enter") == 0)
        return app_keyboard_submit_alt_enter(runtime, app_keyboard_deliver_input);
    if (strcmp(event->data.hotkey.identifier, "release-window-mouse") == 0) {
        app_runtime_request_window_mouse_release(runtime);
        return 1;
    }
    return 0;
}
#endif
