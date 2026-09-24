#include <windows.h>
#include <string.h>

#include "insignia.h"
#include "host_def.h"
#include "conapi.h"
#include "nt_graph.h"
#include "dib_surface.h"

BYTE Red[] = { 0, 0, 0, 0, 128, 128, 128, 192, 128, 0, 0, 0,
    255, 255, 255, 255 };
BYTE Green[] = { 0, 0, 128, 128, 0, 0, 128, 192, 128, 0, 255, 255,
    0, 0, 255, 255 };
BYTE Blue[] = { 0, 128, 0, 128, 0, 128, 0, 192, 128, 255, 0, 255,
    0, 255, 0, 255 };
BOOL ConsoleInitialised = TRUE;
BOOL ConsoleNoUpdates = FALSE;
static LONG softpc_compat_cursor_column;
static LONG softpc_compat_cursor_row;
static DWORD softpc_compat_cursor_size = 20u;
static int softpc_compat_cursor_position_valid;
static int softpc_compat_cursor_visible = 1;
/* This is the original renderer's logical Console buffer, not the 80x50
   shared text surface copied by nt_cga.  Mode transitions resize this buffer
   before repainting it. */
static COORD softpc_compat_console_size = { 80, 25 };
static SMALL_RECT softpc_compat_console_window = { 0, 0, 79, 24 };

BOOL CreateDisplayPalette(void)
{
    sc.ColPalette = (HPALETTE)(UINT_PTR)1;
    return TRUE;
}

void MouseDetachMenuItem(BOOL detached)
{
    UNUSED(detached);
}

void MouseAttachMenuItem(HANDLE output)
{
    UNUSED(output);
}

void MouseDisplay(void)
{
}

void DelayMouseEvents(ULONG count)
{
    UNUSED(count);
}

HANDLE softpc_compat_create_graphics_buffer(
    PCONSOLE_GRAPHICS_BUFFER_INFO info)
{
    if (info == NULL || info->lpBitMapInfo == NULL ||
        !softpc_standalone_dib_bind(info->lpBitMapInfo))
        return INVALID_HANDLE_VALUE;
    info->lpBitMap = sc.ConsoleBufInfo.lpBitMap;
    return (HANDLE)(UINT_PTR)1;
}

BOOL softpc_compat_set_console_palette(HANDLE output, HPALETTE palette,
    UINT usage)
{
    UNUSED(output); UNUSED(palette); UNUSED(usage);
    return TRUE;
}

BOOL softpc_compat_set_console_active_buffer(HANDLE output)
{
    UNUSED(output);
    return TRUE;
}

BOOL softpc_compat_close_handle(HANDLE object)
{
    UNUSED(object);
    return TRUE;
}

UINT softpc_compat_set_palette_entries(HPALETTE palette, UINT start,
    UINT count, const PALETTEENTRY *entries)
{
    UNUSED(palette);
    if (entries == NULL || start != 0 || count > 256u) return 0;
    softpc_standalone_dib_set_palette_entries(entries, (int)count);
    return count;
}

BOOL softpc_compat_get_console_buffer_info(HANDLE output,
    PCONSOLE_SCREEN_BUFFER_INFO info)
{
    UNUSED(output);
    if (info == NULL) return FALSE;
    memset(info, 0, sizeof(*info));
    info->dwSize = softpc_compat_console_size;
    info->dwMaximumWindowSize = softpc_compat_console_size;
    info->srWindow = softpc_compat_console_window;
    if (softpc_compat_cursor_position_valid) {
        info->dwCursorPosition.X = (SHORT)softpc_compat_cursor_column;
        info->dwCursorPosition.Y = (SHORT)softpc_compat_cursor_row;
    }
    return TRUE;
}

BOOL softpc_compat_set_console_window_info(HANDLE output, BOOL absolute,
    const SMALL_RECT *window)
{
    SMALL_RECT next;

    UNUSED(output);
    if (window == NULL) return FALSE;
    next = *window;
    if (!absolute) {
        next.Left = (SHORT)(next.Left + softpc_compat_console_window.Left);
        next.Right = (SHORT)(next.Right + softpc_compat_console_window.Right);
        next.Top = (SHORT)(next.Top + softpc_compat_console_window.Top);
        next.Bottom = (SHORT)(next.Bottom + softpc_compat_console_window.Bottom);
    }
    if (next.Left < 0 || next.Top < 0 || next.Right < next.Left ||
        next.Bottom < next.Top || next.Right >= softpc_compat_console_size.X ||
        next.Bottom >= softpc_compat_console_size.Y) return FALSE;
    softpc_compat_console_window = next;
    return TRUE;
}

BOOL softpc_compat_set_console_buffer_size(HANDLE output, COORD size)
{
    UNUSED(output);
    if (size.X <= 0 || size.Y <= 0) return FALSE;
    softpc_compat_console_size = size;
    if (softpc_compat_console_window.Right >= size.X)
        softpc_compat_console_window.Right = (SHORT)(size.X - 1);
    if (softpc_compat_console_window.Bottom >= size.Y)
        softpc_compat_console_window.Bottom = (SHORT)(size.Y - 1);
    if (softpc_compat_console_window.Left > softpc_compat_console_window.Right)
        softpc_compat_console_window.Left = 0;
    if (softpc_compat_console_window.Top > softpc_compat_console_window.Bottom)
        softpc_compat_console_window.Top = 0;
    return TRUE;
}

static BOOL softpc_compat_fill_console_cell(HANDLE output, unsigned char value,
    DWORD count, COORD coordinate, LPDWORD written, int attribute)
{
    unsigned long columns;
    unsigned long rows;
    unsigned long actual;
    unsigned long start;
    unsigned long index;
    unsigned long surface_columns;
    unsigned long surface_rows;

    UNUSED(output);
    if (written != NULL) *written = 0u;
    if (coordinate.X < 0 || coordinate.Y < 0 ||
        softpc_compat_console_size.X <= 0 || softpc_compat_console_size.Y <= 0 ||
        coordinate.X >= softpc_compat_console_size.X ||
        coordinate.Y >= softpc_compat_console_size.Y) return FALSE;
    columns = (unsigned long)softpc_compat_console_size.X;
    rows = (unsigned long)softpc_compat_console_size.Y;
    if (!softpc_standalone_text_surface_geometry(&surface_columns, &surface_rows))
        return FALSE;
    start = (unsigned long)coordinate.Y * columns + (unsigned long)coordinate.X;
    actual = (unsigned long)count;
    if (actual > columns * rows - start) actual = columns * rows - start;
    for (index = 0u; index < actual; ++index) {
        unsigned long cell = start + index;
        unsigned long column = cell % columns;
        unsigned long row = cell / columns;

        /* The original Console buffer can temporarily be wider/taller than
           the 80x50 shared surface.  Only its visible intersection has a
           presentation slot. */
        if (!softpc_standalone_text_surface_write_cell(column, row,
                attribute ? 1u : 0u, value) &&
            (column < surface_columns && row < surface_rows))
            return FALSE;
    }
    if (written != NULL) *written = (DWORD)actual;
    return TRUE;
}

BOOL softpc_compat_fill_console_character(HANDLE output, CHAR value,
    DWORD count, COORD coordinate, LPDWORD written)
{
    return softpc_compat_fill_console_cell(output, (unsigned char)value, count,
        coordinate, written, 0);
}

BOOL softpc_compat_fill_console_attribute(HANDLE output, WORD value,
    DWORD count, COORD coordinate, LPDWORD written)
{
    return softpc_compat_fill_console_cell(output, (unsigned char)value, count,
        coordinate, written, 1);
}

BOOL softpc_compat_scroll_console_buffer(HANDLE output,
    const SMALL_RECT *scroll, const SMALL_RECT *clip, COORD destination,
    const CHAR_INFO *fill)
{
    UNUSED(output); UNUSED(scroll); UNUSED(clip); UNUSED(destination);
    UNUSED(fill);
    return TRUE;
}

BOOL softpc_compat_set_console_cursor_position(HANDLE output, COORD position)
{
    UNUSED(output);
    softpc_compat_cursor_column = position.X;
    softpc_compat_cursor_row = position.Y;
    softpc_compat_cursor_position_valid = 1;
    return TRUE;
}

BOOL softpc_compat_set_console_cursor_info(HANDLE output,
    const CONSOLE_CURSOR_INFO *cursor)
{
    UNUSED(output);
    if (cursor != NULL) {
        softpc_compat_cursor_size = cursor->dwSize;
        softpc_compat_cursor_visible = cursor->bVisible != FALSE;
    }
    return TRUE;
}

int softpc_compat_presentation_cursor(long *column_out, long *row_out,
    unsigned long *size_out)
{
    if (column_out == NULL || row_out == NULL || size_out == NULL ||
        !softpc_compat_cursor_position_valid || !softpc_compat_cursor_visible)
        return 0;
    *column_out = softpc_compat_cursor_column;
    *row_out = softpc_compat_cursor_row;
    *size_out = softpc_compat_cursor_size;
    return 1;
}

BOOL softpc_compat_get_current_console_font(HANDLE output, BOOL maximum,
    PCONSOLE_FONT_INFO font)
{
    UNUSED(output); UNUSED(maximum); UNUSED(font);
    return FALSE;
}

COORD softpc_compat_get_console_font_size(HANDLE output, DWORD font)
{
    COORD size;
    UNUSED(output); UNUSED(font);
    size.X = 0; size.Y = 0;
    return size;
}

BOOL softpc_compat_write_console(HANDLE output, const void *buffer,
    DWORD count, LPDWORD written, void *reserved)
{
    UNUSED(output); UNUSED(buffer); UNUSED(reserved);
    if (written != NULL) *written = count;
    return TRUE;
}
