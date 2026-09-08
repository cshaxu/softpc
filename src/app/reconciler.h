#ifndef APP_RECONCILER_H
#define APP_RECONCILER_H

#include "presentation_plan.h"

/* This is a pure SoftPC policy reducer.  It owns no native handle, worker, or
 * VM pointer.  The control thread feeds it completed facts and executes only
 * the one action it returns; completion of that action is fed back on the
 * same FIFO before another action is chosen. */
typedef enum app_reconciler_intent {
    APP_RECONCILER_INTENT_NONE,
    APP_RECONCILER_INTENT_START,
    APP_RECONCILER_INTENT_RESUME,
    APP_RECONCILER_INTENT_PAUSE,
    APP_RECONCILER_INTENT_STOP,
    APP_RECONCILER_INTENT_RESET,
    APP_RECONCILER_INTENT_WINDOW_CLOSE
} app_reconciler_intent;

typedef enum app_reconciler_console_actual {
    APP_RECONCILER_CONSOLE_MONITOR,
    APP_RECONCILER_CONSOLE_VM
} app_reconciler_console_actual;

typedef enum app_reconciler_action {
    APP_RECONCILER_ACTION_NONE,
    APP_RECONCILER_ACTION_RUNTIME_START,
    APP_RECONCILER_ACTION_RUNTIME_PAUSE,
    APP_RECONCILER_ACTION_RUNTIME_RESUME,
    APP_RECONCILER_ACTION_RUNTIME_STOP,
    APP_RECONCILER_ACTION_CREATE_WINDOW,
    APP_RECONCILER_ACTION_CREATE_VM_CONSOLE,
    APP_RECONCILER_ACTION_BIND_VM_CONSOLE,
    APP_RECONCILER_ACTION_BIND_MONITOR,
    APP_RECONCILER_ACTION_DESTROY_VM_CONSOLE,
    APP_RECONCILER_ACTION_DESTROY_WINDOW
} app_reconciler_action;

typedef struct app_reconciler {
    softpc_presentation display;
    int console_control;
    app_runtime_state runtime_actual;
    int frame_actual;
    int graphics_actual;
    int window_actual;
    int vm_console_actual;
    app_reconciler_console_actual current_console_actual;
    app_reconciler_intent intent;
    /* A requested effect is not an actual fact.  Until its completion is
     * returned on the control FIFO, no second transition may be emitted. */
    app_reconciler_action in_flight;
    int reset_started;
    int close_requested;
} app_reconciler;

void app_reconciler_initialize(app_reconciler *reconciler,
    softpc_presentation display, int console_control);
void app_reconciler_note_intent(app_reconciler *reconciler,
    app_reconciler_intent intent);
void app_reconciler_note_runtime(app_reconciler *reconciler,
    app_runtime_state state);
void app_reconciler_note_frame(app_reconciler *reconciler, int graphics);
void app_reconciler_note_window(app_reconciler *reconciler, int exists);
void app_reconciler_note_vm_console(app_reconciler *reconciler, int exists);
void app_reconciler_note_current_console(app_reconciler *reconciler,
    app_reconciler_console_actual current);
app_presentation_plan app_reconciler_desired(const app_reconciler *reconciler);
app_reconciler_action app_reconciler_next_action(const app_reconciler *reconciler);
app_reconciler_action app_reconciler_take_action(app_reconciler *reconciler);

#endif
