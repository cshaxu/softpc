#include <assert.h>
#include <windows.h>

#include "core/compat/conapi.h"
#include "core/compat/dib_surface.h"

static unsigned char character_at(const unsigned char *cells,
    unsigned long column, unsigned long row, unsigned long stride,
    unsigned long cell_bytes)
{
    return cells[(row * stride + column) * cell_bytes];
}

static unsigned char attribute_at(const unsigned char *cells,
    unsigned long column, unsigned long row, unsigned long stride,
    unsigned long cell_bytes)
{
    return cells[(row * stride + column) * cell_bytes + 1u];
}

static void verify_dib_update_transaction(void)
{
    BITMAPINFO info;
    PALETTEENTRY palette;
    SMALL_RECT rect;
    long left;
    long top;
    long right;
    long bottom;

    ZeroMemory(&info, sizeof(info));
    info.bmiHeader.biSize = sizeof(info.bmiHeader);
    info.bmiHeader.biWidth = 640;
    info.bmiHeader.biHeight = -480;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 8;
    assert(softpc_standalone_dib_bind(&info));
    assert(!softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));

    ZeroMemory(&palette, sizeof(palette));
    rect.Left = 0;
    rect.Top = 4;
    rect.Right = 5;
    rect.Bottom = 6;
    /* A host callback without an enclosing original display update is still
       one complete transaction; no producer-specific outlet is needed. */
    assert(softpc_standalone_dib_damage(&rect));
    assert(softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    assert(left == 0 && top == 4 && right == 5 && bottom == 6);

    softpc_standalone_dib_begin_update();
    assert(softpc_standalone_dib_damage(&rect));
    assert(!softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    softpc_standalone_dib_set_palette_entries(&palette, 1);
    assert(!softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    softpc_standalone_dib_end_update();
    assert(softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    assert(left == 0 && top == 0 && right == 639 && bottom == 479);

    rect.Left = 7;
    rect.Top = 8;
    rect.Right = 9;
    rect.Bottom = 10;
    softpc_standalone_dib_begin_update();
    softpc_standalone_dib_begin_update();
    assert(softpc_standalone_dib_damage(&rect));
    softpc_standalone_dib_end_update();
    assert(!softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    softpc_standalone_dib_end_update();
    assert(softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    assert(left == 7 && top == 8 && right == 9 && bottom == 10);

    /* Consecutive complete updates before a frame copy retain both damage
       regions.  The presentation side will copy only the newest surface. */
    rect.Left = 11;
    rect.Top = 12;
    rect.Right = 13;
    rect.Bottom = 14;
    softpc_standalone_dib_begin_update();
    assert(softpc_standalone_dib_damage(&rect));
    softpc_standalone_dib_end_update();
    rect.Left = 15;
    rect.Top = 16;
    rect.Right = 17;
    rect.Bottom = 18;
    softpc_standalone_dib_begin_update();
    assert(softpc_standalone_dib_damage(&rect));
    softpc_standalone_dib_end_update();
    assert(softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    assert(left == 11 && top == 12 && right == 17 && bottom == 18);

    softpc_standalone_dib_begin_update();
    softpc_standalone_dib_set_palette_entries(&palette, 1);
    softpc_standalone_dib_end_update();
    assert(softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    assert(left == 0 && top == 0 && right == 639 && bottom == 479);

    info.bmiHeader.biWidth = 1280;
    assert(softpc_standalone_dib_bind(&info));
    assert(!softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    softpc_standalone_dib_invalidate_all();
    assert(softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    assert(left == 0 && top == 0 && right == 1279 && bottom == 479);
}

int main(void)
{
    const void *surface;
    const unsigned char *cells;
    unsigned long columns;
    unsigned long rows;
    unsigned long stride;
    unsigned long cell_bytes;
    CONSOLE_SCREEN_BUFFER_INFO info;
    COORD origin;
    COORD final_cell;
    COORD invalid;
    DWORD written;
    unsigned long index;

    assert(softpc_standalone_dib_init());
    verify_dib_update_transaction();
    assert(softpc_standalone_text_surface(&surface, &columns, &rows, &stride,
        &cell_bytes));
    assert(surface != NULL && columns == 80u && rows == 50u &&
        stride == 80u && cell_bytes >= 2u);
    cells = (const unsigned char *)surface;

    assert(softpc_compat_get_console_buffer_info(NULL, &info));
    assert(info.dwSize.X == 80 && info.dwSize.Y == 25);
    assert(info.srWindow.Left == 0 && info.srWindow.Top == 0 &&
        info.srWindow.Right == 79 && info.srWindow.Bottom == 24);

    origin.X = 0;
    origin.Y = 0;
    assert(softpc_compat_fill_console_character(NULL, 'X', 80u * 25u,
        origin, &written));
    assert(written == 80u * 25u);
    assert(softpc_compat_fill_console_attribute(NULL, 0x1eu,
        80u * 25u, origin, &written));
    assert(written == 80u * 25u);
    for (index = 0u; index < 80u * 25u; ++index) {
        assert(cells[index * cell_bytes] == 'X');
        assert(cells[index * cell_bytes + 1u] == 0x1eu);
    }

    final_cell.X = 0;
    final_cell.Y = 25;
    assert(!softpc_compat_fill_console_character(NULL, 'Z', 1u, final_cell,
        &written));
    assert(written == 0u);

    final_cell.X = 132;
    final_cell.Y = 25;
    assert(softpc_compat_set_console_buffer_size(NULL, final_cell));
    assert(softpc_compat_get_console_buffer_info(NULL, &info));
    assert(info.dwSize.X == 132 && info.dwSize.Y == 25);
    assert(info.dwMaximumWindowSize.X == 132 && info.dwMaximumWindowSize.Y == 25);
    final_cell.X = 0;
    final_cell.Y = 0;
    invalid.X = 131;
    invalid.Y = 24;
    {
        SMALL_RECT viewport;
        viewport.Left = final_cell.X;
        viewport.Top = final_cell.Y;
        viewport.Right = invalid.X;
        viewport.Bottom = invalid.Y;
        assert(softpc_compat_set_console_window_info(NULL, TRUE, &viewport));
        assert(softpc_compat_get_console_buffer_info(NULL, &info));
        assert(info.srWindow.Right == 131 && info.srWindow.Bottom == 24);
    }

    origin.X = 0;
    origin.Y = 0;
    assert(softpc_compat_fill_console_character(NULL, ' ', 132u * 25u,
        origin, &written));
    assert(written == 132u * 25u);
    for (index = 0u; index < 80u * 25u; ++index)
        assert(cells[index * cell_bytes] == ' ');

    final_cell.X = 80;
    final_cell.Y = 50;
    assert(softpc_compat_set_console_buffer_size(NULL, final_cell));

    final_cell.X = 78;
    final_cell.Y = 49;
    assert(softpc_compat_fill_console_character(NULL, ' ', 5u, final_cell,
        &written));
    assert(written == 2u);
    assert(character_at(cells, 78u, 49u, stride, cell_bytes) == ' ');
    assert(character_at(cells, 79u, 49u, stride, cell_bytes) == ' ');
    assert(attribute_at(cells, 78u, 49u, stride, cell_bytes) == 0u);

    assert(softpc_compat_fill_console_attribute(NULL, 0x07u, 5u, final_cell,
        &written));
    assert(written == 2u);
    assert(attribute_at(cells, 78u, 49u, stride, cell_bytes) == 0x07u);
    assert(attribute_at(cells, 79u, 49u, stride, cell_bytes) == 0x07u);
    assert(character_at(cells, 78u, 49u, stride, cell_bytes) == ' ');

    invalid.X = -1;
    invalid.Y = 0;
    assert(!softpc_compat_fill_console_character(NULL, 'Z', 1u, invalid,
        &written));
    assert(written == 0u);
    assert(character_at(cells, 0u, 0u, stride, cell_bytes) == ' ');

    return 0;
}
