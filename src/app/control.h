#ifndef APP_CONTROL_H
#define APP_CONTROL_H

#include "lib/console/console_interface.h"
#include "lib/ui-base/event_interface.h"
#include "runtime.h"

typedef struct app_control_queue app_control_queue;

typedef enum app_control_event_kind {
    APP_CONTROL_UI_INPUT,
    APP_CONTROL_MONITOR_LINE,
    /* Completion records are intentionally distinct from user input.  The
     * sole control consumer feeds them to the reducer as actual facts. */
    APP_CONTROL_RUNTIME_COMPLETED,
    APP_CONTROL_FRAME_COMPLETED,
    APP_CONTROL_COMPONENT_COMPLETED,
    APP_CONTROL_BROKER_COMPLETED,
    APP_CONTROL_UI_DELIVERY_FAILED,
    /* The queue could not retain a control fact.  Continuing would leave the
       reducer waiting for an event which was silently lost. */
    APP_CONTROL_QUEUE_DELIVERY_FAILED,
    APP_CONTROL_CONSOLE_FAILED
} app_control_event_kind;

typedef enum app_control_component_kind {
    APP_CONTROL_COMPONENT_WINDOW,
    APP_CONTROL_COMPONENT_VM_CONSOLE
} app_control_component_kind;

typedef struct app_control_event {
    app_control_event_kind kind;
    /* Zero is monitor/local input.  UI producers stamp the currently active
     * machine run so a queued old input cannot affect a later start. */
    uint32_t run_generation;
    union {
        ui_input_event ui;
        lib_console_line line;
        app_runtime_state runtime_state;
        struct { uint32_t sequence; int graphics; } frame;
        struct { app_control_component_kind component; int exists; } component;
        int broker_vm_console_current;
        struct { uint64_t source_identity; lib_status status; } delivery_failure;
        lib_status queue_delivery_status;
    } value;
} app_control_event;

int app_control_queue_create(app_control_queue **out_queue);
void app_control_queue_destroy(app_control_queue *queue);
int app_control_queue_push_ux(app_control_queue *queue,
    const ui_input_event *event);
int app_control_queue_push_ui_for_run(app_control_queue *queue,
    const ui_input_event *event, uint32_t run_generation);
int app_control_queue_push_monitor_line(app_control_queue *queue,
    const lib_console_line *line);
int app_control_queue_push_console_failed(app_control_queue *queue);
int app_control_queue_push_runtime_completed(app_control_queue *queue,
    app_runtime_state state, uint32_t run_generation);
int app_control_queue_push_frame_completed(app_control_queue *queue,
    uint32_t sequence, int graphics, uint32_t run_generation);
int app_control_queue_push_component_completed(app_control_queue *queue,
    app_control_component_kind component, int exists, uint32_t run_generation);
int app_control_queue_push_broker_completed(app_control_queue *queue,
    int vm_console_current, uint32_t run_generation);
int app_control_queue_push_ui_delivery_failed(app_control_queue *queue,
    uint64_t source_identity, lib_status status, uint32_t run_generation);
int app_control_queue_take(app_control_queue *queue,
    app_control_event *out_event, unsigned long timeout_ms);
/* A UI producer belongs to one VM run. Paused admits cleanup/lifecycle and
 * registered-hotkey records for product handling, but ordinary guest input
 * remains rejected. Monitor lines and completion facts use separate rules. */
int app_control_accept_ui_event(const app_control_event *event,
    uint32_t current_run_generation, app_runtime_state runtime_state);
int app_control_handle_ux(app_control_queue *queue, app_runtime *runtime,
    const ui_input_event *event, app_runtime_state runtime_state);

#endif
