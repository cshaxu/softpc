#include "console.h"

#ifdef _WIN32
#include <windows.h>
#include <string.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD KeyEvent);

#define SOFTPC_TEXT_COLUMNS 80u
#define SOFTPC_TEXT_ROWS 25u
/* This remains a bounded CPU slice for keyboard polling, while matching the
 * real-media boot probe closely enough to keep the original POST responsive. */
#define SOFTPC_RUN_SLICE 5000u

/* A console selected in softpc.ini is a presentation choice, not a
 * requirement that the launcher inherited a usable stdin/stdout console.
 * Explorer, GUI launchers, and redirected shells can all start us without
 * CONIN$/CONOUT$ handles.  Create a private console in that case so the VM
 * remains interactive instead of reporting a machine host-I/O failure. */
static int softpc_console_open(HANDLE *input_out, HANDLE *output_out,
    DWORD *original_mode_out, int *private_console_out)
{
    HANDLE input;
    HANDLE output;
    DWORD original_mode;
    int private_console = 0;

    /* Standard handles can be redirected, or can be pseudo-console handles
       that report a console mode without belonging to the visible screen
       buffer.  SoftPC must paint the console device itself. */
    input = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    output = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (input == INVALID_HANDLE_VALUE || output == INVALID_HANDLE_VALUE ||
        !GetConsoleMode(input, &original_mode)) {
        if (input != INVALID_HANDLE_VALUE) CloseHandle(input);
        if (output != INVALID_HANDLE_VALUE) CloseHandle(output);
        if (GetConsoleCP() != 0u || !AllocConsole()) return 0;
        private_console = 1;
        input = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        output = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (input == INVALID_HANDLE_VALUE || output == INVALID_HANDLE_VALUE ||
            !GetConsoleMode(input, &original_mode)) {
            if (input != INVALID_HANDLE_VALUE) CloseHandle(input);
            if (output != INVALID_HANDLE_VALUE) CloseHandle(output);
            FreeConsole();
            return 0;
        }
        SetConsoleTitleA("SoftPC VM");
    }
    *input_out = input;
    *output_out = output;
    *original_mode_out = original_mode;
    *private_console_out = private_console;
    return 1;
}

static void softpc_console_close(HANDLE input, HANDLE output,
    int private_console)
{
    CloseHandle(input);
    CloseHandle(output);
    if (private_console) FreeConsole();
}

static int softpc_console_deliver_virtual_key(softpc_runtime *runtime,
    WORD virtual_key, DWORD control_state, int released)
{
    KEY_EVENT_RECORD event;
    BYTE key_number;
    UINT scan_code;
    ZeroMemory(&event, sizeof(event));
    scan_code = MapVirtualKeyW(virtual_key, MAPVK_VK_TO_VSC);
    if (scan_code == 0u) return -1;
    event.wVirtualKeyCode = virtual_key;
    event.wVirtualScanCode = (WORD)(scan_code & 0xffu);
    event.dwControlKeyState = control_state;
    key_number = KeyMsgToKeyCode(&event);
    if (key_number == 0u) return -1;
    return softpc_runtime_enqueue_key(runtime, key_number, (uint8_t)released) ?
        -1 : SOFTPC_VM_FRONTEND_ERROR;
}

static int softpc_console_deliver_unicode(softpc_runtime *runtime,
    WCHAR character)
{
    SHORT translated = VkKeyScanW(character);
    BYTE modifiers;
    int action;
    if (translated == -1) return -2;
    modifiers = HIBYTE(translated);
    /* Mobile RDP keyboards commonly supply character packets instead of a
       key-down/key-up pair. Synthesize one complete physical sequence so
       modifiers cannot remain stuck when the matching packet has no key-up. */
    if ((modifiers & 1u) != 0u && (action = softpc_console_deliver_virtual_key(
            runtime, VK_SHIFT, 0u, 0)) >= 0) return action;
    if ((modifiers & 2u) != 0u && (action = softpc_console_deliver_virtual_key(
            runtime, VK_CONTROL, 0u, 0)) >= 0) return action;
    if ((modifiers & 4u) != 0u && (action = softpc_console_deliver_virtual_key(
            runtime, VK_MENU, 0u, 0)) >= 0) return action;
    if ((action = softpc_console_deliver_virtual_key(runtime,
            LOBYTE(translated), 0u, 0)) >= 0) return action;
    if ((action = softpc_console_deliver_virtual_key(runtime,
            LOBYTE(translated), 0u, 1)) >= 0) return action;
    if ((modifiers & 4u) != 0u && (action = softpc_console_deliver_virtual_key(
            runtime, VK_MENU, 0u, 1)) >= 0) return action;
    if ((modifiers & 2u) != 0u && (action = softpc_console_deliver_virtual_key(
            runtime, VK_CONTROL, 0u, 1)) >= 0) return action;
    if ((modifiers & 1u) != 0u) return softpc_console_deliver_virtual_key(
        runtime, VK_SHIFT, 0u, 1);
    return -1;
}

static int softpc_console_key(softpc_runtime *runtime, const KEY_EVENT_RECORD *key)
{
    KEY_EVENT_RECORD event;
    BYTE key_number;
    if (key->bKeyDown && key->wVirtualKeyCode == 'P' &&
        (key->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) &&
        (key->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)))
        return SOFTPC_VM_FRONTEND_PAUSED;
    if (key->bKeyDown && key->wVirtualKeyCode == VK_ESCAPE)
        return SOFTPC_VM_FRONTEND_STOPPED;
    event = *key;
    key_number = KeyMsgToKeyCode(&event);
    /* Some RDP console transports preserve wVirtualKeyCode but omit the
       physical scan code required by the original nt_keycd tables. Recover
       that host detail through the Win32 keyboard layout, then retain the
       original SoftPC key-number translation and 8042 path. */
    if (key_number == 0u && key->bKeyDown && event.uChar.UnicodeChar != 0u) {
        int action = softpc_console_deliver_unicode(runtime,
            event.uChar.UnicodeChar);
        if (action >= 0) return action;
        if (action == -1) return -1;
    }
    if (key_number == 0u && event.wVirtualKeyCode != 0u) {
        UINT scan_code = MapVirtualKeyW(event.wVirtualKeyCode,
            MAPVK_VK_TO_VSC);
        if (scan_code != 0u) {
            event.wVirtualScanCode = (WORD)(scan_code & 0xffu);
            key_number = KeyMsgToKeyCode(&event);
        }
    }
    /* A console can report layout/dead-key and focus records that have no
       original SoftPC key number. They are not a monitor stop request. */
    if (key_number == 0u) return -1;
    return softpc_runtime_enqueue_key(runtime, key_number,
        (uint8_t)!key->bKeyDown) ? -1 :
        SOFTPC_VM_FRONTEND_ERROR;
}

static void softpc_console_paint(HANDLE output,
    const softpc_runtime_frame *frame, unsigned char *previous)
{
    unsigned int row;
    if (frame->valid == 0u || frame->graphics != 0u ||
        memcmp(frame->text, previous, sizeof(frame->text)) == 0) return;
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        CHAR line[SOFTPC_TEXT_COLUMNS];
        unsigned int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = frame->text[row * SOFTPC_TEXT_COLUMNS + column];
            line[column] = character >= 0x20u && character < 0x7fu ?
                (CHAR)character : ' ';
        }
        { COORD position = { 0, (SHORT)row }; DWORD written;
          (void)WriteConsoleOutputCharacterA(output, line, SOFTPC_TEXT_COLUMNS, position, &written); }
    }
    memcpy(previous, frame->text, sizeof(frame->text));
    if (frame->cursor_column >= 0 && frame->cursor_row >= 0 &&
        frame->cursor_column < (int32_t)SOFTPC_TEXT_COLUMNS &&
        frame->cursor_row < (int32_t)SOFTPC_TEXT_ROWS) {
        COORD position;
        position.X = (SHORT)frame->cursor_column;
        position.Y = (SHORT)frame->cursor_row;
        (void)SetConsoleCursorPosition(output, position);
    }
}

int softpc_vm_run_console(softpc_runtime *runtime)
{
    HANDLE input;
    HANDLE output;
    DWORD original_mode;
    unsigned char previous[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    softpc_runtime_frame *frame;
    uint32_t displayed_sequence = 0u;
    int running = 1;
    int result = SOFTPC_VM_FRONTEND_STOPPED;
    int private_console;
    if (runtime == NULL) return 1;
    if (!softpc_console_open(&input, &output, &original_mode,
            &private_console)) return 1;
    if (!SetConsoleMode(input, original_mode & ~(ENABLE_ECHO_INPUT |
            ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT))) {
        softpc_console_close(input, output, private_console);
        return 1;
    }
    frame = (softpc_runtime_frame *)calloc(1u, sizeof(*frame));
    if (frame == NULL) {
        softpc_console_close(input, output, private_console);
        return 1;
    }
    memset(previous, 0xff, sizeof(previous));
    while (running && softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_RUNNING) {
        INPUT_RECORD record;
        DWORD available;
        DWORD read;
        while (PeekConsoleInputA(input, &record, 1u, &available) && available != 0u) {
            if (!ReadConsoleInputA(input, &record, 1u, &read)) {
                running = 0;
                break;
            }
            int action = record.EventType == KEY_EVENT ? softpc_console_key(
                runtime, &record.Event.KeyEvent) : -1;
            if (action >= 0) {
                result = action;
                running = 0;
                break;
            }
        }
        if (softpc_runtime_published_frame_sequence(runtime) !=
            displayed_sequence && softpc_runtime_copy_frame(runtime, frame)) {
            softpc_console_paint(output, frame, previous);
            displayed_sequence = frame->sequence;
        }
        Sleep(10u);
    }
    if (result == SOFTPC_VM_FRONTEND_STOPPED &&
        softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED)
        result = SOFTPC_VM_FRONTEND_PAUSED;
    if (result == SOFTPC_VM_FRONTEND_PAUSED)
        (void)softpc_runtime_pause(runtime);
    else if (result == SOFTPC_VM_FRONTEND_STOPPED)
        (void)softpc_runtime_stop(runtime);
    if (softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_ERROR)
        result = SOFTPC_VM_FRONTEND_ERROR;
    free(frame);
    (void)SetConsoleMode(input, original_mode);
    softpc_console_close(input, output, private_console);
    return result;
}

#else
int softpc_vm_run_console(softpc_runtime *runtime)
{
    (void)runtime;
    return SOFTPC_VM_FRONTEND_ERROR;
}
#endif
