#include "reconciler.h"

void app_reconciler_initialize(app_reconciler *reconciler,
    softpc_presentation display, int console_control)
{
    if (reconciler == NULL) return;
    *reconciler = (app_reconciler) { 0 };
    reconciler->display = display;
    reconciler->console_control = console_control != 0;
    reconciler->runtime_actual = SOFTPC_RUNTIME_STOPPED;
    reconciler->current_console_actual = APP_RECONCILER_CONSOLE_MONITOR;
}

void app_reconciler_note_intent(app_reconciler *reconciler,
    app_reconciler_intent intent)
{
    if (reconciler == NULL || intent == APP_RECONCILER_INTENT_NONE) return;
    if (intent == APP_RECONCILER_INTENT_WINDOW_CLOSE)
        reconciler->close_requested = 1;
    if (intent == APP_RECONCILER_INTENT_RESET)
        reconciler->reset_started = 0;
    reconciler->intent = intent;
}

void app_reconciler_note_runtime(app_reconciler *reconciler,
    app_runtime_state state)
{
    if (reconciler == NULL) return;
    reconciler->runtime_actual = state;
    if (state == SOFTPC_RUNTIME_STOPPED || state == SOFTPC_RUNTIME_ERROR) {
        /* A subsequent run must not inherit the previous run's display
         * route before it has committed a frame of its own. */
        reconciler->frame_actual = 0;
        reconciler->graphics_actual = 0;
    }
    if ((reconciler->in_flight == APP_RECONCILER_ACTION_RUNTIME_START ||
         reconciler->in_flight == APP_RECONCILER_ACTION_RUNTIME_RESUME) &&
        state == SOFTPC_RUNTIME_RUNNING)
        reconciler->in_flight = APP_RECONCILER_ACTION_NONE;
    else if (reconciler->in_flight == APP_RECONCILER_ACTION_RUNTIME_PAUSE &&
        state == SOFTPC_RUNTIME_PAUSED)
        reconciler->in_flight = APP_RECONCILER_ACTION_NONE;
    else if (reconciler->in_flight == APP_RECONCILER_ACTION_RUNTIME_STOP &&
        (state == SOFTPC_RUNTIME_STOPPED || state == SOFTPC_RUNTIME_ERROR))
        reconciler->in_flight = APP_RECONCILER_ACTION_NONE;
    if (reconciler->intent == APP_RECONCILER_INTENT_RESET &&
        reconciler->reset_started && state == SOFTPC_RUNTIME_PAUSED)
        reconciler->intent = APP_RECONCILER_INTENT_NONE;
}

void app_reconciler_note_frame(app_reconciler *reconciler, int graphics)
{
    if (reconciler != NULL) {
        reconciler->frame_actual = 1;
        reconciler->graphics_actual = graphics != 0;
    }
}

void app_reconciler_note_window(app_reconciler *reconciler, int exists)
{
    if (reconciler == NULL) return;
    reconciler->window_actual = exists != 0;
    if ((exists && reconciler->in_flight == APP_RECONCILER_ACTION_CREATE_WINDOW) ||
        (!exists && reconciler->in_flight == APP_RECONCILER_ACTION_DESTROY_WINDOW))
        reconciler->in_flight = APP_RECONCILER_ACTION_NONE;
}

void app_reconciler_note_vm_console(app_reconciler *reconciler, int exists)
{
    if (reconciler == NULL) return;
    reconciler->vm_console_actual = exists != 0;
    if ((exists && reconciler->in_flight == APP_RECONCILER_ACTION_CREATE_VM_CONSOLE) ||
        (!exists && reconciler->in_flight == APP_RECONCILER_ACTION_DESTROY_VM_CONSOLE))
        reconciler->in_flight = APP_RECONCILER_ACTION_NONE;
}

void app_reconciler_note_current_console(app_reconciler *reconciler,
    app_reconciler_console_actual current)
{
    if (reconciler == NULL) return;
    reconciler->current_console_actual = current;
    if ((current == APP_RECONCILER_CONSOLE_VM &&
         reconciler->in_flight == APP_RECONCILER_ACTION_BIND_VM_CONSOLE) ||
        (current == APP_RECONCILER_CONSOLE_MONITOR &&
         reconciler->in_flight == APP_RECONCILER_ACTION_BIND_MONITOR))
        reconciler->in_flight = APP_RECONCILER_ACTION_NONE;
}

app_presentation_plan app_reconciler_desired(const app_reconciler *reconciler)
{
    app_presentation_plan plan = { 0, 0, 1 };
    if (reconciler == NULL) return plan;
    plan = app_presentation_derive(reconciler->display,
        reconciler->console_control, reconciler->runtime_actual,
        reconciler->frame_actual,
        reconciler->graphics_actual);
    if (reconciler->close_requested &&
        reconciler->runtime_actual == SOFTPC_RUNTIME_PAUSED)
        plan.window_enabled = 0;
    return plan;
}

int app_reconciler_console_only_actual(const app_reconciler *reconciler)
{
    app_presentation_plan desired;

    if (reconciler == NULL || reconciler->window_actual) return 0;
    desired = app_reconciler_desired(reconciler);
    if (desired.window_enabled) return 0;
    if (reconciler->current_console_actual == APP_RECONCILER_CONSOLE_MONITOR)
        return 1;
    return desired.vm_console_enabled && reconciler->vm_console_actual &&
        reconciler->current_console_actual == APP_RECONCILER_CONSOLE_VM;
}

app_reconciler_action app_reconciler_next_action(const app_reconciler *reconciler)
{
    app_presentation_plan desired;
    if (reconciler == NULL) return APP_RECONCILER_ACTION_NONE;
    if (reconciler->in_flight != APP_RECONCILER_ACTION_NONE)
        return APP_RECONCILER_ACTION_NONE;

    if (reconciler->intent == APP_RECONCILER_INTENT_STOP) {
        if (reconciler->runtime_actual != SOFTPC_RUNTIME_STOPPED &&
            reconciler->runtime_actual != SOFTPC_RUNTIME_ERROR)
            return APP_RECONCILER_ACTION_RUNTIME_STOP;
    }
    if (reconciler->intent == APP_RECONCILER_INTENT_RESET) {
        if (!reconciler->reset_started &&
            reconciler->runtime_actual != SOFTPC_RUNTIME_STOPPED &&
            reconciler->runtime_actual != SOFTPC_RUNTIME_ERROR)
            return APP_RECONCILER_ACTION_RUNTIME_STOP;
        if (!reconciler->reset_started)
            return APP_RECONCILER_ACTION_RUNTIME_START;
        if (reconciler->runtime_actual == SOFTPC_RUNTIME_RUNNING)
            return APP_RECONCILER_ACTION_RUNTIME_PAUSE;
    }
    if (reconciler->intent == APP_RECONCILER_INTENT_PAUSE ||
        reconciler->intent == APP_RECONCILER_INTENT_WINDOW_CLOSE) {
        if (reconciler->runtime_actual == SOFTPC_RUNTIME_RUNNING)
            return APP_RECONCILER_ACTION_RUNTIME_PAUSE;
    }
    if (reconciler->intent == APP_RECONCILER_INTENT_START &&
        reconciler->runtime_actual == SOFTPC_RUNTIME_STOPPED)
        return APP_RECONCILER_ACTION_RUNTIME_START;

    desired = app_reconciler_desired(reconciler);
    if (desired.window_enabled && !reconciler->window_actual)
        return APP_RECONCILER_ACTION_CREATE_WINDOW;
    if (desired.vm_console_enabled && !reconciler->vm_console_actual)
        return APP_RECONCILER_ACTION_CREATE_VM_CONSOLE;
    if (desired.vm_console_enabled &&
        reconciler->current_console_actual != APP_RECONCILER_CONSOLE_VM)
        return APP_RECONCILER_ACTION_BIND_VM_CONSOLE;
    if (!desired.vm_console_enabled &&
        reconciler->current_console_actual != APP_RECONCILER_CONSOLE_MONITOR)
        return APP_RECONCILER_ACTION_BIND_MONITOR;
    if (!desired.vm_console_enabled && reconciler->vm_console_actual)
        return APP_RECONCILER_ACTION_DESTROY_VM_CONSOLE;
    if (!desired.window_enabled && reconciler->window_actual)
        return APP_RECONCILER_ACTION_DESTROY_WINDOW;

    if (reconciler->intent == APP_RECONCILER_INTENT_RESUME &&
        reconciler->runtime_actual == SOFTPC_RUNTIME_PAUSED)
        return APP_RECONCILER_ACTION_RUNTIME_RESUME;
    return APP_RECONCILER_ACTION_NONE;
}

app_reconciler_action app_reconciler_take_action(app_reconciler *reconciler)
{
    app_reconciler_action action;
    if (reconciler == NULL) return APP_RECONCILER_ACTION_NONE;
    action = app_reconciler_next_action(reconciler);
    if (action != APP_RECONCILER_ACTION_NONE)
        reconciler->in_flight = action;
    if (action == APP_RECONCILER_ACTION_RUNTIME_START &&
        reconciler->intent == APP_RECONCILER_INTENT_RESET)
        reconciler->reset_started = 1;
    return action;
}
