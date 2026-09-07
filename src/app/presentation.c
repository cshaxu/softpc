#include "presentation.h"

#ifdef _WIN32
#include "keyboard.h"
#include "lib/ux/win32/runner.h"

#include <string.h>

enum { APP_ACTION_PAUSE_TOGGLE = 1u, APP_ACTION_SEND_CTRL_ALT_DEL = 2u,
    APP_ACTION_SEND_ALT_ENTER = 3u, APP_ACTION_RELEASE_MOUSE = 4u };

static ux_run_state app_presentation_state(void *context)
{
    app_runtime_state state = app_runtime_get_state((app_runtime *)context);
    if (state == SOFTPC_RUNTIME_RUNNING) return UX_RUN_RUNNING;
    if (state == SOFTPC_RUNTIME_PAUSED) return UX_RUN_PAUSED;
    if (state == SOFTPC_RUNTIME_ERROR) return UX_RUN_ERROR;
    return UX_RUN_STOPPED;
}

static int app_presentation_release_inputs(void *context, ux_event_sink sink)
{
    return app_keyboard_release_ctrl_alt(context, sink);
}

/* Product policy lives here, outside the reusable component: SoftPC's
 * registered actions map to original 8042 ingress or its one executor's
 * pause transition. */
static ux_run_result app_presentation_action(void *context,
    ux_action action, ux_event_sink sink)
{
    app_runtime *runtime = (app_runtime *)context;

    if (runtime == NULL || sink == NULL) return UX_RUN_ERROR_RESULT;
    switch (action) {
    case APP_ACTION_PAUSE_TOGGLE:
        (void)app_keyboard_release_ctrl_alt(runtime, sink);
        if (app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED)
            return app_runtime_resume(runtime) ? UX_RUN_CONTINUE : UX_RUN_ERROR_RESULT;
        return app_runtime_pause(runtime) ? UX_RUN_PAUSED_RESULT : UX_RUN_ERROR_RESULT;
    case APP_ACTION_SEND_CTRL_ALT_DEL:
        (void)app_keyboard_release_ctrl_alt(runtime, sink);
        return app_keyboard_submit_ctrl_alt_del(runtime, sink) ?
            UX_RUN_CONTINUE : UX_RUN_ERROR_RESULT;
    case APP_ACTION_SEND_ALT_ENTER:
        return app_keyboard_submit_alt_enter(runtime, sink) ?
            UX_RUN_CONTINUE : UX_RUN_ERROR_RESULT;
    case APP_ACTION_RELEASE_MOUSE:
        (void)app_keyboard_release_ctrl_alt(runtime, sink);
        return UX_RUN_CONTINUE;
    default:
        return UX_RUN_CONTINUE;
    }
}

/* Closing a presenter is a product lifecycle request, not the toggle hotkey:
 * if SoftPC is already paused, close must not resume it. */
static ux_run_result app_presentation_close(void *context, ux_event_sink sink)
{
    app_runtime *runtime = (app_runtime *)context;

    (void)sink;
    if (runtime == NULL) return UX_RUN_ERROR_RESULT;
    if (app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED)
        return UX_RUN_PAUSED_RESULT;
    return app_runtime_pause(runtime) ? UX_RUN_PAUSED_RESULT : UX_RUN_ERROR_RESULT;
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
    ux_action_registry *actions, ux_binding *binding)
{
    if (runtime == NULL || actions == NULL || binding == NULL ||
        !app_keyboard_register_actions(actions)) return 0;
    memset(binding, 0, sizeof(*binding));
    binding->context = runtime;
    binding->mailbox = app_runtime_presentation_mailbox(runtime);
    binding->router = app_runtime_presentation_router(runtime);
    binding->actions = actions;
    binding->input_sink = app_keyboard_deliver_input;
    binding->release_inputs = app_presentation_release_inputs;
    binding->get_state = app_presentation_state;
    binding->handle_action = app_presentation_action;
    binding->handle_close = app_presentation_close;
    binding->get_title = app_presentation_title;
    return ux_binding_validate(binding) == LIB_STATUS_OK;
}

int app_presentation_result(ux_run_result result)
{
    switch (result) {
    case UX_RUN_PAUSED_RESULT:
        return SOFTPC_VM_FRONTEND_PAUSED;
    case UX_RUN_STOPPED_RESULT:
        return SOFTPC_VM_FRONTEND_STOPPED;
    default:
        return SOFTPC_VM_FRONTEND_ERROR;
    }
}

int app_presentation_run(app_runtime *runtime,
    softpc_presentation presentation)
{
    ux_action_registry actions;
    ux_binding binding;

    app_runtime_set_presentation_mode(runtime, presentation);
    if (!app_presentation_binding(runtime, &actions, &binding))
        return SOFTPC_VM_FRONTEND_ERROR;
    return app_presentation_result(ux_win32_run(&binding));
}
#endif
