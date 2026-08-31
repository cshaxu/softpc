#include "console.h"

#ifdef _WIN32
#include <windows.h>
#include <string.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD KeyEvent);

#define SOFTPC_TEXT_COLUMNS 80u
#define SOFTPC_TEXT_ROWS 25u
/* A 512-instruction slice together with Sleep(1) throttles POST to a crawl
 * on normal Windows timer resolution.  This is still a bounded slice for
 * keyboard polling, while matching the proven standalone boot probe. */
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

    input = GetStdHandle(STD_INPUT_HANDLE);
    output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (input == INVALID_HANDLE_VALUE || output == INVALID_HANDLE_VALUE ||
        !GetConsoleMode(input, &original_mode)) {
        /* A launcher can leave this process attached to a console while
           redirecting its standard handles.  Reopen the console devices
           before deciding that we need a private console. */
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
    if (!private_console) return;
    CloseHandle(input);
    CloseHandle(output);
    FreeConsole();
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
    const void *surface;
    const unsigned char *cells;
    uint32_t columns;
    uint32_t rows;
    uint32_t stride;
    uint32_t cell_bytes;
    unsigned char text[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    unsigned int row;
    if (!softpc_machine_presentation_text(machine, &surface,
            &columns, &rows, &stride, &cell_bytes) || cell_bytes < 1u ||
        stride < columns) return;
    cells = (const unsigned char *)surface;
    memset(text, ' ', sizeof(text));
    if (columns > SOFTPC_TEXT_COLUMNS) columns = SOFTPC_TEXT_COLUMNS;
    if (rows > SOFTPC_TEXT_ROWS) rows = SOFTPC_TEXT_ROWS;
    for (row = 0; row < rows; ++row) {
        unsigned int column;
        for (column = 0; column < columns; ++column)
            text[row * SOFTPC_TEXT_COLUMNS + column] =
                cells[(row * stride + column) * cell_bytes];
    }
    if (memcmp(text, previous, sizeof(text)) == 0) return;
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        CHAR line[SOFTPC_TEXT_COLUMNS];
        COORD position;
        DWORD written;
        unsigned int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = text[row * SOFTPC_TEXT_COLUMNS + column];
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
    unsigned char previous[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
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
