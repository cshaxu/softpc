#include "insignia.h"
#include "host_def.h"

#include "gmi.h"
#include "gfx_upd.h"
#include "gvi.h"
#include "egaports.h"
#include "egamode.h"
#include "egagraph.h"
#include "egavideo.h"
#include "softpc_standalone_dib.h"

/* V7's original controller owns the hardware-pointer registers.  Its pattern
   storage is 32 rows of four screen-mask plane bytes, followed by 32 rows of
   four XOR-mask plane bytes.  The controller supplies the pattern's physical
   address and current coordinates here; the standalone host merely composites
   that already-selected hardware pointer into the DIB which the original
   nt_ega/nt_vga painters own. */
#define SOFTPC_V7_POINTER_WIDTH 32u
#define SOFTPC_V7_POINTER_HEIGHT 32u
#define SOFTPC_V7_POINTER_MASK_BYTES_PER_ROW 4u
#define SOFTPC_V7_POINTER_MASK_SIZE \
    (SOFTPC_V7_POINTER_HEIGHT * SOFTPC_V7_POINTER_MASK_BYTES_PER_ROW)

static unsigned char softpc_v7_pointer_background[
    SOFTPC_V7_POINTER_WIDTH * SOFTPC_V7_POINTER_HEIGHT];
static unsigned long softpc_v7_pointer_background_width;
static unsigned long softpc_v7_pointer_background_height;
static long softpc_v7_pointer_left;
static long softpc_v7_pointer_top;
static long softpc_v7_pointer_right;
static long softpc_v7_pointer_bottom;
static int softpc_v7_pointer_visible;

extern byte *EGA_planes;

static void softpc_v7_pointer_clear(x, y)
word x;
word y;
{
    const void *surface;
    const void *info;
    unsigned long width;
    unsigned long height;
    unsigned char *pixels;
    unsigned long stride;
    long row;

    UNUSED(x);
    UNUSED(y);
    if (!softpc_v7_pointer_visible ||
        !softpc_standalone_dib_surface(&surface, &info, &width, &height) ||
        width != softpc_v7_pointer_background_width ||
        height != softpc_v7_pointer_background_height)
    {
        softpc_v7_pointer_visible = 0;
        return;
    }
    pixels = (unsigned char *)surface;
    stride = (width + 3u) & ~3u;
    for (row = softpc_v7_pointer_top; row <= softpc_v7_pointer_bottom; ++row)
    {
        long column;
        for (column = softpc_v7_pointer_left;
             column <= softpc_v7_pointer_right; ++column)
        {
            unsigned long saved = (unsigned long)(row - softpc_v7_pointer_top) *
                SOFTPC_V7_POINTER_WIDTH +
                (unsigned long)(column - softpc_v7_pointer_left);
            pixels[(unsigned long)row * stride + (unsigned long)column] =
                softpc_v7_pointer_background[saved];
        }
    }
    {
        SMALL_RECT dirty;
        dirty.Left = (SHORT)softpc_v7_pointer_left;
        dirty.Top = (SHORT)softpc_v7_pointer_top;
        dirty.Right = (SHORT)softpc_v7_pointer_right;
        dirty.Bottom = (SHORT)softpc_v7_pointer_bottom;
        (void)softpc_standalone_invalidate_dibits(NULL, &dirty);
    }
    softpc_v7_pointer_visible = 0;
}

static void softpc_v7_pointer_paint(pattern, x, y)
sys_addr pattern;
word x;
word y;
{
    const void *surface;
    const void *info;
    unsigned long width;
    unsigned long height;
    unsigned char *pixels;
    const unsigned char *cursor;
    unsigned long stride;
    unsigned long row;
    long left = (long)x;
    long top = (long)y;

    /* A V7 pattern occupies 256 physical bytes.  EGA_planes contains all
       four interleaved VGA planes, so this check also covers the one-MiB
       compatibility allocation retained by the original V7 controller. */
    if (EGA_planes == NULL || pattern > (4u * EGA_PLANE_SIZE) -
        (2u * SOFTPC_V7_POINTER_MASK_SIZE) ||
        !softpc_standalone_dib_surface(&surface, &info, &width, &height) ||
        width == 0u || height == 0u)
        return;

    softpc_v7_pointer_clear(0, 0);
    if (left >= (long)width || top >= (long)height ||
        left + (long)SOFTPC_V7_POINTER_WIDTH <= 0 ||
        top + (long)SOFTPC_V7_POINTER_HEIGHT <= 0)
        return;

    softpc_v7_pointer_left = left < 0 ? 0 : left;
    softpc_v7_pointer_top = top < 0 ? 0 : top;
    softpc_v7_pointer_right = left + (long)SOFTPC_V7_POINTER_WIDTH - 1;
    if (softpc_v7_pointer_right >= (long)width)
        softpc_v7_pointer_right = (long)width - 1;
    softpc_v7_pointer_bottom = top + (long)SOFTPC_V7_POINTER_HEIGHT - 1;
    if (softpc_v7_pointer_bottom >= (long)height)
        softpc_v7_pointer_bottom = (long)height - 1;
    softpc_v7_pointer_background_width = width;
    softpc_v7_pointer_background_height = height;
    pixels = (unsigned char *)surface;
    cursor = EGA_planes + pattern;
    stride = (width + 3u) & ~3u;

    for (row = 0u; row < SOFTPC_V7_POINTER_HEIGHT; ++row)
    {
        const unsigned char *screen_mask_bytes = cursor + row *
            SOFTPC_V7_POINTER_MASK_BYTES_PER_ROW;
        const unsigned char *xor_mask_bytes = cursor + SOFTPC_V7_POINTER_MASK_SIZE +
            row * SOFTPC_V7_POINTER_MASK_BYTES_PER_ROW;
        unsigned long column;
        for (column = 0u; column < SOFTPC_V7_POINTER_WIDTH; ++column)
        {
            long pixel_x = left + (long)column;
            long pixel_y = top + (long)row;
            unsigned char screen_mask;
            unsigned char cursor_mask;
            unsigned char *pixel;
            unsigned long saved;

            if (pixel_x < softpc_v7_pointer_left ||
                pixel_x > softpc_v7_pointer_right ||
                pixel_y < softpc_v7_pointer_top ||
                pixel_y > softpc_v7_pointer_bottom)
                continue;
            screen_mask = (unsigned char)((screen_mask_bytes[column >> 3u] >>
                (7u - (column & 7u))) & 1u);
            cursor_mask = (unsigned char)((xor_mask_bytes[column >> 3u] >>
                (7u - (column & 7u))) & 1u);
            pixel = pixels + (unsigned long)pixel_y * stride +
                (unsigned long)pixel_x;
            saved = (unsigned long)(pixel_y - softpc_v7_pointer_top) *
                SOFTPC_V7_POINTER_WIDTH +
                (unsigned long)(pixel_x - softpc_v7_pointer_left);
            softpc_v7_pointer_background[saved] = *pixel;
            *pixel = (unsigned char)((*pixel & (screen_mask ? 0xffu : 0u)) ^
                (cursor_mask ? 0xffu : 0u));
        }
    }
    softpc_v7_pointer_visible = 1;
    {
        SMALL_RECT dirty;
        dirty.Left = (SHORT)softpc_v7_pointer_left;
        dirty.Top = (SHORT)softpc_v7_pointer_top;
        dirty.Right = (SHORT)softpc_v7_pointer_right;
        dirty.Bottom = (SHORT)softpc_v7_pointer_bottom;
        (void)softpc_standalone_invalidate_dibits(NULL, &dirty);
    }
}

void (*paint_v7ptr)() = softpc_v7_pointer_paint;
void (*clear_v7ptr)() = softpc_v7_pointer_clear;
