#include "presentation.h"

#ifdef _WIN32
#include "keyboard.h"

#include <string.h>

static win32_presentation_run_state app_presentation_state(void *context)
{
    app_runtime_state state = app_runtime_get_state((app_runtime *)context);
    if (state == SOFTPC_RUNTIME_RUNNING) return WIN32_PRESENTATION_RUN_RUNNING;
    if (state == SOFTPC_RUNTIME_PAUSED) return WIN32_PRESENTATION_RUN_PAUSED;
    if (state == SOFTPC_RUNTIME_ERROR) return WIN32_PRESENTATION_RUN_ERROR;
    return WIN32_PRESENTATION_RUN_STOPPED;
}

/* Product policy lives here, outside the reusable component: SoftPC's
 * registered actions map to original 8042 ingress or its one executor's
 * pause transition. */
static win32_presentation_run_result app_presentation_action(void *context,
    win32_presentation_action action, win32_presentation_event_sink sink)
{
    app_runtime *runtime = (app_runtime *)context;

    if (runtime == NULL || sink == NULL) return WIN32_PRESENTATION_RUN_ERROR_RESULT;
    switch (action) {
    case WIN32_PRESENTATION_ACTION_PAUSE_TOGGLE:
        (void)win32_presentation_keyboard_release_ctrl_alt(runtime, sink);
        if (app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED)
            return app_runtime_resume(runtime) ? WIN32_PRESENTATION_RUN_CONTINUE :
                WIN32_PRESENTATION_RUN_ERROR_RESULT;
        return app_runtime_pause(runtime) ? WIN32_PRESENTATION_RUN_PAUSED_RESULT :
            WIN32_PRESENTATION_RUN_ERROR_RESULT;
    case WIN32_PRESENTATION_ACTION_SEND_CTRL_ALT_DEL:
        (void)win32_presentation_keyboard_release_ctrl_alt(runtime, sink);
        return win32_presentation_keyboard_submit_ctrl_alt_del(runtime, sink) ?
            WIN32_PRESENTATION_RUN_CONTINUE : WIN32_PRESENTATION_RUN_ERROR_RESULT;
    case WIN32_PRESENTATION_ACTION_SEND_ALT_ENTER:
        return win32_presentation_keyboard_submit_alt_enter(runtime, sink) ?
            WIN32_PRESENTATION_RUN_CONTINUE : WIN32_PRESENTATION_RUN_ERROR_RESULT;
    case WIN32_PRESENTATION_ACTION_RELEASE_MOUSE:
        (void)win32_presentation_keyboard_release_ctrl_alt(runtime, sink);
        return WIN32_PRESENTATION_RUN_CONTINUE;
    default:
        return WIN32_PRESENTATION_RUN_CONTINUE;
    }
}

/* Closing a presenter is a product lifecycle request, not the toggle hotkey:
 * if SoftPC is already paused, close must not resume it. */
static win32_presentation_run_result app_presentation_close(void *context,
    win32_presentation_event_sink sink)
{
    app_runtime *runtime = (app_runtime *)context;

    (void)sink;
    if (runtime == NULL) return WIN32_PRESENTATION_RUN_ERROR_RESULT;
    if (app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED)
        return WIN32_PRESENTATION_RUN_PAUSED_RESULT;
    return app_runtime_pause(runtime) ? WIN32_PRESENTATION_RUN_PAUSED_RESULT :
        WIN32_PRESENTATION_RUN_ERROR_RESULT;
}

static void app_presentation_title(void *context, char *buffer,
    unsigned int buffer_size)
{
    app_runtime_state state;

    if (buffer == NULL || buffer_size == 0u) return;
    state = app_runtime_get_state((app_runtime *)context);
    (void)strncpy(buffer, state == SOFTPC_RUNTIME_PAUSED ?
        "Insignia SoftPC (Paused)" : "Insignia SoftPC (Running)",
        buffer_size - 1u);
    buffer[buffer_size - 1u] = '\0';
}

int app_presentation_binding(app_runtime *runtime,
    win32_presentation_router *router,
    win32_presentation_action_registry *actions,
    win32_presentation_binding *binding)
{
    if (runtime == NULL || router == NULL || actions == NULL || binding == NULL ||
        !app_keyboard_register_actions(actions)) return 0;
    memset(binding, 0, sizeof(*binding));
    binding->context = runtime;
    binding->mailbox = app_runtime_presentation_mailbox(runtime);
    binding->router = router;
    binding->actions = actions;
    binding->input_sink = app_keyboard_deliver_input;
    binding->get_state = app_presentation_state;
    binding->handle_action = app_presentation_action;
    binding->handle_close = app_presentation_close;
    binding->get_window_title = app_presentation_title;
    return win32_presentation_binding_valid(binding);
}

int app_presentation_result(win32_presentation_run_result result)
{
    switch (result) {
    case WIN32_PRESENTATION_RUN_PAUSED_RESULT:
        return SOFTPC_VM_FRONTEND_PAUSED;
    case WIN32_PRESENTATION_RUN_STOPPED_RESULT:
        return SOFTPC_VM_FRONTEND_STOPPED;
    default:
        return SOFTPC_VM_FRONTEND_ERROR;
    }
}

int app_presentation_run(app_runtime *runtime,
    softpc_presentation presentation)
{
    win32_presentation_router router;
    win32_presentation_action_registry actions;
    win32_presentation_binding binding;

    win32_presentation_router_init(&router,
        presentation == SOFTPC_PRESENTATION_WINDOW ?
        WIN32_PRESENTATION_DISPLAY_WINDOW : WIN32_PRESENTATION_DISPLAY_CONSOLE);
    if (!app_presentation_binding(runtime, &router, &actions, &binding))
        return SOFTPC_VM_FRONTEND_ERROR;
    return app_presentation_result(win32_presentation_run(&binding));
}
#endif
