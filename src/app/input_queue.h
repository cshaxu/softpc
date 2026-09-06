#ifndef APP_INPUT_QUEUE_H
#define APP_INPUT_QUEUE_H

#include "lib/ux/event.h"

typedef struct app_input_queue app_input_queue;

/* SoftPC runtime coordination: UI producers publish copied UX events, while
 * the sole machine executor consumes them.  This is deliberately product
 * runtime ownership, not a second UX implementation. */
int app_input_queue_create(app_input_queue **out_queue);
void app_input_queue_destroy(app_input_queue *queue);
int app_input_queue_push(app_input_queue *queue, const ux_event *event);
int app_input_queue_pop(app_input_queue *queue, ux_event *event);
int app_input_queue_pending(app_input_queue *queue);

#endif
