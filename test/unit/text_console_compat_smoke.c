#include <assert.h>
#include <windows.h>

#include "compat/conapi.h"
#include "compat/dib_surface.h"

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

static void verify_dib_bind_does_not_publish(void)
{
    BITMAPINFO info;
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

    rect.Left = 3;
    rect.Top = 4;
    rect.Right = 5;
    rect.Bottom = 6;
    assert(softpc_standalone_invalidate_dibits(NULL, &rect));
    assert(softpc_standalone_dib_take_dirty(&left, &top, &right, &bottom));
    assert(left == 3 && top == 4 && right == 5 && bottom == 6);

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
    verify_dib_bind_does_not_publish();
    assert(softpc_standalone_text_surface(&surface, &columns, &rows, &stride,
        &cell_bytes));
    assert(surface != NULL && columns == 80u && rows == 50u &&
        stride == 80u && cell_bytes >= 2u);
    cells = (const unsigned char *)surface;

    assert(softpc_compat_get_console_buffer_info(NULL, &info));
    assert(info.dwSize.X == 80 && info.dwSize.Y == 50);
    assert(info.srWindow.Left == 0 && info.srWindow.Top == 0 &&
        info.srWindow.Right == 79 && info.srWindow.Bottom == 49);

    origin.X = 0;
    origin.Y = 0;
    assert(softpc_compat_fill_console_character(NULL, 'X',
        (DWORD)(columns * rows), origin, &written));
    assert(written == columns * rows);
    assert(softpc_compat_fill_console_attribute(NULL, 0x1eu,
        (DWORD)(columns * rows), origin, &written));
    assert(written == columns * rows);
    for (index = 0u; index < columns * rows; ++index) {
        assert(cells[index * cell_bytes] == 'X');
        assert(cells[index * cell_bytes + 1u] == 0x1eu);
    }

    final_cell.X = 78;
    final_cell.Y = 49;
    assert(softpc_compat_fill_console_character(NULL, ' ', 5u, final_cell,
        &written));
    assert(written == 2u);
    assert(character_at(cells, 78u, 49u, stride, cell_bytes) == ' ');
    assert(character_at(cells, 79u, 49u, stride, cell_bytes) == ' ');
    assert(attribute_at(cells, 78u, 49u, stride, cell_bytes) == 0x1eu);

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
    assert(character_at(cells, 0u, 0u, stride, cell_bytes) == 'X');

    return 0;
}
