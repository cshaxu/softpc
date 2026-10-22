#include <assert.h>
#include "lib/types/win32/file.h"
#include "product/extensions.c"

static int capture;
static unsigned media_calls;
static lib_storage_medium_mode media_mode;

lib_status emulator_machine_read_state(emulator_machine *machine,
    const emulator_machine_state_writer *writer)
{
    lib_status status;
    (void)machine;
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
    assert(reader->read(reader->context, bytes, sizeof(bytes)) == LIB_STATUS_OK);
    assert(lib_memory_compare(bytes, "new state", sizeof(bytes)) == 0);
    return LIB_STATUS_OK;
}

lib_bool emulator_machine_set_removable_media(emulator_machine *machine,
    const char *path, lib_storage_medium_mode mode)
{
    (void)machine;
    assert(lib_text_compare(path, "Disk Image.img") == 0);
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

int main(void)
{
    const char *path = "snapshot-\xe6\xb5\x8b-\xf0\x9f\x8e\xb5.spcs";
    const char *commands[] = {
        "floppy insert READONLY Disk Image.img",
        "floppy insert direct Disk Image.img",
        "floppy insert overlay Disk Image.img"
    };
    const lib_storage_medium_mode modes[] = {
        LIB_STORAGE_MEDIUM_READONLY, LIB_STORAGE_MEDIUM_DIRECT,
        LIB_STORAGE_MEDIUM_OVERLAY
    };
    char identity;
    emulator_machine *machine = (emulator_machine *)&identity;
    emulator_session_command_result result;
    x86_ibmpc_product_command_extensions extensions;
    lib_storage_file_writer *file = LIB_NULL;
    unsigned index;

    assert(softpc_product_configure_extensions(LIB_NULL, &extensions) == LIB_STATUS_OK);
    assert(lib_storage_file_writer_open(path, LIB_STORAGE_FILE_WRITER_TRUNCATE,
        &file) == LIB_STATUS_OK);
    assert(lib_storage_file_writer_write(file, "original snapshot", 17u) == LIB_STATUS_OK);
    assert(lib_storage_file_writer_close(file) == LIB_STATUS_OK);
    for (capture = 0; capture < 4; ++capture) {
        assert(extensions.submit_snapshot(LIB_NULL, machine,
            EMULATOR_PRODUCT_MONITOR_COMMAND_SAVE, EMULATOR_SESSION_MACHINE_PAUSED,
            path, &result));
        contents(path, capture < 2 ? "original snapshot" :
            capture == 2 ? "new" : "new state");
        assert(lib_text_find_substring(result.text, capture == 3 ?
            "Machine saved and paused." : "Cannot save machine state.") != LIB_NULL);
    }
    assert(extensions.submit_snapshot(LIB_NULL, machine,
        EMULATOR_PRODUCT_MONITOR_COMMAND_LOAD, EMULATOR_SESSION_MACHINE_INIT,
        path, &result));
    assert(lib_text_find_substring(result.text, "Machine loaded and paused.") != LIB_NULL);
    assert(lib_win32_wremove(L"snapshot-\u6d4b-\U0001f3b5.spcs") == 0);
    /* A real open failure is reported by the first write, not as a saved image. */
    assert(extensions.submit_snapshot(LIB_NULL, machine,
        EMULATOR_PRODUCT_MONITOR_COMMAND_SAVE, EMULATOR_SESSION_MACHINE_PAUSED,
        ".", &result));
    assert(lib_text_find_substring(result.text, "Cannot save machine state.") != LIB_NULL);
    /* Failed capture must not even create a previously absent destination. */
    capture = 0;
    assert(extensions.submit_snapshot(LIB_NULL, machine,
        EMULATOR_PRODUCT_MONITOR_COMMAND_SAVE, EMULATOR_SESSION_MACHINE_RUNNING,
        path, &result));
    {
        lib_storage_file_reader *reader = LIB_NULL;
        assert(lib_storage_file_reader_open(path, &reader) != LIB_STATUS_OK);
        assert(reader == LIB_NULL);
    }
    for (index = 0; index < 3u; ++index) {
        assert(extensions.submit(LIB_NULL, machine, EMULATOR_SESSION_MACHINE_PAUSED,
            commands[index], &result));
        assert(media_calls == index + 1u && media_mode == modes[index]);
    }
    assert(extensions.submit(LIB_NULL, machine, EMULATOR_SESSION_MACHINE_PAUSED,
        "floppy insert invalid Disk Image.img", &result));
    assert(media_calls == 3u);
    lib_c_printf("SoftPC extensions: deferred output and media modes OK\n");
    return 0;
}
