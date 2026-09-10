#include "presentation.h"
#include "monitor.h"
#include "command.h"
#include "control_state.h"
#include "runtime.h"
#include "machine.h"
#include "prompt_trace.h"
#include "lib/storage/file_interface.h"

#include <windows.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOFTPC_CONFIG_PATH_MAX 1024u

typedef struct app_startup_config {
    char floppy_path[SOFTPC_CONFIG_PATH_MAX];
    char hard_disk_path[SOFTPC_CONFIG_PATH_MAX];
    char serial_output_path[SOFTPC_CONFIG_PATH_MAX];
    char printer_output_path[SOFTPC_CONFIG_PATH_MAX];
    uint32_t memory_bytes;
    softpc_presentation presentation;
    int console_control;
    softpc_media_mode media_mode;
} app_startup_config;

typedef struct app_monitor_control {
    app_control_state state;
    app_runtime_frame frame;
} app_monitor_control;

static char *app_trim(char *text)
{
    char *end;
    while (*text != '\0' && isspace((unsigned char)*text)) ++text;
    end = text + strlen(text);
    while (end != text && isspace((unsigned char)end[-1])) --end;
    *end = '\0';
    if (*text == '"' && end > text + 1 && end[-1] == '"') {
        end[-1] = '\0';
        ++text;
    }
    return text;
}

static int app_copy_value(char *target, const char *value)
{
    size_t length = strlen(value);
    if (length >= SOFTPC_CONFIG_PATH_MAX) return 0;
    memcpy(target, value, length + 1u);
    return 1;
}

static int app_get_config_path(char *path)
{
    DWORD length = GetModuleFileNameA(NULL, path, SOFTPC_CONFIG_PATH_MAX);
    char *separator;
    char *forward_separator;

    if (length == 0u || length >= SOFTPC_CONFIG_PATH_MAX) return 0;
    separator = strrchr(path, '\\');
    forward_separator = strrchr(path, '/');
    if (forward_separator != NULL &&
        (separator == NULL || forward_separator > separator))
        separator = forward_separator;
    if (separator == NULL) return 0;
    if ((size_t)(separator - path) + sizeof("softpc.ini") >=
        SOFTPC_CONFIG_PATH_MAX)
        return 0;
    memcpy(separator + 1, "softpc.ini", sizeof("softpc.ini"));
    return 1;
}

static int app_path_is_absolute(const char *path)
{
    return path[0] == '/' || path[0] == '\\' ||
        (isalpha((unsigned char)path[0]) && path[1] == ':' &&
            (path[2] == '/' || path[2] == '\\'));
}

static int app_resolve_image_path(char *path, const char *config_path)
{
    const char *separator;
    const char *forward_separator;
    char resolved[SOFTPC_CONFIG_PATH_MAX];
    size_t directory_length;
    size_t image_length;

    if (path[0] == '\0' || app_path_is_absolute(path)) return 1;
    separator = strrchr(config_path, '\\');
    forward_separator = strrchr(config_path, '/');
    if (forward_separator != NULL &&
        (separator == NULL || forward_separator > separator))
        separator = forward_separator;
    if (separator == NULL) return 0;
    directory_length = (size_t)(separator - config_path) + 1u;
    image_length = strlen(path);
    if (directory_length + image_length >= sizeof(resolved)) return 0;
    memcpy(resolved, config_path, directory_length);
    memcpy(resolved + directory_length, path, image_length + 1u);
    return app_copy_value(path, resolved);
}

static int app_load_startup_config(const char *path,
    app_startup_config *config)
{
    void *owned = NULL;
    size_t byte_count;
    char *contents;
    char *line;
    if (lib_storage_file_read_owned(path, 64u * 1024u, &owned, &byte_count) !=
        LIB_STATUS_OK) return 0;
    contents = malloc(byte_count + 1u);
    if (contents == NULL) {
        free(owned);
        return 0;
    }
    memcpy(contents, owned, byte_count);
    contents[byte_count] = '\0';
    free(owned);
    line = contents;
    while (line != NULL && *line != '\0') {
        char *next = strpbrk(line, "\r\n");
        char *key;
        char *value;
        char *equals;
        char *comment;
        char *semicolon;
        if (next != NULL) {
            *next++ = '\0';
            while (*next == '\r' || *next == '\n') ++next;
        }
        /* Delimit the current record before scanning it.  Scanning the
           unsplit buffer lets a leading comment consume an '=' from a later
           setting and silently discard that setting. */
        equals = strchr(line, '=');
        comment = strchr(line, '#');
        semicolon = strchr(line, ';');
        if (semicolon != NULL && (comment == NULL || semicolon < comment))
            comment = semicolon;
        if (comment != NULL) *comment = '\0';
        if (equals == NULL) {
            line = next;
            continue;
        }
        *equals = '\0';
        key = app_trim(line);
        value = app_trim(equals + 1);
        if (*key == '\0') {
            line = next;
            continue;
        }
        if (strcmp(key, "memory_mb") == 0) {
            char *end;
            unsigned long mib = strtoul(value, &end, 10);
            if (*end != '\0' || mib == 0u || mib > 4095u) goto invalid;
            config->memory_bytes = (uint32_t)(mib * 1024u * 1024u);
        } else if (strcmp(key, "floppy") == 0) {
            if (!app_copy_value(config->floppy_path, value)) goto invalid;
        } else if (strcmp(key, "hard_disk") == 0) {
            if (!app_copy_value(config->hard_disk_path, value)) goto invalid;
        } else if (strcmp(key, "serial_output") == 0) {
            if (!app_copy_value(config->serial_output_path, value)) goto invalid;
        } else if (strcmp(key, "printer_output") == 0) {
            if (!app_copy_value(config->printer_output_path, value)) goto invalid;
        } else if (strcmp(key, "display") == 0) {
            if (strcmp(value, "console") == 0)
                config->presentation = SOFTPC_PRESENTATION_CONSOLE;
            else if (strcmp(value, "window") == 0)
                config->presentation = SOFTPC_PRESENTATION_WINDOW;
            else goto invalid;
        } else if (strcmp(key, "console_control") == 0) {
            if (strcmp(value, "0") == 0) config->console_control = 0;
            else if (strcmp(value, "1") == 0) config->console_control = 1;
            else goto invalid;
        } else if (strcmp(key, "media_mode") == 0) {
            if (strcmp(value, "readonly") == 0)
                config->media_mode = SOFTPC_MEDIA_READONLY;
            else if (strcmp(value, "direct") == 0)
                config->media_mode = SOFTPC_MEDIA_DIRECT;
            else if (strcmp(value, "overlay") == 0)
                config->media_mode = SOFTPC_MEDIA_OVERLAY;
            else goto invalid;
        } else goto invalid;
        line = next;
    }
    free(contents);
    return 1;
invalid:
    free(contents);
    return 0;
}

static int app_monitor_drive(app_runtime *runtime, app_presentation *presentation,
    app_monitor_control *control)
{
    app_reconciler_action action;
    int console_status_surface;
    app_control_state *state;
    if (presentation == NULL || control == NULL) return 0;
    state = &control->state;
    action = app_control_state_take_action(state);
    if (action != APP_RECONCILER_ACTION_NONE &&
        !app_presentation_apply_action(presentation, action,
            state->presentation.runtime_actual)) return 0;
    if (state->observed_frame_sequence == 0u ||
        !app_control_state_frame_targets_ready(state)) return 1;
    console_status_surface = state->presentation.display ==
        SOFTPC_PRESENTATION_CONSOLE && !state->presentation.console_control &&
        control->frame.graphics != 0u;
    return app_presentation_publish_frame(presentation, &control->frame,
        state->presentation.window_actual,
        state->presentation.vm_console_actual &&
            state->presentation.current_console_actual == APP_RECONCILER_CONSOLE_VM,
        console_status_surface);
}

/* The control loop is the only caller that translates a parsed lifecycle
 * request into runtime work.  Presentation reconciliation is deliberately
 * separate and cannot manufacture or consume this request. */
static int app_monitor_dispatch_lifecycle(app_runtime *runtime,
    app_lifecycle_request request)
{
    switch (request) {
    case APP_LIFECYCLE_REQUEST_NONE: return 1;
    case APP_LIFECYCLE_REQUEST_START: return app_runtime_start(runtime);
    case APP_LIFECYCLE_REQUEST_PAUSE: return app_runtime_pause(runtime);
    case APP_LIFECYCLE_REQUEST_RESUME: return app_runtime_resume(runtime);
    case APP_LIFECYCLE_REQUEST_STOP: return app_runtime_stop(runtime);
    case APP_LIFECYCLE_REQUEST_RESET: return app_runtime_reset(runtime);
    }
    return 0;
}

static int app_monitor_arm_if_ready(app_command_session *session,
    const app_control_state *state, app_monitor_console *monitor)
{
    app_command_effect effect = { 0 };
    app_command_session_note_monitor_current(session,
        app_control_state_monitor_is_current(state), &effect);
    if (!effect.arm_prompt) return 1;
    return (effect.text[0] == '\0' ||
        app_monitor_console_write(monitor, effect.text)) &&
        app_monitor_console_write(monitor, "SoftPC> ") &&
        app_monitor_console_request_line(monitor);
}

static void app_runtime_state_completed(void *opaque, app_runtime_state state,
    uint32_t run_generation)
{
    app_control_queue *queue = (app_control_queue *)opaque;
    if (queue == NULL) return;
    (void)app_control_queue_push_runtime_completed(queue, state, run_generation);
}

static void app_runtime_frame_published(void *opaque, uint32_t sequence,
    int graphics, uint32_t run_generation)
{
    app_control_queue *queue = (app_control_queue *)opaque;
    if (queue == NULL) return;
    (void)app_control_queue_push_frame_completed(queue, sequence, graphics,
        run_generation);
}

/* Product hotkey interpretation lives with the control/reconciler.  The UX
 * leaf has already converted a matching chord into a copied identifier; the
 * control path alone decides its lifecycle effect and preserves the resume
 * ordering required by the Console-object contract. */
static int app_monitor_handle_ux(app_control_queue *queue, app_runtime *runtime,
    app_presentation *presentation, app_command_session *session,
    app_monitor_control *control,
    const ux_input_event *event)
{
    app_control_state *state = control == NULL ? NULL : &control->state;
    if (state == NULL) return 0;
    if (event != NULL && event->type == UX_EVENT_WINDOW_CLOSE) {
        if (state->monitor_actual == APP_MONITOR_RUNNING &&
            !app_command_session_begin_external(session,
                state->monitor_actual, APP_LIFECYCLE_REQUEST_PAUSE)) return 0;
        app_control_state_note_window_close(state);
        return app_runtime_get_state(runtime) != SOFTPC_RUNTIME_RUNNING ||
            app_runtime_pause(runtime);
    }
    if (event != NULL && event->type == UX_EVENT_HOTKEY &&
        strcmp(event->data.hotkey.identifier, "pause-toggle") == 0) {
        app_lifecycle_request request =
            state->monitor_actual == APP_MONITOR_PAUSED ?
                APP_LIFECYCLE_REQUEST_RESUME : APP_LIFECYCLE_REQUEST_PAUSE;
        if (!app_command_session_begin_external(session, state->monitor_actual,
                request)) return 1;
        return request == APP_LIFECYCLE_REQUEST_RESUME ?
            app_runtime_resume(runtime) : app_runtime_pause(runtime);
    }
    if (event != NULL && event->type == UX_EVENT_HOTKEY &&
        strcmp(event->data.hotkey.identifier, "release-window-mouse") == 0) {
        app_presentation_release_window_mouse(presentation);
        return 1;
    }
    return app_control_handle_ux(queue, runtime, event,
        state->monitor_actual == APP_MONITOR_RUNNING ? SOFTPC_RUNTIME_RUNNING :
        state->monitor_actual == APP_MONITOR_PAUSED ? SOFTPC_RUNTIME_PAUSED :
        SOFTPC_RUNTIME_STOPPED);
}

static int app_monitor(app_runtime *runtime, softpc_presentation presentation,
    int console_control, app_monitor_console *monitor,
    app_control_queue *control_queue)
{
    char line[SOFTPC_CONFIG_PATH_MAX + 64u];
    app_command_session session;
    app_command_effect command_effect;
    app_presentation *presenter = NULL;
    app_monitor_control control = { 0 };
    app_control_state *state = &control.state;
    if (!app_presentation_create(&presenter, runtime, monitor, control_queue))
        return 1;
    app_control_state_initialize(state, presentation, console_control);
    app_command_session_initialize(&session, presentation);
    app_command_session_open(&session, &command_effect);
    if (command_effect.text[0] != '\0')
        (void)app_monitor_console_write(monitor, command_effect.text);
    if (!app_monitor_arm_if_ready(&session, state, monitor)) goto failed;
    for (;;) {
        for (;;) {
            app_control_event control_event;
            int broker_monitor_completed = 0;
            if (app_control_queue_take(control_queue, &control_event, 100u)) {
                if (control_event.kind == APP_CONTROL_UX_INPUT) {
                    /* A component can have queued an event just before an
                     * old VM run stopped. It cannot affect a later run, nor
                     * enter the already-stopped machine path. */
                    if (!app_control_accept_ux_event(&control_event,
                            app_runtime_run_generation(runtime),
                            state->monitor_actual == APP_MONITOR_RUNNING ?
                                SOFTPC_RUNTIME_RUNNING :
                            state->monitor_actual == APP_MONITOR_PAUSED ?
                                SOFTPC_RUNTIME_PAUSED : SOFTPC_RUNTIME_STOPPED))
                        continue;
                    if (!app_monitor_handle_ux(control_queue, runtime, presenter,
                            &session, &control, &control_event.value.ux))
                        goto failed;
                    if (!app_monitor_drive(runtime, presenter, &control)) goto failed;
                    if (!app_monitor_arm_if_ready(&session, state, monitor)) goto failed;
                    continue;
                }
                if (control_event.kind == APP_CONTROL_MONITOR_LINE) {
                    if (control_event.value.line.length >= sizeof(line)) return 1;
                    memcpy(line, control_event.value.line.text,
                        control_event.value.line.length);
                    line[control_event.value.line.length] = '\0';
                    break;
                }
                if (control_event.kind == APP_CONTROL_UX_DELIVERY_FAILED) {
                    app_monitor_console_write(monitor,
                        "UX input delivery failed.\r\n");
                    goto failed;
                }
                if (control_event.kind == APP_CONTROL_QUEUE_DELIVERY_FAILED) {
                    app_monitor_console_write(monitor,
                        "Control queue delivery failed.\r\n");
                    goto failed;
                }
                if (control_event.run_generation != 0u &&
                    control_event.run_generation != app_runtime_run_generation(runtime))
                    continue;
                if (control_event.kind == APP_CONTROL_RUNTIME_COMPLETED)
                {
                    app_runtime_state completed = control_event.value.runtime_state;
                    app_command_session_note_runtime(&session,
                        state->monitor_actual, completed, &command_effect);
                    app_control_state_note_runtime(state, completed);
                    app_presentation_set_runtime_state(presenter,
                        completed == SOFTPC_RUNTIME_RESET_COMPLETED ?
                            SOFTPC_RUNTIME_PAUSED : completed);
                }
                else if (control_event.kind == APP_CONTROL_FRAME_COMPLETED) {
                    uint32_t frame_run;
                    uint32_t sequence = control_event.value.frame.sequence;
                    if (sequence > state->observed_frame_sequence &&
                        app_runtime_copy_published_frame(runtime, &control.frame,
                            &frame_run) && control.frame.sequence == sequence &&
                        frame_run == control_event.run_generation) {
                        (void)app_control_state_note_frame(state, sequence,
                            control_event.value.frame.graphics);
                    }
                }
                else if (control_event.kind == APP_CONTROL_COMPONENT_COMPLETED) {
                    if (control_event.value.component.component ==
                        APP_CONTROL_COMPONENT_WINDOW)
                        app_control_state_note_window(state,
                            control_event.value.component.exists);
                    else app_control_state_note_vm_console(state,
                        control_event.value.component.exists);
                }
                else if (control_event.kind == APP_CONTROL_BROKER_COMPLETED) {
                    int vm_console_current =
                        control_event.value.broker_vm_console_current;
                    app_control_state_note_current_console(state,
                        vm_console_current);
                    broker_monitor_completed = !vm_console_current;
                }
                if (!app_monitor_drive(runtime, presenter, &control)) goto failed;
                /* A monitor completion can be stale with respect to a new
                 * Console-mode VM binding.  Only the reconciled actual owner
                 * may make its prompt due. */
                if (broker_monitor_completed &&
                    app_control_state_monitor_is_current(state))
                    app_command_session_note_broker(&session,
                        state->monitor_actual, 0,
                        app_control_state_monitor_is_running_graphics_surface(state));
                if (!app_monitor_arm_if_ready(&session, state, monitor)) goto failed;
                continue;
            }
        }
        if (!app_monitor_drive(runtime, presenter, &control)) goto failed;
        app_command_session_submit_line(&session, state->monitor_actual, line,
            &command_effect);
        if (command_effect.exit_requested) {
            (void)app_runtime_stop(runtime);
            (void)app_monitor_drive(runtime, presenter, &control);
            app_presentation_destroy(presenter);
            return 0;
        }
        if (command_effect.action != APP_COMMAND_ACTION_NONE) {
            int succeeded = command_effect.action == APP_COMMAND_ACTION_EJECT_FLOPPY ?
                app_runtime_set_floppy(runtime, NULL) :
                app_runtime_set_floppy(runtime, command_effect.path);
            app_command_session_complete_floppy(&session, command_effect.action,
                succeeded, &command_effect);
        }
        if (command_effect.text[0] != '\0')
            (void)app_monitor_console_write(monitor, command_effect.text);
        if (!app_monitor_dispatch_lifecycle(runtime,
                app_command_session_take_request(&session))) goto failed;
        if (!app_monitor_drive(runtime, presenter, &control)) goto failed;
        if (!app_monitor_arm_if_ready(&session, state, monitor)) goto failed;
    }
failed:
    app_presentation_destroy(presenter);
    return 1;
}

int main(int argc, char **argv)
{
    char config_path[SOFTPC_CONFIG_PATH_MAX];
    app_startup_config config = { { 0 }, { 0 }, { 0 }, { 0 }, 16u * 1024u * 1024u,
        SOFTPC_PRESENTATION_CONSOLE, 1, SOFTPC_MEDIA_OVERLAY };
    softpc_machine_options options = { 0 };
    softpc_machine *machine = NULL;
    app_runtime *runtime = NULL;
    app_monitor_console *monitor = NULL;
    app_control_queue *control_queue = NULL;
    softpc_machine_result result;
    (void)argv;

    app_prompt_trace_reset();

    if (argc != 1) {
        fprintf(stderr, "softpcvm: command-line arguments are not supported\n");
        return 2;
    }
    if (!app_get_config_path(config_path)) {
        fprintf(stderr, "softpcvm: cannot determine adjacent softpc.ini path\n");
        return 1;
    }
    if (!app_load_startup_config(config_path, &config)) {
        fprintf(stderr, "softpcvm: cannot read fixed-machine config '%s'\n",
            config_path);
        return 1;
    }
    if (!app_resolve_image_path(config.floppy_path, config_path) ||
        !app_resolve_image_path(config.hard_disk_path, config_path) ||
        !app_resolve_image_path(config.serial_output_path, config_path) ||
        !app_resolve_image_path(config.printer_output_path, config_path)) {
        fprintf(stderr, "softpcvm: path in '%s' is too long\n", config_path);
        return 1;
    }
    options.floppy_path = config.floppy_path[0] == '\0' ? NULL : config.floppy_path;
    options.hard_disk_path = config.hard_disk_path[0] == '\0' ? NULL : config.hard_disk_path;
    options.memory_bytes = config.memory_bytes;
    options.presentation = config.presentation;
    options.media_mode = config.media_mode;
    options.serial_output_path = config.serial_output_path[0] == '\0' ? NULL :
        config.serial_output_path;
    options.printer_output_path = config.printer_output_path[0] == '\0' ? NULL :
        config.printer_output_path;
    result = softpc_machine_create(&options, &machine);
    if (result != SOFTPC_MACHINE_OK) goto done;
    if (!app_runtime_create(machine, &runtime)) {
        result = SOFTPC_MACHINE_IO_ERROR;
        goto done;
    }
    if (!app_control_queue_create(&control_queue) ||
        !app_monitor_console_create(&monitor, control_queue)) {
        result = SOFTPC_MACHINE_IO_ERROR;
        goto done;
    }
    app_runtime_set_state_sink(runtime, app_runtime_state_completed, control_queue);
    app_runtime_set_frame_sink(runtime, app_runtime_frame_published, control_queue);
    if (app_monitor(runtime, options.presentation, config.console_control,
            monitor, control_queue) != 0)
        result = SOFTPC_MACHINE_IO_ERROR;
done:
    if (result != SOFTPC_MACHINE_OK)
        fprintf(stderr, "softpcvm: %s\n", softpc_machine_result_name(result));
    app_monitor_console_destroy(monitor);
    app_control_queue_destroy(control_queue);
    app_runtime_destroy(runtime);
    softpc_machine_destroy(machine);
    return result != SOFTPC_MACHINE_OK;
}
