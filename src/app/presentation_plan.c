#include "presentation_plan.h"

app_presentation_plan app_presentation_derive(softpc_presentation display,
    int console_control, app_runtime_state state, int graphics)
{
    app_presentation_plan plan = { 0, 0, 1 };

    if (state != SOFTPC_RUNTIME_RUNNING) return plan;
    if (display == SOFTPC_PRESENTATION_WINDOW) {
        plan.window_enabled = 1;
        return plan;
    }
    if (!graphics) {
        plan.vm_console_enabled = 1;
        plan.monitor_console_enabled = 0;
    } else if (!console_control) {
        plan.window_enabled = 1;
        plan.vm_console_enabled = 1;
        plan.monitor_console_enabled = 0;
    } else {
        plan.window_enabled = 1;
    }
    return plan;
}
