#include <ctype.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

#ifndef SOFTPC_PACKAGE_DIRECTORY
#error SOFTPC_PACKAGE_DIRECTORY is required
#endif
#ifndef SOFTPC_PACKAGE_EXECUTABLE
#error SOFTPC_PACKAGE_EXECUTABLE is required
#endif
#ifndef SOFTPC_PACKAGE_MEDIA_DIRECTORY
#error SOFTPC_PACKAGE_MEDIA_DIRECTORY is required
#endif

static char *trim(char *text)
{
    char *end;
    while (*text != '\0' && isspace((unsigned char)*text)) ++text;
    end = text + strlen(text);
    while (end != text && isspace((unsigned char)end[-1])) --end;
    *end = '\0';
    return text;
}

static int absolute_path(const char *path, char *resolved, DWORD capacity)
{
    DWORD length = GetFullPathNameA(path, capacity, resolved, NULL);
    return length != 0u && length < capacity;
}

static int is_below(const char *candidate, const char *root)
{
    char full_candidate[MAX_PATH];
    char full_root[MAX_PATH];
    size_t root_length;

    if (!absolute_path(candidate, full_candidate, sizeof(full_candidate)) ||
        !absolute_path(root, full_root, sizeof(full_root))) return 0;
    root_length = strlen(full_root);
    if (root_length != 0u && full_root[root_length - 1u] != '\\') {
        if (root_length + 1u >= sizeof(full_root)) return 0;
        full_root[root_length++] = '\\';
        full_root[root_length] = '\0';
    }
    return _strnicmp(full_candidate, full_root, root_length) == 0;
}

static int resolve_package_value(const char *value, char *resolved,
    size_t capacity)
{
    int length;
    if ((value[0] == '\\' || value[0] == '/') ||
        (isalpha((unsigned char)value[0]) && value[1] == ':' &&
            (value[2] == '\\' || value[2] == '/')))
        length = snprintf(resolved, capacity, "%s", value);
    else length = snprintf(resolved, capacity, "%s\\%s",
        SOFTPC_PACKAGE_DIRECTORY, value);
    return length > 0 && (size_t)length < capacity;
}

static int verify_fixed_ini(void)
{
    char ini_path[MAX_PATH];
    char floppy[MAX_PATH] = { 0 };
    char hard_disk[MAX_PATH] = { 0 };
    char line[1200];
    FILE *file;
    int valid = 1;

    if (snprintf(ini_path, sizeof(ini_path), "%s\\softpc.ini",
            SOFTPC_PACKAGE_DIRECTORY) <= 0) return 0;
    file = fopen(ini_path, "r");
    if (file == NULL) return 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        char *equals = strchr(line, '=');
        char *key;
        char *value;
        char *comment = strchr(line, ';');
        if (comment != NULL) *comment = '\0';
        if (equals == NULL) continue;
        *equals = '\0';
        key = trim(line);
        value = trim(equals + 1);
        if (strcmp(key, "floppy") == 0) {
            if (!resolve_package_value(value, floppy, sizeof(floppy))) valid = 0;
        } else if (strcmp(key, "hard_disk") == 0) {
            if (!resolve_package_value(value, hard_disk, sizeof(hard_disk))) valid = 0;
        } else if (strcmp(key, "media_mode") == 0 &&
            strcmp(value, "overlay") != 0 && strcmp(value, "readonly") != 0) {
            valid = 0;
        }
    }
    fclose(file);
    return valid && floppy[0] != '\0' && hard_disk[0] != '\0' &&
        is_below(floppy, SOFTPC_PACKAGE_MEDIA_DIRECTORY) &&
        is_below(hard_disk, SOFTPC_PACKAGE_MEDIA_DIRECTORY) &&
        GetFileAttributesA(floppy) != INVALID_FILE_ATTRIBUTES &&
        GetFileAttributesA(hard_disk) != INVALID_FILE_ATTRIBUTES;
}

int main(void)
{
    STARTUPINFOA startup = { sizeof(startup) };
    PROCESS_INFORMATION process = { 0 };
    DWORD wait_result;

    if (!verify_fixed_ini()) {
        fputs("softpc-package-smoke: invalid fixed package configuration\n", stderr);
        return 1;
    }
    /* NULL command line is intentional: the package has no CLI surface. */
    if (!CreateProcessA(SOFTPC_PACKAGE_EXECUTABLE, NULL, NULL, NULL, FALSE,
            CREATE_NO_WINDOW, NULL, SOFTPC_PACKAGE_DIRECTORY, &startup,
            &process)) {
        fprintf(stderr, "softpc-package-smoke: CreateProcess failed: %lu\n",
            (unsigned long)GetLastError());
        return 1;
    }
    wait_result = WaitForSingleObject(process.hProcess, 1500u);
    if (wait_result != WAIT_TIMEOUT) {
        fputs("softpc-package-smoke: package exited before monitor readiness\n",
            stderr);
        CloseHandle(process.hThread);
        CloseHandle(process.hProcess);
        return 1;
    }
    TerminateProcess(process.hProcess, 0u);
    wait_result = WaitForSingleObject(process.hProcess, 5000u);
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return wait_result == WAIT_OBJECT_0 ? 0 : 1;
}
#else
int main(void) { return 1; }
#endif
