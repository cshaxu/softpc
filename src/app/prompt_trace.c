#include "prompt_trace.h"
#include "lib/storage/file.h"

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
    lib_storage_file_writer *writer;
    const char *trace_path = app_prompt_trace_get_path();

    if (trace_path == NULL) return;
    if (lib_storage_file_writer_open(trace_path,
            LIB_STORAGE_FILE_WRITER_TRUNCATE, &writer) != LIB_STATUS_OK)
        return;
    (void)lib_storage_file_writer_write(writer,
        "Insignia SoftPC Prompt transition trace\n");
    (void)lib_storage_file_writer_close(writer);
}

void app_prompt_trace(const char *format, ...)
{
    va_list arguments;
    va_list copied_arguments;
    char *text;
    int length;
    lib_storage_file_writer *writer;
    const char *trace_path;

    if (!app_prompt_trace_enabled()) return;
    trace_path = app_prompt_trace_get_path();
    if (trace_path == NULL) return;
    va_start(arguments, format);
    va_copy(copied_arguments, arguments);
    length = vsnprintf(NULL, 0u, format, copied_arguments);
    va_end(copied_arguments);
    if (length < 0 || (text = malloc((size_t)length + 2u)) == NULL) {
        va_end(arguments);
        return;
    }
    (void)vsnprintf(text, (size_t)length + 1u, format, arguments);
    va_end(arguments);
    text[length] = '\n';
    text[length + 1] = '\0';
    if (lib_storage_file_writer_open(trace_path,
            LIB_STORAGE_FILE_WRITER_APPEND, &writer) == LIB_STATUS_OK) {
        (void)lib_storage_file_writer_write(writer, text);
        (void)lib_storage_file_writer_close(writer);
    }
    free(text);
}
