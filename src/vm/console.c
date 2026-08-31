#include "console.h"

#ifdef _WIN32
#include <windows.h>
#include <string.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD KeyEvent);

#define SOFTPC_TEXT_COLUMNS 80u
#define SOFTPC_TEXT_ROWS 25u
/* This remains a bounded CPU slice for keyboard polling, while matching the
 * real-media boot probe closely enough to keep the original POST responsive. */
#define SOFTPC_RUN_SLICE 50000u

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

static int softpc_console_key(softpc_machine *machine, const KEY_EVENT_RECORD *key)
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
    return softpc_machine_key_number(machine, key_number,
        (uint8_t)!key->bKeyDown) == SOFTPC_MACHINE_OK ? -1 :
        SOFTPC_VM_FRONTEND_ERROR;
}

static void softpc_console_paint(HANDLE output, softpc_machine *machine,
    unsigned char *previous)
{
    const void *surface;
    const unsigned char *cells;
    uint32_t columns;
    uint32_t rows;
    uint32_t stride;
    uint32_t cell_bytes;
    int32_t cursor_column;
    int32_t cursor_row;
    unsigned char text[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    unsigned int row;
    /* nt_text() is the original controller-aware presenter.  It already
       applies CRTC page origin, row stride and mode geometry, which raw
       B8000 reads cannot reconstruct. */
    if (!softpc_machine_presentation_text(machine, &surface,
            &columns, &rows, &stride, &cell_bytes) || cell_bytes < 1u ||
        stride < columns) return;
    cells = (const unsigned char *)surface;
    memset(text, ' ', sizeof(text));
    if (columns > SOFTPC_TEXT_COLUMNS) columns = SOFTPC_TEXT_COLUMNS;
    if (rows > SOFTPC_TEXT_ROWS) rows = SOFTPC_TEXT_ROWS;
    for (row = 0u; row < rows; ++row) {
        unsigned int column;
        for (column = 0u; column < columns; ++column)
            text[row * SOFTPC_TEXT_COLUMNS + column] =
                cells[(row * stride + column) * cell_bytes];
    }
    if (memcmp(text, previous, sizeof(text)) != 0) {
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        CHAR line[SOFTPC_TEXT_COLUMNS];
        unsigned int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = text[row * SOFTPC_TEXT_COLUMNS + column];
            line[column] = character >= 0x20u && character < 0x7fu ?
                (CHAR)character : ' ';
        }
        { COORD position = { 0, (SHORT)row }; DWORD written;
          (void)WriteConsoleOutputCharacterA(output, line, SOFTPC_TEXT_COLUMNS, position, &written); }
    }
    memcpy(previous, text, sizeof(text));
    }
    if (softpc_machine_presentation_cursor(machine, &cursor_column,
            &cursor_row) && cursor_column >= 0 && cursor_row >= 0 &&
        cursor_column < (int32_t)SOFTPC_TEXT_COLUMNS &&
        cursor_row < (int32_t)SOFTPC_TEXT_ROWS) {
        COORD position;
        position.X = (SHORT)cursor_column;
        position.Y = (SHORT)cursor_row;
        (void)SetConsoleCursorPosition(output, position);
    }
}

int softpc_vm_run_console(softpc_machine *machine)
{
    HANDLE input;
    HANDLE output;
    DWORD original_mode;
    unsigned char previous[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    int running = 1;
    int result = SOFTPC_VM_FRONTEND_STOPPED;
    int private_console;
    if (machine == NULL) return 1;
    if (!softpc_console_open(&input, &output, &original_mode,
            &private_console)) return 1;
    if (!SetConsoleMode(input, original_mode & ~(ENABLE_ECHO_INPUT |
            ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT))) {
        softpc_console_close(input, output, private_console);
        return 1;
    }
    memset(previous, 0xff, sizeof(previous));
    while (running) {
        INPUT_RECORD record;
        DWORD available;
        DWORD read;
        while (PeekConsoleInputA(input, &record, 1u, &available) && available != 0u) {
            if (!ReadConsoleInputA(input, &record, 1u, &read)) {
                running = 0;
                break;
            }
            int action = record.EventType == KEY_EVENT ? softpc_console_key(
                machine, &record.Event.KeyEvent) : -1;
            if (action >= 0) {
                result = action;
                running = 0;
                break;
            }
        }
        if (softpc_machine_run(machine, SOFTPC_RUN_SLICE) != SOFTPC_MACHINE_OK) {
            result = SOFTPC_VM_FRONTEND_ERROR;
            running = 0;
        }
        softpc_console_paint(output, machine, previous);
        Sleep(0u);
    }
    (void)SetConsoleMode(input, original_mode);
    softpc_console_close(input, output, private_console);
    return result;
}

#else
int softpc_vm_run_console(softpc_machine *machine)
{
    (void)machine;
    return SOFTPC_VM_FRONTEND_ERROR;
}
#endif
