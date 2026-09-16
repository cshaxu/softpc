#include "compat/machine.h"
#include "../lib/cleanup.h"
#include "insignia.h"
#include "host_def.h"
#include "xt.h"
#include "gmi.h"
#include "gfx_upd.h"
#include "egamode.h"
#include "gvi.h"
#include "egagraph.h"
#include "egaports.h"
#include "compat/dib_surface.h"
#include "nt_graph.h"

/* Original SoftPC headers erase const for pre-ANSI compilers.  Restore it
   for this C17 test's public-machine API calls. */
#undef const

#include <assert.h>
#include <stdio.h>
#include <limits.h>

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char reserved;
} softpc_test_rgbquad;

typedef struct {
    unsigned char header[40];
    softpc_test_rgbquad colours[256];
} softpc_test_dib_info;

/* Direct original-renderer probes use the existing detached DIB, not a UI. */
extern unsigned char *EGA_planes;
extern void nt_ega_lo_graph_std(int offset, int screen_x, int screen_y,
    int width, int height);
extern void nt_ega_med_graph_std(int offset, int screen_x, int screen_y,
    int width, int height);
extern void nt_ega_hi_graph_std(int offset, int screen_x, int screen_y,
    int width, int height);
extern void nt_vga_hi_graph_std(int offset, int screen_x, int screen_y,
    int width, int height);
extern void nt_vga_graph_std(int offset, int screen_x, int screen_y,
    int width, int height);
extern void nt_vga_med_graph_std(int offset, int screen_x, int screen_y,
    int width, int height);
extern void nt_init_ega_lo_graph(void);
extern void nt_v7vga_hi_graph_std(int offset, int screen_x, int screen_y,
    int width, int height);
extern void nt_text(int ScreenOffset, int ScreenX, int ScreenY, int len,
    int height);
extern int softpc_device_bop_dispatch(unsigned char number,
    unsigned int argument);
extern void c_setAL(unsigned char value);
extern void c_setAH(unsigned char value);
extern void c_setBX(unsigned short value);
extern unsigned short c_getAX(void);
extern unsigned short c_getBX(void);
extern unsigned short c_getCX(void);
extern unsigned char Currently_emulated_video_mode;
extern void host_timer_event(void);
extern PC_palette *DAC;

typedef struct {
    void (*paint)(int, int, int, int, int);
    int x_pixels, width_pixels, y_pixels, source_group;
} painter_probe;

static void check_paint(const painter_probe *p, unsigned char *guard,
    int offset, int x, int y, int width, int height, int expected_w,
    int expected_h)
{
    int dx, dy;
    size_t size = (size_t)sc.PC_W_Width * sc.PC_W_Height;
    memset(guard, 0xa5, size + 32u);
    p->paint(offset, x, y, width, height);
    for (dy = 0; dy < sc.PC_W_Height; ++dy)
        for (dx = 0; dx < sc.PC_W_Width; ++dx) {
            int inside = expected_w > 0 && expected_h > 0 &&
                dx >= x * p->x_pixels && dy >= y * p->y_pixels &&
                dx < x * p->x_pixels + expected_w * p->width_pixels &&
                dy < y * p->y_pixels + expected_h * p->y_pixels;
            assert(guard[16u + (size_t)dy * sc.PC_W_Width + dx] ==
                (inside ? 0 : 0xa5));
        }
    for (dx = 0; dx < 16; ++dx) {
        assert(guard[dx] == 0xa5);
        assert(guard[16u + size + dx] == 0xa5);
    }
}

static void verify_painter_bounds(void)
{
    static const painter_probe painters[] = {
        { nt_ega_lo_graph_std, 2, 16, 2, 4 },
        { nt_ega_med_graph_std, 1, 8, 2, 4 },
        { nt_ega_hi_graph_std, 1, 8, 1, 4 },
        { nt_vga_graph_std, 2, 2, 2, 1 },
        { nt_vga_med_graph_std, 8, 8, 2, 4 },
        { nt_vga_hi_graph_std, 8, 8, 1, 4 },
        { nt_v7vga_hi_graph_std, 1, 1, 1, 1 }
    };
    void *saved = sc.ConsoleBufInfo.lpBitMap;
    size_t size = (size_t)sc.PC_W_Width * sc.PC_W_Height;
    unsigned char *guard = malloc(size + 32u);
    size_t i;
    assert(guard != NULL);
    nt_init_ega_lo_graph();
    memset(EGA_planes, 0, 4u * EGA_PLANE_SIZE);
    sc.ConsoleBufInfo.lpBitMap = guard + 16u;
    for (i = 0; i < sizeof(painters) / sizeof(painters[0]); ++i) {
        const painter_probe *p = &painters[i];
        int x = (sc.PC_W_Width - p->width_pixels) / p->x_pixels;
        int y = sc.PC_W_Height / p->y_pixels - 1;
        int capacity = 4 * EGA_PLANE_SIZE / p->source_group;
        check_paint(p, guard, 0, 8, 2, 2, 2, 2, 2);
        check_paint(p, guard, 0, 0, 0, 1, 0, 0, 0);
        check_paint(p, guard, 0, 0, 0, 0, 1, 0, 0);
        check_paint(p, guard, 0, 0, 0, -1, 1, 0, 0);
        check_paint(p, guard, 0, 0, 0, 1, -1, 0, 0);
        check_paint(p, guard, 0, -1, 0, 1, 1, 0, 0);
        check_paint(p, guard, 0, 0, -1, 1, 1, 0, 0);
        check_paint(p, guard, -1, 0, 0, 1, 1, 0, 0);
        check_paint(p, guard, INT_MAX, 0, 0, 1, 1, 0, 0);
        check_paint(p, guard, 0, INT_MAX, INT_MAX, 1, 1, 0, 0);
        check_paint(p, guard, 0, x, y, INT_MAX, INT_MAX, 1, 1);
        check_paint(p, guard, capacity - 1, 8, 2, 2, 2, 1, 1);
        check_paint(p, guard, capacity, 0, 0, 1, 1, 0, 0);
    }
    sc.ConsoleBufInfo.lpBitMap = saved;
    free(guard);
}

static void make_boot_disk(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file = fopen(path, "wb");
    assert(file != NULL);
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fseek(file, 1474560L - 1L, SEEK_SET) == 0);
    assert(fputc(0, file) == 0);
    assert(fclose(file) == 0);
}

int main(void)
{
    const char *path = "softpc-original-dib-smoke.img";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    const void *bits = NULL;
    const void *info = NULL;
    uint32_t width = 0;
    uint32_t height = 0;

    make_boot_disk(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);

    /* ModeType becomes authoritative only after the original controller has
       selected its first display mode.  Do not impose the retired standalone
       dispatcher's reset-time TEXT default here. */
    assert(softpc_machine_presentation_dib(machine, &bits, &info, &width,
        &height));
    assert(bits != NULL && info != NULL);
    assert(width == 1280u && height == 768u);

    /* This fixed V7 model is the original 512 KiB card: INT 10h/6Fh/07h
       reports two 256 KiB blocks.  Do not infer a 1 MiB virtual adapter from
       any host-side surface allocation. */
    c_setAH(0x6fu);
    c_setAL(7u);
    assert(softpc_device_bop_dispatch(0x42u, 0u));
    assert(c_getAX() == 0x826fu);
    assert(c_getBX() == 0x7070u);
    assert(c_getCX() == 0u);

    /* The standalone front end gets its glyphs from the original loaded
       EGA/VGA font plane, never a host-installed TrueType font. */
    {
        unsigned char font[256u * 16u];
        unsigned char secondary_font[256u * 16u];
        uint32_t font_height = 0;
        uint32_t attribute_font_select = 0;
        unsigned int row;
        unsigned int populated = 0;
        assert(softpc_machine_presentation_font(machine, font, &font_height));
        assert(font_height == 16u);
        for (row = 0u; row < font_height; ++row)
            populated |= font[(unsigned int)'A' * 16u + row];
        assert(populated != 0u);
        assert(softpc_machine_presentation_fonts(machine, font,
            secondary_font, &font_height, &attribute_font_select));
        assert(font_height == 16u);
    }

    /* The C-VID mode-transition path can emit an empty text repaint.  The
       original nt_text bulk-copy calculation must discard it rather than
       treating (height - 1) as an enormous byte count. */
    nt_text(0, 0, 0, 160, 0);

    /* The V7 ROM hands INT 10h extension 6Fh to the original EGA BOP 42h.
       Per v7_video.c, subfunction 5 selects 60h: 752x410, 16 colours. */
    c_setAH(0x6fu);
    c_setAL(5u);
    c_setBX(0x0060u);
    assert(softpc_device_bop_dispatch(0x42u, 0u));
    assert(c_getAX() == 0x6f05u);
    assert(c_getBX() == 0x0060u);
    assert(Currently_emulated_video_mode == 0x60u);
    host_timer_event();
    host_timer_event();
    assert(softpc_machine_presentation_is_graphics(machine));
    /* nt_graph must carry its original mode-change tail into the standalone
       host: selecting V7 640x400 recreates the DIB at the controller's
       original 752x410 host geometry (including its border), rather than
       leaving painters on the initial 1280x768 allocation. */
    assert(softpc_machine_presentation_dib(machine, &bits, &info, &width,
        &height));
    assert(width == 752u && height == 410u);

    /* A dirty-region transition may be clipped to zero height.  The original
       VGA painter must reject that empty region before its historical
       do/while loop consumes the VGA-plane buffer. */
    nt_ega_hi_graph_std(0, 0, 0, 1, 0);
    nt_ega_med_graph_std(0, 0, 0, 1, 0);
    nt_ega_lo_graph_std(0, 0, 0, 1, 0);
    nt_vga_hi_graph_std(0, 0, 0, 1, 0);

    /* A V7 wrap-edge dirty record can name the final source byte but an
       overlarge height.  The original painter must clip the source rows to
       the allocated four-plane buffer before dereferencing the next row. */
    nt_vga_hi_graph_std(EGA_PLANE_SIZE - 1, 0, 0, 1, 768);

    /* 63h is the original 1024x768 two-colour V7 mode, not 800x600 packed
       256-colour.  This test deliberately does not call the packed painter
       while it is selected. */
    c_setAH(0x6fu);
    c_setAL(5u);
    c_setBX(0x0063u);
    assert(softpc_device_bop_dispatch(0x42u, 0u));
    assert(c_getAX() == 0x6f05u);
    assert(c_getBX() == 0x0063u);
    assert(Currently_emulated_video_mode == 0x63u);
    host_timer_event();
    host_timer_event();
    assert(softpc_machine_presentation_is_graphics(machine));
    assert(softpc_machine_presentation_dib(machine, &bits, &info, &width,
        &height));
    assert(width == 1024u && height == 768u);

    /* Modes 60h-65h are not all packed 256-colour layouts.  The original
       V7 BIOS selects 65h as its 1024x768 16-colour mode; nt_graph therefore
       selects nt_ega_hi_graph_std and its original four-plane LUT path.  The
       standalone host owns only the destination DIB and dirty callback. */
    c_setAH(0x6fu);
    c_setAL(5u);
    c_setBX(0x0065u);
    assert(softpc_device_bop_dispatch(0x42u, 0u));
    assert(Currently_emulated_video_mode == 0x65u);
    host_timer_event();
    host_timer_event();
    assert(softpc_machine_presentation_is_graphics(machine));
    assert(softpc_machine_presentation_dib(machine, &bits, &info, &width,
        &height));
    assert(width == 1024u && height == 768u);
    {
        int32_t left;
        int32_t top;
        int32_t right;
        int32_t bottom;

        while (softpc_machine_presentation_take_dirty(machine, &left, &top,
            &right, &bottom)) {
        }
        EGA_planes[0] = 0x80u;
        EGA_planes[1] = 0x00u;
        EGA_planes[2] = 0x00u;
        EGA_planes[3] = 0x00u;
        nt_ega_hi_graph_std(0, 0, 0, 1, 1);
        assert(softpc_machine_presentation_take_dirty(machine, &left, &top,
            &right, &bottom));
        assert(left == 0 && top == 0 && right == 7 && bottom == 0);
    }

    /* 66h and 67h are the two original packed 256-colour V7 modes whose
       nt_v7vga_hi_graph_std output contract is exercised here. */
    c_setAH(0x6fu);
    c_setAL(5u);
    c_setBX(0x0066u);
    assert(softpc_device_bop_dispatch(0x42u, 0u));
    assert(Currently_emulated_video_mode == 0x66u);
    host_timer_event();
    host_timer_event();
    assert(softpc_machine_presentation_dib(machine, &bits, &info, &width,
        &height));
    assert(width == 640u && height == 400u);

    c_setAH(0x6fu);
    c_setAL(5u);
    c_setBX(0x0067u);
    assert(softpc_device_bop_dispatch(0x42u, 0u));
    assert(Currently_emulated_video_mode == 0x67u);
    host_timer_event();
    host_timer_event();
    assert(softpc_machine_presentation_dib(machine, &bits, &info, &width,
        &height));
    assert(width == 640u && height == 480u);
    {
        int32_t left;
        int32_t top;
        int32_t right;
        int32_t bottom;
        unsigned char *surface = (unsigned char *)bits;

        while (softpc_machine_presentation_take_dirty(machine, &left, &top,
            &right, &bottom)) {
        }
        EGA_planes[0] = 0x2au;
        EGA_planes[1] = 0x7eu;
        nt_v7vga_hi_graph_std(0, 0, 0, 2, 1);
        assert(surface[0] == 0x2au);
        assert(surface[1] == 0x7eu);
        assert(softpc_machine_presentation_take_dirty(machine, &left, &top,
            &right, &bottom));
        assert(left == 0 && top == 0 && right == 1 && bottom == 0);
    }

    /* 68h and 69h use the same original packed V7 painter at the remaining
       two geometries listed in v7_video.c. */
    c_setAH(0x6fu);
    c_setAL(5u);
    c_setBX(0x0068u);
    assert(softpc_device_bop_dispatch(0x42u, 0u));
    assert(Currently_emulated_video_mode == 0x68u);
    host_timer_event();
    host_timer_event();
    assert(softpc_machine_presentation_dib(machine, &bits, &info, &width,
        &height));
    assert(width == 720u && height == 540u);

    c_setAH(0x6fu);
    c_setAL(5u);
    c_setBX(0x0069u);
    assert(softpc_device_bop_dispatch(0x42u, 0u));
    assert(Currently_emulated_video_mode == 0x69u);
    host_timer_event();
    host_timer_event();
    assert(softpc_machine_presentation_dib(machine, &bits, &info, &width,
        &height));
    assert(width == 800u && height == 600u);
    verify_painter_bounds();
    {
        int32_t left;
        int32_t top;
        int32_t right;
        int32_t bottom;
        unsigned char *surface = (unsigned char *)bits;

        while (softpc_machine_presentation_take_dirty(machine, &left, &top,
            &right, &bottom)) {
        }
        EGA_planes[0] = 0x33u;
        EGA_planes[1] = 0x66u;
        nt_v7vga_hi_graph_std(0, 0, 0, 2, 1);
        assert(surface[0] == 0x33u);
        assert(surface[1] == 0x66u);
        assert(softpc_machine_presentation_take_dirty(machine, &left, &top,
            &right, &bottom));
        assert(left == 0 && top == 0 && right == 1 && bottom == 0);
    }

    /* The original nt_graph VLT maps the attribute-controller palette to
       DIB entries; simply copying DAC[] would leave index zero black here. */
    {
        const softpc_test_dib_info *dib = (const softpc_test_dib_info *)info;
        int32_t left;
        int32_t top;
        int32_t right;
        int32_t bottom;
        set_256_colour_mode(FALSE);
        set_colour_select(FALSE);
        set_top_pixel_pad(0);
        set_DAC_mask(0xffu);
        set_palette_val(0, 1u);
        DAC[1].red = 1u;
        DAC[1].green = 2u;
        DAC[1].blue = 3u;
        while (softpc_machine_presentation_take_dirty(machine, &left, &top,
            &right, &bottom)) {
        }
        /* Route through the original VIDEOFUNCS mark-refresh callback. */
        host_mark_screen_refresh();
        host_timer_event();
        host_timer_event();
        assert(dib->colours[0].red == 4u);
        assert(dib->colours[0].green == 8u);
        assert(dib->colours[0].blue == 12u);
        /* Palette-only writes change no EGA plane.  The original console
           redraws after SetConsolePalette; the standalone RGB snapshot must
           publish the same change so a guest such as Windows Setup cannot
           retain stale red/white entries after programming blue/yellow. */
        assert(softpc_machine_presentation_take_dirty(machine, &left, &top,
            &right, &bottom));
        assert(left == 0 && top == 0 && right == (int32_t)width - 1 &&
            bottom == (int32_t)height - 1);
    }
    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
