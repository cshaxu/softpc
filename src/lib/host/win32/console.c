#include "lib/base/internal/console.h"
#include "lib/host/internal/console_native.h"

#ifdef _WIN32
#include <windows.h>

struct host_console_native {
    atomic_flag output_lock;
    HANDLE input;
    HANDLE output;
    HANDLE stop_event;
    HANDLE reader;
    DWORD original_mode;
    lib_bool private_console;
    lib_console *console;
    host_console_mode mode;
    lib_u32 generation;
    COORD prior_mouse_position;
    lib_bool prior_mouse_position_valid;
};

static void host_console_output_lock(host_console_native *native_console)
{
    while (atomic_flag_test_and_set_explicit(&native_console->output_lock,
        memory_order_acquire)) { }
}

static void host_console_output_unlock(host_console_native *native_console)
{
    atomic_flag_clear_explicit(&native_console->output_lock, memory_order_release);
}

static lib_u8 host_console_modifiers(DWORD state)
{
    lib_u8 modifiers = 0u;
    if ((state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0u) modifiers |= 1u;
    if ((state & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0u) modifiers |= 2u;
    if ((state & SHIFT_PRESSED) != 0u) modifiers |= 4u;
    return modifiers;
}

static void host_console_emit_key(host_console_native *native_console,
    const KEY_EVENT_RECORD *key)
{
    lib_console_event event = { 0 };
    event.kind = LIB_CONSOLE_EVENT_RAW_KEY;
    event.binding_generation = native_console->generation;
    event.value.raw_key.scan_code = key->wVirtualScanCode;
    if ((key->dwControlKeyState & ENHANCED_KEY) != 0u)
        event.value.raw_key.scan_code |= 0x0100u;
    event.value.raw_key.key = key->wVirtualKeyCode;
    event.value.raw_key.unicode = key->uChar.UnicodeChar;
    event.value.raw_key.modifiers = host_console_modifiers(key->dwControlKeyState);
    event.value.raw_key.pressed = key->bKeyDown ? LIB_TRUE : LIB_FALSE;
    (void)lib_console_deliver_event(native_console->console, &event);
}

static void host_console_emit_mouse(host_console_native *native_console,
    const MOUSE_EVENT_RECORD *mouse)
{
    lib_console_event event = { 0 };
    event.kind = LIB_CONSOLE_EVENT_RAW_MOUSE;
    event.binding_generation = native_console->generation;
    if (native_console->prior_mouse_position_valid != LIB_FALSE) {
        /* Win32 Console reports character-cell positions.  Raw Console and
         * Window input must retain the old common guest-pixel contract. */
        event.value.raw_mouse.delta_x = ((lib_i32)mouse->dwMousePosition.X -
            native_console->prior_mouse_position.X) * 8;
        event.value.raw_mouse.delta_y = ((lib_i32)mouse->dwMousePosition.Y -
            native_console->prior_mouse_position.Y) * 16;
    }
    native_console->prior_mouse_position = mouse->dwMousePosition;
    native_console->prior_mouse_position_valid = LIB_TRUE;
    event.value.raw_mouse.buttons =
        (mouse->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0u ? 0x01u : 0u;
    if ((mouse->dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0u)
        event.value.raw_mouse.buttons |= 0x02u;
    (void)lib_console_deliver_event(native_console->console, &event);
}

static DWORD WINAPI host_console_reader(void *context)
{
    host_console_native *native_console = (host_console_native *)context;
    if (native_console->mode == HOST_CONSOLE_COOKED_LINES) {
        for (;;) {
            char text[LIB_CONSOLE_LINE_MAX];
            DWORD read = 0u;
            lib_console_event event = { 0 };
            if (!ReadConsoleA(native_console->input, text,
                    LIB_CONSOLE_LINE_MAX - 1u, &read, NULL)) break;
            if (WaitForSingleObject(native_console->stop_event, 0u) == WAIT_OBJECT_0)
                break;
            while (read != 0u && (text[read - 1u] == '\r' || text[read - 1u] == '\n'))
                --read;
            event.kind = LIB_CONSOLE_EVENT_COOKED_LINE;
            event.binding_generation = native_console->generation;
            event.value.line.length = read;
            memcpy(event.value.line.text, text, read);
            event.value.line.text[read] = '\0';
            (void)lib_console_deliver_event(native_console->console, &event);
        }
    } else {
        HANDLE waits[2] = { native_console->stop_event, native_console->input };
        while (WaitForMultipleObjects(2u, waits, FALSE, INFINITE) == WAIT_OBJECT_0 + 1u) {
            INPUT_RECORD record;
            DWORD read = 0u;
            if (!ReadConsoleInputA(native_console->input, &record, 1u, &read)) break;
            if (record.EventType == KEY_EVENT) host_console_emit_key(native_console,
                &record.Event.KeyEvent);
            else if (record.EventType == MOUSE_EVENT) host_console_emit_mouse(native_console,
                &record.Event.MouseEvent);
        }
    }
    return 0u;
}

lib_status host_console_native_create(host_console_native **out_native)
{
    host_console_native *native_console;
    DWORD mode;
    if (out_native == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_native = LIB_NULL;
    native_console = calloc(1u, sizeof(*native_console));
    if (native_console == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    native_console->output_lock = (atomic_flag)ATOMIC_FLAG_INIT;
    atomic_flag_clear_explicit(&native_console->output_lock, memory_order_release);
    native_console->input = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    native_console->output = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (native_console->input == INVALID_HANDLE_VALUE ||
        native_console->output == INVALID_HANDLE_VALUE ||
        !GetConsoleMode(native_console->input, &mode)) {
        if (native_console->input != INVALID_HANDLE_VALUE) CloseHandle(native_console->input);
        if (native_console->output != INVALID_HANDLE_VALUE) CloseHandle(native_console->output);
        if (GetConsoleCP() != 0u || !AllocConsole()) {
            free(native_console);
            return LIB_STATUS_UNSUPPORTED;
        }
        native_console->private_console = LIB_TRUE;
        native_console->input = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        native_console->output = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (native_console->input == INVALID_HANDLE_VALUE ||
            native_console->output == INVALID_HANDLE_VALUE ||
            !GetConsoleMode(native_console->input, &mode)) {
            if (native_console->input != INVALID_HANDLE_VALUE) CloseHandle(native_console->input);
            if (native_console->output != INVALID_HANDLE_VALUE) CloseHandle(native_console->output);
            FreeConsole();
            free(native_console);
            return LIB_STATUS_UNSUPPORTED;
        }
    }
    native_console->original_mode = mode;
    *out_native = native_console;
    return LIB_STATUS_OK;
}

void host_console_native_destroy(host_console_native *native_console)
{
    if (native_console == LIB_NULL) return;
    host_console_native_deactivate(native_console);
    if (native_console->input != INVALID_HANDLE_VALUE) CloseHandle(native_console->input);
    if (native_console->output != INVALID_HANDLE_VALUE) CloseHandle(native_console->output);
    if (native_console->private_console != LIB_FALSE) FreeConsole();
    free(native_console);
}

lib_status host_console_native_activate(host_console_native *native_console,
    lib_console *console, host_console_mode mode, lib_u32 generation)
{
    DWORD configured;
    if (native_console == LIB_NULL || console == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    configured = native_console->original_mode;
    if (mode == HOST_CONSOLE_RAW_EVENTS)
        configured = (configured & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT |
            ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE)) |
            ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    else configured |= ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT;
    if (!SetConsoleMode(native_console->input, configured)) return LIB_STATUS_IO_ERROR;
    native_console->stop_event = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (native_console->stop_event == NULL) {
        (void)SetConsoleMode(native_console->input, native_console->original_mode);
        return LIB_STATUS_NO_MEMORY;
    }
    native_console->console = console;
    native_console->mode = mode;
    native_console->generation = generation;
    native_console->prior_mouse_position_valid = LIB_FALSE;
    {
        HWND window = GetConsoleWindow();
        if (window != NULL) {
            if (IsIconic(window)) ShowWindow(window, SW_RESTORE);
            (void)SetForegroundWindow(window);
            (void)SetActiveWindow(window);
            (void)SetFocus(window);
        }
    }
    native_console->reader = CreateThread(NULL, 0u, host_console_reader,
        native_console, 0u, NULL);
    if (native_console->reader == NULL) {
        CloseHandle(native_console->stop_event);
        native_console->stop_event = NULL;
        native_console->console = LIB_NULL;
        (void)SetConsoleMode(native_console->input,
            native_console->original_mode);
        return LIB_STATUS_NO_MEMORY;
    }
    return LIB_STATUS_OK;
}

void host_console_native_deactivate(host_console_native *native_console)
{
    if (native_console == LIB_NULL) return;
    if (native_console->reader != NULL) {
        (void)SetEvent(native_console->stop_event);
        (void)CancelSynchronousIo(native_console->reader);
        (void)WaitForSingleObject(native_console->reader, INFINITE);
        CloseHandle(native_console->reader);
        native_console->reader = NULL;
    }
    if (native_console->stop_event != NULL) {
        CloseHandle(native_console->stop_event);
        native_console->stop_event = NULL;
    }
    native_console->console = LIB_NULL;
    native_console->prior_mouse_position_valid = LIB_FALSE;
    if (native_console->input != INVALID_HANDLE_VALUE)
        (void)SetConsoleMode(native_console->input, native_console->original_mode);
}

lib_status host_console_native_write(void *context, const char *text,
    lib_size length)
{
    host_console_native *native_console = (host_console_native *)context;
    DWORD written = 0u;
    lib_status status;
    if (native_console == LIB_NULL || (text == LIB_NULL && length != 0u) ||
        length > (lib_size)UINT32_MAX) return LIB_STATUS_INVALID_ARGUMENT;
    host_console_output_lock(native_console);
    status = WriteConsoleA(native_console->output, text, (DWORD)length, &written, NULL) &&
        written == (DWORD)length ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
    host_console_output_unlock(native_console);
    return status;
}

lib_status host_console_native_present_text_frame(void *context,
    const lib_console_text_frame *frame)
{
    host_console_native *native_console = (host_console_native *)context;
    CHAR_INFO cells[LIB_CONSOLE_TEXT_COLUMNS * LIB_CONSOLE_TEXT_ROWS];
    CONSOLE_SCREEN_BUFFER_INFOEX info = { 0 };
    CONSOLE_CURSOR_INFO cursor;
    COORD size = { LIB_CONSOLE_TEXT_COLUMNS, LIB_CONSOLE_TEXT_ROWS };
    COORD position = { 0, 0 };
    SMALL_RECT region = { 0, 0, LIB_CONSOLE_TEXT_COLUMNS - 1,
        LIB_CONSOLE_TEXT_ROWS - 1 };
    lib_u32 row;
    lib_status status = LIB_STATUS_OK;

    if (native_console == LIB_NULL || frame == LIB_NULL || frame->columns == 0u ||
        frame->columns > LIB_CONSOLE_TEXT_COLUMNS || frame->rows == 0u ||
        frame->rows > LIB_CONSOLE_TEXT_ROWS) return LIB_STATUS_INVALID_ARGUMENT;
    host_console_output_lock(native_console);
    {
        CONSOLE_SCREEN_BUFFER_INFO surface;
        COORD required;
        SMALL_RECT viewport = { 0, 0, LIB_CONSOLE_TEXT_COLUMNS - 1,
            LIB_CONSOLE_TEXT_ROWS - 1 };
        if (!GetConsoleScreenBufferInfo(native_console->output, &surface)) {
            host_console_output_unlock(native_console);
            return LIB_STATUS_IO_ERROR;
        }
        required.X = surface.dwSize.X < (SHORT)LIB_CONSOLE_TEXT_COLUMNS ?
            (SHORT)LIB_CONSOLE_TEXT_COLUMNS : surface.dwSize.X;
        required.Y = surface.dwSize.Y < (SHORT)LIB_CONSOLE_TEXT_ROWS ?
            (SHORT)LIB_CONSOLE_TEXT_ROWS : surface.dwSize.Y;
        if ((required.X != surface.dwSize.X || required.Y != surface.dwSize.Y) &&
            !SetConsoleScreenBufferSize(native_console->output, required)) {
            host_console_output_unlock(native_console);
            return LIB_STATUS_IO_ERROR;
        }
        (void)SetConsoleWindowInfo(native_console->output, TRUE, &viewport);
    }
    info.cbSize = sizeof(info);
    if (GetConsoleScreenBufferInfoEx(native_console->output, &info)) {
        for (row = 0u; row < 16u; ++row) {
            lib_u32 color = frame->palette[row];
            info.ColorTable[row] = RGB((color >> 16u) & 0xffu,
                (color >> 8u) & 0xffu, color & 0xffu);
        }
        (void)SetConsoleScreenBufferInfoEx(native_console->output, &info);
    }
    for (row = 0u; row < LIB_CONSOLE_TEXT_ROWS; ++row) {
        lib_u32 column;
        for (column = 0u; column < LIB_CONSOLE_TEXT_COLUMNS; ++column) {
            lib_size offset = (lib_size)row * LIB_CONSOLE_TEXT_COLUMNS + column;
            cells[offset].Char.AsciiChar = row < frame->rows && column < frame->columns &&
                frame->text[offset] >= 0x20u && frame->text[offset] < 0x7fu ?
                    (CHAR)frame->text[offset] : ' ';
            cells[offset].Attributes = (WORD)(row < frame->rows &&
                column < frame->columns ? frame->attributes[offset] : 0u);
        }
    }
    if (!WriteConsoleOutputA(native_console->output, cells, size, position, &region))
        status = LIB_STATUS_IO_ERROR;
    cursor.dwSize = frame->font_height != 0u &&
        frame->cursor_bottom >= frame->cursor_top ?
            (DWORD)((frame->cursor_bottom - frame->cursor_top + 1u) *
                100u / frame->font_height) : 100u;
    if (cursor.dwSize == 0u || cursor.dwSize > 100u) cursor.dwSize = 100u;
    cursor.bVisible = status == LIB_STATUS_OK && frame->cursor_visible != LIB_FALSE &&
        frame->cursor_column >= 0 && frame->cursor_column < frame->columns &&
        frame->cursor_row >= 0 && frame->cursor_row < frame->rows;
    if (cursor.bVisible) {
        position.X = (SHORT)frame->cursor_column;
        position.Y = (SHORT)frame->cursor_row;
        if (!SetConsoleCursorPosition(native_console->output, position))
            status = LIB_STATUS_IO_ERROR;
    }
    (void)SetConsoleCursorInfo(native_console->output, &cursor);
    host_console_output_unlock(native_console);
    return status;
}
#endif
