#ifndef APP_CONTROL_STATE_H
#define APP_CONTROL_STATE_H

#include "command.h"
#include "reconciler.h"

/* Control-thread-only product facts.  This module has no worker, native, or
 * queue dependency: callers feed copied completion facts and consume derived
 * presentation actions. */
typedef struct app_control_state {
    app_reconciler presentation;
    app_monitor_state monitor_actual;
    uint32_t observed_frame_sequence;
} app_control_state;

void app_control_state_initialize(app_control_state *state,
    softpc_presentation display, int console_control);
void app_control_state_note_window_close(app_control_state *state);
/* Records one public runtime completion.  RESET_COMPLETED is normalized to
 * PAUSED only for presentation; its distinct completion identity remains
 * available to the command session that owns monitor wording. */
void app_control_state_note_runtime(app_control_state *state,
    app_runtime_state completed);
int app_control_state_note_frame(app_control_state *state, uint32_t sequence,
    int graphics);
void app_control_state_note_window(app_control_state *state, int exists);
void app_control_state_note_vm_console(app_control_state *state, int exists);
void app_control_state_note_current_console(app_control_state *state,
    int vm_console_current);
app_reconciler_action app_control_state_take_action(app_control_state *state);
int app_control_state_monitor_is_current(const app_control_state *state);
int app_control_state_monitor_is_running_graphics_surface(
    const app_control_state *state);
int app_control_state_frame_targets_ready(const app_control_state *state);

#endif
