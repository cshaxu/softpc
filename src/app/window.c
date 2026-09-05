#include "window.h"

#ifdef _WIN32
#include "presentation.h"

#include "../lib/platform/win32/presenter.h"

/* The product binding has no GDI, message-loop, cursor-capture, or frame
 * mailbox implementation.  It simply hands SoftPC's registered actions and
 * copied-frame producer to the reusable presenter. */
int app_vm_run_window(app_runtime *runtime,
    win32_presentation_router *router)
{
    win32_presentation_action_registry actions;
    win32_presentation_binding binding;

    if (!app_presentation_binding(runtime, router, &actions, &binding))
        return SOFTPC_VM_FRONTEND_ERROR;
    return app_presentation_result(win32_presentation_run_window(&binding));
}

#else
int app_vm_run_window(app_runtime *runtime,
    win32_presentation_router *router)
{
    (void)runtime;
    (void)router;
    return SOFTPC_VM_FRONTEND_ERROR;
}
#endif
