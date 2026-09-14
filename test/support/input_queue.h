#ifndef TEST_INPUT_QUEUE_COMPATIBILITY_H
#define TEST_INPUT_QUEUE_COMPATIBILITY_H

#include "common/machine/input_queue.h"

typedef common_machine_input_queue app_input_queue;
static inline int app_input_queue_create(app_input_queue **queue)
{ return common_machine_input_queue_create(queue) == LIB_STATUS_OK; }
static inline void app_input_queue_destroy(app_input_queue *queue)
{ common_machine_input_queue_destroy(queue); }
static inline int app_input_queue_push(app_input_queue *queue,
    const kvm_input_event *event)
{ return common_machine_input_queue_push(queue, event) != 0; }
static inline int app_input_queue_pop(app_input_queue *queue,
    kvm_input_event *event)
{ return common_machine_input_queue_pop(queue, event) != 0; }
static inline int app_input_queue_pending(app_input_queue *queue)
{ return common_machine_input_queue_pending(queue) != 0; }
static inline void app_input_queue_clear(app_input_queue *queue)
{ common_machine_input_queue_clear(queue); }

#endif
