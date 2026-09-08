#ifndef APP_PRESENTATION_PLAN_H
#define APP_PRESENTATION_PLAN_H

#include "machine.h"
#include "runtime.h"

typedef struct app_presentation_plan {
    int window_enabled;
    int vm_console_enabled;
    int monitor_console_enabled;
} app_presentation_plan;

/* Pure product-policy derivation.  It neither creates components nor touches
 * host Console registration; the reconciler applies the returned facts. */
app_presentation_plan app_presentation_derive(softpc_presentation display,
    int console_control, app_runtime_state state, int graphics);

#endif
