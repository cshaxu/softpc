#include "app/command_binding.h"
#include "app/keyboard.h"

#include <stdio.h>
#include <string.h>

_Static_assert(COMMON_SESSION_TEXT_CAPACITY >= 2u * COMMON_DEBUG_TEXT_CAPACITY,
    "Session text must hold debugger output with CRLF expansion");
_Static_assert(COMMON_SESSION_PROMPT_CAPACITY >= COMMON_DEBUG_PROMPT_CAPACITY,
    "Session prompt must hold debugger continuation prompts");

/* app/ owns product command policy. common/session owns its neutral copied
 * completion facts; convert explicitly at this one composition boundary.
 * The numeric enum values are deliberately not a cross-component contract. */
static common_machine_state app_machine_completed_state(
    common_session_machine_state state)
{
    switch (state) {
    case COMMON_SESSION_MACHINE_RUNNING: return COMMON_MACHINE_RUNNING;
    case COMMON_SESSION_MACHINE_PAUSED: return COMMON_MACHINE_PAUSED;
    case COMMON_SESSION_MACHINE_ERROR: return COMMON_MACHINE_ERROR;
    case COMMON_SESSION_MACHINE_RESET_COMPLETED:
        return COMMON_MACHINE_RESET_COMPLETED;
    default: return COMMON_MACHINE_STOPPED;
    }
}

static app_monitor_state app_command_state(common_session_machine_state state)
{
    switch (state) {
    case COMMON_SESSION_MACHINE_INIT: return APP_MONITOR_INIT;
    case COMMON_SESSION_MACHINE_RUNNING: return APP_MONITOR_RUNNING;
    case COMMON_SESSION_MACHINE_PAUSED: return APP_MONITOR_PAUSED;
    default: return APP_MONITOR_STOPPED;
    }
}

static common_session_request app_session_request(app_lifecycle_request request)
{
    switch (request) {
    case APP_LIFECYCLE_REQUEST_START: return COMMON_SESSION_REQUEST_START;
    case APP_LIFECYCLE_REQUEST_RESUME: return COMMON_SESSION_REQUEST_RESUME;
    case APP_LIFECYCLE_REQUEST_PAUSE: return COMMON_SESSION_REQUEST_PAUSE;
    case APP_LIFECYCLE_REQUEST_STOP: return COMMON_SESSION_REQUEST_STOP;
    case APP_LIFECYCLE_REQUEST_RESET: return COMMON_SESSION_REQUEST_RESET;
    default: return COMMON_SESSION_REQUEST_NONE;
    }
}

static app_lifecycle_request app_lifecycle_request_from_session(
    common_session_request request)
{
    switch (request) {
    case COMMON_SESSION_REQUEST_START: return APP_LIFECYCLE_REQUEST_START;
    case COMMON_SESSION_REQUEST_RESUME: return APP_LIFECYCLE_REQUEST_RESUME;
    case COMMON_SESSION_REQUEST_PAUSE: return APP_LIFECYCLE_REQUEST_PAUSE;
    case COMMON_SESSION_REQUEST_STOP: return APP_LIFECYCLE_REQUEST_STOP;
    case COMMON_SESSION_REQUEST_RESET: return APP_LIFECYCLE_REQUEST_RESET;
    default: return APP_LIFECYCLE_REQUEST_NONE;
    }
}

static void app_command_copy_effect(common_session_command_result *out,
    const app_command_effect *effect)
{
    if (out == NULL || effect == NULL) return;
    *out = (common_session_command_result) { 0 };
    (void)snprintf(out->text, sizeof(out->text), "%s", effect->text);
    out->exit_requested = effect->exit_requested != 0;
    out->arm_prompt = effect->arm_prompt != 0;
}

static void app_command_provider_open(void *opaque,
    common_session_command_result *out)
{
    app_command_binding *binding = (app_command_binding *)opaque;
    app_command_effect effect = { 0 };
    app_command_session_open(&binding->session, &effect);
    app_command_copy_effect(out, &effect);
}

static void app_command_provider_reject_line(void *opaque,
    common_session_command_result *out)
{
    app_command_binding *binding = (app_command_binding *)opaque;
    app_command_effect effect = { 0 };
    app_command_session_reject_line(&binding->session, &effect);
    app_command_copy_effect(out, &effect);
}

static void app_command_copy_debug(app_command_binding *binding,
    common_session_machine_state state, const common_debug_result *result,
    common_session_command_result *out)
{
    size_t source, target = strlen(out->text);
    if (target + 2u * strlen(result->text) >= sizeof(out->text)) {
        (void)snprintf(out->text, sizeof(out->text), "Debug output exceeds Console capacity.\r\n\r\n");
        common_machine_debug_cancel(binding->machine);
        return;
    }
    for (source = 0u; result->text[source] != '\0'; ++source) {
        if (result->text[source] == '\n' &&
            (source == 0u || result->text[source - 1u] != '\r')) out->text[target++] = '\r';
        out->text[target++] = result->text[source];
    }
    out->text[target] = '\0';
    (void)snprintf(binding->debug_prompt, sizeof(binding->debug_prompt), "%s", result->prompt);
    if (!result->keep_active) {
        common_debug_close(binding->debug);
        binding->debug_active = LIB_FALSE;
        binding->debug_completed_pending = LIB_FALSE;
    }
    if (result->lifecycle_request == COMMON_DEBUG_LIFECYCLE_RESUME &&
        app_command_session_begin_external(&binding->session,
            app_command_state(state), APP_LIFECYCLE_REQUEST_RESUME))
        out->request = COMMON_SESSION_REQUEST_RESUME;
    else if (result->lifecycle_request != COMMON_DEBUG_LIFECYCLE_NONE)
        (void)snprintf(out->text, sizeof(out->text), "Debug lifecycle request is not applicable.\r\n\r\n");
}

static void app_command_provider_submit_line(void *opaque,
    common_session_machine_state state, const char *line,
    common_session_command_result *out)
{
    app_command_binding *binding = (app_command_binding *)opaque;
    app_command_effect effect = { 0 };
    if (binding->debug_active) {
        common_debug_result result = { 0 };
        lib_status status = common_debug_submit_line(binding->debug, line, &result);
        *out = (common_session_command_result) { 0 };
        if (status != LIB_STATUS_OK) {
            (void)snprintf(out->text, sizeof(out->text), "Debug command failed.\r\n\r\n");
        } else {
            app_command_copy_debug(binding, state, &result, out);
        }
        binding->session.prompt_due = out->request == COMMON_SESSION_REQUEST_NONE;
        return;
    }
    app_command_session_submit_line(&binding->session, app_command_state(state), line,
        &effect);
    if (effect.action == APP_COMMAND_ACTION_DEBUG) {
        if (common_debug_open(binding->debug, binding->machine) == LIB_STATUS_OK) {
            binding->debug_active = LIB_TRUE;
            (void)snprintf(binding->debug_prompt, sizeof(binding->debug_prompt), "-");
            (void)snprintf(effect.text, sizeof(effect.text),
                "Debugger: ? for help, q to return. Machine access requires pause.\r\n\r\n");
        } else (void)snprintf(effect.text, sizeof(effect.text), "Cannot open debugger.\r\n\r\n");
    } else if (effect.action != APP_COMMAND_ACTION_NONE) {
        int succeeded = effect.action == APP_COMMAND_ACTION_EJECT_FLOPPY ?
            common_machine_set_removable_media(binding->machine, NULL) :
            common_machine_set_removable_media(binding->machine, effect.path);
        app_command_session_complete_floppy(&binding->session, effect.action,
            succeeded, &effect);
    }
    app_command_copy_effect(out, &effect);
    out->request = app_session_request(
        app_command_session_take_request(&binding->session));
}

static lib_bool app_command_provider_begin_external(void *opaque,
    common_session_machine_state state, common_session_request request)
{
    app_command_binding *binding = (app_command_binding *)opaque;
    return app_command_session_begin_external(&binding->session,
        app_command_state(state), app_lifecycle_request_from_session(request)) != 0;
}

static void app_command_provider_note_runtime(void *opaque,
    common_session_machine_state prior, common_session_machine_state completed,
    common_session_command_result *out)
{
    app_command_binding *binding = (app_command_binding *)opaque;
    app_command_effect effect = { 0 };
    app_command_session_note_runtime(&binding->session, app_command_state(prior),
        app_machine_completed_state(completed), &effect);
    app_command_copy_effect(out, &effect);
    if (binding->debug_active) {
        if (completed != COMMON_SESSION_MACHINE_PAUSED)
            binding->debug_completed_pending = LIB_FALSE;
        common_debug_machine_state state = completed == COMMON_SESSION_MACHINE_PAUSED ?
            COMMON_DEBUG_MACHINE_PAUSED : completed == COMMON_SESSION_MACHINE_RUNNING ?
            COMMON_DEBUG_MACHINE_RUNNING : COMMON_DEBUG_MACHINE_STOPPED;
        common_debug_result result = { 0 };
        (void)common_debug_observe_machine(binding->debug, state, LIB_STATUS_OK, &result);
        if (result.prompt_ready) {
            binding->debug_completed = result;
            binding->debug_completed_pending = LIB_TRUE;
        }
    }
}

static void app_command_provider_note_broker(void *opaque,
    common_session_machine_state state, lib_bool vm_console_current,
    lib_bool monitor_running_surface)
{
    app_command_binding *binding = (app_command_binding *)opaque;
    app_command_session_note_broker(&binding->session, app_command_state(state),
        vm_console_current != 0, monitor_running_surface != 0);
}

static void app_command_provider_note_monitor_current(void *opaque,
    lib_bool current, common_session_command_result *out)
{
    app_command_binding *binding = (app_command_binding *)opaque;
    app_command_effect effect = { 0 };
    app_command_session_note_monitor_current(&binding->session, current != 0,
        &effect);
    app_command_copy_effect(out, &effect);
    if (current && binding->debug_completed_pending) {
        binding->debug_completed_pending = LIB_FALSE;
        app_command_copy_debug(binding, COMMON_SESSION_MACHINE_PAUSED,
            &binding->debug_completed, out);
        out->arm_prompt = out->request == COMMON_SESSION_REQUEST_NONE;
    }
    (void)snprintf(out->prompt, sizeof(out->prompt), "%s",
        binding->debug_active ? binding->debug_prompt : "SoftPC> ");
}

static lib_bool app_command_provider_handle_hotkey(void *opaque,
    common_session_machine_state state, const char *identifier,
    common_session_command_result *out)
{
    app_command_binding *binding = (app_command_binding *)opaque;
    common_session_request request = COMMON_SESSION_REQUEST_NONE;
    *out = (common_session_command_result) { 0 };
    if (identifier == NULL) return LIB_FALSE;
    if (strcmp(identifier, "pause-toggle") == 0) {
        request = state == COMMON_SESSION_MACHINE_PAUSED ?
            COMMON_SESSION_REQUEST_RESUME : COMMON_SESSION_REQUEST_PAUSE;
        if (!app_command_provider_begin_external(binding, state, request))
            return LIB_TRUE;
        out->request = request;
        return LIB_TRUE;
    }
    if (strcmp(identifier, "release-window-mouse") == 0) {
        out->release_window_mouse = LIB_TRUE;
        return LIB_TRUE;
    }
    if (state != COMMON_SESSION_MACHINE_RUNNING) return LIB_TRUE;
    if (strcmp(identifier, "send-ctrl-alt-del") == 0)
        return app_keyboard_submit_ctrl_alt_del(binding->machine,
            app_keyboard_deliver_input) != 0;
    if (strcmp(identifier, "send-alt-enter") == 0)
        return app_keyboard_submit_alt_enter(binding->machine,
            app_keyboard_deliver_input) != 0;
    return LIB_TRUE;
}


lib_status app_command_binding_initialize(app_command_binding *binding,
    common_machine *machine, common_session_display display,
    common_session_command_provider *provider)
{
    if (binding == NULL || machine == NULL || provider == NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    memset(binding, 0, sizeof(*binding));
    binding->machine = machine;
    app_command_session_initialize(&binding->session, display);
    *provider = (common_session_command_provider) {
        .context = binding,
        .open = app_command_provider_open,
        .reject_line = app_command_provider_reject_line,
        .submit_line = app_command_provider_submit_line,
        .begin_external = app_command_provider_begin_external,
        .note_runtime = app_command_provider_note_runtime,
        .note_broker = app_command_provider_note_broker,
        .note_monitor_current = app_command_provider_note_monitor_current,
        .handle_hotkey = app_command_provider_handle_hotkey
    };
    return common_debug_create(&binding->debug);
}

void app_command_binding_dispose(app_command_binding *binding)
{
    if (binding != NULL) common_debug_destroy(binding->debug);
}
