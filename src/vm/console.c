#include "console.h"
#include "win32_keyboard.h"

#ifdef _WIN32
#include <windows.h>
#include <string.h>

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

static int softpc_console_keyboard_sink(void *context, uint8_t key_number,
    uint8_t released)
{
    return softpc_runtime_enqueue_key((softpc_runtime *)context, key_number,
        released);
}

static int softpc_console_key(softpc_runtime *runtime,
    softpc_win32_keyboard_normalizer *normalizer,
    const KEY_EVENT_RECORD *key)
{
    if (key->bKeyDown && key->wVirtualKeyCode == 'P' &&
        (key->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) &&
        (key->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)))
        return SOFTPC_VM_FRONTEND_PAUSED;
    if (key->bKeyDown && key->wVirtualKeyCode == VK_ESCAPE)
        return SOFTPC_VM_FRONTEND_STOPPED;
    /* RDP soft keyboards may report UTF-16 text without a physical scan.
       The shared normalizer produces a complete host-layout sequence, then
       the original nt_keycd table and 8042 ingress handle it normally. */
    if (key->wVirtualScanCode == 0u && key->bKeyDown &&
        key->uChar.UnicodeChar != 0u) {
        (void)softpc_win32_keyboard_submit_utf16(normalizer, runtime,
            softpc_console_keyboard_sink, key->uChar.UnicodeChar);
    } else {
        (void)softpc_win32_keyboard_submit_transition(runtime,
            softpc_console_keyboard_sink, key->wVirtualScanCode,
            key->wVirtualKeyCode, key->dwControlKeyState, key->bKeyDown != 0);
    }
    return -1;
}

static void softpc_console_paint(HANDLE output,
    const softpc_runtime_frame *frame, unsigned char *previous,
    unsigned short *previous_attributes)
{
    unsigned int row;
    if (frame->valid == 0u || frame->graphics != 0u ||
        (memcmp(frame->text, previous, sizeof(frame->text)) == 0 &&
         memcmp(frame->attributes, previous_attributes,
             sizeof(frame->attributes)) == 0)) return;
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        CHAR_INFO line[SOFTPC_TEXT_COLUMNS];
        unsigned int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = frame->text[row * SOFTPC_TEXT_COLUMNS + column];
            line[column].Char.AsciiChar = character >= 0x20u && character < 0x7fu ?
                (CHAR)character : ' ';
            /* nt_cga's original surface provides the IBM-PC foreground and
               background nibble unchanged; CONSOLE output uses the same
               16-colour ordering.  Do not flatten Setup's palette to the
               host console's default black background. */
            line[column].Attributes = (WORD)frame->attributes[
                row * SOFTPC_TEXT_COLUMNS + column];
        }
        { COORD position = { 0, (SHORT)row };
          COORD size = { SOFTPC_TEXT_COLUMNS, 1 };
          SMALL_RECT region = { 0, (SHORT)row, SOFTPC_TEXT_COLUMNS - 1,
              (SHORT)row };
          (void)WriteConsoleOutputA(output, line, size, position, &region); }
    }
    memcpy(previous, frame->text, sizeof(frame->text));
    memcpy(previous_attributes, frame->attributes,
        sizeof(frame->attributes));
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
    unsigned short previous_attributes[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    softpc_runtime_frame *frame;
    uint32_t displayed_sequence = 0u;
    softpc_win32_keyboard_normalizer keyboard_normalizer = { 0 };
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
    memset(previous_attributes, 0xff, sizeof(previous_attributes));
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
                runtime, &keyboard_normalizer, &record.Event.KeyEvent) : -1;
            if (action >= 0) {
                result = action;
                running = 0;
                break;
            }
        }
        if (softpc_runtime_published_frame_sequence(runtime) !=
            displayed_sequence && softpc_runtime_copy_frame(runtime, frame)) {
            softpc_console_paint(output, frame, previous, previous_attributes);
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
