#include "console.h"
#include "softpc_machine.h"
#include "win32_window.h"

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

static int softpc_load_startup_config(const char *path,
    softpc_startup_config *config)
{
    FILE *file = fopen(path, "r");
    char line[1200];
    if (file == NULL) return 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        char *key;
        char *value;
        char *colon = strchr(line, ':');
        char *comment = strchr(line, '#');
        if (comment != NULL) *comment = '\0';
        if (colon == NULL) continue;
        *colon = '\0';
        key = softpc_trim(line);
        value = softpc_trim(colon + 1);
        if (*key == '\0' || *value == '\0') continue;
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
        } else goto invalid;
    }
    fclose(file);
    return 1;
invalid:
    fclose(file);
    return 0;
}

static void softpc_usage(const char *program)
{
    fprintf(stderr, "Usage: %s [--config softpc.yaml]\n", program);
}

int main(int argc, char **argv)
{
    const char *config_path = "softpc.yaml";
    softpc_startup_config config = { { 0 }, { 0 }, 16u * 1024u * 1024u,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine_options options = { 0 };
    softpc_machine *machine = NULL;
    softpc_machine_result result;
    int index;

    for (index = 1; index < argc; ++index) {
        if (!strcmp(argv[index], "--help") || !strcmp(argv[index], "-h")) {
            softpc_usage(argv[0]);
            return 0;
        }
        if (!strcmp(argv[index], "--config") && index + 1 < argc) {
            config_path = argv[++index];
            continue;
        }
        softpc_usage(argv[0]);
        return 2;
    }
    if (!softpc_load_startup_config(config_path, &config)) {
        fprintf(stderr, "softpcvm: cannot read fixed-machine config '%s'\n",
            config_path);
        return 1;
    }
    options.floppy_path = config.floppy_path[0] == '\0' ? NULL : config.floppy_path;
    options.hard_disk_path = config.hard_disk_path[0] == '\0' ? NULL : config.hard_disk_path;
    options.memory_bytes = config.memory_bytes;
    options.presentation = config.presentation;
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
