#include "prompt_trace.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static char app_prompt_trace_path[MAX_PATH];

static const char *app_prompt_trace_get_path(void)
{
    char module[MAX_PATH];
    char *separator;

    if (app_prompt_trace_path[0] != '\0') return app_prompt_trace_path;
    if (!GetModuleFileNameA(NULL, module, sizeof(module))) return NULL;
    separator = strrchr(module, '\\');
    if (separator == NULL) return NULL;
    *separator = '\0';
    if (snprintf(app_prompt_trace_path, sizeof(app_prompt_trace_path),
            "%s\\..\\..\\build\\softpc-prompt-trace.log", module) <= 0)
        return NULL;
    return app_prompt_trace_path;
}

int app_prompt_trace_enabled(void)
{
    /* T28 S1 is deliberately self-contained: a package run always leaves
       compact transition evidence in the repository build directory. */
    return 1;
}

void app_prompt_trace_reset(void)
{
    FILE *file;
    const char *trace_path = app_prompt_trace_get_path();

    if (trace_path == NULL) return;
    file = fopen(trace_path, "w");
    if (file == NULL) return;
    fputs("Insignia SoftPC Prompt transition trace\n", file);
    fclose(file);
}

void app_prompt_trace(const char *format, ...)
{
    FILE *file;
    va_list arguments;
    const char *trace_path;

    if (!app_prompt_trace_enabled()) return;
    trace_path = app_prompt_trace_get_path();
    if (trace_path == NULL) return;
    file = fopen(trace_path, "a");
    if (file == NULL) return;
    va_start(arguments, format);
    vfprintf(file, format, arguments);
    va_end(arguments);
    fputc('\n', file);
    fclose(file);
}
