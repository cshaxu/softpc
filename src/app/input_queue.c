#include "input_queue.h"

#ifdef _WIN32
#include <windows.h>
#include <stdlib.h>

#define APP_INPUT_QUEUE_CAPACITY 256u

struct app_input_queue {
    CRITICAL_SECTION lock;
    ui_input_event entries[APP_INPUT_QUEUE_CAPACITY];
    unsigned int head;
    unsigned int tail;
};

int app_input_queue_create(app_input_queue **out_queue)
{
    app_input_queue *queue;
    if (out_queue == NULL) return 0;
    *out_queue = NULL;
    queue = calloc(1u, sizeof(*queue));
    if (queue == NULL) return 0;
    InitializeCriticalSection(&queue->lock);
    *out_queue = queue;
    return 1;
}

void app_input_queue_destroy(app_input_queue *queue)
{
    if (queue == NULL) return;
    DeleteCriticalSection(&queue->lock);
    free(queue);
}

int app_input_queue_push(app_input_queue *queue, const ui_input_event *event)
{
    unsigned int next;
    if (queue == NULL || event == NULL) return 0;
    EnterCriticalSection(&queue->lock);
    next = (queue->head + 1u) % APP_INPUT_QUEUE_CAPACITY;
    if (next == queue->tail) {
        LeaveCriticalSection(&queue->lock);
        return 0;
    }
    queue->entries[queue->head] = *event;
    queue->head = next;
    LeaveCriticalSection(&queue->lock);
    return 1;
}

int app_input_queue_pop(app_input_queue *queue, ui_input_event *event)
{
    if (queue == NULL || event == NULL) return 0;
    EnterCriticalSection(&queue->lock);
    if (queue->tail == queue->head) {
        LeaveCriticalSection(&queue->lock);
        return 0;
    }
    *event = queue->entries[queue->tail];
    queue->tail = (queue->tail + 1u) % APP_INPUT_QUEUE_CAPACITY;
    LeaveCriticalSection(&queue->lock);
    return 1;
}

int app_input_queue_pending(app_input_queue *queue)
{
    int pending;
    if (queue == NULL) return 0;
    EnterCriticalSection(&queue->lock);
    pending = queue->tail != queue->head;
    LeaveCriticalSection(&queue->lock);
    return pending;
}

void app_input_queue_clear(app_input_queue *queue)
{
    if (queue == NULL) return;
    EnterCriticalSection(&queue->lock);
    queue->head = 0u;
    queue->tail = 0u;
    LeaveCriticalSection(&queue->lock);
}
#endif
