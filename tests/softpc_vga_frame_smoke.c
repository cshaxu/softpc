#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "gmi.h"
#include "gfx_upd.h"
#include "egaports.h"
#include "video.h"

extern byte *EGA_planes;
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
    DAC[2].green = 63u;
    DAC[3].blue = 63u;
    DAC[4].red = DAC[4].green = DAC[4].blue = 63u;

    assert(softpc_machine_vga_mode13_active(machine));
    assert(softpc_machine_vga_mode13_frame(machine, pixels,
        (uint32_t)(sizeof(pixels) / sizeof(pixels[0]))) == SOFTPC_MACHINE_OK);
    assert(pixels[0] == 0x00ff0000u);
    assert(pixels[1] == 0x0000ff00u);
    assert(pixels[2] == 0x000000ffu);
    assert(pixels[3] == 0x00ffffffu);

    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
