#include "app/config.h"
#include "lib/storage/file_interface.h"
#include <stdlib.h>
#include <string.h>
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

static const char *input;
lib_status lib_storage_file_read_owned(const char *path, lib_size limit,
    void **bytes, lib_size *count)
{
    (void)path;
    *count = strlen(input);
    assert(*count <= limit);
    *bytes = malloc(*count + 1u);
    assert(*bytes != NULL);
    memcpy(*bytes, input, *count + 1u);
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
    assert(strcmp(config.floppy_path, "disk.img") == 0);
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
    assert(strcmp(path, "C:/test/disk.img") == 0);
    assert(app_resolve_image_path(path, "D:/else/softpc.ini"));
    assert(strcmp(path, "C:/test/disk.img") == 0);
    path[0] = '\0';
    assert(app_resolve_image_path(path, "no-directory"));
    assert(app_get_config_path(path));
    assert(strstr(path, "softpc.ini") != NULL);
    return 0;
}
