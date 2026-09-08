#ifndef APP_PRESENTATION_H
#define APP_PRESENTATION_H

#include "runtime.h"
#include "monitor.h"

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
/* Before a paused VM resumes, install the derived running components and
 * Current Console Object.  The control thread then calls app_runtime_resume. */
int app_presentation_prepare_resume(app_presentation *presentation);

#ifdef SOFTPC_WINDOW_TESTING
/* Test-only blocking adapter.  Production must use the functions above from
 * its sole control loop. */
int app_presentation_run(app_runtime *runtime,
    softpc_presentation presentation, int console_control,
    app_monitor_console *monitor, app_control_queue *control_queue);
#endif
#endif

#endif
