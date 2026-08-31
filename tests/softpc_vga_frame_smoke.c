#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "gmi.h"
#include "gfx_upd.h"
#include "gvi.h"
#include "egagraph.h"
#include "egaports.h"
#include "video.h"

extern byte *EGA_planes;
extern PC_palette *DAC;
extern IBOOL softpc_device_bop_dispatch(IU8 number, IU32 argument);
extern void c_setAL(IU8 value);
extern void c_setAH(IU8 value);

static uint32_t mode12_pixels[1024u * 768u];

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
    const char *path = "softpc-vga-frame-smoke.img";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    uint32_t pixels[320u * 200u];

    make_boot_disk(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(!softpc_machine_vga_mode13_active(machine));
    assert(EGA_planes != NULL && DAC != NULL);

    /* Mode 13h's original chain-4 storage is plane 0/1/2/3 interleaved. */
    Video_mode = 0x13u;
    EGA_planes[0] = 1u;
    EGA_planes[1] = 2u;
    EGA_planes[2] = 3u;
    EGA_planes[3] = 4u;
    DAC[1].red = 63u;
    DAC[1].green = DAC[1].blue = 0u;
    DAC[2].red = DAC[2].blue = 0u;
    DAC[2].green = 63u;
    DAC[3].red = DAC[3].green = 0u;
    DAC[3].blue = 63u;
    DAC[4].red = DAC[4].green = DAC[4].blue = 63u;

    assert(softpc_machine_vga_mode13_active(machine));
    assert(softpc_machine_vga_mode13_frame(machine, pixels,
        (uint32_t)(sizeof(pixels) / sizeof(pixels[0]))) == SOFTPC_MACHINE_OK);
    assert(pixels[0] == 0x00ff0000u);
    assert(pixels[1] == 0x0000ff00u);
    assert(pixels[2] == 0x000000ffu);
    assert(pixels[3] == 0x00ffffffu);

    /* V7 extension modes are still rendered from the original controller
       state, not from a second standalone video buffer. */
    /* The V7 ROM reaches its extended controller service through the
       original EGA BOP. AL 19h maps to Video Seven mode 65h. */
    c_setAH(0u);
    c_setAL(0x19u);
    assert(softpc_device_bop_dispatch(0x42u, 0u) == TRUE);
    EGA_planes[0] = 0x80u;
    EGA_planes[1] = 0x80u;
    EGA_planes[2] = EGA_planes[3] = 0u;
    EGA_GRAPH.palette[3].red = 0xa0u;
    EGA_GRAPH.palette[3].green = 0x50u;
    EGA_GRAPH.palette[3].blue = 0u;
    {
        uint32_t width = 0u;
        uint32_t height = 0u;
        assert(softpc_machine_v7_graphics_dimensions(machine, &width,
            &height));
        assert(width == 1024u && height == 768u);
        assert(softpc_machine_v7_graphics_frame(machine, mode12_pixels,
            (uint32_t)(sizeof(mode12_pixels) / sizeof(mode12_pixels[0]))) ==
            SOFTPC_MACHINE_OK);
        assert(mode12_pixels[0] == 0x00a05000u);
    }
    /* Likewise AL 1Dh is V7 packed 256-colour mode 69h. */
    c_setAH(0u);
    c_setAL(0x1du);
    assert(softpc_device_bop_dispatch(0x42u, 0u) == TRUE);
    EGA_planes[0] = 4u;
    DAC[4].red = DAC[4].green = DAC[4].blue = 63u;
    {
        uint32_t width = 0u;
        uint32_t height = 0u;
        assert(softpc_machine_v7_graphics_dimensions(machine, &width,
            &height));
        assert(width == 800u && height == 600u);
        assert(softpc_machine_v7_graphics_frame(machine, mode12_pixels,
            (uint32_t)(sizeof(mode12_pixels) / sizeof(mode12_pixels[0]))) ==
            SOFTPC_MACHINE_OK);
        assert(mode12_pixels[0] == 0x00ffffffu);
    }
    /* The original planar mode uses one bit from each plane as the palette
       index. Plane zero is the low bit, exactly as the controller exposes
       it to the old SoftPC host renderer. */
    Video_mode = 0x12u;
    EGA_planes[0] = 0x80u;
    EGA_planes[1] = 0x80u;
    EGA_planes[2] = 0u;
    EGA_planes[3] = 0u;
    EGA_GRAPH.palette[3].red = 0xa0u;
    EGA_GRAPH.palette[3].green = 0x50u;
    EGA_GRAPH.palette[3].blue = 0u;
    assert(softpc_machine_vga_mode12_active(machine));
    assert(softpc_machine_vga_mode12_frame(machine, mode12_pixels,
        (uint32_t)(sizeof(mode12_pixels) / sizeof(mode12_pixels[0]))) ==
        SOFTPC_MACHINE_OK);
    assert(mode12_pixels[0] == 0x00a05000u);

    /* The same original EGA plane layout also carries the lower BIOS modes.
       The detached presentation must not replace it with mode-specific VM
       memory. */
    Video_mode = 0x0du;
    EGA_planes[0] = 0x80u;
    EGA_planes[1] = 0x80u;
    EGA_planes[2] = 0u;
    EGA_planes[3] = 0u;
    {
        uint32_t width = 0u;
        uint32_t height = 0u;
        assert(softpc_machine_vga_planar_dimensions(machine, &width,
            &height));
        assert(width == 320u && height == 200u);
        assert(softpc_machine_vga_planar_frame(machine, mode12_pixels,
            (uint32_t)(sizeof(mode12_pixels) / sizeof(mode12_pixels[0]))) ==
            SOFTPC_MACHINE_OK);
        assert(mode12_pixels[0] == 0x00a05000u);
    }
    Video_mode = 0x0eu;
    {
        uint32_t width = 0u;
        uint32_t height = 0u;
        assert(softpc_machine_vga_planar_dimensions(machine, &width,
            &height));
        assert(width == 640u && height == 200u);
        assert(softpc_machine_vga_planar_frame(machine, mode12_pixels,
            (uint32_t)(sizeof(mode12_pixels) / sizeof(mode12_pixels[0]))) ==
            SOFTPC_MACHINE_OK);
        assert(mode12_pixels[0] == 0x00a05000u);
    }
    Video_mode = 0x0fu;
    {
        uint32_t width = 0u;
        uint32_t height = 0u;
        assert(softpc_machine_vga_planar_dimensions(machine, &width,
            &height));
        assert(width == 640u && height == 350u);
    }
    Video_mode = 0x10u;
    {
        uint32_t width = 0u;
        uint32_t height = 0u;
        assert(softpc_machine_vga_planar_dimensions(machine, &width,
            &height));
        assert(width == 640u && height == 350u);
    }
    Video_mode = 0x11u;
    {
        uint32_t width = 0u;
        uint32_t height = 0u;
        assert(softpc_machine_vga_planar_dimensions(machine, &width,
            &height));
        assert(width == 640u && height == 480u);
        assert(softpc_machine_vga_planar_frame(machine, mode12_pixels,
            (uint32_t)(sizeof(mode12_pixels) / sizeof(mode12_pixels[0]))) ==
            SOFTPC_MACHINE_OK);
        assert(mode12_pixels[0] == 0x00a05000u);
    }

    /* In this V7 VGA profile the original CGA-compatible graphics paths
       retain their historical odd/even CGA banking inside EGA_planes. */
    Video_mode = 0x04u;
    DAC[1].red = 63u;
    DAC[1].green = DAC[1].blue = 0u;
    DAC[2].red = DAC[2].blue = 0u;
    DAC[2].green = 63u;
    DAC[3].red = DAC[3].green = 0u;
    DAC[3].blue = 63u;
    EGA_planes[0] = 0x1bu; /* 00, 01, 10, 11 */
    {
        uint32_t width = 0u;
        uint32_t height = 0u;
        assert(softpc_machine_cga_graphics_dimensions(machine, &width,
            &height));
        assert(width == 320u && height == 200u);
        assert(softpc_machine_cga_graphics_frame(machine, mode12_pixels,
            (uint32_t)(sizeof(mode12_pixels) / sizeof(mode12_pixels[0]))) ==
            SOFTPC_MACHINE_OK);
        assert(mode12_pixels[0] == 0u);
        assert(mode12_pixels[1] == 0x00ff0000u);
        assert(mode12_pixels[2] == 0x0000ff00u);
        assert(mode12_pixels[3] == 0x000000ffu);
    }
    Video_mode = 0x06u;
    EGA_planes[0] = 0x80u;
    {
        uint32_t width = 0u;
        uint32_t height = 0u;
        assert(softpc_machine_cga_graphics_dimensions(machine, &width,
            &height));
        assert(width == 640u && height == 200u);
        assert(softpc_machine_cga_graphics_frame(machine, mode12_pixels,
            (uint32_t)(sizeof(mode12_pixels) / sizeof(mode12_pixels[0]))) ==
            SOFTPC_MACHINE_OK);
        assert(mode12_pixels[0] == 0x00ff0000u);
        assert(mode12_pixels[1] == 0u);
    }

    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
