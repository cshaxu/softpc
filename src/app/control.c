#include "control.h"
#include "keyboard.h"

#include <string.h>

#ifdef _WIN32
#include <windows.h>

#include <stdlib.h>

#define APP_CONTROL_QUEUE_CAPACITY 64u

struct app_control_queue {
    CRITICAL_SECTION lock;
    HANDLE available;
    app_control_event events[APP_CONTROL_QUEUE_CAPACITY];
    unsigned int first;
    unsigned int count;
};

static int app_control_queue_push(app_control_queue *queue,
    const app_control_event *event)
{
    if (queue == NULL || event == NULL) return 0;
    EnterCriticalSection(&queue->lock);
    if (queue->count == APP_CONTROL_QUEUE_CAPACITY) {
        LeaveCriticalSection(&queue->lock);
        return 0;
    }
    queue->events[(queue->first + queue->count) % APP_CONTROL_QUEUE_CAPACITY] =
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
    queue->available = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (queue->available == NULL) {
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
    DeleteCriticalSection(&queue->lock);
    free(queue);
}

int app_control_queue_push_ux(app_control_queue *queue,
    const ux_input_event *event)
{
    app_control_event copied = { 0 };
    if (event == NULL) return 0;
    copied.kind = APP_CONTROL_UX_INPUT;
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
    queue->first = (queue->first + 1u) % APP_CONTROL_QUEUE_CAPACITY;
    --queue->count;
    if (queue->count == 0u) (void)ResetEvent(queue->available);
    LeaveCriticalSection(&queue->lock);
    return 1;
}

int app_control_handle_ux(app_runtime *runtime, const ux_input_event *event)
{
    if (runtime == NULL || event == NULL) return 0;
    if (event->type == UX_EVENT_KEY || event->type == UX_EVENT_MOUSE)
        return app_keyboard_deliver_input(runtime, event);
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
