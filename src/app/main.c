#include "command.h"
#include "command_binding.h"
#include "common/session/session_interface.h"
#include "common/machine/machine_interface.h"
#include "machine_driver.h"
#include "machine.h"
#include "audio.h"
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
    if (softpc_platform_audio_start() != LIB_STATUS_OK) {
        result = SOFTPC_MACHINE_IO_ERROR;
        goto done;
    }
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
    session_options.display = app_session_display(options.presentation);
    session_options.console_control = config.console_control != 0;
    session_options.machine = machine_runtime;
    if (app_command_binding_initialize(&command_binding, machine_runtime,
            options.presentation, &session_options.command) != LIB_STATUS_OK) {
        result = SOFTPC_MACHINE_IO_ERROR;
        goto done;
    }
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
    app_command_binding_dispose(&command_binding);
    common_machine_destroy(machine_runtime);
    app_machine_driver_destroy(machine_driver);
    (void)common_ui_destroy(ui);
    (void)common_session_destroy(session);
    softpc_platform_audio_shutdown();
    softpc_machine_destroy(machine);
    return result != SOFTPC_MACHINE_OK;
}
