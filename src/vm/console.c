#include "console.h"

#ifdef _WIN32
#include <windows.h>
#include <string.h>

#define SOFTPC_TEXT_COLUMNS 80u
#define SOFTPC_TEXT_ROWS 25u
#define SOFTPC_TEXT_ADDRESS 0xb8000u
#define SOFTPC_RUN_SLICE 50000u

static int softpc_console_key(softpc_machine *machine, const KEY_EVENT_RECORD *key)
{
    uint8_t scan_code;
    if (!key->bKeyDown) return 0;
    if (key->wVirtualKeyCode == VK_ESCAPE) return 1;
    scan_code = (uint8_t)(key->wVirtualScanCode & 0xffu);
    if (scan_code == 0u) return 0;
    if ((key->dwControlKeyState & ENHANCED_KEY) != 0u &&
        softpc_machine_key_scancode(machine, 0xe0u) != SOFTPC_MACHINE_OK)
        return 1;
    if (softpc_machine_key_scancode(machine, scan_code) != SOFTPC_MACHINE_OK ||
        softpc_machine_key_scancode(machine, (uint8_t)(scan_code | 0x80u)) !=
            SOFTPC_MACHINE_OK)
        return 1;
    return 0;
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
        COORD position;
        DWORD written;
        unsigned int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = text[(row * SOFTPC_TEXT_COLUMNS + column) * 2u];
            line[column] = character >= 0x20u && character < 0x7fu ?
                (CHAR)character : ' ';
        }
        position.X = 0;
        position.Y = (SHORT)row;
        (void)WriteConsoleOutputCharacterA(output, line, SOFTPC_TEXT_COLUMNS,
            position, &written);
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
    if (machine == NULL) return 1;
    input = GetStdHandle(STD_INPUT_HANDLE);
    output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (input == INVALID_HANDLE_VALUE || output == INVALID_HANDLE_VALUE ||
        !GetConsoleMode(input, &original_mode)) return 1;
    if (!SetConsoleMode(input, original_mode & ~(ENABLE_ECHO_INPUT |
            ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT))) return 1;
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
        Sleep(1u);
    }
    (void)SetConsoleMode(input, original_mode);
    return 0;
}

#else
int softpc_vm_run_console(softpc_machine *machine)
{
    (void)machine;
    return 1;
}
#endif
