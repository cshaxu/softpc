#include "insignia.h"
#include "host_def.h"

#include <stdint.h>
#include <string.h>

#include "gmi.h"
#include "gfx_upd.h"
#include "gvi.h"
#include "egaports.h"
#include "egamode.h"
#include "egagraph.h"
#include "egavideo.h"
#include "video.h"
#include "conapi.h"
#include "softpc_standalone_dib.h"
#include "nt_graph.h"

/* The original video core's optional stream-I/O path is a product console
   optimization.  The detached VM presents through its own console/window,
   so it remains disabled while retaining the original controller behavior. */
half_word *stream_io_buffer = NULL;
boolean stream_io_enabled = FALSE;
word stream_io_buffer_size = 0;
word *stream_io_dirty_count_ptr = NULL;

extern byte *EGA_planes;
extern byte *video_copy;
extern PC_palette *DAC;
extern void nt_mark_screen_refresh(void);

int softpc_platform_presentation_is_graphics(void)
{
    return sc.ModeType == GRAPHICS;
}

void softpc_platform_presentation_request_refresh(void)
{
    if (softpc_platform_presentation_is_graphics())
        nt_mark_screen_refresh();
}

int softpc_platform_presentation_cursor(long *column_out, long *row_out,
                                        unsigned long *size_out)
{
    return softpc_compat_presentation_cursor(column_out, row_out, size_out);
}

int softpc_platform_presentation_fonts(uint8_t *primary, uint8_t *secondary,
                                       unsigned long *height_out, unsigned long *attribute_select_out)
{
    static const unsigned long font_offsets[8] = {
        0u, 0x4000u, 0x8000u, 0xc000u, 0x2000u, 0x6000u, 0xa000u, 0xe000u};
    unsigned long height;
    unsigned long font;
    unsigned long character;

    unsigned long secondary_font;

    if (primary == NULL || secondary == NULL || height_out == NULL ||
        attribute_select_out == NULL || EGA_planes == NULL)
        return 0;
    height = sas_hw_at_no_check(ega_char_height);
    if (height == 0u || height > 16u)
        height = 16u;
    font = (unsigned long)get_prim_font_index() & 7u;
    secondary_font = (unsigned long)get_sec_font_index() & 7u;
    memset(primary, 0, 256u * 16u);
    memset(secondary, 0, 256u * 16u);
    for (character = 0u; character < 256u; ++character)
    {
        unsigned long row;
        byte *source = EGA_planes + FONT_BASE_ADDR + (font_offsets[font] << 2) +
                       ((unsigned long)FONT_MAX_HEIGHT * character << 2);
        byte *secondary_source = EGA_planes + FONT_BASE_ADDR +
                                 (font_offsets[secondary_font] << 2) +
                                 ((unsigned long)FONT_MAX_HEIGHT * character << 2);
        for (row = 0u; row < height; ++row)
        {
            primary[character * 16u + row] = source[row << 2];
            secondary[character * 16u + row] = secondary_source[row << 2];
        }
    }
    *height_out = height;
    *attribute_select_out = get_attrib_font_select() ? 1u : 0u;
    return 1;
}

int softpc_platform_presentation_font(uint8_t *glyphs,
                                      unsigned long *height_out)
{
    unsigned char secondary[256u * 16u];
    unsigned long attribute_select;

    return softpc_platform_presentation_fonts(glyphs, secondary, height_out,
                                              &attribute_select);
}

int softpc_platform_video_buffers_init(void)
{
    (void)softpc_standalone_dib_init();
    host_init_screen();
    return video_copy != NULL && EGA_planes != NULL && DAC != NULL;
}

void stream_io_update(void) {}
