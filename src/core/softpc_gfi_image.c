#include "insignia.h"
#include "host_def.h"

#include <stdio.h>

#include "xt.h"
#include "bios.h"
#include "dma.h"
#include "fla.h"
#include "gfi.h"

/*
 * This is a host media port, not an FDC implementation.  The original FLA,
 * GFI router and BIOS own command sequencing, ports, DMA programming and
 * result delivery.  This module supplies only the image-file surface behind
 * the original GFI function table.
 */

#define SOFTPC_GFI_SECTOR_BYTES 512u

typedef struct {
    FILE *file;
    int writable;
    unsigned int cylinders;
    unsigned int heads;
    unsigned int sectors;
    unsigned int drive_type;
    unsigned int cylinder;
    int changed;
} softpc_gfi_image_drive;

static softpc_gfi_image_drive softpc_gfi_drives[MAX_DISKETTES];
static char softpc_gfi_attached_config_value[] = "floppy";
static char softpc_gfi_empty_config_value[] = "";

static int softpc_gfi_geometry(long bytes, softpc_gfi_image_drive *drive)
{
    unsigned long sectors = (unsigned long)bytes / SOFTPC_GFI_SECTOR_BYTES;
    if ((unsigned long)bytes % SOFTPC_GFI_SECTOR_BYTES != 0u) return 0;
    if (sectors == 720ul) {
        drive->cylinders = 40u; drive->heads = 2u; drive->sectors = 9u;
        drive->drive_type = GFI_DRIVE_TYPE_360;
    } else if (sectors == 1440ul) {
        drive->cylinders = 80u; drive->heads = 2u; drive->sectors = 9u;
        drive->drive_type = GFI_DRIVE_TYPE_720;
    } else if (sectors == 2400ul) {
        drive->cylinders = 80u; drive->heads = 2u; drive->sectors = 15u;
        drive->drive_type = GFI_DRIVE_TYPE_12;
    } else if (sectors == 2880ul) {
        drive->cylinders = 80u; drive->heads = 2u; drive->sectors = 18u;
        drive->drive_type = GFI_DRIVE_TYPE_144;
    } else if (sectors == 5760ul) {
        drive->cylinders = 80u; drive->heads = 2u; drive->sectors = 36u;
        drive->drive_type = GFI_DRIVE_TYPE_288;
    } else if (sectors >= 1ul) {
        /* Preserve the VM's long-standing permissive image contract for
           small boot fixtures; physical sector access beyond EOF still
           reports the original FDC no-data result. */
        drive->cylinders = 80u; drive->heads = 2u; drive->sectors = 18u;
        drive->drive_type = GFI_DRIVE_TYPE_144;
    } else return 0;
    return 1;
}

static void softpc_gfi_result(FDC_RESULT_BLOCK *result, UTINY drive,
    unsigned int cylinder, unsigned int head, unsigned int sector,
    unsigned int size, int failed, int write_protected)
{
    put_r0_ST0(result, (half_word)((head << 2u) | drive));
    put_r0_ST1(result, 0);
    put_r0_ST2(result, 0);
    if (failed) {
        put_r1_ST0_int_code(result, 1);
        if (write_protected) put_r1_ST1_write_protected(result, 1);
        else put_r1_ST1_no_data(result, 1);
    }
    put_r0_cyl(result, (half_word)cylinder);
    put_r0_head(result, (half_word)head);
    put_r0_sector(result, (half_word)sector);
    put_r0_N(result, (half_word)size);
}

static int softpc_gfi_transfer(softpc_gfi_image_drive *drive,
    FDC_CMD_BLOCK *command, int writing)
{
    sys_addr ignored_address;
    word dma_count;
    unsigned int cylinder = get_c0_cyl(command);
    unsigned int head = get_c0_hd(command);
    unsigned int sector = get_c0_sector(command);
    unsigned int size_code = get_c0_N(command);
    unsigned int sector_bytes;
    unsigned int count;
    unsigned int index;
    char buffer[8192];

    if (size_code > 6u) return 0;
    sector_bytes = 128u << size_code;
    if (sector_bytes > sizeof(buffer) || drive->file == NULL ||
        cylinder >= drive->cylinders || head >= drive->heads ||
        sector == 0u || sector > drive->sectors) return 0;
    dma_enquire(DMA_DISKETTE_CHANNEL, &ignored_address, &dma_count);
    count = ((unsigned int)dma_count + 1u + sector_bytes - 1u) / sector_bytes;
    if (count == 0u) count = 1u;
    for (index = 0u; index < count; ++index) {
        unsigned int current_sector = sector + index;
        unsigned long offset;
        if (current_sector > drive->sectors) return 0;
        offset = (((unsigned long)cylinder * drive->heads + head) *
            drive->sectors + (current_sector - 1u)) * sector_bytes;
        if (fseek(drive->file, (long)offset, SEEK_SET) != 0) return 0;
        if (writing) {
            if (!drive->writable) return 0;
            (void)dma_request(DMA_DISKETTE_CHANNEL, buffer,
                (word)sector_bytes);
            if (
                fwrite(buffer, 1u, sector_bytes, drive->file) != sector_bytes ||
                fflush(drive->file) != 0) return 0;
        } else {
            if (fread(buffer, 1u, sector_bytes, drive->file) != sector_bytes)
                return 0;
            (void)dma_request(DMA_DISKETTE_CHANNEL, buffer,
                (word)sector_bytes);
        }
    }
    drive->cylinder = cylinder;
    drive->changed = 0;
    return 1;
}

static SHORT softpc_gfi_command(FDC_CMD_BLOCK *command,
    FDC_RESULT_BLOCK *result)
{
    UTINY unit = (UTINY)get_type_drive(command);
    softpc_gfi_image_drive *drive;
    unsigned int cylinder;
    unsigned int head;
    unsigned int sector;
    unsigned int size;
    int writing;
    int okay;

    if (unit >= MAX_DISKETTES) return FAILURE;
    drive = &softpc_gfi_drives[unit];
    cylinder = get_c0_cyl(command);
    head = get_c0_hd(command);
    sector = get_c0_sector(command);
    size = get_c0_N(command);
    switch (get_type_cmd(command)) {
    case FDC_SPECIFY:
        return SUCCESS;
    case FDC_RECALIBRATE:
        drive->cylinder = 0u;
        put_r3_ST0(result, (half_word)(0x20u | unit));
        put_r3_PCN(result, 0);
        return SUCCESS;
    case FDC_SEEK:
        drive->cylinder = get_c8_new_cyl(command);
        put_r3_ST0(result, (half_word)(0x20u | unit));
        put_r3_PCN(result, (half_word)drive->cylinder);
        return SUCCESS;
    case FDC_SENSE_DRIVE_STATUS:
        put_r2_ST3_fault(result, 0);
        put_r2_ST3_write_protected(result, (drive->file != NULL && !drive->writable));
        put_r2_ST3_ready(result, (drive->file != NULL));
        put_r2_ST3_track_0(result, (drive->cylinder == 0u));
        put_r2_ST3_two_sided(result, (drive->heads > 1u));
        put_r2_ST3_head_address(result, get_c7_head(command));
        put_r2_ST3_unit(result, unit);
        return SUCCESS;
    case FDC_READ_ID:
        softpc_gfi_result(result, unit, drive->cylinder, get_c4_head(command),
            1u, 2u, drive->file == NULL, 0);
        return SUCCESS;
    case FDC_READ_DATA:
    case FDC_READ_DELETED_DATA:
    case FDC_WRITE_DATA:
    case FDC_WRITE_DELETED_DATA:
        writing = get_type_cmd(command) == FDC_WRITE_DATA ||
            get_type_cmd(command) == FDC_WRITE_DELETED_DATA;
        okay = softpc_gfi_transfer(drive, command, writing);
        softpc_gfi_result(result, unit, cylinder, head, sector, size, !okay,
            writing && !drive->writable);
        return SUCCESS;
    default:
        return FAILURE;
    }
}

static SHORT softpc_gfi_drive_on IFN1(UTINY, drive)
{ UNUSED(drive); return SUCCESS; }
static SHORT softpc_gfi_drive_off IFN1(UTINY, drive)
{ UNUSED(drive); return SUCCESS; }
static SHORT softpc_gfi_high IFN2(UTINY, drive, half_word, rate)
{ UNUSED(drive); UNUSED(rate); return SUCCESS; }
static SHORT softpc_gfi_drive_type IFN1(UTINY, drive)
{ return drive < MAX_DISKETTES ? (SHORT)softpc_gfi_drives[drive].drive_type : GFI_DRIVE_TYPE_NULL; }
static SHORT softpc_gfi_change IFN1(UTINY, drive)
{
    /* GFI returns SUCCESS while the disk-change line is clear.  A mounted
     * image is immediately stable from the fixed VM's point of view; return
     * FAILURE only for an absent drive, otherwise the original rd_wr_vf()
     * treats every boot read as an open-drive condition and never issues its
     * FDC READ DATA command. */
    return drive < MAX_DISKETTES && softpc_gfi_drives[drive].file != NULL ?
        SUCCESS : FAILURE;
}
static SHORT softpc_gfi_reset IFN2(FDC_RESULT_BLOCK *, result, UTINY, drive)
{
    if (drive >= MAX_DISKETTES) return FAILURE;
    softpc_gfi_drives[drive].cylinder = 0u;
    put_r3_ST0(result, 0);
    put_r3_PCN(result, 0);
    return SUCCESS;
}

static void softpc_gfi_install(UTINY drive)
{
    gfi_function_table[drive].command_fn = softpc_gfi_command;
    gfi_function_table[drive].drive_on_fn = softpc_gfi_drive_on;
    gfi_function_table[drive].drive_off_fn = softpc_gfi_drive_off;
    gfi_function_table[drive].reset_fn = softpc_gfi_reset;
    gfi_function_table[drive].high_fn = softpc_gfi_high;
    gfi_function_table[drive].drive_type_fn = softpc_gfi_drive_type;
    gfi_function_table[drive].change_fn = softpc_gfi_change;
}

int softpc_platform_floppy_attach(const char *path)
{
    softpc_gfi_image_drive *drive = &softpc_gfi_drives[0];
    long bytes;
    if (drive->file != NULL) fclose(drive->file);
    memset(drive, 0, sizeof(*drive));
    if (path == NULL) return 1;
    drive->file = fopen(path, "rb+");
    if (drive->file != NULL) drive->writable = 1;
    else drive->file = fopen(path, "rb");
    if (drive->file == NULL || fseek(drive->file, 0L, SEEK_END) != 0 ||
        (bytes = ftell(drive->file)) < 0 || fseek(drive->file, 0L, SEEK_SET) != 0 ||
        !softpc_gfi_geometry(bytes, drive)) {
        if (drive->file != NULL) fclose(drive->file);
        memset(drive, 0, sizeof(*drive));
        return 0;
    }
    drive->changed = 0;
    softpc_gfi_install(0);
    return 1;
}

void softpc_platform_floppy_detach(void)
{
    softpc_gfi_image_drive *drive = &softpc_gfi_drives[0];
    if (drive->file != NULL) fclose(drive->file);
    memset(drive, 0, sizeof(*drive));
}

/* The original CMOS POST asks the product configuration layer whether drive
 * A exists.  Expose only that fixed-machine fact; image paths remain private
 * to this host media backend. */
char *softpc_platform_floppy_config_value(void)
{
    return softpc_gfi_drives[0].file != NULL ?
        softpc_gfi_attached_config_value : softpc_gfi_empty_config_value;
}
