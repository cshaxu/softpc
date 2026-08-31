#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

/* Keep this regression on the standalone DIB contract; nt_graph.h carries
   the historical console-server structure and is intentionally not needed by
   this direct original-renderer call. */
extern unsigned char *EGA_planes;
extern void nt_v7vga_hi_graph_std(int offset, int screen_x, int screen_y,
    int width, int height);
extern int softpc_device_bop_dispatch(unsigned char number,
    unsigned int argument);
extern void c_setAL(unsigned char value);
extern void c_setAH(unsigned char value);
extern void c_setBX(unsigned short value);
extern unsigned short c_getAX(void);
extern unsigned short c_getBX(void);
extern unsigned char Currently_emulated_video_mode;
extern void host_timer_event(void);

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
    assert(!softpc_machine_presentation_is_graphics(machine));
    assert(softpc_machine_presentation_dib(machine, &bits, &info, &width,
        &height));
    assert(bits != NULL && info != NULL);
    assert(width == 1056u && height == 768u);

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
    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
