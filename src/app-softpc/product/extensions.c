#include "lib/types/types_interface.h"
#include "lib/types/file.h"
#include "lib/storage/file_interface.h"

#include "extensions.h"

#define SOFTPC_EXTENSION_PATH_CAPACITY 1024u

static void softpc_extension_message(common_session_command_result *out,
    const char *message)
{
    (void)lib_c_snprintf((char *)out->text, sizeof(out->text), "%s\r\n\r\n",
        message);
    out->arm_prompt = LIB_TRUE;
}

static char *softpc_extension_trim(char *text)
{
    char *end;

    while (*text == ' ' || *text == '\t') ++text;
    end = text + lib_text_length(text);
    while (end != text && (end[-1] == ' ' || end[-1] == '\t')) --end;
    *end = '\0';
    return text;
}

static void softpc_extension_lower(char *text)
{
    while (*text != '\0') {
        if (*text >= 'A' && *text <= 'Z')
            *text = (char)(*text + ('a' - 'A'));
        ++text;
    }
}

static lib_bool softpc_extension_split(char *text, char **out_command,
    char **out_arguments)
{
    char *arguments = text;

    while (*arguments != '\0' && *arguments != ' ' && *arguments != '\t')
        ++arguments;
    if (*arguments != '\0') *arguments++ = '\0';
    *out_command = text;
    *out_arguments = softpc_extension_trim(arguments);
    softpc_extension_lower(*out_command);
    return **out_command != '\0';
}

static lib_status softpc_extension_snapshot_write(void *context,
    const lib_u8 *bytes, lib_size byte_count)
{
    return lib_storage_file_writer_write((lib_storage_file_writer *)context,
        bytes, byte_count);
}

static lib_status softpc_extension_snapshot_read(void *context,
    lib_u8 *bytes, lib_size byte_count)
{
    return lib_storage_file_reader_read((lib_storage_file_reader *)context,
        bytes, byte_count);
}

static void softpc_extension_save(common_machine *machine,
    common_session_machine_state state, const char *path,
    common_session_command_result *out)
{
    lib_storage_file_writer *writer = LIB_NULL;
    lib_status status;

    if (*path == '\0') {
        softpc_extension_message(out, "Usage: save <file>");
        return;
    }
    if (state != COMMON_SESSION_MACHINE_RUNNING &&
        state != COMMON_SESSION_MACHINE_PAUSED) {
        softpc_extension_message(out, "Machine is stopped; use start before save.");
        return;
    }
    status = lib_storage_file_writer_open(path, LIB_STORAGE_FILE_WRITER_TRUNCATE,
        &writer);
    if (status == LIB_STATUS_OK)
        status = common_machine_read_state(machine,
            &(common_machine_state_writer){ softpc_extension_snapshot_write, writer });
    if (writer != LIB_NULL && lib_storage_file_writer_close(writer) != LIB_STATUS_OK &&
        status == LIB_STATUS_OK)
        status = LIB_STATUS_IO_ERROR;
    softpc_extension_message(out, status == LIB_STATUS_OK ?
        "Machine saved and paused." : "Cannot save machine state.");
}

static void softpc_extension_load(common_machine *machine,
    common_session_machine_state state, const char *path,
    common_session_command_result *out)
{
    lib_storage_file_reader *reader = LIB_NULL;
    lib_status status;

    if (*path == '\0') {
        softpc_extension_message(out, "Usage: load <file>");
        return;
    }
    /* Monitor INIT was historically the same load-safe condition as STOPPED.
     * The shared Product delegates this product-only command with Common's
     * original state, so preserve that command contract here. */
    if (state != COMMON_SESSION_MACHINE_INIT &&
        state != COMMON_SESSION_MACHINE_STOPPED) {
        softpc_extension_message(out, state == COMMON_SESSION_MACHINE_RUNNING ?
            "Machine is running; stop it before load." :
            "Machine is paused; stop it before load.");
        return;
    }
    status = lib_storage_file_reader_open(path, &reader);
    if (status == LIB_STATUS_OK)
        status = common_machine_write_state(machine,
            &(common_machine_state_reader){ softpc_extension_snapshot_read, reader });
    if (reader != LIB_NULL && lib_storage_file_reader_close(reader) != LIB_STATUS_OK &&
        status == LIB_STATUS_OK)
        status = LIB_STATUS_IO_ERROR;
    softpc_extension_message(out, status == LIB_STATUS_OK ?
        "Machine loaded and paused." : "Cannot load machine state.");
}

static lib_bool softpc_extension_mode(const char *text,
    lib_storage_medium_mode *out_mode)
{
    if (lib_text_compare(text, "readonly") == 0)
        *out_mode = LIB_STORAGE_MEDIUM_READONLY;
    else if (lib_text_compare(text, "direct") == 0)
        *out_mode = LIB_STORAGE_MEDIUM_DIRECT;
    else if (lib_text_compare(text, "overlay") == 0)
        *out_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    else
        return LIB_FALSE;
    return LIB_TRUE;
}

static void softpc_extension_floppy(common_machine *machine,
    common_session_machine_state state, char *arguments,
    common_session_command_result *out)
{
    char *operation;
    char *rest;
    char *mode;
    lib_storage_medium_mode medium_mode;

    if (state == COMMON_SESSION_MACHINE_RUNNING) {
        softpc_extension_message(out, "Cannot change floppy media now.");
        return;
    }
    if (!softpc_extension_split(arguments, &operation, &rest)) {
        softpc_extension_message(out,
            "Usage: floppy insert <readonly|direct|overlay> <image> | eject");
        return;
    }
    if (lib_text_compare(operation, "eject") == 0 && *rest == '\0') {
        softpc_extension_message(out,
            common_machine_set_removable_media(machine, LIB_NULL,
                LIB_STORAGE_MEDIUM_OVERLAY) ? "Floppy ejected." : "Cannot eject floppy.");
        return;
    }
    if (lib_text_compare(operation, "insert") != 0 ||
        !softpc_extension_split(rest, &mode, &rest) || *rest == '\0' ||
        !softpc_extension_mode(mode, &medium_mode)) {
        softpc_extension_message(out,
            "Usage: floppy insert <readonly|direct|overlay> <image> | eject");
        return;
    }
    softpc_extension_message(out,
        common_machine_set_removable_media(machine, rest, medium_mode) ?
        "Floppy inserted." : "Cannot insert floppy.");
}

static lib_bool softpc_extension_submit(void *context, common_machine *machine,
    common_session_machine_state state, const char *line,
    common_session_command_result *out)
{
    char buffer[SOFTPC_EXTENSION_PATH_CAPACITY];
    char *command;
    char *arguments;
    lib_size length;

    (void)context;
    if (machine == LIB_NULL || line == LIB_NULL || out == LIB_NULL)
        return LIB_FALSE;
    length = lib_text_length(line);
    if (length >= sizeof(buffer)) return LIB_FALSE;
    lib_memory_copy(buffer, line, length + 1u);
    if (!softpc_extension_split(softpc_extension_trim(buffer), &command, &arguments))
        return LIB_FALSE;
    *out = (common_session_command_result){0};
    if (lib_text_compare(command, "save") == 0) {
        softpc_extension_save(machine, state, arguments, out);
        return LIB_TRUE;
    }
    if (lib_text_compare(command, "load") == 0) {
        softpc_extension_load(machine, state, arguments, out);
        return LIB_TRUE;
    }
    if (lib_text_compare(command, "floppy") == 0) {
        softpc_extension_floppy(machine, state, arguments, out);
        return LIB_TRUE;
    }
    return LIB_FALSE;
}

lib_status softpc_product_configure_extensions(vm_app *app,
    app_command_extensions *out_extensions)
{
    (void)app;
    if (out_extensions == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_extensions = (app_command_extensions){
        .submit = softpc_extension_submit,
        .help_text = "  floppy insert <mode> <image>\r\n"
            "                 insert drive A media while stopped/paused\r\n"
            "  floppy eject   eject drive A media while stopped/paused\r\n"
            "  save <file>    save a running or paused machine\r\n"
            "  load <file>    load a snapshot while stopped\r\n"
    };
    return LIB_STATUS_OK;
}
