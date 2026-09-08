#include "presentation_plan.h"

app_presentation_plan app_presentation_derive(softpc_presentation display,
    int console_control, app_runtime_state state, int frame_available,
    int graphics)
{
    app_presentation_plan plan = { 0, 0, 1 };

    if (state == SOFTPC_RUNTIME_STOPPED || state == SOFTPC_RUNTIME_ERROR)
        return plan;
    /* Pause releases the raw VM Console before its component is retired, but
     * an already meaningful Window remains a paused view.  Static Window
     * display always retains it; Console display retains it only when the
     * last completed guest frame was graphical. */
    if (state == SOFTPC_RUNTIME_PAUSED) {
        plan.window_enabled = display == SOFTPC_PRESENTATION_WINDOW || graphics;
        return plan;
    }
    if (display == SOFTPC_PRESENTATION_WINDOW) {
        plan.window_enabled = 1;
        return plan;
    }
    /* A running Console-display machine remains on the cooked monitor until
     * it has actually committed its first guest frame.  In particular, do
     * not cancel a ReadConsole line merely because the VM announced RUNNING. */
    if (!frame_available)
        return plan;
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
