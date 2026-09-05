#include "presenter.h"

int win32_presentation_binding_valid(
    const win32_presentation_binding *binding)
{
    return binding != 0 && binding->mailbox != 0 && binding->router != 0 &&
        binding->actions != 0 && binding->input_sink != 0 &&
        binding->get_state != 0 && binding->handle_action != 0 &&
        binding->handle_close != 0;
}
