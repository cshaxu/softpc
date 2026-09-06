#ifndef APP_PRESENTATION_H
#define APP_PRESENTATION_H

#include "runtime.h"
#include "lib/ux/presenter.h"

#ifdef _WIN32
#define SOFTPC_VM_FRONTEND_STOPPED 0
#define SOFTPC_VM_FRONTEND_ERROR 1
#define SOFTPC_VM_FRONTEND_PAUSED 2

int app_presentation_binding(app_runtime *runtime,
    ux_router *router,
    ux_action_registry *actions,
    ux_binding *binding);
int app_presentation_result(ux_run_result result);
int app_presentation_run(app_runtime *runtime,
    softpc_presentation presentation);
#endif

#endif
