#include "presenter.h"

int win32_presentation_binding_valid(
    const win32_presentation_binding *binding)
{
    return binding != 0 && binding->mailbox != 0 && binding->router != 0 &&
        binding->actions != 0 && binding->input_sink != 0 &&
        binding->get_state != 0 && binding->handle_action != 0 &&
        binding->handle_close != 0;
}

#ifdef _WIN32
win32_presentation_run_result win32_presentation_run(
    const win32_presentation_binding *binding)
{
    win32_presentation_run_result result;

    if (!win32_presentation_binding_valid(binding))
        return WIN32_PRESENTATION_RUN_ERROR_RESULT;
    for (;;) {
        result = win32_presentation_router_target(binding->router) ==
            WIN32_PRESENTATION_TARGET_WINDOW ?
            win32_presentation_run_window(binding) :
            win32_presentation_run_console(binding);
        if (result != WIN32_PRESENTATION_RUN_SWITCH_WINDOW &&
            result != WIN32_PRESENTATION_RUN_SWITCH_CONSOLE)
            return result;
    }
}
#endif
