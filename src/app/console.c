#include "console.h"

#ifdef _WIN32
#include "presentation.h"

#include "../lib/platform/win32/console.h"

/* SoftPC supplies only its binding: executor state, copied mailbox, original
 * keyboard ingress, and its product action table.  The console itself,
 * private-console lifetime, host input normalization, painting, and routing
 * are reusable Win32 presentation code. */
int app_vm_run_console(app_runtime *runtime,
    win32_presentation_router *router)
{
    win32_presentation_action_registry actions;
    win32_presentation_binding binding;

    if (!app_presentation_binding(runtime, router, &actions, &binding))
        return SOFTPC_VM_FRONTEND_ERROR;
    return app_presentation_result(win32_presentation_run_console(&binding));
}

#else
int app_vm_run_console(app_runtime *runtime,
    win32_presentation_router *router)
{
    (void)runtime;
    (void)router;
    return SOFTPC_VM_FRONTEND_ERROR;
}
#endif
