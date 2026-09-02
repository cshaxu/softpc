#include <windows.h>
#include <string.h>

#include "insignia.h"
#include "host_def.h"
#include "conapi.h"
#include "nt_graph.h"
#include "softpc_standalone_dib.h"

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
static int softpc_compat_cursor_position_valid;
static int softpc_compat_cursor_visible = 1;

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
    info->dwSize.X = (SHORT)(sc.PC_W_Width > 0 ? sc.PC_W_Width : 80);
    info->dwSize.Y = (SHORT)(sc.PC_W_Height > 0 ? sc.PC_W_Height : 25);
    info->srWindow.Right = (SHORT)(info->dwSize.X - 1);
    info->srWindow.Bottom = (SHORT)(info->dwSize.Y - 1);
    return TRUE;
}

BOOL softpc_compat_set_console_window_info(HANDLE output, BOOL absolute,
    const SMALL_RECT *window)
{
    UNUSED(output); UNUSED(absolute); UNUSED(window);
    return TRUE;
}

BOOL softpc_compat_set_console_buffer_size(HANDLE output, COORD size)
{
    UNUSED(output); UNUSED(size);
    return TRUE;
}

BOOL softpc_compat_fill_console_character(HANDLE output, CHAR value,
    DWORD count, COORD coordinate, LPDWORD written)
{
    UNUSED(output); UNUSED(value); UNUSED(coordinate);
    if (written != NULL) *written = count;
    return TRUE;
}

BOOL softpc_compat_fill_console_attribute(HANDLE output, WORD value,
    DWORD count, COORD coordinate, LPDWORD written)
{
    UNUSED(output); UNUSED(value); UNUSED(coordinate);
    if (written != NULL) *written = count;
    return TRUE;
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
    if (cursor != NULL) softpc_compat_cursor_visible = cursor->bVisible != FALSE;
    return TRUE;
}

int softpc_compat_presentation_cursor(long *column_out, long *row_out)
{
    if (column_out == NULL || row_out == NULL ||
        !softpc_compat_cursor_position_valid || !softpc_compat_cursor_visible)
        return 0;
    *column_out = softpc_compat_cursor_column;
    *row_out = softpc_compat_cursor_row;
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
