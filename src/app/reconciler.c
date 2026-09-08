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
    reconciler->intent = intent;
}

void app_reconciler_note_runtime(app_reconciler *reconciler,
    app_runtime_state state)
{ if (reconciler != NULL) reconciler->runtime_actual = state; }

void app_reconciler_note_frame(app_reconciler *reconciler, int graphics)
{ if (reconciler != NULL) reconciler->graphics_actual = graphics != 0; }

void app_reconciler_note_window(app_reconciler *reconciler, int exists)
{ if (reconciler != NULL) reconciler->window_actual = exists != 0; }

void app_reconciler_note_vm_console(app_reconciler *reconciler, int exists)
{ if (reconciler != NULL) reconciler->vm_console_actual = exists != 0; }

void app_reconciler_note_current_console(app_reconciler *reconciler,
    app_reconciler_console_actual current)
{ if (reconciler != NULL) reconciler->current_console_actual = current; }

app_presentation_plan app_reconciler_desired(const app_reconciler *reconciler)
{
    app_presentation_plan plan = { 0, 0, 1 };
    if (reconciler == NULL) return plan;
    plan = app_presentation_derive(reconciler->display,
        reconciler->console_control, reconciler->runtime_actual,
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

    if (reconciler->intent == APP_RECONCILER_INTENT_STOP ||
        reconciler->intent == APP_RECONCILER_INTENT_RESET) {
        if (reconciler->runtime_actual != SOFTPC_RUNTIME_STOPPED &&
            reconciler->runtime_actual != SOFTPC_RUNTIME_ERROR)
            return APP_RECONCILER_ACTION_RUNTIME_STOP;
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
