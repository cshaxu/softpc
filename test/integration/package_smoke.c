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

static char package_last_screen[4096];
static DWORD package_last_screen_width;
static DWORD package_last_screen_length;
static int package_window_display;
static int package_compact_console;

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
        } else if ((strcmp(key, "floppy_mode") == 0 ||
            strcmp(key, "hard_disk_mode") == 0) &&
            strcmp(value, "overlay") != 0 && strcmp(value, "readonly") != 0 &&
            strcmp(value, "direct") != 0) {
            valid = 0;
        } else if (strcmp(key, "display") == 0) {
            package_window_display = strcmp(value, "window") == 0;
        }
    }
    fclose(file);
    /* A fixed SoftPC may boot either an installed hard disk alone or a
       floppy plus hard disk.  The user-owned adjacent INI therefore makes
       floppy optional; when supplied it must still resolve inside package
       media and exist. */
    return valid && hard_disk[0] != '\0' &&
        is_below(hard_disk, SOFTPC_PACKAGE_MEDIA_DIRECTORY) &&
        GetFileAttributesA(hard_disk) != INVALID_FILE_ATTRIBUTES &&
        (floppy[0] == '\0' ||
            (is_below(floppy, SOFTPC_PACKAGE_MEDIA_DIRECTORY) &&
             GetFileAttributesA(floppy) != INVALID_FILE_ATTRIBUTES));
}

static int package_send_key_event(HANDLE input, WORD virtual_key,
    WORD scan_code, CHAR character, DWORD modifiers, int pressed)
{
    INPUT_RECORD record = { 0 };
    DWORD written = 0u;

    record.EventType = KEY_EVENT;
    record.Event.KeyEvent.bKeyDown = pressed != 0;
    record.Event.KeyEvent.wRepeatCount = 1u;
    record.Event.KeyEvent.wVirtualKeyCode = virtual_key;
    record.Event.KeyEvent.wVirtualScanCode = scan_code;
    record.Event.KeyEvent.uChar.AsciiChar = pressed != 0 ? character : 0;
    record.Event.KeyEvent.dwControlKeyState = modifiers;
    return WriteConsoleInputA(input, &record, 1u, &written) && written == 1u;
}

static int package_send_key(HANDLE input, WORD virtual_key, WORD scan_code,
    CHAR character, DWORD modifiers)
{
    return package_send_key_event(input, virtual_key, scan_code, character,
        modifiers, 1) && package_send_key_event(input, virtual_key, scan_code,
        0, modifiers, 0);
}

static int package_send_text(HANDLE input, const char *text)
{
    size_t index;
    if (input == INVALID_HANDLE_VALUE || text == NULL) return 0;
    for (index = 0u; text[index] != '\0'; ++index) {
        CHAR character = text[index];
        WORD virtual_key = character == '\r' ? VK_RETURN : 0u;
        if (!package_send_key(input, virtual_key, 0u, character, 0u)) return 0;
    }
    return 1;
}

static int package_send_pause_hotkey(HANDLE input)
{
    /* Console records carry their current modifier mask with the trigger
       transition.  The raw adapter contracts on that copied fact, so this
       is the smallest deterministic representation of host Ctrl+Alt+P. */
    return package_send_key_event(input, 'P', 0x19u, 'p',
            LEFT_CTRL_PRESSED | LEFT_ALT_PRESSED, 1) &&
        package_send_key_event(input, 'P', 0x19u, 0,
            LEFT_CTRL_PRESSED | LEFT_ALT_PRESSED, 0);
}

static int package_screen_contains(HANDLE output, const char *needle)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    char text[4096];
    DWORD read = 0u;
    COORD origin;
    DWORD count;

    if (output == INVALID_HANDLE_VALUE || needle == NULL) return 0;
    /* CONOUT$ resolves the currently displayed buffer at open time. A saved
       handle still observes cooked history while the raw surface is active. */
    output = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0u, NULL);
    if (output == INVALID_HANDLE_VALUE) return 0;
    if (!GetConsoleScreenBufferInfo(output, &info)) {
        CloseHandle(output);
        return 0;
    }
    origin.X = info.srWindow.Left;
    origin.Y = info.srWindow.Top;
    count = (DWORD)(info.srWindow.Right - info.srWindow.Left + 1) *
        (DWORD)(info.srWindow.Bottom - info.srWindow.Top + 1);
    if (count >= sizeof(text)) count = sizeof(text) - 1u;
    if (!ReadConsoleOutputCharacterA(output, text, count, origin, &read)) {
        CloseHandle(output);
        return 0;
    }
    CloseHandle(output);
    text[read] = '\0';
    memcpy(package_last_screen, text, read + 1u);
    package_last_screen_width =
        (DWORD)(info.srWindow.Right - info.srWindow.Left + 1);
    package_last_screen_length = read;
    return strstr(text, needle) != NULL;
}

static void package_report_last_screen(void)
{
    DWORD offset;
    if (package_last_screen_width == 0u || package_last_screen_length == 0u)
        return;
    fputs("softpc-package-smoke: final Console viewport:\n", stderr);
    for (offset = 0u; offset < package_last_screen_length;
        offset += package_last_screen_width) {
        DWORD length = package_last_screen_width;
        if (length > package_last_screen_length - offset)
            length = package_last_screen_length - offset;
        fwrite(package_last_screen + offset, 1u, length, stderr);
        fputc('\n', stderr);
    }
}

static int package_wait_for_text(HANDLE output, const char *needle,
    DWORD timeout_ms)
{
    DWORD deadline = GetTickCount() + timeout_ms;
    do {
        if (package_screen_contains(output, needle)) return 1;
        Sleep(20u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

static int package_wait_debug_prompt(HANDLE output)
{
    DWORD deadline = GetTickCount() + 5000u;
    do {
        CONSOLE_SCREEN_BUFFER_INFO info;
        COORD position;
        char character;
        DWORD read;
        if (GetConsoleScreenBufferInfo(output, &info) && info.dwCursorPosition.X == 1) {
            position = info.dwCursorPosition;
            position.X = 0;
            if (ReadConsoleOutputCharacterA(output, &character, 1u, position, &read) &&
                read == 1u && character == '-') return 1;
        }
        Sleep(20u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

static int package_wait_for_absent_text(HANDLE output, const char *needle,
    DWORD timeout_ms)
{
    DWORD deadline = GetTickCount() + timeout_ms;
    do {
        if (!package_screen_contains(output, needle)) return 1;
        Sleep(20u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

typedef struct package_window_probe { DWORD process; HWND window; } package_window_probe;
static BOOL CALLBACK package_find_window(HWND window, LPARAM opaque)
{
    package_window_probe *probe = (package_window_probe *)opaque;
    DWORD process;
    char name[64];
    GetWindowThreadProcessId(window, &process);
    if (process == probe->process && GetClassNameA(window, name, sizeof(name)) &&
        strcmp(name, "LibKvmWindow") == 0) probe->window = window;
    return TRUE;
}

static int package_wait_window(DWORD process, const char *state)
{
    DWORD deadline = GetTickCount() + 10000u;
    do {
        package_window_probe probe = { process, NULL };
        char title[128];
        EnumWindows(package_find_window, (LPARAM)&probe);
        if (state == NULL && probe.window == NULL) return 1;
        if (state != NULL && probe.window != NULL && IsWindowVisible(probe.window) &&
            GetWindowTextA(probe.window, title, sizeof(title)) && strstr(title, state))
            return SendMessageTimeoutA(probe.window, WM_NULL, 0, 0,
                SMTO_ABORTIFHUNG, 1000u, NULL) != 0;
        Sleep(20u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

/* A Window-configured package keeps this Console cooked. Assert that actual
 * route instead of looking for DOS pixels in the monitor, without editing INI.
 * DOS boot/frame correctness is independently covered by the runtime suites. */
static int package_debug_execution(HANDLE input, HANDLE output)
{
    /* Disposable RAM only; the surrounding test stops and cold-starts next.
     * Unique IP values prove actual G/T completion through the shipping queue,
     * not an old prompt or a library-only callback. */
    return package_send_text(input, "debug\r") &&
        package_wait_debug_prompt(output) &&
        package_send_text(input, "e 0:500 fa 90 90 90 90 eb fe\r") &&
        package_send_text(input, "g 0:500 0:501\r") &&
        package_wait_for_text(output, "IP=0501", 5000u) &&
        package_send_text(input, "t 2\r") &&
        package_wait_for_text(output, "IP=0503", 5000u) &&
        package_send_text(input, "xw e 504\r") &&
        package_send_text(input, "g\r") &&
        package_wait_for_text(output, "Watch-execute hit: Lin=00000504", 5000u) &&
        package_send_text(input, "xw u\r") &&
        package_wait_for_text(output, "All watch points removed.", 5000u) &&
        package_send_text(input, "q\r");
}

static int package_window_restart(PROCESS_INFORMATION *process, HANDLE input, HANDLE output)
{
    if (!package_wait_for_text(output, "Machine started.", 10000u) ||
        !package_wait_window(process->dwProcessId, "Running")) return 0;
    if (!package_send_text(input, "pause\r") ||
        !package_wait_for_text(output, "Machine paused.", 5000u) ||
        !package_wait_window(process->dwProcessId, "Paused")) return 0;
    if (!package_debug_execution(input, output)) return 0;
    if (!package_send_text(input, "stop\r") ||
        !package_wait_for_text(output, "Machine stopped.", 5000u) ||
        !package_wait_window(process->dwProcessId, NULL)) return 0;
    if (!package_send_text(input, "start\r") ||
        !package_wait_window(process->dwProcessId, "Running")) return 0;
    /* A fresh monitor command proves input has been rearmed after restart. */
    return package_send_text(input, "pause\r") &&
        package_wait_window(process->dwProcessId, "Paused");
}

/* This drives the shipping executable through the same native Console route
 * as the reported failure.  It intentionally does not call app_runtime_*
 * directly: monitor lines, raw CAP recognition, reconciler actions, broker
 * replacement, and the new cold run must all occur in the child package. */
static int verify_package_monitor_restart(PROCESS_INFORMATION *process,
    int *out_stage, DWORD *out_error)
{
    HANDLE input = INVALID_HANDLE_VALUE;
    HANDLE output = INVALID_HANDLE_VALUE;
    DWORD deadline;
    int stage = 0;
    int success = 0;

    if (out_stage != NULL) *out_stage = stage;
    if (out_error != NULL) *out_error = ERROR_SUCCESS;
    if (process == NULL) return 0;
    (void)FreeConsole();
    deadline = GetTickCount() + 5000u;
    do {
        if (AttachConsole(process->dwProcessId)) break;
        Sleep(20u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    if (GetConsoleCP() == 0u) { stage = 1; goto done; }
    if (!package_window_display && IsWindowVisible(GetConsoleWindow())) {
        stage = 20; goto done;
    }
    input = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0u, NULL);
    output = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0u, NULL);
    if (input == INVALID_HANDLE_VALUE || output == INVALID_HANDLE_VALUE) {
        stage = 2; goto done;
    }
    /* This hidden test-owned Console needs room for an 80x25 raw surface.
     * Narrow desktops can make the inherited font too large. Change only
     * this child's fixture font, never the user's Console or product policy. */
    if (!package_window_display) {
        COORD maximum = GetLargestConsoleWindowSize(output);
        if (maximum.X < 80 || maximum.Y < 25) {
            CONSOLE_FONT_INFOEX font = {0};
            font.cbSize = sizeof(font);
            font.dwFontSize.Y = 8;
            font.FontFamily = FF_MODERN;
            font.FontWeight = FW_NORMAL;
            memcpy(font.FaceName, L"Consolas", sizeof(L"Consolas"));
            if (!SetCurrentConsoleFontEx(output, FALSE, &font)) { stage = 21; goto done; }
            maximum = GetLargestConsoleWindowSize(output);
            if (maximum.X < 80 || maximum.Y < 25) { stage = 21; goto done; }
        }
    }
    if (package_compact_console) {
        SMALL_RECT viewport = {0, 0, 29, 11};
        COORD extent = {30, 12};
        /* Only this test-owned child Console is resized, never the parent. */
        if (!SetConsoleWindowInfo(output, TRUE, &viewport) ||
            !SetConsoleScreenBufferSize(output, extent)) { stage = 19; goto done; }
    }
    if (!package_wait_for_text(output, "SoftPC>", 5000u)) { stage = 3; goto done; }
    /* Exercise the shipping CLI provider, not only the debug library link.
       Entering before start must leave the machine stopped and permit help. */
    if (!package_compact_console && (!package_send_text(input, "debug\r") ||
        !package_wait_debug_prompt(output) ||
        !package_send_text(input, "r\r") ||
        !package_wait_for_text(output, "Machine must be paused", 5000u) ||
        !package_send_text(input, "?\r") ||
        !package_wait_for_text(output, "debug32", 5000u) ||
        !package_send_text(input, "q\r"))) { stage = 17; goto done; }
    if (!package_send_text(input, "start\r")) { stage = 4; goto done; }
    if (package_window_display) {
        success = package_window_restart(process, input, output);
        stage = 16;
        goto done;
    }
    if (!package_wait_for_text(output, "C:\\>", 10000u)) { stage = 5; goto done; }
    if (IsWindowVisible(GetConsoleWindow())) { stage = 20; goto done; }
    if (!package_send_text(input, "ver\r") ||
        !package_wait_for_text(output, "Version", 5000u)) { stage = 14; goto done; }
    if (!package_send_text(input, "cls\r") ||
        !package_wait_for_absent_text(output, "Version", 5000u)) { stage = 15; goto done; }
    if (!package_send_pause_hotkey(input)) { stage = 6; goto done; }
    /* The product attaches the monitor Console only after PAUSED completion;
       the visible proof is its rearmed cooked prompt, not text written while
       the former VM Console was still Current. */
    if (!package_wait_for_text(output, "SoftPC>", 5000u)) {
        stage = 7; goto done;
    }
    /* Debugger column-layout assertions belong to the normal-size route. */
    if (!package_compact_console && !package_debug_execution(input, output)) {
        stage = 18; goto done;
    }
    if (!package_send_text(input, "stop\r")) { stage = 8; goto done; }
    if (!package_wait_for_text(output, "Machine stopped.", 5000u)) {
        stage = 9; goto done;
    }
    if (!package_send_text(input, "start\r")) { stage = 10; goto done; }
    /* The Console screen retains its last cells until the new raw renderer
       writes.  Require the former DOS prompt to disappear, then observe the
       fresh boot banner and a newly rendered prompt in that order: otherwise
       a stale first-run C:\\> would be a false green result. */
    if (!package_wait_for_absent_text(output, "C:\\>", 5000u)) {
        stage = 11; goto done;
    }
    if (!package_wait_for_text(output, "Starting MS-DOS", 10000u)) {
        stage = 12; goto done;
    }
    if (!package_wait_for_text(output, "C:\\>", 10000u)) { stage = 13; goto done; }
    success = 1;
done:
    if (!success && out_stage != NULL) *out_stage = stage;
    if (!success && out_error != NULL) *out_error = GetLastError();
    if (input != INVALID_HANDLE_VALUE) CloseHandle(input);
    if (output != INVALID_HANDLE_VALUE) CloseHandle(output);
    (void)FreeConsole();
    return success;
}

int main(int argc, char **argv)
{
    STARTUPINFOA startup = { sizeof(startup) };
    PROCESS_INFORMATION process = { 0 };
    DWORD wait_result;
    DWORD error = ERROR_SUCCESS;
    int stage = 0;

    package_compact_console = argc == 2 && strcmp(argv[1], "--compact-console") == 0;
    if (argc != 1 && !package_compact_console) return 1;

    if (!verify_fixed_ini()) {
        fputs("softpc-package-smoke: invalid fixed package configuration\n", stderr);
        return 1;
    }
    /* NULL command line is intentional: the package has no CLI surface. */
    /* Observe the test-owned Console through handles, not a foreground window
       that can receive the developer's keyboard input during this test. */
    startup.dwFlags = STARTF_USESHOWWINDOW;
    startup.wShowWindow = SW_HIDE;
    if (!CreateProcessA(SOFTPC_PACKAGE_EXECUTABLE, NULL, NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, SOFTPC_PACKAGE_DIRECTORY, &startup,
            &process)) {
        fprintf(stderr, "softpc-package-smoke: CreateProcess failed: %lu\n",
            (unsigned long)GetLastError());
        return 1;
    }
    if (!verify_package_monitor_restart(&process, &stage, &error)) {
        fprintf(stderr, "softpc-package-smoke: configured monitor pause-stop-start route failed (stage=%d error=%lu)\n",
            stage, (unsigned long)error);
        package_report_last_screen();
        TerminateProcess(process.hProcess, 1u);
        (void)WaitForSingleObject(process.hProcess, 5000u);
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
