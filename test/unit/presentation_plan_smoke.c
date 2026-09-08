#include "presentation_plan.h"

#include <assert.h>

int main(void)
{
    app_presentation_plan plan;

    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 1,
        SOFTPC_RUNTIME_RUNNING, 0);
    assert(!plan.window_enabled && plan.vm_console_enabled &&
        !plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 0,
        SOFTPC_RUNTIME_RUNNING, 1);
    assert(plan.window_enabled && plan.vm_console_enabled &&
        !plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 1,
        SOFTPC_RUNTIME_RUNNING, 1);
    assert(plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_WINDOW, 0,
        SOFTPC_RUNTIME_RUNNING, 0);
    assert(plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_WINDOW, 1,
        SOFTPC_RUNTIME_PAUSED, 1);
    assert(!plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    return 0;
}
