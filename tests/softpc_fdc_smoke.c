#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "insignia.h"
#include "host_def.h"
#include "xt.h"
#include "ios.h"
#include "fla.h"
#include "gfi.h"
#include "cmos.h"

static void write_floppy(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file = fopen(path, "wb");
    assert(file != NULL);
    sector[0] = 0x5au;
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fseek(file, 1474560L - 1L, SEEK_SET) == 0);
    assert(fputc(0, file) == 0);
    assert(fclose(file) == 0);
}

static void program_dma_read(void)
{
    /* Channel 2, 0600h, 512 bytes, device-to-memory single transfer. */
    outb(0x0au, 0x06u);
    outb(0x0cu, 0u);
    outb(0x04u, 0x00u);
    outb(0x04u, 0x06u);
    outb(0x81u, 0x00u);
    outb(0x0cu, 0u);
    outb(0x05u, 0xffu);
    outb(0x05u, 0x01u);
    outb(0x0bu, 0x46u);
    outb(0x0au, 0x02u);
}

static void fdc_write(unsigned char value)
{
    unsigned char status = 0u;
    unsigned int attempts;
    for (attempts = 0u; attempts < 4u; ++attempts) {
        inb(DISKETTE_STATUS_REG, &status);
        if ((status & (FDC_RQM | FDC_DIO)) == FDC_RQM) break;
    }
    assert((status & (FDC_RQM | FDC_DIO)) == FDC_RQM);
    outb(DISKETTE_DATA_REG, value);
}

int main(void)
{
    const char *path = "softpc-fdc-smoke.img";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    unsigned char byte = 0u;
    unsigned char result;
    unsigned short cmos_diskette = 0u;
    unsigned int attempts;

    write_floppy(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(gfi_drive_type(0) == GFI_DRIVE_TYPE_144);
    /* The original CMOS POST must receive the drive capability supplied by
       the standalone image GFI port: A: is a 1.44M drive (high nibble 4). */
    cmos_post();
    assert(cmos_read_byte(CMOS_DISKETTE, &cmos_diskette) == SUCCESS);
    assert(cmos_diskette == 0x40u);
    program_dma_read();
    outb(DISKETTE_DOR_REG, 0x1cu);
    fdc_write(FDC_READ_DATA);
    fdc_write(0x00u);
    fdc_write(0x00u);
    fdc_write(0x00u);
    fdc_write(0x01u);
    fdc_write(0x02u);
    fdc_write(0x01u);
    fdc_write(0x1bu);
    fdc_write(0xffu);
    for (attempts = 0u; attempts < 4u; ++attempts) {
        inb(DISKETTE_STATUS_REG, &result);
        if ((result & (FDC_RQM | FDC_DIO)) == (FDC_RQM | FDC_DIO)) break;
    }
    assert((result & (FDC_RQM | FDC_DIO)) == (FDC_RQM | FDC_DIO));
    inb(DISKETTE_DATA_REG, &result);
    assert(result == 0u);
    assert(softpc_machine_read_physical(machine, 0x600u, &byte, 1u) ==
        SOFTPC_MACHINE_OK);
    assert(byte == 0x5au);
    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
