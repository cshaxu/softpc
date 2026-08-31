#include "console.h"

#ifdef _WIN32
#include <windows.h>
#include <string.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD KeyEvent);

#define SOFTPC_TEXT_COLUMNS 80u
#define SOFTPC_TEXT_ROWS 25u
/* This remains a bounded CPU slice for keyboard polling, while matching the
 * real-media boot probe closely enough to keep the original POST responsive. */
#define SOFTPC_TEXT_ADDRESS 0xb8000u
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
    if (key->bKeyDown && key->wVirtualKeyCode == VK_ESCAPE) return 1;
    event = *key;
    key_number = KeyMsgToKeyCode(&event);
    if (key_number == 0u) return 0;
    return softpc_machine_key_number(machine, key_number,
        (uint8_t)!key->bKeyDown) == SOFTPC_MACHINE_OK ? 0 : 1;
}

static void softpc_console_paint(HANDLE output, softpc_machine *machine,
    unsigned char *previous)
{
    unsigned char text[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS * 2u];
    unsigned int row;
    if (softpc_machine_read_physical(machine, SOFTPC_TEXT_ADDRESS,
            text, sizeof(text)) != SOFTPC_MACHINE_OK ||
        memcmp(text, previous, sizeof(text)) == 0) return;
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        CHAR line[SOFTPC_TEXT_COLUMNS];
        unsigned int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = text[(row * SOFTPC_TEXT_COLUMNS + column) * 2u];
            line[column] = character >= 0x20u && character < 0x7fu ?
                (CHAR)character : ' ';
        }
        { COORD position = { 0, (SHORT)row }; DWORD written;
          (void)WriteConsoleOutputCharacterA(output, line, SOFTPC_TEXT_COLUMNS, position, &written); }
    }
    memcpy(previous, text, sizeof(text));
}

int softpc_vm_run_console(softpc_machine *machine)
{
    HANDLE input;
    HANDLE output;
    DWORD original_mode;
    unsigned char previous[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS * 2u];
    int running = 1;
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
            if (record.EventType == KEY_EVENT &&
                softpc_console_key(machine, &record.Event.KeyEvent)) {
                running = 0;
                break;
            }
        }
        if (softpc_machine_run(machine, SOFTPC_RUN_SLICE) != SOFTPC_MACHINE_OK)
            running = 0;
        softpc_console_paint(output, machine, previous);
        Sleep(0u);
    }
    (void)SetConsoleMode(input, original_mode);
    softpc_console_close(input, output, private_console);
    return 0;
}

#else
int softpc_vm_run_console(softpc_machine *machine)
{
    (void)machine;
    return 1;
}
#endif
