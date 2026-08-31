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
#define SOFTPC_BOOT_SLICE_LIMIT 4000u
#define SOFTPC_TEXT_ADDRESS 0xb8000u

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

static void softpc_console_prepare_output(HANDLE output)
{
    SMALL_RECT window;
    COORD buffer;

    /* The VM owns an 80x25 text presentation.  Keeping the console's visible
       window and backing buffer at that size ensures a desktop launcher does
       not leave writes at (0,0) above an inherited scrollback viewport. */
    window.Left = 0;
    window.Top = 0;
    window.Right = (SHORT)(SOFTPC_TEXT_COLUMNS - 1u);
    window.Bottom = (SHORT)(SOFTPC_TEXT_ROWS - 1u);
    buffer.X = (SHORT)SOFTPC_TEXT_COLUMNS;
    buffer.Y = (SHORT)SOFTPC_TEXT_ROWS;
    (void)SetConsoleWindowInfo(output, TRUE, &window);
    (void)SetConsoleScreenBufferSize(output, buffer);
    (void)SetConsoleWindowInfo(output, TRUE, &window);
    (void)SetConsoleCursorPosition(output, (COORD){ 0, 0 });
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

static int softpc_console_has_text(softpc_machine *machine)
{
    const void *surface;
    const unsigned char *cells;
    uint32_t columns;
    uint32_t rows;
    uint32_t stride;
    uint32_t cell_bytes;
    uint32_t row;
    if (!softpc_machine_presentation_text(machine, &surface, &columns, &rows,
            &stride, &cell_bytes) || cell_bytes < 1u || stride < columns)
        return 0;
    cells = (const unsigned char *)surface;
    if (columns > SOFTPC_TEXT_COLUMNS) columns = SOFTPC_TEXT_COLUMNS;
    if (rows > SOFTPC_TEXT_ROWS) rows = SOFTPC_TEXT_ROWS;
    for (row = 0u; row < rows; ++row) {
        uint32_t column;
        for (column = 0u; column < columns; ++column) {
            unsigned char character = cells[(row * stride + column) * cell_bytes];
            if (character > 0x20u && character < 0x7fu) return 1;
        }
    }
    return 0;
}

static int softpc_console_bootstrap(softpc_machine *machine)
{
    unsigned int slice;
    for (slice = 0u; slice < SOFTPC_BOOT_SLICE_LIMIT; ++slice) {
        if (softpc_machine_run(machine, SOFTPC_RUN_SLICE) != SOFTPC_MACHINE_OK)
            return 0;
        if (softpc_console_has_text(machine)) return 1;
    }
    return 1;
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
    CHAR_INFO output_cells[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    unsigned char fallback[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS * 2u];
    unsigned char text[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    COORD output_size;
    COORD output_origin;
    SMALL_RECT output_region;
    unsigned int row;
    int has_nonblank = 0;
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
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        unsigned int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            if (text[row * SOFTPC_TEXT_COLUMNS + column] > 0x20u) {
                has_nonblank = 1;
                break;
            }
        }
        if (has_nonblank) break;
    }
    if (!has_nonblank && softpc_machine_read_physical(machine,
            SOFTPC_TEXT_ADDRESS, fallback, sizeof(fallback)) ==
            SOFTPC_MACHINE_OK) {
        for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
            unsigned int column;
            for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column)
                text[row * SOFTPC_TEXT_COLUMNS + column] =
                    fallback[(row * SOFTPC_TEXT_COLUMNS + column) * 2u];
        }
    }
    if (memcmp(text, previous, sizeof(text)) == 0) return;
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        unsigned int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = text[row * SOFTPC_TEXT_COLUMNS + column];
            CHAR_INFO *cell = &output_cells[row * SOFTPC_TEXT_COLUMNS + column];
            cell->Char.AsciiChar = character >= 0x20u && character < 0x7fu ?
                (CHAR)character : ' ';
            cell->Attributes = (WORD)(0x07u | ((character == ' ') ? 0u : 0u));
            if ((uint32_t)row < rows && (uint32_t)column < columns &&
                cell_bytes >= 2u)
                cell->Attributes = cells[((uint32_t)row * stride + column) *
                    cell_bytes + 1u];
        }
    }
    output_size.X = (SHORT)SOFTPC_TEXT_COLUMNS;
    output_size.Y = (SHORT)SOFTPC_TEXT_ROWS;
    output_origin.X = 0;
    output_origin.Y = 0;
    output_region.Left = 0;
    output_region.Top = 0;
    output_region.Right = (SHORT)(SOFTPC_TEXT_COLUMNS - 1u);
    output_region.Bottom = (SHORT)(SOFTPC_TEXT_ROWS - 1u);
    if (!WriteConsoleOutputA(output, output_cells, output_size, output_origin,
            &output_region)) return;
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
    softpc_console_prepare_output(output);
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
