#include "insignia.h"
#include "host_def.h"
#include "platform.h"
#include "devices/snapshot.h"

#include <stdint.h>
#include <string.h>

#include "gmi.h"
#include "xt.h"
#include "config.h"
#include "gfx_upd.h"
#include "gvi.h"
#include "egaports.h"
#include "egamode.h"
#include "egagraph.h"
#include "egavideo.h"
#include "video.h"
#include "conapi.h"
#include "dib_surface.h"
#include "nt_graph.h"


/* The original video core's optional stream-I/O path is a product console
   optimization.  The detached VM presents through its own console/window,
   so it remains disabled while retaining the original controller behavior. */
half_word *stream_io_buffer = NULL;
boolean stream_io_enabled = FALSE;
word stream_io_buffer_size = 0;
word *stream_io_dirty_count_ptr = NULL;


int softpc_platform_presentation_is_graphics(void)
{
    return sc.ModeType == GRAPHICS;
}

int softpc_platform_presentation_state(uint32_t *mode_type_out,
    uint32_t *screen_state_out)
{
    if (mode_type_out == NULL || screen_state_out == NULL) return 0;
    *mode_type_out = (uint32_t)sc.ModeType;
    *screen_state_out = (uint32_t)sc.ScreenState;
    return 1;
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

int softpc_platform_presentation_text_extent(unsigned long *columns_out,
    unsigned long *rows_out)
{
    extern int now_width, now_height;
    if (columns_out == NULL || rows_out == NULL ||
        get_display_disabled() || get_mode_change_required() ||
        now_width <= 0 || now_height <= 0)
        return 0;
    /* textResize commits these dimensions for the selected text painter.
       Live controller registers may already describe the next graphics mode
       while the renderer still presents the preceding text surface. */
    *columns_out = (unsigned long)now_width;
    *rows_out = (unsigned long)now_height;
    return 1;
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
    height = get_char_height();
    font = (unsigned long)get_prim_font_index() & 7u;
    secondary_font = (unsigned long)get_sec_font_index() & 7u;
    memset(primary, 0, 256u * 16u);
    memset(secondary, 0, 256u * 16u);
    *height_out = height;
    *attribute_select_out = get_attrib_font_select() ? 1u : 0u;
    /* Preserve unsupported metadata for the receiving frame validator. No
       glyph may be copied beyond the fixed destination bank. Zero means the
       existing default height, not an unsupported font. */
    if (height > 16u) return 1;
    if (height == 0u) height = 16u;
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
    return 1;
}

int softpc_platform_video_buffers_init(void)
{
    (void)softpc_standalone_dib_init();
    host_init_screen();
    return video_copy != NULL && EGA_planes != NULL && DAC != NULL;
}

void stream_io_update(void) {}

int softpc_device_snapshot_capture_video_memory(
    softpc_device_video_memory_state *state)
{
    unsigned long index;

    if (state == NULL || EGA_planes == NULL || DAC == NULL ||
        sizeof(state->plane) != 4u * EGA_PLANE_SIZE ||
        SOFTPC_DEVICE_VIDEO_DAC_COUNT != VGA_DAC_SIZE)
        return 0;
    memcpy(state->plane, EGA_planes, sizeof(state->plane));
    for (index = 0u; index < SOFTPC_DEVICE_VIDEO_DAC_COUNT; ++index) {
        state->dac[index][0] = (uint8_t)DAC[index].red;
        state->dac[index][1] = (uint8_t)DAC[index].green;
        state->dac[index][2] = (uint8_t)DAC[index].blue;
    }
    return 1;
}

int softpc_device_snapshot_restore_video_memory(
    const softpc_device_video_memory_state *state)
{
    unsigned long index;

    if (state == NULL || EGA_planes == NULL || DAC == NULL ||
        !softpc_standalone_dib_init() ||
        sizeof(state->plane) != 4u * EGA_PLANE_SIZE ||
        SOFTPC_DEVICE_VIDEO_DAC_COUNT != VGA_DAC_SIZE)
        return 0;
    memcpy(EGA_planes, state->plane, sizeof(state->plane));
    for (index = 0u; index < SOFTPC_DEVICE_VIDEO_DAC_COUNT; ++index) {
        DAC[index].red = (half_word)state->dac[index][0];
        DAC[index].green = (half_word)state->dac[index][1];
        DAC[index].blue = (half_word)state->dac[index][2];
    }
    flag_palette_change_required();
    return 1;
}

int softpc_device_snapshot_rebuild_video_presentation(void)
{
    if (!softpc_standalone_dib_init()) return 0;
    /* The archive restores controller registers directly.  Unlike normal
       port writes, that does not select the original painter or recreate its
       DIB.  Select the restored mode unconditionally: ModeType itself is
       derived by that original path, so it cannot decide whether to call it.
       An initial 1280x768 blank allocation is not a restored frame. */
    /* The original host caches geometry to avoid resize work on ordinary
       mode changes.  Those caches describe the pre-load surface, so reset
       them before the original selector derives the restored one. */
    resetWindowParams();
    if (choose_display_mode == NULL || !(*choose_display_mode)()) {
        return 0;
    }
    /* This direct selection consumes the pending register-mode change; leave
       the original painter eligible for the one full repaint below. */
    set_mode_change_required(FALSE);
    host_mark_screen_refresh();
    /* The normal graphics tick applies the restored indexed DAC before it
       paints.  Snapshot rebuilding paints synchronously instead, so perform
       that one required palette step here rather than publishing pixels
       through the still-zero default palette. */
    set_the_vlt();
    host_flush_screen();
    if (softpc_platform_presentation_is_graphics())
        softpc_standalone_dib_invalidate_all();
    return 1;
}
