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

void app_reconciler_note_window_close(app_reconciler *reconciler)
{
    if (reconciler != NULL) reconciler->close_requested = 1;
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

app_reconciler_action app_reconciler_next_action(const app_reconciler *reconciler)
{
    app_presentation_plan desired;
    if (reconciler == NULL) return APP_RECONCILER_ACTION_NONE;
    if (reconciler->in_flight != APP_RECONCILER_ACTION_NONE)
        return APP_RECONCILER_ACTION_NONE;

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
    return APP_RECONCILER_ACTION_NONE;
}

app_reconciler_action app_reconciler_take_action(app_reconciler *reconciler)
{
    app_reconciler_action action;
    if (reconciler == NULL) return APP_RECONCILER_ACTION_NONE;
    action = app_reconciler_next_action(reconciler);
    if (action != APP_RECONCILER_ACTION_NONE)
        reconciler->in_flight = action;
    return action;
}
