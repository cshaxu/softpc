#ifndef APP_INPUT_QUEUE_H
#define APP_INPUT_QUEUE_H

#include "lib/kvm-base/event_interface.h"

typedef struct app_input_queue app_input_queue;

/* SoftPC runtime coordination: KVM producers publish copied KVM events, while
 * the sole machine executor consumes them.  This is deliberately product
 * runtime ownership, not a second KVM implementation. */
int app_input_queue_create(app_input_queue **out_queue);
void app_input_queue_destroy(app_input_queue *queue);
int app_input_queue_push(app_input_queue *queue, const kvm_input_event *event);
int app_input_queue_pop(app_input_queue *queue, kvm_input_event *event);
int app_input_queue_pending(app_input_queue *queue);
/* A new cold VM run has no guest-input history.  This atomically discards
   records accepted for an earlier run; it does not affect the host monitor
   or its independent control queue. */
void app_input_queue_clear(app_input_queue *queue);

#endif
