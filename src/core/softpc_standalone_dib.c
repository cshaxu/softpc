#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "insignia.h"
#include "host_def.h"
#include "gmi.h"
#include "gfx_upd.h"
#include "softpc_standalone_dib.h"
#include "nt_graph.h"

#define SOFTPC_DIB_MAX_WIDTH 1056u
#define SOFTPC_DIB_MAX_HEIGHT 768u
#define SOFTPC_DIB_COLOURS 256u
#define SOFTPC_TEXT_COLUMNS 80u
#define SOFTPC_TEXT_ROWS 50u
#define SOFTPC_TEXT_CELL_BYTES 4u

SCREEN_DESCRIPTION sc;
int host_screen_scale = 2;
extern half_word bg_col_mask;
char *DIBData;
PBITMAPINFO MonoDIB;
PBITMAPINFO CGADIB;
PBITMAPINFO EGADIB;
PBITMAPINFO VGADIB;
BOOL FunnyPaintMode;

static BITMAPINFO *softpc_dib_info;
static unsigned char *softpc_dib_bits;
static unsigned long softpc_dib_width;
static unsigned long softpc_dib_height;
static SMALL_RECT softpc_dib_dirty;
static int softpc_dib_dirty_valid;

/* nt_cga.c owns the original text update algorithm.  Its Windows console
   sharing buffer becomes standalone-owned storage; the frontend consumes it
   through the DIB/text presenter instead of a console server. */
PBYTE textBuffer;
int now_height = 50;
int now_width = 80;

void closeGraphicsBuffer(void)
{
    /* The DIB is VM-owned and remains valid across text/graphics changes. */
}

int softpc_standalone_dib_init(void)
{
    size_t info_bytes;
    size_t bitmap_bytes;

    if (softpc_dib_bits != NULL) return 1;
    info_bytes = sizeof(BITMAPINFOHEADER) +
        SOFTPC_DIB_COLOURS * sizeof(RGBQUAD);
    bitmap_bytes = SOFTPC_DIB_MAX_WIDTH * SOFTPC_DIB_MAX_HEIGHT;
    softpc_dib_info = (BITMAPINFO *)calloc(1u, info_bytes);
    softpc_dib_bits = (unsigned char *)calloc(1u, bitmap_bytes);
    if (softpc_dib_info == NULL || softpc_dib_bits == NULL) {
        free(softpc_dib_info);
        free(softpc_dib_bits);
        softpc_dib_info = NULL;
        softpc_dib_bits = NULL;
        return 0;
    }
    softpc_dib_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    softpc_dib_info->bmiHeader.biWidth = SOFTPC_DIB_MAX_WIDTH;
    softpc_dib_info->bmiHeader.biHeight = -(LONG)SOFTPC_DIB_MAX_HEIGHT;
    softpc_dib_info->bmiHeader.biPlanes = 1;
    softpc_dib_info->bmiHeader.biBitCount = 8;
    softpc_dib_info->bmiHeader.biCompression = BI_RGB;
    softpc_dib_info->bmiHeader.biClrUsed = SOFTPC_DIB_COLOURS;
    sc.ScreenBufHandle = (HANDLE)softpc_dib_bits;
    sc.ConsoleBufInfo.lpBitMap = softpc_dib_bits;
    sc.ConsoleBufInfo.lpBitMapInfo = softpc_dib_info;
    sc.ConsoleBufInfo.hMutex = NULL;
    sc.PC_W_Width = SOFTPC_DIB_MAX_WIDTH;
    sc.PC_W_Height = SOFTPC_DIB_MAX_HEIGHT;
    if (textBuffer == NULL)
        textBuffer = (PBYTE)calloc(SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS,
            SOFTPC_TEXT_CELL_BYTES);
    if (textBuffer == NULL) return 0;
    DIBData = (char *)softpc_dib_bits;
    MonoDIB = softpc_dib_info;
    CGADIB = softpc_dib_info;
    EGADIB = softpc_dib_info;
    VGADIB = softpc_dib_info;
    softpc_dib_width = SOFTPC_DIB_MAX_WIDTH;
    softpc_dib_height = SOFTPC_DIB_MAX_HEIGHT;
    return 1;
}

int softpc_standalone_dib_resize(int width, int height, int bits_per_pixel)
{
    size_t stride;
    if (width <= 0 || height <= 0 || width > (int)SOFTPC_DIB_MAX_WIDTH ||
        height > (int)SOFTPC_DIB_MAX_HEIGHT || bits_per_pixel != 8 ||
        !softpc_standalone_dib_init()) return 0;

    /* The imported EGA/VGA painters calculate their addresses from the
       current DIB header.  Keep the backing allocation large, but publish
       the actual mode geometry exactly as nt_graph::graphicsResize did. */
    softpc_dib_info->bmiHeader.biWidth = width;
    softpc_dib_info->bmiHeader.biHeight = -height;
    softpc_dib_info->bmiHeader.biBitCount = (WORD)bits_per_pixel;
    softpc_dib_info->bmiHeader.biSizeImage =
        (DWORD)(((unsigned long)width + 3u) & ~3u) * (DWORD)height;
    softpc_dib_width = (unsigned long)width;
    softpc_dib_height = (unsigned long)height;
    stride = ((size_t)width + 3u) & ~(size_t)3u;
    memset(softpc_dib_bits, 0, stride * (size_t)height);
    sc.ScreenBufHandle = (HANDLE)softpc_dib_bits;
    sc.ConsoleBufInfo.lpBitMap = softpc_dib_bits;
    sc.ConsoleBufInfo.lpBitMapInfo = softpc_dib_info;
    sc.ActiveOutputBufferHandle = sc.ScreenBufHandle;
    sc.BitmapLastLine = (char *)softpc_dib_bits +
        ((size_t)height - 1u) * stride;
    DIBData = (char *)softpc_dib_bits;
    MonoDIB = softpc_dib_info;
    CGADIB = softpc_dib_info;
    EGADIB = softpc_dib_info;
    VGADIB = softpc_dib_info;
    return 1;
}

BOOL softpc_standalone_invalidate_dibits(HANDLE ignored,
    const SMALL_RECT *rect)
{
    UNUSED(ignored);
    if (rect == NULL) return FALSE;
    if (!softpc_dib_dirty_valid) {
        softpc_dib_dirty = *rect;
        softpc_dib_dirty_valid = 1;
    } else {
        if (rect->Left < softpc_dib_dirty.Left) softpc_dib_dirty.Left = rect->Left;
        if (rect->Top < softpc_dib_dirty.Top) softpc_dib_dirty.Top = rect->Top;
        if (rect->Right > softpc_dib_dirty.Right) softpc_dib_dirty.Right = rect->Right;
        if (rect->Bottom > softpc_dib_dirty.Bottom) softpc_dib_dirty.Bottom = rect->Bottom;
    }
    return TRUE;
}

int softpc_standalone_dib_surface(const void **bits_out, const void **info_out,
    unsigned long *width_out, unsigned long *height_out)
{
    if (bits_out == NULL || info_out == NULL || width_out == NULL ||
        height_out == NULL || softpc_dib_bits == NULL || softpc_dib_info == NULL)
        return 0;
    *bits_out = softpc_dib_bits;
    *info_out = softpc_dib_info;
    *width_out = softpc_dib_width;
    *height_out = softpc_dib_height;
    return 1;
}

int softpc_standalone_text_surface(const void **cells_out,
    unsigned long *columns_out, unsigned long *rows_out,
    unsigned long *stride_out, unsigned long *cell_bytes_out)
{
    if (cells_out == NULL || columns_out == NULL || rows_out == NULL ||
        stride_out == NULL || cell_bytes_out == NULL || textBuffer == NULL)
        return 0;
    *cells_out = textBuffer;
    *columns_out = SOFTPC_TEXT_COLUMNS;
    *rows_out = SOFTPC_TEXT_ROWS;
    *stride_out = SOFTPC_TEXT_COLUMNS;
    *cell_bytes_out = SOFTPC_TEXT_CELL_BYTES;
    return 1;
}
void softpc_standalone_dib_set_palette(PC_palette *palette, int count)
{
    int index;
    if (softpc_dib_info == NULL || palette == NULL || count <= 0) return;
    if (count > (int)SOFTPC_DIB_COLOURS) count = (int)SOFTPC_DIB_COLOURS;
    for (index = 0; index < count; ++index) {
        softpc_dib_info->bmiColors[index].rgbRed =
            (BYTE)(palette[index].red << 2);
        softpc_dib_info->bmiColors[index].rgbGreen =
            (BYTE)(palette[index].green << 2);
        softpc_dib_info->bmiColors[index].rgbBlue =
            (BYTE)(palette[index].blue << 2);
        softpc_dib_info->bmiColors[index].rgbReserved = 0;
    }
}
int softpc_standalone_dib_take_dirty(long *left, long *top, long *right,
    long *bottom)
{
    if (left == NULL || top == NULL || right == NULL || bottom == NULL ||
        !softpc_dib_dirty_valid) return 0;
    *left = softpc_dib_dirty.Left;
    *top = softpc_dib_dirty.Top;
    *right = softpc_dib_dirty.Right;
    *bottom = softpc_dib_dirty.Bottom;
    softpc_dib_dirty_valid = 0;
    return 1;
}
