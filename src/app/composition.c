#include "composition.h"
#include "keyboard.h"
#include "vm/vm_interface.h"
#include "common/ui/ui_interface.h"

#include <stdio.h>
#include <stdlib.h>

/* Adapt the provider's one context and reserve the same command admission
 * boundary for keyboard-derived requests. Product meaning stays in keyboard. */
static lib_bool app_composition_hotkey(void *opaque,
    common_session_machine_state state, const char *identifier,
    common_session_command_result *out)
{
    app_command_context *command = opaque;
    lib_bool accepted = app_keyboard_handle_hotkey(command->machine, state, identifier, out);
    if (accepted && out->request != COMMON_SESSION_REQUEST_NONE &&
        !app_command_provider_begin_external(command, state, out->request))
        out->request = COMMON_SESSION_REQUEST_NONE;
    return accepted;
}

lib_status app_composition_initialize(app_command_context *command,
    common_machine *machine, common_session_display display,
    common_session_command_provider *provider)
{
    if (command == NULL || machine == NULL || provider == NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *provider = (common_session_command_provider) {
        .context = command,
        .open = app_command_provider_open,
        .reject_line = app_command_provider_reject_line,
        .submit_line = app_command_provider_submit_line,
        .begin_external = app_command_provider_begin_external,
        .note_runtime = app_command_provider_note_runtime,
        .note_broker = app_command_provider_note_broker,
        .note_monitor_current = app_command_provider_note_monitor_current,
        .handle_hotkey = app_composition_hotkey
    };
    return app_command_initialize(command, machine, display);
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

lib_status app_composition_run(const app_startup_config *config)
{
    vm_options options = { 0 };
    common_machine *machine_runtime = NULL;
    vm_driver *machine_driver = NULL;
    common_machine_driver driver = { 0 };
    common_ui *ui = NULL;
    common_ui_options common_options = { 0 };
    common_session_options session_options = { 0 };
    app_command_context commands = { 0 };
    kvm_hotkey_registry hotkeys;
    char graphics_console_status[APP_COMMAND_TEXT_CAPACITY];
    common_session *session = NULL;
    lib_status result;

    vm_trace_reset();
    options.floppy_path = config->floppy_path[0] == '\0' ? NULL : config->floppy_path;
    options.hard_disk_path = config->hard_disk_path[0] == '\0' ? NULL : config->hard_disk_path;
    options.memory_bytes = config->memory_bytes;
    options.floppy_mode = config->floppy_mode;
    options.hard_disk_mode = config->hard_disk_mode;
    options.serial_output_path = config->serial_output_path[0] == '\0' ? NULL :
        config->serial_output_path;
    options.printer_output_path = config->printer_output_path[0] == '\0' ? NULL :
        config->printer_output_path;
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
    session_options.display = config->presentation;
    session_options.console_control = config->console_control != 0;
    session_options.machine = machine_runtime;
    if (app_composition_initialize(&commands, machine_runtime,
            config->presentation,
            &session_options.command) != LIB_STATUS_OK) {
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
    if (common_machine_shutdown(machine_runtime) != LIB_STATUS_OK) {
        fputs("softpcvm: cannot join machine worker\n", stderr);
        exit(EXIT_FAILURE); /* Callback targets must outlive an unjoined worker. */
    }
    if (common_ui_destroy(ui) != LIB_STATUS_OK) {
        fputs("softpcvm: cannot destroy UI workers\n", stderr);
        exit(EXIT_FAILURE); /* Retain Session and all live callback dependencies. */
    }
    (void)common_session_destroy(session);
    app_command_dispose(&commands);
    common_machine_destroy(machine_runtime);
    vm_destroy(machine_driver);
    return result;
}
