#include "control_state.h"

void app_control_state_initialize(app_control_state *state,
    softpc_presentation display, int console_control)
{
    if (state == NULL) return;
    *state = (app_control_state) { 0 };
    state->monitor_actual = APP_MONITOR_INIT;
    app_reconciler_initialize(&state->presentation, display, console_control);
}

void app_control_state_note_window_close(app_control_state *state)
{
    if (state != NULL) app_reconciler_note_window_close(&state->presentation);
}

void app_control_state_note_runtime(app_control_state *state,
    app_runtime_state completed)
{
    app_runtime_state presentation_state = completed;
    if (state == NULL) return;
    if (completed == SOFTPC_RUNTIME_RESET_COMPLETED ||
        completed == SOFTPC_RUNTIME_PAUSED)
        state->monitor_actual = APP_MONITOR_PAUSED;
    else if (completed == SOFTPC_RUNTIME_RUNNING)
        state->monitor_actual = APP_MONITOR_RUNNING;
    else if (completed == SOFTPC_RUNTIME_STOPPED || completed == SOFTPC_RUNTIME_ERROR)
        state->monitor_actual = APP_MONITOR_STOPPED;
    if (presentation_state == SOFTPC_RUNTIME_RESET_COMPLETED)
        presentation_state = SOFTPC_RUNTIME_PAUSED;
    app_reconciler_note_runtime(&state->presentation, presentation_state);
}

int app_control_state_note_frame(app_control_state *state, uint32_t sequence,
    int graphics)
{
    if (state == NULL || sequence == 0u ||
        sequence <= state->observed_frame_sequence) return 0;
    state->observed_frame_sequence = sequence;
    app_reconciler_note_frame(&state->presentation, graphics);
    return 1;
}

void app_control_state_note_window(app_control_state *state, int exists)
{
    if (state != NULL) app_reconciler_note_window(&state->presentation, exists);
}

void app_control_state_note_vm_console(app_control_state *state, int exists)
{
    if (state != NULL)
        app_reconciler_note_vm_console(&state->presentation, exists);
}

void app_control_state_note_current_console(app_control_state *state,
    int vm_console_current)
{
    if (state != NULL) app_reconciler_note_current_console(&state->presentation,
        vm_console_current ? APP_RECONCILER_CONSOLE_VM :
        APP_RECONCILER_CONSOLE_MONITOR);
}

app_reconciler_action app_control_state_take_action(app_control_state *state)
{
    return state == NULL ? APP_RECONCILER_ACTION_NONE :
        app_reconciler_take_action(&state->presentation);
}

int app_control_state_monitor_is_current(const app_control_state *state)
{
    app_presentation_plan desired;
    if (state == NULL) return 0;
    desired = app_reconciler_desired(&state->presentation);
    return desired.monitor_console_enabled &&
        state->presentation.current_console_actual ==
            APP_RECONCILER_CONSOLE_MONITOR &&
        state->presentation.in_flight != APP_RECONCILER_ACTION_BIND_VM_CONSOLE;
}

int app_control_state_monitor_is_running_graphics_surface(
    const app_control_state *state)
{
    return state != NULL &&
        state->presentation.display == SOFTPC_PRESENTATION_CONSOLE &&
        state->presentation.runtime_actual == SOFTPC_RUNTIME_RUNNING &&
        state->presentation.frame_actual && state->presentation.graphics_actual &&
        app_control_state_monitor_is_current(state);
}

int app_control_state_frame_targets_ready(const app_control_state *state)
{
    app_presentation_plan desired;
    if (state == NULL) return 0;
    desired = app_reconciler_desired(&state->presentation);
    if (!desired.window_enabled && !desired.vm_console_enabled) return 0;
    if (desired.window_enabled && !state->presentation.window_actual) return 0;
    return !desired.vm_console_enabled ||
        (state->presentation.vm_console_actual &&
         state->presentation.current_console_actual == APP_RECONCILER_CONSOLE_VM);
}
