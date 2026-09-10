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
    app_runtime *runtime, app_monitor_console *monitor,
    app_control_queue *control_queue);
void app_presentation_destroy(app_presentation *presentation);
/* Control supplies the one already-derived actuator action.  The adapter
 * performs it and returns a copied completion to control's FIFO. */
int app_presentation_apply_action(app_presentation *presentation,
    app_reconciler_action action, app_runtime_state runtime_actual);
void app_presentation_set_runtime_state(app_presentation *presentation,
    app_runtime_state runtime_actual);
int app_presentation_publish_frame(app_presentation *presentation,
    const app_runtime_frame *frame, int window_actual,
    int vm_console_current, int console_status_surface);
void app_presentation_release_window_mouse(app_presentation *presentation);

#endif

#endif
