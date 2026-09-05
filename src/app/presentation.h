#ifndef APP_PRESENTATION_H
#define APP_PRESENTATION_H

#include "runtime.h"
#include "../lib/platform/win32/presenter.h"

#ifdef _WIN32
#define SOFTPC_VM_FRONTEND_STOPPED 0
#define SOFTPC_VM_FRONTEND_ERROR 1
#define SOFTPC_VM_FRONTEND_PAUSED 2

int app_presentation_binding(app_runtime *runtime,
    win32_presentation_router *router,
    win32_presentation_action_registry *actions,
    win32_presentation_binding *binding);
int app_presentation_result(win32_presentation_run_result result);
int app_presentation_run(app_runtime *runtime,
    softpc_presentation presentation);
#endif

#endif
