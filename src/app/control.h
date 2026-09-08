#ifndef APP_CONTROL_H
#define APP_CONTROL_H

#include "lib/base/console.h"
#include "lib/ux-base/event.h"

typedef struct app_control_queue app_control_queue;

typedef enum app_control_event_kind {
    APP_CONTROL_UX_INPUT,
    APP_CONTROL_MONITOR_LINE
} app_control_event_kind;

typedef struct app_control_event {
    app_control_event_kind kind;
    union {
        ux_input_event ux;
        lib_console_line line;
    } value;
} app_control_event;

int app_control_queue_create(app_control_queue **out_queue);
void app_control_queue_destroy(app_control_queue *queue);
int app_control_queue_push_ux(app_control_queue *queue,
    const ux_input_event *event);
int app_control_queue_push_monitor_line(app_control_queue *queue,
    const lib_console_line *line);
int app_control_queue_take(app_control_queue *queue,
    app_control_event *out_event, unsigned long timeout_ms);

#endif
