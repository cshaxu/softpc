#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "common/session/session_interface.h"
#include "lib/storage/medium_interface.h"

#define SOFTPC_CONFIG_PATH_MAX 1024u

typedef struct app_startup_config {
    char floppy_path[SOFTPC_CONFIG_PATH_MAX];
    char hard_disk_path[SOFTPC_CONFIG_PATH_MAX];
    char serial_output_path[SOFTPC_CONFIG_PATH_MAX];
    char printer_output_path[SOFTPC_CONFIG_PATH_MAX];
    lib_u32 memory_bytes;
    common_session_display presentation;
    int console_control;
    lib_storage_medium_mode media_mode;
} app_startup_config;

int app_get_config_path(char *path);
int app_resolve_image_path(char *path, const char *config_path);
int app_load_startup_config(const char *path, app_startup_config *config);

#endif
