#include "console.h"
#include "softpc_machine.h"
#include "win32_window.h"

#include <windows.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOFTPC_CONFIG_PATH_MAX 1024u

typedef struct softpc_startup_config {
    char floppy_path[SOFTPC_CONFIG_PATH_MAX];
    char hard_disk_path[SOFTPC_CONFIG_PATH_MAX];
    uint32_t memory_bytes;
    softpc_presentation presentation;
    softpc_media_mode media_mode;
} softpc_startup_config;

static char *softpc_trim(char *text)
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

static int softpc_copy_value(char *target, const char *value)
{
    size_t length = strlen(value);
    if (length >= SOFTPC_CONFIG_PATH_MAX) return 0;
    memcpy(target, value, length + 1u);
    return 1;
}

static int softpc_get_config_path(char *path)
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

static int softpc_path_is_absolute(const char *path)
{
    return path[0] == '/' || path[0] == '\\' ||
        (isalpha((unsigned char)path[0]) && path[1] == ':' &&
            (path[2] == '/' || path[2] == '\\'));
}

static int softpc_resolve_image_path(char *path, const char *config_path)
{
    const char *separator;
    const char *forward_separator;
    char resolved[SOFTPC_CONFIG_PATH_MAX];
    size_t directory_length;
    size_t image_length;

    if (path[0] == '\0' || softpc_path_is_absolute(path)) return 1;
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
    return softpc_copy_value(path, resolved);
}

static int softpc_load_startup_config(const char *path,
    softpc_startup_config *config)
{
    FILE *file = fopen(path, "r");
    char line[1200];
    if (file == NULL) return 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        char *key;
        char *value;
        char *equals = strchr(line, '=');
        char *comment = strchr(line, '#');
        char *semicolon = strchr(line, ';');
        if (semicolon != NULL && (comment == NULL || semicolon < comment))
            comment = semicolon;
        if (comment != NULL) *comment = '\0';
        if (equals == NULL) continue;
        *equals = '\0';
        key = softpc_trim(line);
        value = softpc_trim(equals + 1);
        if (*key == '\0') continue;
        if (strcmp(key, "memory_mb") == 0) {
            char *end;
            unsigned long mib = strtoul(value, &end, 10);
            if (*end != '\0' || mib == 0u || mib > 4095u) goto invalid;
            config->memory_bytes = (uint32_t)(mib * 1024u * 1024u);
        } else if (strcmp(key, "floppy") == 0) {
            if (!softpc_copy_value(config->floppy_path, value)) goto invalid;
        } else if (strcmp(key, "hard_disk") == 0) {
            if (!softpc_copy_value(config->hard_disk_path, value)) goto invalid;
        } else if (strcmp(key, "display") == 0) {
            if (strcmp(value, "console") == 0)
                config->presentation = SOFTPC_PRESENTATION_CONSOLE;
            else if (strcmp(value, "window") == 0)
                config->presentation = SOFTPC_PRESENTATION_WINDOW;
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
    }
    fclose(file);
    return 1;
invalid:
    fclose(file);
    return 0;
}

int main(int argc, char **argv)
{
    char config_path[SOFTPC_CONFIG_PATH_MAX];
    softpc_startup_config config = { { 0 }, { 0 }, 16u * 1024u * 1024u,
        SOFTPC_PRESENTATION_CONSOLE, SOFTPC_MEDIA_OVERLAY };
    softpc_machine_options options = { 0 };
    softpc_machine *machine = NULL;
    softpc_machine_result result;
    (void)argv;

    if (argc != 1) {
        fprintf(stderr, "softpcvm: command-line arguments are not supported\n");
        return 2;
    }
    if (!softpc_get_config_path(config_path)) {
        fprintf(stderr, "softpcvm: cannot determine adjacent softpc.ini path\n");
        return 1;
    }
    if (!softpc_load_startup_config(config_path, &config)) {
        fprintf(stderr, "softpcvm: cannot read fixed-machine config '%s'\n",
            config_path);
        return 1;
    }
    if (!softpc_resolve_image_path(config.floppy_path, config_path) ||
        !softpc_resolve_image_path(config.hard_disk_path, config_path)) {
        fprintf(stderr, "softpcvm: image path in '%s' is too long\n", config_path);
        return 1;
    }
    options.floppy_path = config.floppy_path[0] == '\0' ? NULL : config.floppy_path;
    options.hard_disk_path = config.hard_disk_path[0] == '\0' ? NULL : config.hard_disk_path;
    options.memory_bytes = config.memory_bytes;
    options.presentation = config.presentation;
    options.media_mode = config.media_mode;
    result = softpc_machine_create(&options, &machine);
    if (result != SOFTPC_MACHINE_OK) goto done;
    result = softpc_machine_reset(machine);
    if (result == SOFTPC_MACHINE_OK &&
        options.presentation == SOFTPC_PRESENTATION_WINDOW) {
        if (softpc_vm_run_window(machine) != 0) result = SOFTPC_MACHINE_IO_ERROR;
    } else if (result == SOFTPC_MACHINE_OK &&
        softpc_vm_run_console(machine) != 0) result = SOFTPC_MACHINE_IO_ERROR;
done:
    if (result != SOFTPC_MACHINE_OK)
        fprintf(stderr, "softpcvm: %s\n", softpc_machine_result_name(result));
    softpc_machine_destroy(machine);
    return result != SOFTPC_MACHINE_OK;
}
