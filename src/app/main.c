#include "presentation.h"
#include "monitor.h"
#include "runtime.h"
#include "machine.h"
#include "prompt_trace.h"
#include "lib/storage/file.h"

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

typedef enum app_monitor_state {
    SOFTPC_MONITOR_STOPPED,
    SOFTPC_MONITOR_RUNNING,
    SOFTPC_MONITOR_PAUSED
} app_monitor_state;

static void app_monitor_help(app_monitor_console *monitor)
{
    app_monitor_console_write(monitor,
        "Insignia SoftPC\r\n===============\r\n"
        "  start                 cold-reset and run the machine\r\n"
        "  resume                continue a paused machine\r\n"
        "  pause                 request machine pause\r\n"
        "  stop                  stop execution\r\n"
        "  reset                 cold-reset and pause at firmware entry\r\n"
        "  floppy insert <image> insert drive A media while stopped/paused\r\n"
        "  floppy eject          eject drive A media while stopped/paused\r\n"
        "  help                  show this help\r\n"
        "  exit                  quit\r\n\r\n"
        "Raw VM Console hotkeys: Ctrl+Alt+P/D/F/M\r\n");
}

static int app_monitor_drive(app_runtime *runtime, app_presentation *presentation)
{
    app_reconciler_action action;
    if (!app_presentation_reconcile(presentation)) return 0;
    action = app_presentation_take_runtime_action(presentation);
    switch (action) {
    case APP_RECONCILER_ACTION_NONE: return 1;
    case APP_RECONCILER_ACTION_RUNTIME_START: return app_runtime_start(runtime);
    case APP_RECONCILER_ACTION_RUNTIME_PAUSE: return app_runtime_pause(runtime);
    case APP_RECONCILER_ACTION_RUNTIME_RESUME: return app_runtime_resume(runtime);
    case APP_RECONCILER_ACTION_RUNTIME_STOP: return app_runtime_stop(runtime);
    default: return 0;
    }
}

static void app_runtime_completed(void *opaque, app_runtime_state state,
    uint32_t sequence, int graphics, uint32_t run_generation)
{
    app_control_queue *queue = (app_control_queue *)opaque;
    if (queue == NULL) return;
    (void)app_control_queue_push_runtime_completed(queue, state, run_generation);
    if (sequence != 0u)
        (void)app_control_queue_push_frame_completed(queue, sequence, graphics,
            run_generation);
}

/* Product hotkey interpretation lives with the control/reconciler.  The UX
 * leaf has already converted a matching chord into a copied identifier; the
 * control path alone decides its lifecycle effect and preserves the resume
 * ordering required by the Console-object contract. */
static int app_monitor_handle_ux(app_control_queue *queue, app_runtime *runtime,
    app_presentation *presentation, const ux_input_event *event)
{
    if (event != NULL && event->type == UX_EVENT_WINDOW_CLOSE) {
        app_presentation_request_intent(presentation,
            APP_RECONCILER_INTENT_WINDOW_CLOSE);
        return 1;
    }
    if (event != NULL && event->type == UX_EVENT_HOTKEY &&
        strcmp(event->data.hotkey.identifier, "pause-toggle") == 0) {
        app_presentation_request_intent(presentation,
            app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED ?
                APP_RECONCILER_INTENT_RESUME : APP_RECONCILER_INTENT_PAUSE);
        return 1;
    }
    return app_control_handle_ux(queue, runtime, event);
}

static int app_monitor(app_runtime *runtime, softpc_presentation presentation,
    int console_control, app_monitor_console *monitor,
    app_control_queue *control_queue)
{
    char line[SOFTPC_CONFIG_PATH_MAX + 64u];
    app_monitor_state state = SOFTPC_MONITOR_STOPPED;
    app_presentation *presenter = NULL;

    int prompt_pending = 0;
    if (!app_presentation_create(&presenter, runtime, presentation,
            console_control, monitor, control_queue)) return 1;
    app_monitor_help(monitor);
    prompt_pending = 1;
    for (;;) {
        char *command;
        char *argument;
        if (prompt_pending && app_monitor_console_write(monitor, "SoftPC> "))
            prompt_pending = 0;
        for (;;) {
            app_control_event control_event;
            if (app_control_queue_take(control_queue, &control_event, 100u)) {
                if (control_event.kind == APP_CONTROL_UX_INPUT) {
                    /* A component can have queued an event just before an
                     * old VM run stopped.  It belongs to that run, never to
                     * a later start which happens to reuse the same UI. */
                    if (control_event.run_generation != 0u &&
                        control_event.run_generation !=
                            app_runtime_run_generation(runtime))
                        continue;
                    if (!app_monitor_handle_ux(control_queue, runtime, presenter,
                            &control_event.value.ux))
                        goto failed;
                    if (!app_monitor_drive(runtime, presenter)) goto failed;
                    continue;
                }
                if (control_event.kind == APP_CONTROL_MONITOR_LINE) {
                    if (control_event.value.line.length >= sizeof(line)) return 1;
                    memcpy(line, control_event.value.line.text,
                        control_event.value.line.length);
                    line[control_event.value.line.length] = '\0';
                    break;
                }
                if (control_event.run_generation != 0u &&
                    control_event.run_generation != app_runtime_run_generation(runtime))
                    continue;
                if (control_event.kind == APP_CONTROL_RUNTIME_COMPLETED)
                    app_presentation_note_runtime_completed(presenter,
                        control_event.value.runtime_state);
                else if (control_event.kind == APP_CONTROL_FRAME_COMPLETED)
                    app_presentation_note_frame_completed(presenter,
                        control_event.value.frame.graphics);
                else if (control_event.kind == APP_CONTROL_COMPONENT_COMPLETED)
                    app_presentation_note_component_completed(presenter,
                        control_event.value.component.component,
                        control_event.value.component.exists);
                else if (control_event.kind == APP_CONTROL_BROKER_COMPLETED)
                    app_presentation_note_broker_completed(presenter,
                        control_event.value.broker_vm_console_current);
                if (!app_monitor_drive(runtime, presenter)) goto failed;
                continue;
            }
            app_runtime_state actual = app_runtime_get_state(runtime);
            if (actual == SOFTPC_RUNTIME_PAUSED && state != SOFTPC_MONITOR_PAUSED) {
                state = SOFTPC_MONITOR_PAUSED;
                app_monitor_console_write(monitor, "Machine paused.\r\n");
                prompt_pending = 1;
            } else if (actual == SOFTPC_RUNTIME_STOPPED &&
                state == SOFTPC_MONITOR_RUNNING) {
                state = SOFTPC_MONITOR_STOPPED;
                app_monitor_console_write(monitor, "Machine stopped.\r\n");
                prompt_pending = 1;
            }
            if (prompt_pending && app_monitor_console_write(monitor, "SoftPC> "))
                prompt_pending = 0;
            if (!app_monitor_drive(runtime, presenter)) goto failed;
        }
        if (!app_monitor_drive(runtime, presenter)) goto failed;
        command = app_trim(line);
        argument = command;
        while (*argument != '\0' && !isspace((unsigned char)*argument))
            ++argument;
        if (*argument != '\0') *argument++ = '\0';
        argument = app_trim(argument);
        for (char *letter = command; *letter != '\0'; ++letter)
            *letter = (char)tolower((unsigned char)*letter);
        if (*command == '\0') continue;
        prompt_pending = 1;
        if (strcmp(command, "help") == 0) app_monitor_help(monitor);
        else if (strcmp(command, "exit") == 0) {
            (void)app_runtime_stop(runtime);
            (void)app_presentation_reconcile(presenter);
            app_presentation_destroy(presenter);
            return 0;
        }
        else if (strcmp(command, "start") == 0) {
            app_presentation_request_intent(presenter, APP_RECONCILER_INTENT_START);
            if (!app_monitor_drive(runtime, presenter)) goto failed;
        } else if (strcmp(command, "resume") == 0) {
            if (state != SOFTPC_MONITOR_PAUSED)
                app_monitor_console_write(monitor, "Machine is not paused.\r\n");
            else { app_presentation_request_intent(presenter, APP_RECONCILER_INTENT_RESUME);
                if (!app_monitor_drive(runtime, presenter)) goto failed; }
        } else if (strcmp(command, "pause") == 0) {
            if (state == SOFTPC_MONITOR_PAUSED)
                app_monitor_console_write(monitor, "Machine is paused.\r\n");
            else {
                app_presentation_request_intent(presenter,
                    APP_RECONCILER_INTENT_PAUSE);
                if (!app_monitor_drive(runtime, presenter)) goto failed;
            }
        } else if (strcmp(command, "stop") == 0) {
            app_presentation_request_intent(presenter, APP_RECONCILER_INTENT_STOP);
            if (!app_monitor_drive(runtime, presenter)) goto failed;
            state = SOFTPC_MONITOR_STOPPED;
            app_monitor_console_write(monitor, "Machine stopped.\r\n");
        } else if (strcmp(command, "reset") == 0) {
            if (!app_runtime_stop(runtime) || !app_runtime_start(runtime) ||
                !app_runtime_pause(runtime) ||
                !app_presentation_reconcile(presenter)) goto failed;
            state = SOFTPC_MONITOR_PAUSED;
            app_monitor_console_write(monitor, "Machine reset and pause requested.\r\n");
        } else if (strcmp(command, "floppy") == 0) {
            char *verb = argument;
            char *path = verb;
            while (*path != '\0' && !isspace((unsigned char)*path)) ++path;
            if (*path != '\0') *path++ = '\0';
            path = app_trim(path);
            for (char *letter = verb; *letter != '\0'; ++letter)
                *letter = (char)tolower((unsigned char)*letter);
            if (strcmp(verb, "eject") == 0 && *path == '\0') {
                if (!app_runtime_set_floppy(runtime, NULL))
                    app_monitor_console_write(monitor, "Cannot eject floppy.\r\n");
                else app_monitor_console_write(monitor, "Floppy ejected.\r\n");
            } else if (strcmp(verb, "insert") == 0 && *path != '\0') {
                if (!app_runtime_set_floppy(runtime, path))
                    app_monitor_console_write(monitor, "Cannot insert floppy.\r\n");
                else app_monitor_console_write(monitor, "Floppy inserted.\r\n");
            } else app_monitor_console_write(monitor,
                "Usage: floppy insert <image> | eject\r\n");
        } else app_monitor_console_write(monitor, "Unknown command.\r\n");
        app_monitor_console_write(monitor, "\r\n");
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
    app_runtime_set_completion_sink(runtime, app_runtime_completed, control_queue);
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
