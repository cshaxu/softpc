#include "command.h"
#include "common/session/session_interface.h"
#include "common/machine/machine_interface.h"
#include "machine_driver.h"
#include "machine.h"
#include "prompt_trace.h"
#include "keyboard.h"
#include "common/ui/ui_interface.h"
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

/* app owns the SoftPC CLI policy and machine adapter.  common/session only
 * sees this copied, product-neutral provider contract. */
typedef struct app_command_binding {
    app_command_session session;
    common_machine *machine;
} app_command_binding;

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

static common_session_display app_session_display(softpc_presentation display)
{
    return display == SOFTPC_PRESENTATION_WINDOW ?
        COMMON_SESSION_DISPLAY_WINDOW : COMMON_SESSION_DISPLAY_CONSOLE;
}

static common_session_machine_state app_session_state(common_machine_state state)
{
    switch (state) {
    case COMMON_MACHINE_RUNNING: return COMMON_SESSION_MACHINE_RUNNING;
    case COMMON_MACHINE_PAUSED: return COMMON_SESSION_MACHINE_PAUSED;
    case COMMON_MACHINE_ERROR: return COMMON_SESSION_MACHINE_ERROR;
    case COMMON_MACHINE_RESET_COMPLETED:
        return COMMON_SESSION_MACHINE_RESET_COMPLETED;
    default: return COMMON_SESSION_MACHINE_STOPPED;
    }
}

/* app/ owns the product machine ABI. common/session owns its neutral copied
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

static void app_command_provider_submit_line(void *opaque,
    common_session_machine_state state, const char *line,
    common_session_command_result *out)
{
    app_command_binding *binding = (app_command_binding *)opaque;
    app_command_effect effect = { 0 };
    app_command_session_submit_line(&binding->session, app_command_state(state), line,
        &effect);
    if (effect.action != APP_COMMAND_ACTION_NONE) {
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

static void app_machine_state_completed(void *opaque, common_machine_state state,
    lib_u32 run_generation)
{
    common_session *session = (common_session *)opaque;
    (void)common_session_enqueue_runtime_completed(session, app_session_state(state),
        run_generation);
}

static void app_machine_frame_published(void *opaque, lib_u32 sequence,
    lib_bool graphics, lib_u32 run_generation)
{
    common_session *session = (common_session *)opaque;
    (void)common_session_enqueue_frame_completed(session, sequence,
        graphics != 0, run_generation);
}

int main(int argc, char **argv)
{
    char config_path[SOFTPC_CONFIG_PATH_MAX];
    app_startup_config config = { { 0 }, { 0 }, { 0 }, { 0 }, 16u * 1024u * 1024u,
        SOFTPC_PRESENTATION_CONSOLE, 1, SOFTPC_MEDIA_OVERLAY };
    softpc_machine_options options = { 0 };
    softpc_machine *machine = NULL;
    common_machine *machine_runtime = NULL;
    app_machine_driver *machine_driver = NULL;
    common_machine_driver driver = { 0 };
    common_ui *ui = NULL;
    common_ui_options common_options = { 0 };
    common_session_options session_options = { 0 };
    app_command_binding command_binding = { 0 };
    kvm_hotkey_registry hotkeys;
    char graphics_console_status[APP_COMMAND_TEXT_CAPACITY];
    common_session *session = NULL;
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
    if (app_machine_driver_create(&machine_driver, machine) != LIB_STATUS_OK) {
        result = SOFTPC_MACHINE_IO_ERROR;
        goto done;
    }
    app_machine_driver_describe(machine_driver, &driver);
    if (common_machine_create(&machine_runtime, &driver) != LIB_STATUS_OK) {
        result = SOFTPC_MACHINE_IO_ERROR;
        goto done;
    }
    if (!app_keyboard_hotkeys(&hotkeys)) {
        result = SOFTPC_MACHINE_IO_ERROR;
        goto done;
    }
    command_binding.machine = machine_runtime;
    app_command_session_initialize(&command_binding.session, options.presentation);
    session_options.display = app_session_display(options.presentation);
    session_options.console_control = config.console_control != 0;
    session_options.machine = machine_runtime;
    session_options.command.context = &command_binding;
    session_options.command.open = app_command_provider_open;
    session_options.command.reject_line = app_command_provider_reject_line;
    session_options.command.submit_line = app_command_provider_submit_line;
    session_options.command.begin_external = app_command_provider_begin_external;
    session_options.command.note_runtime = app_command_provider_note_runtime;
    session_options.command.note_broker = app_command_provider_note_broker;
    session_options.command.note_monitor_current = app_command_provider_note_monitor_current;
    session_options.command.handle_hotkey = app_command_provider_handle_hotkey;
    if (common_session_create(&session, &session_options) != LIB_STATUS_OK) {
        result = SOFTPC_MACHINE_IO_ERROR;
        goto done;
    }
    (void)snprintf(graphics_console_status, sizeof(graphics_console_status),
        "Insignia SoftPC is running in the Window.\r\n\r\n%s\r\n",
        app_command_hotkey_help());
    common_options.event_context = session;
    common_options.event_sink = common_session_enqueue_ui_event;
    common_options.hotkeys = hotkeys;
    common_options.running_window_title = "Insignia SoftPC (Running)";
    common_options.paused_window_title = "Insignia SoftPC (Paused)";
    common_options.graphics_console_status_text = graphics_console_status;
    if (common_ui_create(&ui, &common_options) != LIB_STATUS_OK) {
        result = SOFTPC_MACHINE_IO_ERROR;
        goto done;
    }
    if (common_session_bind_ui(session, ui) != LIB_STATUS_OK) {
        result = SOFTPC_MACHINE_IO_ERROR;
        goto done;
    }
    common_machine_set_state_sink(machine_runtime, app_machine_state_completed, session);
    common_machine_set_frame_sink(machine_runtime, app_machine_frame_published, session);
    if (common_session_run(session) == 0)
        result = SOFTPC_MACHINE_IO_ERROR;
done:
    if (result != SOFTPC_MACHINE_OK)
        fprintf(stderr, "softpcvm: %s\n", softpc_machine_result_name(result));
    common_machine_destroy(machine_runtime);
    app_machine_driver_destroy(machine_driver);
    (void)common_ui_destroy(ui);
    (void)common_session_destroy(session);
    softpc_machine_destroy(machine);
    return result != SOFTPC_MACHINE_OK;
}
