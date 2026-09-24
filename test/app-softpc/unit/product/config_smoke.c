#include "lib/types/types_interface.h"
#include "product/config.h"
#include "lib/storage/file_interface.h"
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

static const char *input;
lib_status lib_storage_file_read_owned(const char *path, lib_size limit,
    void **bytes, lib_size *count)
{
    (void)path;
    *count = lib_text_length(input);
    assert(*count <= limit);
    *bytes = lib_allocate(*count + 1u);
    assert(*bytes != NULL);
    lib_memory_copy(*bytes, input, *count + 1u);
    return LIB_STATUS_OK;
}

int main(void)
{
    app_startup_config config = { {0}, {0}, {0}, {0}, 16u * 1024u * 1024u,
        COMMON_SESSION_DISPLAY_CONSOLE, 1, LIB_STORAGE_MEDIUM_OVERLAY,
        LIB_STORAGE_MEDIUM_OVERLAY };
    char path[SOFTPC_CONFIG_PATH_MAX] = "disk.img";
    input = "# comment\r\n; comment\r\nfloppy = \"disk.img\"\r\n"
        "memory_mb=32\ndisplay=window\nconsole_control=0\n"
        "floppy_mode=readonly\nhard_disk_mode=readonly\n";
    assert(app_load_startup_config("unused", &config));
    assert(lib_text_compare(config.floppy_path, "disk.img") == 0);
    assert(config.memory_bytes == 32u * 1024u * 1024u);
    assert(config.presentation == COMMON_SESSION_DISPLAY_WINDOW);
    assert(config.console_control == 0);
    assert(config.floppy_mode == LIB_STORAGE_MEDIUM_READONLY);
    assert(config.hard_disk_mode == LIB_STORAGE_MEDIUM_READONLY);
    input = "floppy_mode=direct\nhard_disk_mode=readonly\ndisplay=console\nconsole_control=1\n";
    assert(app_load_startup_config("unused", &config));
    assert(config.floppy_mode == LIB_STORAGE_MEDIUM_DIRECT);
    assert(config.hard_disk_mode == LIB_STORAGE_MEDIUM_READONLY);
    assert(config.presentation == COMMON_SESSION_DISPLAY_CONSOLE);
    assert(config.console_control == 1);
    input = "media_mode=overlay\n";
    assert(!app_load_startup_config("unused", &config));
    input = "memory_mb=0";
    assert(!app_load_startup_config("unused", &config));
    input = "display=invalid";
    assert(!app_load_startup_config("unused", &config));
    input = "unknown=1";
    assert(!app_load_startup_config("unused", &config));
    assert(app_resolve_image_path(path, "C:/test/softpc.ini"));
    assert(lib_text_compare(path, "C:/test/disk.img") == 0);
    assert(app_resolve_image_path(path, "D:/else/softpc.ini"));
    assert(lib_text_compare(path, "C:/test/disk.img") == 0);
    path[0] = '\0';
    assert(app_resolve_image_path(path, "no-directory"));
    assert(app_get_config_path(path));
    assert(lib_text_find_substring(path, "softpc.ini") != NULL);
    return 0;
}
