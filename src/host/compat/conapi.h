#ifndef SOFTPC_COMPAT_CONAPI_H
#define SOFTPC_COMPAT_CONAPI_H

/* Standalone replacement for the small Console API surface used by the
 * original SoftPC renderers.  It deliberately exposes a DIB endpoint, not a
 * console-server or VDM service. */
#include <windows.h>
#include "dib_surface.h"

#define CONSOLE_GRAPHICS_BUFFER 2
#define SYSPAL_STATIC 1

HANDLE softpc_compat_create_graphics_buffer(
    PCONSOLE_GRAPHICS_BUFFER_INFO info);
BOOL softpc_compat_set_console_palette(HANDLE output, HPALETTE palette,
    UINT usage);
BOOL softpc_compat_set_console_active_buffer(HANDLE output);
BOOL softpc_compat_close_handle(HANDLE object);
UINT softpc_compat_set_palette_entries(HPALETTE palette, UINT start,
    UINT count, const PALETTEENTRY *entries);
BOOL softpc_compat_get_console_buffer_info(HANDLE output,
    PCONSOLE_SCREEN_BUFFER_INFO info);
BOOL softpc_compat_set_console_window_info(HANDLE output, BOOL absolute,
    const SMALL_RECT *window);
BOOL softpc_compat_set_console_buffer_size(HANDLE output, COORD size);
BOOL softpc_compat_fill_console_character(HANDLE output, CHAR value,
    DWORD count, COORD coordinate, LPDWORD written);
BOOL softpc_compat_fill_console_attribute(HANDLE output, WORD value,
    DWORD count, COORD coordinate, LPDWORD written);
BOOL softpc_compat_scroll_console_buffer(HANDLE output,
    const SMALL_RECT *scroll, const SMALL_RECT *clip, COORD destination,
    const CHAR_INFO *fill);
BOOL softpc_compat_set_console_cursor_position(HANDLE output, COORD position);
BOOL softpc_compat_set_console_cursor_info(HANDLE output,
    const CONSOLE_CURSOR_INFO *cursor);
/* The original nt_graph renderer reaches the standalone presenter through
 * the two Console cursor calls above.  This copied value is presentation
 * metadata only; frontends never inspect the video controller directly. */
int softpc_compat_presentation_cursor(long *column_out, long *row_out,
    unsigned long *size_out);
BOOL softpc_compat_get_current_console_font(HANDLE output, BOOL maximum,
    PCONSOLE_FONT_INFO font);
COORD softpc_compat_get_console_font_size(HANDLE output, DWORD font);
BOOL softpc_compat_write_console(HANDLE output, const void *buffer,
    DWORD count, LPDWORD written, void *reserved);

#define CreateConsoleScreenBuffer(access, share, security, type, info) \
    softpc_compat_create_graphics_buffer((PCONSOLE_GRAPHICS_BUFFER_INFO)(info))
#define SetConsolePalette(output, palette, usage) \
    softpc_compat_set_console_palette((output), (palette), (usage))
#define SetConsoleActiveScreenBuffer(output) \
    softpc_compat_set_console_active_buffer((output))
#define CloseHandle(object) softpc_compat_close_handle((object))
#define SetPaletteEntries(palette, start, count, entries) \
    softpc_compat_set_palette_entries((palette), (start), (count), (entries))
#define GetConsoleScreenBufferInfo(output, info) \
    softpc_compat_get_console_buffer_info((output), (info))
#define SetConsoleWindowInfo(output, absolute, window) \
    softpc_compat_set_console_window_info((output), (absolute), (window))
#define SetConsoleScreenBufferSize(output, size) \
    softpc_compat_set_console_buffer_size((output), (size))
#define FillConsoleOutputCharacter(output, value, count, coordinate, written) \
    softpc_compat_fill_console_character((output), (value), (count), \
        (coordinate), (written))
#define FillConsoleOutputAttribute(output, value, count, coordinate, written) \
    softpc_compat_fill_console_attribute((output), (value), (count), \
        (coordinate), (written))
#define ScrollConsoleScreenBuffer(output, scroll, clip, destination, fill) \
    softpc_compat_scroll_console_buffer((output), (scroll), (clip), \
        (destination), (fill))
#define SetConsoleCursorPosition(output, position) \
    softpc_compat_set_console_cursor_position((output), (position))
#define SetConsoleCursorInfo(output, cursor) \
    softpc_compat_set_console_cursor_info((output), (cursor))
#define GetCurrentConsoleFont(output, maximum, font) \
    softpc_compat_get_current_console_font((output), (maximum), (font))
#define GetConsoleFontSize(output, font) \
    softpc_compat_get_console_font_size((output), (font))
#define WriteConsoleA(output, buffer, count, written, reserved) \
    softpc_compat_write_console((output), (buffer), (count), (written), \
        (reserved))

#endif
