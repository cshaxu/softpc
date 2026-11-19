#include "lib/types/types_interface.h"
#include "lib/types/file.h"
#include "lib/storage/file_interface.h"

#include "extensions.h"

static void softpc_extension_message(emulator_session_command_result *out,
    const char *message)
{
    (void)lib_c_snprintf((char *)out->text, sizeof(out->text), "%s\r\n",
        message);
    out->arm_prompt = LIB_TRUE;
}

static const emulator_product_help_row softpc_extension_help[] = {
    {"floppy insert <mode> <image>", "insert drive A media while stopped/paused"},
    {"floppy eject", "eject drive A media while stopped/paused"}
};

static lib_bool softpc_extension_keyword(const char *text, const char *word)
{
    while (*text != '\0' && *word != '\0') {
        char value = *text++;
        if (value >= 'A' && value <= 'Z') value = (char)(value + ('a' - 'A'));
        if (value != *word++) return LIB_FALSE;
    }
    return *text == '\0' && *word == '\0';
}

typedef struct softpc_snapshot_output {
    const char *path;
    lib_storage_file_writer *writer;
} softpc_snapshot_output;

static lib_status softpc_extension_snapshot_write(void *context,
    const lib_u8 *bytes, lib_size byte_count)
{
    softpc_snapshot_output *output = context;
    lib_status status;
    if (byte_count == 0u) return LIB_STATUS_OK;
    if (output->writer == LIB_NULL) {
        status = lib_storage_file_writer_open(output->path,
            LIB_STORAGE_FILE_WRITER_TRUNCATE, &output->writer);
        if (status != LIB_STATUS_OK) return status;
    }
    return lib_storage_file_writer_write(output->writer, bytes, byte_count);
}

static lib_status softpc_extension_snapshot_read(void *context,
    lib_u8 *bytes, lib_size byte_count)
{
    return lib_storage_file_reader_read((lib_storage_file_reader *)context,
        bytes, byte_count);
}

static void softpc_extension_save(emulator_machine *machine,
    emulator_session_machine_state state, const char *path,
    emulator_session_command_result *out)
{
    softpc_snapshot_output output = {path, LIB_NULL};
    lib_status status;

    if (*path == '\0') {
        softpc_extension_message(out, "Usage: save <file>");
        return;
    }
    if (state != EMULATOR_SESSION_MACHINE_RUNNING &&
        state != EMULATOR_SESSION_MACHINE_PAUSED) {
        softpc_extension_message(out, "Machine is stopped; use start before save.");
        return;
    }
    status = emulator_machine_read_state(machine,
        &(emulator_machine_state_writer){ softpc_extension_snapshot_write, &output });
    if (output.writer != LIB_NULL &&
        lib_storage_file_writer_close(output.writer) != LIB_STATUS_OK &&
        status == LIB_STATUS_OK)
        status = LIB_STATUS_IO_ERROR;
    softpc_extension_message(out, status == LIB_STATUS_OK ?
        "Machine saved and paused." : "Cannot save machine state.");
}

static void softpc_extension_load(emulator_machine *machine,
    emulator_session_machine_state state, const char *path,
    emulator_session_command_result *out)
{
    lib_storage_file_reader *reader = LIB_NULL;
    lib_status status;

    if (*path == '\0') {
        softpc_extension_message(out, "Usage: load <file>");
        return;
    }
    /* Monitor INIT was historically the same load-safe condition as STOPPED.
     * The shared Product delegates this product-only command with Emulator's
     * original state, so preserve that command contract here. */
    if (state != EMULATOR_SESSION_MACHINE_INIT &&
        state != EMULATOR_SESSION_MACHINE_STOPPED) {
        softpc_extension_message(out, state == EMULATOR_SESSION_MACHINE_RUNNING ?
            "Machine is running; stop it before load." :
            "Machine is paused; stop it before load.");
        return;
    }
    status = lib_storage_file_reader_open(path, &reader);
    if (status == LIB_STATUS_OK)
        status = emulator_machine_write_state(machine,
            &(emulator_machine_state_reader){ softpc_extension_snapshot_read, reader });
    if (reader != LIB_NULL && lib_storage_file_reader_close(reader) != LIB_STATUS_OK &&
        status == LIB_STATUS_OK)
        status = LIB_STATUS_IO_ERROR;
    softpc_extension_message(out, status == LIB_STATUS_OK ?
        "Machine loaded and paused." : "Cannot load machine state.");
}

static void softpc_extension_floppy(emulator_machine *machine,
    emulator_session_machine_state state,
    const emulator_product_monitor_arguments *arguments,
    emulator_session_command_result *out)
{
    lib_storage_medium_mode mode;
    const char *const *values = arguments->values;

    if (state == EMULATOR_SESSION_MACHINE_RUNNING) {
        softpc_extension_message(out, "Cannot change floppy media now.");
        return;
    }
    if (arguments->count == 1u && softpc_extension_keyword(values[0], "eject")) {
        softpc_extension_message(out,
            emulator_machine_set_removable_media(machine, LIB_NULL,
                LIB_STORAGE_MEDIUM_OVERLAY) ? "Floppy ejected." : "Cannot eject floppy.");
        return;
    }
    if (arguments->count != 3u || !softpc_extension_keyword(values[0], "insert") ||
        *values[2] == '\0') goto usage;
    if (softpc_extension_keyword(values[1], "readonly")) mode = LIB_STORAGE_MEDIUM_READONLY;
    else if (softpc_extension_keyword(values[1], "direct")) mode = LIB_STORAGE_MEDIUM_DIRECT;
    else if (softpc_extension_keyword(values[1], "overlay")) mode = LIB_STORAGE_MEDIUM_OVERLAY;
    else goto usage;
    softpc_extension_message(out,
        emulator_machine_set_removable_media(machine, values[2], mode) ?
        "Floppy inserted." : "Cannot insert floppy.");
    return;
usage:
    softpc_extension_message(out,
        "Usage: floppy insert <readonly|direct|overlay> <image> | eject");
}

static lib_bool softpc_extension_submit_snapshot(void *context,
    emulator_machine *machine, emulator_product_monitor_command command,
    emulator_session_machine_state state,
    const emulator_product_monitor_arguments *arguments,
    emulator_session_command_result *out)
{
    const char *path;
    (void)context;
    if (machine == LIB_NULL || arguments == LIB_NULL || out == LIB_NULL)
        return LIB_FALSE;
    path = arguments->count == 1u ? arguments->values[0] : "";
    *out = (emulator_session_command_result){0};
    if (command == EMULATOR_PRODUCT_MONITOR_COMMAND_SAVE)
        softpc_extension_save(machine, state, path, out);
    else if (command == EMULATOR_PRODUCT_MONITOR_COMMAND_LOAD)
        softpc_extension_load(machine, state, path, out);
    else return LIB_FALSE;
    return LIB_TRUE;
}

static lib_bool softpc_extension_submit(void *context, emulator_machine *machine,
    emulator_session_machine_state state, const char *name,
    const emulator_product_monitor_arguments *arguments,
    emulator_session_command_result *out)
{
    (void)context;
    if (machine == LIB_NULL || name == LIB_NULL || arguments == LIB_NULL || out == LIB_NULL)
        return LIB_FALSE;
    *out = (emulator_session_command_result){0};
    if (softpc_extension_keyword(name, "floppy")) {
        softpc_extension_floppy(machine, state, arguments, out);
        return LIB_TRUE;
    }
    return LIB_FALSE;
}

lib_status softpc_product_configure_extensions(emulator_product_machine *machine,
    x86_ibmpc_product_command_extensions *out_extensions)
{
    if (out_extensions == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_extensions = (x86_ibmpc_product_command_extensions){
        .context = machine,
        .submit = softpc_extension_submit,
        .submit_snapshot = softpc_extension_submit_snapshot,
        .help = {softpc_extension_help,
            sizeof(softpc_extension_help) / sizeof(softpc_extension_help[0])}
    };
    return LIB_STATUS_OK;
}
