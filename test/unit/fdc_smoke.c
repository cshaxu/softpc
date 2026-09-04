#include "machine.h"
#include "test_cleanup.h"

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

static void program_dma_read_multi(void)
{
    /* Channel 2, 0600h, 1024 bytes, device-to-memory single transfer. */
    outb(0x0au, 0x06u);
    outb(0x0cu, 0u);
    outb(0x04u, 0x00u);
    outb(0x04u, 0x06u);
    outb(0x81u, 0x00u);
    outb(0x0cu, 0u);
    outb(0x05u, 0xffu);
    outb(0x05u, 0x03u);
    outb(0x0bu, 0x46u);
    outb(0x0au, 0x02u);
}

static void program_dma_write(unsigned short address, unsigned short count)
{
    outb(0x0au, 0x06u);
    outb(0x0cu, 0u);
    outb(0x04u, (unsigned char)(address & 0xffu));
    outb(0x04u, (unsigned char)(address >> 8));
    outb(0x81u, 0x00u);
    outb(0x0cu, 0u);
    outb(0x05u, (unsigned char)(count & 0xffu));
    outb(0x05u, (unsigned char)(count >> 8));
    outb(0x0bu, 0x4au);
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

static void fdc_drain_data_result(void)
{
    unsigned char status = 0u;
    unsigned char ignored;
    unsigned int index;
    unsigned int attempts;
    /* READ/WRITE/FORMAT complete with a seven-byte result phase.  The
       original FLA cannot accept the next command until every byte is read. */
    for (index = 0u; index < 6u; ++index) {
        for (attempts = 0u; attempts < 4u; ++attempts) {
            inb(DISKETTE_STATUS_REG, &status);
            if ((status & (FDC_RQM | FDC_DIO)) == (FDC_RQM | FDC_DIO)) break;
        }
        assert((status & (FDC_RQM | FDC_DIO)) == (FDC_RQM | FDC_DIO));
        inb(DISKETTE_DATA_REG, &ignored);
    }
}

int main(void)
{
    const char *path = "softpc-fdc-smoke.img";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    unsigned char byte = 0u;
    unsigned char result;
    unsigned short cmos_disk = 0u;
    unsigned short cmos_diskette = 0u;
    unsigned int attempts;
    FDC_CMD_BLOCK format_command[MAX_COMMAND_LEN] = { 0 };
    FDC_RESULT_BLOCK format_result[MAX_RESULT_LEN] = { 0 };
    unsigned char format_ids[4] = { 0u, 0u, 1u, 2u };

    write_floppy(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(gfi_drive_type(0) == GFI_DRIVE_TYPE_144);
    /* The original CMOS POST must receive the drive capability supplied by
       the standalone image GFI port: A: is a 1.44M drive (high nibble 4). */
    cmos_post();
    assert(cmos_read_byte(CMOS_DISK, &cmos_disk) == SUCCESS);
    assert(cmos_disk == 0u);
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
    fdc_drain_data_result();
    assert(softpc_machine_read_physical(machine, 0x600u, &byte, 1u) ==
        SOFTPC_MACHINE_OK);
    assert(byte == 0x5au);

    /* Original nt_rflop advances the result CHRN after a multi-sector DMA
       command.  The raw-image port must preserve that GFI-server contract
       rather than reporting the input CHRN after every successful read. */
    {
        FDC_CMD_BLOCK command[MAX_COMMAND_LEN] = { 0 };
        FDC_RESULT_BLOCK command_result[MAX_RESULT_LEN] = { 0 };
        program_dma_read_multi();
        put_c0_cmd(command, FDC_READ_DATA);
        put_c0_drive(command, 0u);
        put_c0_cyl(command, 0u);
        put_c0_hd(command, 0u);
        put_c0_sector(command, 1u);
        put_c0_N(command, 2u);
        put_c0_EOT(command, 18u);
        assert(gfi_fdc_command(command, command_result) == SUCCESS);
        assert(get_r0_ST0(command_result) == 0u);
        assert(get_r0_cyl(command_result) == 0u);
        assert(get_r0_head(command_result) == 0u);
        assert(get_r0_sector(command_result) == 3u);
        assert(get_r0_N(command_result) == 2u);

        /* Raw IBM images have fixed 512-byte sectors.  A mismatched N must
           take the original FDC no-data path, rather than reinterpreting
           the image with a new standalone geometry. */
        put_c0_N(command, 1u);
        assert(gfi_fdc_command(command, command_result) == SUCCESS);
        assert(get_r1_ST1_no_data(command_result) != 0u);

        /* A raw sector stream has no deleted-data marks or track records.
           Do not manufacture those controller formats in the host port. */
        put_c0_N(command, 2u);
        put_c0_cmd(command, FDC_READ_DELETED_DATA);
        assert(gfi_fdc_command(command, command_result) == FAILURE);
        put_c0_cmd(command, FDC_READ_TRACK);
        assert(gfi_fdc_command(command, command_result) == FAILURE);
    }

    /* The raw-image port consumes the original FDC format DMA CHRN list;
       no replacement floppy controller participates. */
    assert(softpc_machine_write_physical(machine, 0x700u, format_ids,
        sizeof(format_ids)) == SOFTPC_MACHINE_OK);
    program_dma_write(0x700u, (unsigned short)(sizeof(format_ids) - 1u));
    put_c3_cmd(format_command, FDC_FORMAT_TRACK);
    put_c3_drive(format_command, 0u);
    put_c3_head(format_command, 0u);
    put_c3_N(format_command, 2u);
    put_c3_SC(format_command, 1u);
    put_c3_filler(format_command, 0xe5u);
    assert(gfi_fdc_command(format_command, format_result) == SUCCESS);
    assert(get_r0_ST0(format_result) == 0u);
    assert(get_r0_ST1(format_result) == 0u);
    assert(get_r0_ST2(format_result) == 0u);
    program_dma_read();
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
    fdc_drain_data_result();
    assert(softpc_machine_read_physical(machine, 0x600u, &byte, 1u) ==
        SOFTPC_MACHINE_OK);
    assert(byte == 0xe5u);
    softpc_machine_destroy(machine);
    /* Detaching the raw-image port must restore the original gfi_mpty
       empty-drive server, just as the original real-floppy backend does. */
    assert(gfi_drive_type(0) == GFI_DRIVE_TYPE_NULL);
    assert(softpc_test_remove_image(path));
    return 0;
}
