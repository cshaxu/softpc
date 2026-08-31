#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "xt.h"
#include "cmos.h"

static void write_boot_image(const char *path, unsigned char marker)
{
    unsigned char sector[512] = { 0 };
    FILE *file;

    /* mov byte ptr [0500], marker; jmp $ */
    sector[0] = 0xc6u;
    sector[1] = 0x06u;
    sector[2] = 0x00u;
    sector[3] = 0x05u;
    sector[4] = marker;
    sector[5] = 0xebu;
    sector[6] = 0xfeu;
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

int main(void)
{
    const char *floppy = "softpc-dual-media-floppy.img";
    const char *hard_disk = "softpc-dual-media-hdd.img";
    softpc_machine_options options = { floppy, hard_disk,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    unsigned char marker = 0u;
    unsigned char fixed_disk_count = 0u;
    unsigned short cmos_disk = 0u;
    unsigned int slice;

    write_boot_image(floppy, 0x42u);
    write_boot_image(hard_disk, 0x77u);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    for (slice = 0u; slice < 16u; ++slice)
        assert(softpc_machine_run(machine, 6000u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) ==
        SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x475u, &fixed_disk_count,
        1u) == SOFTPC_MACHINE_OK);
    assert(marker == 0x42u);
    assert(fixed_disk_count == 1u);
    /* The original CMOS controller obtains fixed-media presence from the
       standalone host configuration port, then retains the original C: type
       3 encoding.  This is not a second disk-profile implementation. */
    cmos_post();
    assert(cmos_read_byte(CMOS_DISK, &cmos_disk) == SUCCESS);
    assert(cmos_disk == 0x30u);
    softpc_machine_destroy(machine);
    assert(remove(floppy) == 0);
    assert(remove(hard_disk) == 0);
    return 0;
}
