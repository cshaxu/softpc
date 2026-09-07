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
    lib_console *console;
    host_console_mode mode;
    lib_u32 generation;
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
    event.value.raw_mouse.delta_x = mouse->dwMousePosition.X;
    event.value.raw_mouse.delta_y = mouse->dwMousePosition.Y;
    event.value.raw_mouse.buttons = mouse->dwButtonState;
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
        free(native_console);
        return LIB_STATUS_UNSUPPORTED;
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
    if (native_console->stop_event == NULL) return LIB_STATUS_NO_MEMORY;
    native_console->console = console;
    native_console->mode = mode;
    native_console->generation = generation;
    native_console->reader = CreateThread(NULL, 0u, host_console_reader,
        native_console, 0u, NULL);
    if (native_console->reader == NULL) {
        CloseHandle(native_console->stop_event);
        native_console->stop_event = NULL;
        native_console->console = LIB_NULL;
        return LIB_STATUS_NO_MEMORY;
    }
    return LIB_STATUS_OK;
}

void host_console_native_deactivate(host_console_native *native_console)
{
    if (native_console == LIB_NULL || native_console->reader == NULL) return;
    (void)SetEvent(native_console->stop_event);
    (void)CancelSynchronousIo(native_console->reader);
    (void)WaitForSingleObject(native_console->reader, INFINITE);
    CloseHandle(native_console->reader);
    CloseHandle(native_console->stop_event);
    native_console->reader = NULL;
    native_console->stop_event = NULL;
    native_console->console = LIB_NULL;
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
    CONSOLE_SCREEN_BUFFER_INFOEX info = { 0 };
    COORD position = { 0, 0 };
    DWORD written;
    lib_u32 row;
    lib_status status = LIB_STATUS_OK;

    if (native_console == LIB_NULL || frame == LIB_NULL || frame->columns == 0u ||
        frame->columns > LIB_CONSOLE_TEXT_COLUMNS || frame->rows == 0u ||
        frame->rows > LIB_CONSOLE_TEXT_ROWS) return LIB_STATUS_INVALID_ARGUMENT;
    host_console_output_lock(native_console);
    info.cbSize = sizeof(info);
    if (GetConsoleScreenBufferInfoEx(native_console->output, &info)) {
        for (row = 0u; row < 16u; ++row) {
            lib_u32 color = frame->palette[row];
            info.ColorTable[row] = RGB((color >> 16u) & 0xffu,
                (color >> 8u) & 0xffu, color & 0xffu);
        }
        (void)SetConsoleScreenBufferInfoEx(native_console->output, &info);
    }
    for (row = 0u; row < frame->rows; ++row) {
        lib_size offset = (lib_size)row * LIB_CONSOLE_TEXT_COLUMNS;
        position.Y = (SHORT)row;
        if (!WriteConsoleOutputCharacterA(native_console->output,
                (const char *)&frame->text[offset], frame->columns, position, &written) ||
            written != frame->columns || !WriteConsoleOutputAttribute(native_console->output,
                (const WORD *)&frame->attributes[offset], frame->columns, position,
                &written) || written != frame->columns) {
            status = LIB_STATUS_IO_ERROR;
            break;
        }
    }
    if (status == LIB_STATUS_OK && frame->cursor_visible != LIB_FALSE &&
        frame->cursor_column >= 0 && frame->cursor_column < frame->columns &&
        frame->cursor_row >= 0 && frame->cursor_row < frame->rows) {
        position.X = (SHORT)frame->cursor_column;
        position.Y = (SHORT)frame->cursor_row;
        if (!SetConsoleCursorPosition(native_console->output, position))
            status = LIB_STATUS_IO_ERROR;
    }
    host_console_output_unlock(native_console);
    return status;
}
#endif
