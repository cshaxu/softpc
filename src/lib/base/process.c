#include "lib/base/process_interface.h"
#include "lib/base/process.h"

lib_status base_process_executable_directory(char *output, lib_size capacity)
{
    char *path;
    char *separator;
    lib_status status;

    if (output == LIB_NULL || capacity < 2u) return LIB_STATUS_INVALID_ARGUMENT;
    path = lib_allocate(capacity);
    if (path == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    status = base_process_platform_executable_path(path, capacity);
    if (status == LIB_STATUS_OK) {
        separator = path + lib_text_length(path);
        while (separator != path && separator[-1] != '/' && separator[-1] != '\\') --separator;
        if (separator == path) status = LIB_STATUS_IO_ERROR;
        else {
            char *root = path + 1;
            if (path[1] == ':') root = path + 3;
            else if ((path[0] == '\\' || path[0] == '/') && path[1] == path[0]) {
                lib_u32 component;
                root = path + 2;
                /* UNC roots contain server/share; extended UNC has a prefix. */
                if (lib_text_compare_n(root, "?\\UNC\\", 6u) == 0) root += 6;
                for (component = 0u; component != 2u && root < separator; ++component) {
                    while (root < separator && *root != '/' && *root != '\\') ++root;
                    if (root < separator) ++root;
                }
            }
            if (separator > root) --separator;
            *separator = '\0';
            lib_memory_copy(output, path, lib_text_length(path) + 1u);
        }
    }
    lib_release(path);
    return status;
}
