#include "softpc_machine.h"
#include "insignia.h"
#include "host_def.h"
#include "xt.h"
#include "gmi.h"
#include "gfx_upd.h"
#include "egamode.h"
#include "gvi.h"
#include "egagraph.h"
#include "egaports.h"

/* Original SoftPC headers erase const for pre-ANSI compilers.  Restore it
   for this C17 test's public-machine API calls. */
#undef const

#include <assert.h>
#include <stdio.h>

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

/* Keep this regression on the standalone DIB contract; nt_graph.h carries
   the historical console-server structure and is intentionally not needed by
   this direct original-renderer call. */
extern unsigned char *EGA_planes;
extern void nt_vga_hi_graph_std(int offset, int screen_x, int screen_y,
    int width, int height);
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
extern unsigned char Currently_emulated_video_mode;
extern void host_timer_event(void);
extern PC_palette *DAC;

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
    assert(width == 1056u && height == 768u);

    /* The standalone front end gets its glyphs from the original loaded
       EGA/VGA font plane, never a host-installed TrueType font. */
    {
        unsigned char font[256u * 16u];
        uint32_t font_height = 0;
        unsigned int row;
        unsigned int populated = 0;
        assert(softpc_machine_presentation_font(machine, font, &font_height));
        assert(font_height == 16u);
        for (row = 0u; row < font_height; ++row)
            populated |= font[(unsigned int)'A' * 16u + row];
        assert(populated != 0u);
    }

    /* The C-VID mode-transition path can emit an empty text repaint.  The
       original nt_text bulk-copy calculation must discard it rather than
       treating (height - 1) as an enormous byte count. */
    nt_text(0, 0, 0, 160, 0);

    /* The V7 ROM hands INT 10h extension 6Fh to the original EGA BOP 42h.
       Subfunction 5 selects an extended 640x400 256-colour mode (60h). */
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
       leaving painters on the initial 1056x768 allocation. */
    assert(softpc_machine_presentation_dib(machine, &bits, &info, &width,
        &height));
    assert(width == 752u && height == 410u);

    /* A dirty-region transition may be clipped to zero height.  The original
       nt_vga_hi_graph_std renderer must reject that empty region before its
       historical do/while loop consumes the VGA-plane buffer. */
    nt_vga_hi_graph_std(0, 0, 0, 1, 0);

    /* A V7 wrap-edge dirty record can name the final source byte but an
       overlarge height.  The original painter must clip the source rows to
       the allocated four-plane buffer before dereferencing the next row. */
    nt_vga_hi_graph_std(EGA_PLANE_SIZE - 1, 0, 0, 1, 768);

    /* V7 VGA's 640-pixel 256-colour path is an original nt_vga.c painter,
       not a standalone pixel converter.  Give it two source pixels and
       verify both the shared DIB and the original dirty-rectangle exit. */
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

    /* The same original V7 extension selects its 800x600 256-colour mode
       through the mode table in v7_video.c.  Exercise a second proprietary
       layout: it must keep nt_v7vga_hi_graph_std as the painter and the
       standalone DIB only as its output surface. */
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
    {
        int32_t left;
        int32_t top;
        int32_t right;
        int32_t bottom;
        unsigned char *surface = (unsigned char *)bits;

        while (softpc_machine_presentation_take_dirty(machine, &left, &top,
            &right, &bottom)) {
        }
        EGA_planes[0] = 0x11u;
        EGA_planes[1] = 0x22u;
        nt_v7vga_hi_graph_std(0, 0, 0, 2, 1);
        assert(surface[0] == 0x11u);
        assert(surface[1] == 0x22u);
        assert(softpc_machine_presentation_take_dirty(machine, &left, &top,
            &right, &bottom));
        assert(left == 0 && top == 0 && right == 1 && bottom == 0);
    }

    /* The original nt_graph VLT maps the attribute-controller palette to
       DIB entries; simply copying DAC[] would leave index zero black here. */
    {
        const softpc_test_dib_info *dib = (const softpc_test_dib_info *)info;
        set_256_colour_mode(FALSE);
        set_colour_select(FALSE);
        set_top_pixel_pad(0);
        set_DAC_mask(0xffu);
        set_palette_val(0, 1u);
        DAC[1].red = 1u;
        DAC[1].green = 2u;
        DAC[1].blue = 3u;
        /* Route through the original VIDEOFUNCS mark-refresh callback. */
        host_mark_screen_refresh();
        host_timer_event();
        host_timer_event();
        assert(dib->colours[0].red == 4u);
        assert(dib->colours[0].green == 8u);
        assert(dib->colours[0].blue == 12u);
    }
    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
