#include "lib/types/test.h"
#include "lib/base/process_interface.h"
#include "lib/base/process.h"

static const char *executable;
static lib_status native_status;

lib_status base_process_platform_executable_path(char *output, lib_size capacity)
{
    lib_size required = lib_text_length(executable) + 1u;
    if (native_status != LIB_STATUS_OK) return native_status;
    if (required > capacity) return LIB_STATUS_IO_ERROR;
    lib_memory_copy(output, executable, required);
    return LIB_STATUS_OK;
}

int main(void)
{
    static const struct { const char *path; const char *directory; } cases[] = {
        {"/bin/program", "/bin"},
        {"/program", "/"},
        {"C:\\tools\\program.exe", "C:\\tools"},
        {"C:\\program.exe", "C:\\"},
        {"C:/program.exe", "C:/"},
        {"\\\\server\\share\\tools\\program.exe", "\\\\server\\share\\tools"},
        {"\\\\server\\share\\program.exe", "\\\\server\\share\\"},
        {"\\\\?\\C:\\program.exe", "\\\\?\\C:\\"},
        {"\\\\?\\UNC\\server\\share\\program.exe", "\\\\?\\UNC\\server\\share\\"},
        {"\\\\?\\UNC\\server\\share\\tools\\program.exe", "\\\\?\\UNC\\server\\share\\tools"}
    };
    char output[128];
    lib_size index;
    for (index = 0u; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        executable = cases[index].path;
        lib_test_assert(base_process_executable_directory(output,
            lib_text_length(executable) + 1u) == LIB_STATUS_OK);
        lib_test_assert(lib_text_compare(output, cases[index].directory) == 0);
        lib_memory_copy(output, "unchanged", sizeof("unchanged"));
        lib_test_assert(base_process_executable_directory(output,
            lib_text_length(executable)) == LIB_STATUS_IO_ERROR);
        lib_test_assert(lib_text_compare(output, "unchanged") == 0);
    }
    executable = "program.exe";
    lib_test_assert(base_process_executable_directory(output, sizeof(output)) == LIB_STATUS_IO_ERROR);
    lib_test_assert(lib_text_compare(output, "unchanged") == 0);
    native_status = LIB_STATUS_UNSUPPORTED;
    lib_test_assert(base_process_executable_directory(output, sizeof(output)) == native_status);
    lib_test_assert(lib_text_compare(output, "unchanged") == 0);
    lib_test_assert(base_process_executable_directory(output, 1u) == LIB_STATUS_INVALID_ARGUMENT);
    lib_test_assert(base_process_executable_directory(LIB_NULL, sizeof(output)) == LIB_STATUS_INVALID_ARGUMENT);
    return 0;
}
