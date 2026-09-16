#include "vm/machine.h"
#include "vm/driver.h"
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
#include "egacpu.h"
#include "config.h"
#include "vgaports.h"
#include "compat/dib_surface.h"
#include "nt_graph.h"
#include "cpu_vid.h"

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
extern void ega_graph_update(void);
extern void ega_split_graph_update(void);
extern void ega_wrap_graph_update(void);
extern void ega_wrap_split_graph_update(void);
extern void vga_graph_update(void);
extern void vga_split_graph_update(void);
extern void vga_ac_outb(io_addr port, half_word value);

extern EVID_WRT_POINTERS c_ev_write_ptr;
extern void write_byte_ev_glue(IU32 offset, IU8 value);
extern void write_word_ev_glue(IU32 offset, IU16 value);
static IU32 writer_offset, writer_value;

/* Selected legacy calls use default argument promotions (IPT2 expands to ()). */
static void capture_byte_write(IU32 offset, IU32 value)
{
    writer_offset = offset;
    writer_value = value;
}

static void capture_word_write(IU32 offset, IU32 value)
{
    writer_offset = offset;
    writer_value = value;
}

static void verify_writer_contract(void)
{
    EVID_WRT_POINTERS saved = c_ev_write_ptr;
    c_ev_write_ptr.b_write = capture_byte_write;
    c_ev_write_ptr.w_write = capture_word_write;
    write_byte_ev_glue(0xa1234u, 0xa5u);
    assert(writer_offset == 0xa1234u && writer_value == 0xa5u);
    write_word_ev_glue(0xb1234u, 0xa55au);
    assert(writer_offset == 0xb1234u && writer_value == 0xa55au);
    c_ev_write_ptr = saved;
}

static void verify_panning_refresh(void)
{
    struct attribute_controller saved = attribute_controller;
    DISPLAY_GLOBS saved_display = PCDisplay;
    unsigned char unpanned[16];
    IU32 dirty = getVideodirty_total();
    int i;
    set_offset_per_line(80);
    for (i = 0; i < 8; ++i)
        EGA_planes[i] = (unsigned char)(i & 1 ? 0x33 : 0x55);
    attribute_controller.horizontal_pel_panning.as.abyte = 0;
    nt_ega_hi_graph_std(0, 0, 0, 2, 1);
    memcpy(unpanned, sc.ConsoleBufInfo.lpBitMap, sizeof(unpanned));
    attribute_controller.address.as_bfld.index_state = 0;
    setVideodirty_total(0);
    vga_ac_outb(0x3c0, 0x33);
    vga_ac_outb(0x3c0, 1);
    assert(attribute_controller.horizontal_pel_panning.as.abyte == 1);
    assert(getVideodirty_total() > 20000);
    setVideodirty_total(0);
    vga_ac_outb(0x3c0, 0x33);
    vga_ac_outb(0x3c0, 1);
    assert(getVideodirty_total() == 0);
    nt_ega_hi_graph_std(0, 0, 0, 1, 1);
    /* One pel selects pixels 1..8, including the following plane byte. */
    assert(memcmp(sc.ConsoleBufInfo.lpBitMap, unpanned + 1, 8) == 0);
    setVideodirty_total(0);
    vga_ac_outb(0x3c0, 0x30);
    vga_ac_outb(0x3c0, attribute_controller.mode_control.as.abyte ^ 0x20);
    assert(getVideodirty_total() > 20000);
    attribute_controller = saved;
    PCDisplay = saved_display;
    setVideodirty_total(dirty);
}

static void verify_panning_pixels(void)
{
    void (*painters[])(int,int,int,int,int) = {
        nt_ega_lo_graph_std, nt_ega_med_graph_std, nt_ega_hi_graph_std
    };
    DISPLAY_GLOBS saved = PCDisplay;
    struct attribute_controller saved_ac = attribute_controller;
    int saved_split = EGA_GRAPH.screen_split.as_word;
    int saved_bank = extensions_controller.ram_bank_select.as_bfld.counter_bank_enable;
    unsigned char expected[32];
    unsigned char *surface = sc.ConsoleBufInfo.lpBitMap;
    int p, bank, wrap, pan, split, row, i;
    nt_init_ega_lo_graph();
    set_offset_per_line(80);
    for (i = 0; i < EGA_PLANE_SIZE * 4; ++i)
        EGA_planes[i] = (unsigned char)(i * 37 + (i >> 3));
    for (p = 0; p < 3; ++p)
    for (bank = 0; bank < 2; ++bank)
    for (wrap = 0; wrap < 2; ++wrap) {
        int xscale = p == 0 ? 2 : 1;
        int yscale = p == 2 ? 1 : 2;
        int address;
        extensions_controller.ram_bank_select.as_bfld.counter_bank_enable = bank;
        address = wrap ? EGA_PLANE_DISP_SIZE - 1 : 0;
        set_pc_pix_height(yscale);
        set_screen_split(yscale - 1);
        attribute_controller.horizontal_pel_panning.as.abyte = 0;
        painters[p](address, 0, 0, 1, 1);
        memcpy(expected, surface, 8 * xscale);
        painters[p](wrap ? 0 : 1, 0, 0, 1, 1);
        memcpy(expected + 8 * xscale, surface, 8 * xscale);
        for (pan = 0; pan < 8; ++pan)
        for (split = 0; split < 2; ++split)
        for (row = 0; row < 2; ++row) {
            int shift = split && row ? 0 : pan;
            attribute_controller.mode_control.as_bfld.horiz_pan_mode = split;
            attribute_controller.horizontal_pel_panning.as.abyte = pan;
            painters[p](address, 0, row, 1, 1);
            for (i = 0; i < yscale; ++i)
                assert(memcmp(surface + (row * yscale + i) * sc.PC_W_Width,
                    expected + shift * xscale, 8 * xscale) == 0);
        }
    }
    PCDisplay = saved;
    attribute_controller = saved_ac;
    set_screen_split(saved_split);
    extensions_controller.ram_bank_select.as_bfld.counter_bank_enable = saved_bank;
}

static void verify_ega_dirty_alignment(void)
{
    DISPLAY_GLOBS saved = PCDisplay;
    struct attribute_controller saved_ac = attribute_controller;
    void (*saved_paint)() = paint_screen;
    unsigned char *surface = sc.ConsoleBufInfo.lpBitMap;
    size_t size = (size_t)sc.PC_W_Width * sc.PC_W_Height;
    unsigned char *complete = malloc(size);
    int offset, i, stride, split, bank, scale, pan;
    int saved_split = EGA_GRAPH.screen_split.as_word;
    int saved_bank = extensions_controller.ram_bank_select.as_bfld.counter_bank_enable;
    assert(complete != NULL);
    set_pc_pix_height(1);
    set_char_height(1);
    set_screen_height(3);
    set_screen_length(320);
    set_display_disabled(0);
    paint_screen = nt_ega_hi_graph_std;
    for (i = 0; i < 4 * EGA_PLANE_SIZE; ++i)
        EGA_planes[i] = (unsigned char)(i * 37 + (i >> 3));
    for (pan = 0; pan < 2; ++pan)
    for (bank = 0; bank < 2; ++bank) {
    int plane_limit;
    attribute_controller.horizontal_pel_panning.as.abyte = pan;
    extensions_controller.ram_bank_select.as_bfld.counter_bank_enable = bank;
    plane_limit = EGA_PLANE_DISP_SIZE;
    for (scale = 1; scale <= 2; ++scale)
    for (stride = 80; stride <= 96; stride += 2)
    for (split = 0; split < 6; ++split)
    for (offset = 0; offset < 4; ++offset) {
        void (*update)(void) = split == 5 ? vga_split_graph_update :
            split == 4 ? vga_graph_update :
            split == 3 ? ega_wrap_split_graph_update :
            split == 2 ? ega_wrap_graph_update :
            split == 1 ? ega_split_graph_update : ega_graph_update;
        int origin = split == 2 || split == 3 ?
            plane_limit - (split == 3 ? 1 : 2) * stride + offset : offset;
        int columns = stride & 2 ? stride : 80;
        paint_screen = split >= 4 ? nt_v7vga_hi_graph_std : nt_ega_hi_graph_std;
        set_pc_pix_height(scale);
        set_screen_height(4 * scale - 1);
        set_bytes_per_line(columns);
        set_offset_per_line(stride);
        set_screen_length(stride * 4);
        set_screen_split(2 * scale - 1);
        set_screen_start(origin);
        /* Independent row/address oracle, still using the real painter. */
        memset(surface, 0xa5, size);
        for (i = 0; i < 4; ++i) {
            int address = (split % 2 && i >= 2 ? (i - 2) * stride :
                origin + i * stride) % plane_limit;
            int first = plane_limit - address;
            if (first > columns) first = columns;
            (*paint_screen)(address, 0, i, first, 1);
            if (first < columns)
                (*paint_screen)(0, first * 8, i, columns - first, 1);
        }
        memcpy(complete, surface, size);
        memset(surface, 0xa5, size);
        memset(video_copy, 1, 0x8000);
        setVideodirty_total(20001);
        update();
        if (memcmp(complete, surface, size) != 0)
            fprintf(stderr, "EGA full/oracle mismatch route %d stride %d residue %d\n", split, stride, offset);
        assert(memcmp(complete, surface, size) == 0);
        for (i = 0; i < columns * 4; ++i) {
            int row = i / columns;
            int address = ((split % 2 && row >= 2 ? (row - 2) * stride :
                origin + row * stride) + i % columns) % plane_limit;
            if (video_copy[address >> 2] != 0)
                fprintf(stderr, "EGA uncleared full mark route %d stride %d residue %d address %d\n",
                    split, stride, offset, address);
            assert(video_copy[address >> 2] == 0);
        }
        memset(surface, 0xa5, size);
        memset(video_copy, 1, 0x8000);
        setVideodirty_low(0);
        setVideodirty_high(0x7fff);
        setVideodirty_total(1);
        update();
        if (memcmp(complete, surface, size) != 0) {
            size_t p = 0;
            while (complete[p] == surface[p]) ++p;
            fprintf(stderr, "EGA dirty/full mismatch route %d stride %d residue %d, pixel %zu: %u/%u, height %d\n",
                split, stride, offset, p, complete[p], surface[p], get_screen_height());
        }
        assert(memcmp(complete, surface, size) == 0);
        /* A lone terminal group must not disappear through floor division;
           nor may clearing a shared row mark lose its next-row coverage. */
        for (i = 0; i < 8; ++i) {
            int row = i / 2;
            int address = ((split % 2 && row >= 2 ? (row - 2) * stride :
                origin + row * stride) + (i % 2 ? columns - 1 : 0)) % plane_limit;
            EGA_planes[address * (split >= 4 ? 1 : 4)] ^= 0xff;
            memset(video_copy, 0, 0x8000);
            video_copy[address >> 2] = 1;
            setVideodirty_low(address >> 2);
            setVideodirty_high(address >> 2);
            setVideodirty_total(1);
            update();
            if (video_copy[address >> 2] != 0)
                fprintf(stderr, "EGA uncleared sparse mark route %d stride %d residue %d address %d\n",
                    split, stride, offset, address);
            assert(video_copy[address >> 2] == 0);
            memcpy(complete, surface, size);
            setVideodirty_total(20001);
            update();
            if (memcmp(complete, surface, size) != 0)
                fprintf(stderr, "EGA sparse/full mismatch route %d stride %d residue %d address %d\n",
                    split, stride, offset, address);
            assert(memcmp(complete, surface, size) == 0);
        }
    }
    }
    paint_screen = saved_paint;
    PCDisplay = saved;
    attribute_controller = saved_ac;
    set_screen_split(saved_split);
    extensions_controller.ram_bank_select.as_bfld.counter_bank_enable = saved_bank;
    free(complete);
}

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

static void verify_driver_geometry(softpc_machine *machine)
{
    static const unsigned modes[] = {
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
        0x12, 0x13, 0x67, 0x03, 0x60
    };
    static const unsigned widths[] = {
        752, 720, 800, 1024, 1024, 1024, 640, 640, 720, 800,
        640, 640, 640, 0, 752
    };
    vm_driver *adapter = NULL;
    common_machine_driver driver;
    kvm_frame *frame = malloc(sizeof(*frame));
    unsigned index, pass;
    assert(frame != NULL);
    assert(vm_driver_create(&adapter, machine) == LIB_STATUS_OK);
    vm_driver_describe(adapter, &driver);
    for (pass = 0; pass < 2; ++pass)
    for (index = 0; index < sizeof(modes) / sizeof(modes[0]); ++index) {
        const void *bits;
        const void *info;
        uint32_t width, height;
        int32_t left, top, right, bottom;
        SMALL_RECT rect;
        c_setAH(modes[index] >= 0x60 ? 0x6f : 0);
        c_setAL(modes[index] >= 0x60 ? 5 : (unsigned char)modes[index]);
        c_setBX((unsigned short)modes[index]);
        assert(softpc_device_bop_dispatch(0x42u, 0u));
        host_timer_event();
        host_timer_event();
        assert(Currently_emulated_video_mode == modes[index]);
        if (modes[index] == 3) {
            assert(driver.copy_frame(driver.context, frame));
            assert(frame->valid && !frame->graphics);
            continue;
        }
        assert(softpc_machine_presentation_dib(machine, &bits, &info,
            &width, &height));
        assert(width == widths[index]);
        while (softpc_machine_presentation_take_dirty(machine, &left, &top,
                &right, &bottom)) { }
        /* A legitimate full-height half repaint must not resize the frame.
         * Both halves are tested across original mode transitions. */
        rect.Left = 0; rect.Top = 0;
        rect.Right = (SHORT)((pass ? width / 2u : width) - 1u);
        rect.Bottom = (SHORT)(height - 1u);
        assert(softpc_standalone_invalidate_dibits(NULL, &rect));
        assert(driver.copy_frame(driver.context, frame));
        assert(frame->valid && frame->graphics);
        if (frame->graphics_width != width || frame->graphics_height != height)
            fprintf(stderr, "mode %02x pass %u: DIB %ux%u, frame %ux%u\n",
                modes[index], pass, width, height,
                frame->graphics_width, frame->graphics_height);
        assert(frame->graphics_width == width && frame->graphics_height == height);
        assert(frame->graphics_stride == width);
        assert(frame->dirty_right == rect.Right);
        rect.Left = (SHORT)(width / 2u); rect.Right = (SHORT)(width - 1u);
        assert(softpc_standalone_invalidate_dibits(NULL, &rect));
        assert(driver.copy_frame(driver.context, frame));
        assert(frame->graphics_width == width && frame->dirty_right == rect.Right);
        assert(memcmp(frame->graphics_pixels, bits, width * height) == 0);
        assert(!driver.copy_frame(driver.context, frame));
    }
    vm_driver_destroy(adapter);
    free(frame);
}

int main(void)
{
    const char *path = "softpc-original-dib-smoke.img";
    softpc_machine_options options = { path, NULL };
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
        assert(softpc_machine_presentation_fonts(machine, font,
            secondary_font, &font_height, &attribute_font_select));
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
    verify_ega_dirty_alignment();
    verify_panning_refresh();
    verify_panning_pixels();
    verify_writer_contract();
    verify_driver_geometry(machine);
    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
