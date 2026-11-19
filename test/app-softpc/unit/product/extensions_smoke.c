#include <assert.h>
#include "lib/types/win32/file.h"
#include "product/extensions.c"

static int capture;
static unsigned media_calls, snapshot_calls;
static lib_storage_medium_mode media_mode;
static const char *media_path = "Disk Image.img";

lib_status emulator_machine_read_state(emulator_machine *machine,
    const emulator_machine_state_writer *writer)
{
    lib_status status;
    (void)machine;
    ++snapshot_calls;
    if (capture == 0) return LIB_STATUS_IO_ERROR;
    assert(writer->write(writer->context, LIB_NULL, 0u) == LIB_STATUS_OK);
    if (capture == 1) return LIB_STATUS_IO_ERROR;
    status = writer->write(writer->context, (const lib_u8 *)"new", 3u);
    if (status != LIB_STATUS_OK) return status;
    if (capture == 2) return LIB_STATUS_IO_ERROR;
    return writer->write(writer->context, (const lib_u8 *)" state", 6u);
}

lib_status emulator_machine_write_state(emulator_machine *machine,
    const emulator_machine_state_reader *reader)
{
    lib_u8 bytes[9];
    (void)machine;
    ++snapshot_calls;
    assert(reader->read(reader->context, bytes, sizeof(bytes)) == LIB_STATUS_OK);
    assert(lib_memory_compare(bytes, "new state", sizeof(bytes)) == 0);
    return LIB_STATUS_OK;
}

lib_bool emulator_machine_set_removable_media(emulator_machine *machine,
    const char *path, lib_storage_medium_mode mode)
{
    (void)machine;
    assert(media_path == LIB_NULL ? path == LIB_NULL :
        path != LIB_NULL && lib_text_compare(path, media_path) == 0);
    ++media_calls;
    media_mode = mode;
    return LIB_TRUE;
}

static void contents(const char *path, const char *expected)
{
    void *bytes = LIB_NULL;
    lib_size size = 0u;
    assert(lib_storage_file_read_owned(path, 128u, &bytes, &size) == LIB_STATUS_OK);
    assert(size == lib_text_length(expected));
    assert(lib_memory_compare(bytes, expected, size) == 0);
    lib_release(bytes);
}

static lib_bool submit_fixed(void *machine, emulator_product_monitor_command command,
    emulator_session_machine_state state, const emulator_product_monitor_arguments *arguments,
    emulator_session_command_result *out)
{
    return softpc_extension_submit_snapshot(LIB_NULL, machine, command, state, arguments, out);
}

static lib_bool submit_extension(void *machine, emulator_session_machine_state state,
    const char *name, const emulator_product_monitor_arguments *arguments,
    emulator_session_command_result *out)
{
    return softpc_extension_submit(LIB_NULL, machine, state, name, arguments, out);
}

int main(void)
{
    const char *path = "snapshot space-\xe6\xb5\x8b-\xf0\x9f\x8e\xb5.spcs";
    const char *modes[] = { "READONLY", "direct", "overlay" };
    const lib_storage_medium_mode mode_values[] = {
        LIB_STORAGE_MEDIUM_READONLY, LIB_STORAGE_MEDIUM_DIRECT, LIB_STORAGE_MEDIUM_OVERLAY};
    const char *invalid[] = {
        "save",
        "save \"\"",
        "save unquoted space",
        "load \"\"",
        "load unquoted space",
        "floppy insert readonly \"\"",
        "floppy insert readonly Disk Image.img",
        "floppy insert invalid disk.img",
        "floppy eject extra",
        "save \"open",
        "load \"file\"tail",
        "floppy insert readonly \"open"};
    char line[1024], identity;
    const emulator_product_monitor_provider provider = {
        .context = &identity, .submit_fixed = submit_fixed, .submit_extension = submit_extension};
    emulator_session_command_result result;
    lib_storage_file_writer *file = LIB_NULL;

    assert(lib_storage_file_writer_open(path, LIB_STORAGE_FILE_WRITER_TRUNCATE,
        &file) == LIB_STATUS_OK);
    assert(lib_storage_file_writer_write(file, "original snapshot", 17u) == LIB_STATUS_OK);
    assert(lib_storage_file_writer_close(file) == LIB_STATUS_OK);
    lib_c_snprintf(line, sizeof(line), "save \"%s\"", path);
    for (capture = 0; capture < 4; ++capture) {
        emulator_product_monitor_provider_submit_line((void *)&provider,
            EMULATOR_SESSION_MACHINE_PAUSED, line, &result);
        contents(path, capture < 2 ? "original snapshot" : capture == 2 ? "new" : "new state");
        assert(lib_text_find_substring(result.text, capture == 3 ?
            "Machine saved and paused." : "Cannot save machine state.") != LIB_NULL);
    }
    lib_c_snprintf(line, sizeof(line), "load \"%s\"", path);
    emulator_product_monitor_provider_submit_line((void *)&provider,
        EMULATOR_SESSION_MACHINE_INIT, line, &result);
    assert(lib_text_find_substring(result.text, "Machine loaded and paused.") != LIB_NULL);
    assert(lib_win32_wremove(L"snapshot space-\u6d4b-\U0001f3b5.spcs") == 0);
    emulator_product_monitor_provider_submit_line((void *)&provider,
        EMULATOR_SESSION_MACHINE_PAUSED, "save .", &result);
    assert(lib_text_find_substring(result.text, "Cannot save machine state.") != LIB_NULL);
    capture = 0;
    lib_c_snprintf(line, sizeof(line), "save \"%s\"", path);
    emulator_product_monitor_provider_submit_line((void *)&provider,
        EMULATOR_SESSION_MACHINE_RUNNING, line, &result);
    {
        lib_storage_file_reader *reader = LIB_NULL;
        assert(lib_storage_file_reader_open(path, &reader) != LIB_STATUS_OK && reader == LIB_NULL);
    }
    for (unsigned i = 0; i < 3u; ++i) {
        lib_c_snprintf(line, sizeof(line), "FlOpPy  insert\t%s \"%s\"", modes[i], media_path);
        emulator_product_monitor_provider_submit_line((void *)&provider,
            EMULATOR_SESSION_MACHINE_PAUSED, line, &result);
        assert(media_calls == i + 1u && media_mode == mode_values[i]);
    }
    media_path = "O:\\Disk Images\\\xe6\xb5\x8b\xe8\xaf\x95.img";
    lib_c_snprintf(line, sizeof(line), "floppy insert readonly \"%s\"", media_path);
    emulator_product_monitor_provider_submit_line((void *)&provider,
        EMULATOR_SESSION_MACHINE_PAUSED, line, &result);
    assert(media_calls == 4u);
    unsigned before = snapshot_calls;
    for (unsigned i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
        emulator_product_monitor_provider_submit_line((void *)&provider,
            EMULATOR_SESSION_MACHINE_PAUSED, invalid[i], &result);
        assert(media_calls == 4u && snapshot_calls == before);
        assert(lib_text_find_substring(result.text, i < 9u ? "Usage:" : "Invalid command syntax.") != LIB_NULL);
    }
    media_path = LIB_NULL;
    emulator_product_monitor_provider_submit_line((void *)&provider,
        EMULATOR_SESSION_MACHINE_PAUSED, "FLOPPY EJECT", &result);
    assert(media_calls == 5u && media_mode == LIB_STORAGE_MEDIUM_OVERLAY);
    assert(lib_text_find_substring(result.text, "Floppy ejected.") != LIB_NULL);
    lib_c_printf("SoftPC extensions: Monitor arguments, deferred output and media modes OK\n");
    return 0;
}
