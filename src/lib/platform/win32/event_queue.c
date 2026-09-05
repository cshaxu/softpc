#include "event_queue.h"

#ifdef _WIN32
#include <windows.h>
#include <stdlib.h>

#define WIN32_PRESENTATION_EVENT_QUEUE_CAPACITY 256u

struct win32_presentation_event_queue {
    CRITICAL_SECTION lock;
    win32_presentation_event entries[WIN32_PRESENTATION_EVENT_QUEUE_CAPACITY];
    uint32_t head;
    uint32_t tail;
    HANDLE event;
};

int win32_presentation_event_queue_create(
    win32_presentation_event_queue **out)
{
    win32_presentation_event_queue *queue;

    if (out == NULL) return 0;
    *out = NULL;
    queue = (win32_presentation_event_queue *)calloc(1u, sizeof(*queue));
    if (queue == NULL) return 0;
    InitializeCriticalSection(&queue->lock);
    queue->event = CreateEventA(NULL, FALSE, FALSE, NULL);
    if (queue->event == NULL) {
        DeleteCriticalSection(&queue->lock);
        free(queue);
        return 0;
    }
    *out = queue;
    return 1;
}

void win32_presentation_event_queue_destroy(
    win32_presentation_event_queue *queue)
{
    if (queue == NULL) return;
    CloseHandle(queue->event);
    DeleteCriticalSection(&queue->lock);
    free(queue);
}

int win32_presentation_event_queue_push(win32_presentation_event_queue *queue,
    const win32_presentation_event *event)
{
    uint32_t next;

    if (queue == NULL || event == NULL) return 0;
    EnterCriticalSection(&queue->lock);
    if (event->type == WIN32_PRESENTATION_EVENT_MOUSE &&
        queue->tail != queue->head) {
        uint32_t last = queue->head == 0u ?
            WIN32_PRESENTATION_EVENT_QUEUE_CAPACITY - 1u : queue->head - 1u;
        win32_presentation_event *prior = &queue->entries[last];
        if (prior->type == WIN32_PRESENTATION_EVENT_MOUSE &&
            prior->data.mouse.left_down == event->data.mouse.left_down &&
            prior->data.mouse.right_down == event->data.mouse.right_down) {
            prior->data.mouse.delta_x += event->data.mouse.delta_x;
            prior->data.mouse.delta_y += event->data.mouse.delta_y;
            LeaveCriticalSection(&queue->lock);
            SetEvent(queue->event);
            return 1;
        }
    }
    next = (queue->head + 1u) % WIN32_PRESENTATION_EVENT_QUEUE_CAPACITY;
    if (next == queue->tail) {
        LeaveCriticalSection(&queue->lock);
        return 0;
    }
    queue->entries[queue->head] = *event;
    queue->head = next;
    LeaveCriticalSection(&queue->lock);
    SetEvent(queue->event);
    return 1;
}

int win32_presentation_event_queue_pop(win32_presentation_event_queue *queue,
    win32_presentation_event *event)
{
    if (queue == NULL || event == NULL) return 0;
    EnterCriticalSection(&queue->lock);
    if (queue->tail == queue->head) {
        LeaveCriticalSection(&queue->lock);
        return 0;
    }
    *event = queue->entries[queue->tail];
    queue->tail = (queue->tail + 1u) % WIN32_PRESENTATION_EVENT_QUEUE_CAPACITY;
    LeaveCriticalSection(&queue->lock);
    return 1;
}

int win32_presentation_event_queue_pending(
    win32_presentation_event_queue *queue)
{
    int pending;

    if (queue == NULL) return 0;
    EnterCriticalSection(&queue->lock);
    pending = queue->tail != queue->head;
    LeaveCriticalSection(&queue->lock);
    return pending;
}

void *win32_presentation_event_queue_event(
    const win32_presentation_event_queue *queue)
{
    return queue == NULL ? NULL : (void *)queue->event;
}
#endif
