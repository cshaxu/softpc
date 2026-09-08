#include "presentation_plan.h"

#include <assert.h>

int main(void)
{
    app_presentation_plan plan;

    /* Console display keeps the cooked monitor through VM startup.  A raw
       Console may take ownership only after a completed guest frame exists. */
    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 1,
        SOFTPC_RUNTIME_RUNNING, 0, 0);
    assert(!plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 1,
        SOFTPC_RUNTIME_RUNNING, 1, 0);
    assert(!plan.window_enabled && plan.vm_console_enabled &&
        !plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 0,
        SOFTPC_RUNTIME_RUNNING, 1, 1);
    assert(plan.window_enabled && plan.vm_console_enabled &&
        !plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 1,
        SOFTPC_RUNTIME_RUNNING, 1, 1);
    assert(plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_WINDOW, 0,
        SOFTPC_RUNTIME_RUNNING, 0, 0);
    assert(plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_WINDOW, 1,
        SOFTPC_RUNTIME_PAUSED, 1, 1);
    assert(plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    /* Static Window ignores console_control for both guest frame routes. */
    plan = app_presentation_derive(SOFTPC_PRESENTATION_WINDOW, 0,
        SOFTPC_RUNTIME_RUNNING, 1, 1);
    assert(plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_WINDOW, 1,
        SOFTPC_RUNTIME_RUNNING, 1, 0);
    assert(plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    /* Console display uses its raw VM Console only for text or explicit
       graphical console-control=0. */
    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 0,
        SOFTPC_RUNTIME_RUNNING, 1, 0);
    assert(!plan.window_enabled && plan.vm_console_enabled &&
        !plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 1,
        SOFTPC_RUNTIME_RUNNING, 1, 1);
    assert(plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    /* Paused/stopped never retain raw VM input. */
    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 0,
        SOFTPC_RUNTIME_PAUSED, 1, 0);
    assert(!plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_CONSOLE, 0,
        SOFTPC_RUNTIME_PAUSED, 1, 1);
    assert(plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    plan = app_presentation_derive(SOFTPC_PRESENTATION_WINDOW, 1,
        SOFTPC_RUNTIME_STOPPED, 1, 1);
    assert(!plan.window_enabled && !plan.vm_console_enabled &&
        plan.monitor_console_enabled);
    return 0;
}
