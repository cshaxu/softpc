#include "console.h"
#include "keyboard.h"
#include "prompt_trace.h"

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
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
static int app_console_open(HANDLE *input_out, HANDLE *output_out,
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
        SetConsoleTitleA("Insignia SoftPC");
    }
    *input_out = input;
    *output_out = output;
    *original_mode_out = original_mode;
    *private_console_out = private_console;
    return 1;
}

/* WriteConsoleOutputA rejects rows outside the screen-buffer dimensions. A
 * console inherited from a launcher/RDP shell may expose only one buffer row
 * even though it visibly hosts a larger terminal. Ensure the presenter's
 * fixed 80x25 text surface is representable before its first frame arrives. */
static void app_console_ensure_text_surface(HANDLE output)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    COORD required;

    if (output == NULL || output == INVALID_HANDLE_VALUE ||
        !GetConsoleScreenBufferInfo(output, &info)) return;
    required.X = info.dwSize.X < (SHORT)SOFTPC_TEXT_COLUMNS ?
        (SHORT)SOFTPC_TEXT_COLUMNS : info.dwSize.X;
    required.Y = info.dwSize.Y < (SHORT)SOFTPC_TEXT_ROWS ?
        (SHORT)SOFTPC_TEXT_ROWS : info.dwSize.Y;
    if (required.X != info.dwSize.X || required.Y != info.dwSize.Y)
        (void)SetConsoleScreenBufferSize(output, required);
}

static void app_console_close(HANDLE input, HANDLE output,
    int private_console)
{
    /* GetStdHandle returns handles owned by the launching shell.  A
       Ctrl+Alt+P pause destroys and later recreates only this frontend, not
       that shell; closing its standard handles made every resumed console
       keyboard-deaf.  Only an AllocConsole session owns these handles. */
    if (private_console) {
        CloseHandle(input);
        CloseHandle(output);
        FreeConsole();
    }
}

static int app_console_keyboard_sink(void *context, uint8_t key_number,
    uint8_t released)
{
    return app_runtime_enqueue_key((app_runtime *)context, key_number,
        released);
}

static int app_console_key(app_runtime *runtime,
    app_win32_keyboard_normalizer *normalizer,
    const KEY_EVENT_RECORD *key)
{
    if (!key->bKeyDown && normalizer->suppressed_virtual_key ==
        key->wVirtualKeyCode) {
        normalizer->suppressed_virtual_key = 0u;
        return -1;
    }
    if (key->bKeyDown && key->wVirtualKeyCode == 'P' &&
        (key->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) &&
        (key->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))) {
        /* The loop now returns to the monitor, so the physical Ctrl/Alt
           key-up records will not necessarily reach this frontend. Release
           the already-forwarded guest modifiers before requesting pause. */
        (void)app_win32_keyboard_release_ctrl_alt(runtime,
            app_console_keyboard_sink);
        return SOFTPC_VM_FRONTEND_PAUSED;
    }
    if (key->bKeyDown && key->wVirtualKeyCode == 'D' &&
        (key->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) &&
        (key->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))) {
        (void)app_win32_keyboard_release_ctrl_alt(runtime,
            app_console_keyboard_sink);
        (void)app_win32_keyboard_submit_ctrl_alt_del(runtime,
            app_console_keyboard_sink);
        normalizer->suppressed_virtual_key = key->wVirtualKeyCode;
        return -1;
    }
    if (key->bKeyDown && key->wVirtualKeyCode == 'F' &&
        (key->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) &&
        (key->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))) {
        (void)app_win32_keyboard_submit_alt_enter(runtime,
            app_console_keyboard_sink);
        normalizer->suppressed_virtual_key = key->wVirtualKeyCode;
        return -1;
    }
    /* Ctrl+Alt+M is reserved consistently with the window frontend.  The
       console never locks its pointer, so its action is deliberately a
       no-op rather than a guest M keystroke. */
    if (key->wVirtualKeyCode == 'M' &&
        (key->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) &&
        (key->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)))
    {
        (void)app_win32_keyboard_release_ctrl_alt(runtime,
            app_console_keyboard_sink);
        normalizer->suppressed_virtual_key = key->wVirtualKeyCode;
        return -1;
    }
    /* RDP soft keyboards may report UTF-16 text without a physical scan.
       The shared normalizer produces a complete host-layout sequence, then
       the original nt_keycd table and 8042 ingress handle it normally. */
    if (key->wVirtualScanCode == 0u && key->bKeyDown &&
        key->uChar.UnicodeChar != 0u) {
        (void)app_win32_keyboard_submit_utf16(normalizer, runtime,
            app_console_keyboard_sink, key->uChar.UnicodeChar);
    } else {
        (void)app_win32_keyboard_submit_transition(runtime,
            app_console_keyboard_sink, key->wVirtualScanCode,
            key->wVirtualKeyCode, key->dwControlKeyState, key->bKeyDown != 0);
    }
    return -1;
}

/* The console is a visible absolute host surface, unlike the window's
 * click-to-capture relative pointer.  Keep its pointer free at all times;
 * translate every ordinary console mouse record to the existing relative
 * Bus Mouse ingress so guest programs with a mouse driver (for example DOS
 * EDIT) can use their menus without a frontend capture state. */
static void app_console_mouse(app_runtime *runtime,
    COORD *previous, int *previous_valid, const MOUSE_EVENT_RECORD *mouse)
{
    int32_t delta_x = 0;
    int32_t delta_y = 0;
    uint8_t left;
    uint8_t right;

    if (runtime == NULL || previous == NULL || previous_valid == NULL ||
        mouse == NULL) return;
    if (*previous_valid) {
        /* Console coordinates are text cells; map them to the fixed 8x16
           guest text surface before handing relative motion to SoftPC. */
        delta_x = ((int32_t)mouse->dwMousePosition.X - previous->X) * 8;
        delta_y = ((int32_t)mouse->dwMousePosition.Y - previous->Y) * 16;
    }
    *previous = mouse->dwMousePosition;
    *previous_valid = 1;
    left = (mouse->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0u;
    right = (mouse->dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0u;
    (void)app_runtime_enqueue_mouse(runtime, delta_x, delta_y, left, right);
}

static int app_console_paint(HANDLE output,
    const app_runtime_frame *frame, unsigned char *previous,
    unsigned short *previous_attributes, uint32_t *previous_palette)
{
    CHAR_INFO cells[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    COORD size = { SOFTPC_TEXT_COLUMNS, SOFTPC_TEXT_ROWS };
    COORD position = { 0, 0 };
    SMALL_RECT region = { 0, 0, SOFTPC_TEXT_COLUMNS - 1,
        SOFTPC_TEXT_ROWS - 1 };
    unsigned int row;
    int palette_changed;
    if (frame->valid == 0u || frame->graphics != 0u) return 1;
    palette_changed = memcmp(frame->text_palette, previous_palette,
        sizeof(frame->text_palette)) != 0;
    if (palette_changed) {
        CONSOLE_SCREEN_BUFFER_INFOEX info;
        unsigned int index;

        memset(&info, 0, sizeof(info));
        info.cbSize = sizeof(info);
        if (GetConsoleScreenBufferInfoEx(output, &info)) {
            for (index = 0u; index < 16u; ++index)
                info.ColorTable[index] = (COLORREF)frame->text_palette[index];
            (void)SetConsoleScreenBufferInfoEx(output, &info);
        }
        memcpy(previous_palette, frame->text_palette,
            sizeof(frame->text_palette));
    }
    if (memcmp(frame->text, previous, sizeof(frame->text)) == 0 &&
        memcmp(frame->attributes, previous_attributes,
            sizeof(frame->attributes)) == 0) return 1;
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        unsigned int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = frame->text[row * SOFTPC_TEXT_COLUMNS + column];
            CHAR_INFO *cell = &cells[row * SOFTPC_TEXT_COLUMNS + column];
            cell->Char.AsciiChar = character >= 0x20u && character < 0x7fu ?
                (CHAR)character : ' ';
            /* nt_cga's original surface provides the IBM-PC foreground and
               background nibble unchanged; CONSOLE output uses the same
               16-colour ordering.  Do not flatten Setup's palette to the
               host console's default black background. */
            cell->Attributes = (WORD)frame->attributes[
                row * SOFTPC_TEXT_COLUMNS + column];
        }
    }
    if (!WriteConsoleOutputA(output, cells, size, position, &region))
        return 0;
    memcpy(previous, frame->text, sizeof(frame->text));
    memcpy(previous_attributes, frame->attributes,
        sizeof(frame->attributes));
    if (frame->cursor_column >= 0 && frame->cursor_row >= 0 &&
        frame->cursor_column < (int32_t)SOFTPC_TEXT_COLUMNS &&
        frame->cursor_row < (int32_t)SOFTPC_TEXT_ROWS) {
        COORD position;
        CONSOLE_CURSOR_INFO cursor;
        position.X = (SHORT)frame->cursor_column;
        position.Y = (SHORT)frame->cursor_row;
        (void)SetConsoleCursorPosition(output, position);
        /* The original renderer used the Console cursor endpoint for the
           controller-selected shape and visibility.  The detached console
           is a presenter, so explicitly establish that state instead of
           inheriting a hidden cursor from the shell that launched SoftPC. */
        cursor.dwSize = frame->cursor_size;
        if (cursor.dwSize == 0u || cursor.dwSize > 100u) cursor.dwSize = 100u;
        cursor.bVisible = TRUE;
        (void)SetConsoleCursorInfo(output, &cursor);
    } else {
        CONSOLE_CURSOR_INFO cursor;
        cursor.dwSize = 100u;
        cursor.bVisible = FALSE;
        (void)SetConsoleCursorInfo(output, &cursor);
    }
    return 1;
}

int app_vm_run_console(app_runtime *runtime)
{
    HANDLE input;
    HANDLE output;
    DWORD original_mode;
    unsigned char previous[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    unsigned short previous_attributes[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    uint32_t previous_palette[16u];
    app_runtime_frame *frame;
    uint32_t displayed_sequence = 0u;
    app_win32_keyboard_normalizer keyboard_normalizer = { 0 };
    COORD mouse_previous = { 0, 0 };
    int mouse_previous_valid = 0;
    int running = 1;
    int result = SOFTPC_VM_FRONTEND_STOPPED;
    int private_console;
    if (runtime == NULL) return 1;
    if (!app_console_open(&input, &output, &original_mode,
            &private_console)) return 1;
    if (!SetConsoleMode(input, (original_mode & ~(ENABLE_ECHO_INPUT |
            ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT |
            ENABLE_QUICK_EDIT_MODE)) | ENABLE_MOUSE_INPUT |
            ENABLE_EXTENDED_FLAGS)) {
        app_console_close(input, output, private_console);
        return 1;
    }
    app_console_ensure_text_surface(output);
    frame = (app_runtime_frame *)calloc(1u, sizeof(*frame));
    if (frame == NULL) {
        app_console_close(input, output, private_console);
        return 1;
    }
    memset(previous, 0xff, sizeof(previous));
    memset(previous_attributes, 0xff, sizeof(previous_attributes));
    memset(previous_palette, 0xff, sizeof(previous_palette));
    while (running && app_runtime_get_state(runtime) == SOFTPC_RUNTIME_RUNNING) {
        INPUT_RECORD record;
        DWORD available;
        DWORD read;
        while (PeekConsoleInputA(input, &record, 1u, &available) && available != 0u) {
            if (!ReadConsoleInputA(input, &record, 1u, &read)) {
                running = 0;
                break;
            }
            int action = -1;
            if (record.EventType == KEY_EVENT)
                action = app_console_key(runtime, &keyboard_normalizer,
                    &record.Event.KeyEvent);
            else if (record.EventType == MOUSE_EVENT)
                app_console_mouse(runtime, &mouse_previous,
                    &mouse_previous_valid, &record.Event.MouseEvent);
            if (action >= 0) {
                result = action;
                running = 0;
                break;
            }
        }
        if (app_runtime_published_frame_sequence(runtime) !=
            displayed_sequence && app_runtime_copy_frame(runtime, frame)) {
            /* The original renderer's mode bit is the routing authority.
               Setup may validly enter graphics on a black DIB and paint it
               on later dirty turns, so a frontend cannot infer this from
               palette contents. */
            if (frame->graphics != 0u) {
                app_prompt_trace("softpc prompt route console->window frame=%lu",
                    (unsigned long)frame->sequence);
                result = SOFTPC_VM_FRONTEND_SWITCH_WINDOW;
                running = 0;
                break;
            }
            if (app_console_paint(output, frame, previous,
                    previous_attributes, previous_palette))
                displayed_sequence = frame->sequence;
        }
        Sleep(10u);
    }
    if (result == SOFTPC_VM_FRONTEND_STOPPED &&
        app_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED)
        result = SOFTPC_VM_FRONTEND_PAUSED;
    if (result == SOFTPC_VM_FRONTEND_PAUSED)
        (void)app_runtime_pause(runtime);
    else if (result == SOFTPC_VM_FRONTEND_STOPPED)
        (void)app_runtime_stop(runtime);
    if (app_runtime_get_state(runtime) == SOFTPC_RUNTIME_ERROR)
        result = SOFTPC_VM_FRONTEND_ERROR;
    free(frame);
    (void)SetConsoleMode(input, original_mode);
    app_console_close(input, output, private_console);
    return result;
}

#else
int app_vm_run_console(app_runtime *runtime)
{
    (void)runtime;
    return SOFTPC_VM_FRONTEND_ERROR;
}
#endif
