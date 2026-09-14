#include "command.h"
#include "config.h"
#include "command_binding.h"
#include "common/session/session_interface.h"
#include "common/machine/machine_interface.h"
#include "vm/vm_interface.h"
#include "keyboard.h"
#include "common/ui/ui_interface.h"

#include <stdio.h>
#include <string.h>

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
        COMMON_SESSION_DISPLAY_CONSOLE, 1, LIB_STORAGE_MEDIUM_OVERLAY };
    vm_options options = { 0 };
    common_machine *machine_runtime = NULL;
    vm_driver *machine_driver = NULL;
    common_machine_driver driver = { 0 };
    common_ui *ui = NULL;
    common_ui_options common_options = { 0 };
    common_session_options session_options = { 0 };
    app_command_binding command_binding = { 0 };
    kvm_hotkey_registry hotkeys;
    char graphics_console_status[APP_COMMAND_TEXT_CAPACITY];
    common_session *session = NULL;
    lib_status result;
    (void)argv;

    vm_trace_reset();

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
    options.media_mode = config.media_mode;
    options.serial_output_path = config.serial_output_path[0] == '\0' ? NULL :
        config.serial_output_path;
    options.printer_output_path = config.printer_output_path[0] == '\0' ? NULL :
        config.printer_output_path;
    result = vm_create(&options, &machine_driver);
    if (result != LIB_STATUS_OK) goto done;
    vm_driver_describe(machine_driver, &driver);
    if (common_machine_create(&machine_runtime, &driver) != LIB_STATUS_OK) {
        result = LIB_STATUS_IO_ERROR;
        goto done;
    }
    if (!app_keyboard_hotkeys(&hotkeys)) {
        result = LIB_STATUS_IO_ERROR;
        goto done;
    }
    session_options.display = config.presentation;
    session_options.console_control = config.console_control != 0;
    session_options.machine = machine_runtime;
    if (app_command_binding_initialize(&command_binding, machine_runtime,
            config.presentation, &session_options.command) != LIB_STATUS_OK) {
        result = LIB_STATUS_IO_ERROR;
        goto done;
    }
    if (common_session_create(&session, &session_options) != LIB_STATUS_OK) {
        result = LIB_STATUS_IO_ERROR;
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
        result = LIB_STATUS_IO_ERROR;
        goto done;
    }
    if (common_session_bind_ui(session, ui) != LIB_STATUS_OK) {
        result = LIB_STATUS_IO_ERROR;
        goto done;
    }
    common_machine_set_state_sink(machine_runtime, app_machine_state_completed, session);
    common_machine_set_frame_sink(machine_runtime, app_machine_frame_published, session);
    if (common_session_run(session) == 0)
        result = LIB_STATUS_IO_ERROR;
done:
    if (result != LIB_STATUS_OK)
        fprintf(stderr, "softpcvm: %s\n", result == LIB_STATUS_INVALID_ARGUMENT ?
            "invalid argument or media" : "host I/O error");
    app_command_binding_dispose(&command_binding);
    common_machine_destroy(machine_runtime);
    (void)common_ui_destroy(ui);
    (void)common_session_destroy(session);
    vm_destroy(machine_driver);
    return result != LIB_STATUS_OK;
}
