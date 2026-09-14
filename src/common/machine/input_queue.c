#include "common/machine/input_queue.h"

#ifdef _WIN32
#include <windows.h>
#include <stdlib.h>

#define COMMON_MACHINE_INPUT_QUEUE_CAPACITY 256u

struct common_machine_input_queue {
    CRITICAL_SECTION lock;
    kvm_input_event entries[COMMON_MACHINE_INPUT_QUEUE_CAPACITY];
    unsigned int head;
    unsigned int tail;
};

lib_status common_machine_input_queue_create(common_machine_input_queue **out_queue)
{
    common_machine_input_queue *queue;
    if (out_queue == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_queue = NULL;
    queue = calloc(1u, sizeof(*queue));
    if (queue == NULL) return LIB_STATUS_NO_MEMORY;
    InitializeCriticalSection(&queue->lock);
    *out_queue = queue;
    return LIB_STATUS_OK;
}

void common_machine_input_queue_destroy(common_machine_input_queue *queue)
{
    if (queue == NULL) return;
    DeleteCriticalSection(&queue->lock);
    free(queue);
}

lib_bool common_machine_input_queue_push(common_machine_input_queue *queue,
    const kvm_input_event *event)
{
    unsigned int next;
    if (queue == NULL || event == NULL) return LIB_FALSE;
    EnterCriticalSection(&queue->lock);
    next = (queue->head + 1u) % COMMON_MACHINE_INPUT_QUEUE_CAPACITY;
    if (next == queue->tail) {
        LeaveCriticalSection(&queue->lock);
        return LIB_FALSE;
    }
    queue->entries[queue->head] = *event;
    queue->head = next;
    LeaveCriticalSection(&queue->lock);
    return LIB_TRUE;
}

lib_bool common_machine_input_queue_pop(common_machine_input_queue *queue,
    kvm_input_event *event)
{
    if (queue == NULL || event == NULL) return LIB_FALSE;
    EnterCriticalSection(&queue->lock);
    if (queue->tail == queue->head) {
        LeaveCriticalSection(&queue->lock);
        return LIB_FALSE;
    }
    *event = queue->entries[queue->tail];
    queue->tail = (queue->tail + 1u) % COMMON_MACHINE_INPUT_QUEUE_CAPACITY;
    LeaveCriticalSection(&queue->lock);
    return LIB_TRUE;
}

lib_bool common_machine_input_queue_pending(common_machine_input_queue *queue)
{
    lib_bool pending;
    if (queue == NULL) return LIB_FALSE;
    EnterCriticalSection(&queue->lock);
    pending = queue->tail != queue->head;
    LeaveCriticalSection(&queue->lock);
    return pending;
}

void common_machine_input_queue_clear(common_machine_input_queue *queue)
{
    if (queue == NULL) return;
    EnterCriticalSection(&queue->lock);
    queue->head = 0u;
    queue->tail = 0u;
    LeaveCriticalSection(&queue->lock);
}
#endif
