#ifndef APP_PRESENTATION_H
#define APP_PRESENTATION_H

#include "runtime.h"
#include "monitor.h"
#include "reconciler.h"

#ifdef _WIN32
typedef struct app_presentation app_presentation;

#define SOFTPC_VM_FRONTEND_STOPPED 0
#define SOFTPC_VM_FRONTEND_ERROR 1
#define SOFTPC_VM_FRONTEND_PAUSED 2

/* The caller is the one SoftPC control thread.  It owns lifecycle requests
 * and invokes reconcile after each FIFO event or state observation; this
 * object only applies its derived component/Console plan on that thread. */
int app_presentation_create(app_presentation **out_presentation,
    app_runtime *runtime, softpc_presentation presentation, int console_control,
    app_monitor_console *monitor, app_control_queue *control_queue);
void app_presentation_destroy(app_presentation *presentation);
int app_presentation_reconcile(app_presentation *presentation);
/* Control records the Window-close fact after it has submitted any required
 * runtime pause.  Presentation then derives only component/broker teardown. */
void app_presentation_note_window_close(app_presentation *presentation);
void app_presentation_release_window_mouse(app_presentation *presentation);
void app_presentation_note_runtime_completed(app_presentation *presentation,
    app_runtime_state state);
void app_presentation_note_frame_completed(app_presentation *presentation,
    uint32_t sequence, int graphics);
void app_presentation_note_component_completed(app_presentation *presentation,
    app_control_component_kind component, int exists);
void app_presentation_note_broker_completed(app_presentation *presentation,
    int vm_console_current);
/* True only after the monitor logical Console is the completed current owner,
 * not merely the reducer's desired target. */
int app_presentation_monitor_is_current(const app_presentation *presentation);
int app_presentation_monitor_is_running_graphics_surface(
    const app_presentation *presentation);

#endif

#endif
