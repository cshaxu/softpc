#include "machine.h"
#include "test_cleanup.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* This image deliberately has one physical disk only.  Its MBR contains a
 * primary partition and an extended partition whose EBR describes a logical
 * volume.  The boot program reads both data sectors through original INT 13h;
 * the standalone host never parses either partition table. */
#define SOFTPC_PARTITION_SECTOR_BYTES 512u
#define SOFTPC_PARTITION_SECTORS 64u
#define SOFTPC_PRIMARY_MARKER 0xa7u
#define SOFTPC_LOGICAL_MARKER 0x5cu

static void softpc_partition_put_le32(unsigned char *target,
    unsigned long value)
{
    target[0] = (unsigned char)value;
    target[1] = (unsigned char)(value >> 8u);
    target[2] = (unsigned char)(value >> 16u);
    target[3] = (unsigned char)(value >> 24u);
}

static void softpc_partition_entry(unsigned char *entry,
    unsigned char type, unsigned long start_lba, unsigned long sectors)
{
    memset(entry, 0, 16u);
    entry[0] = 0x80u;
    entry[4] = type;
    softpc_partition_put_le32(entry + 8u, start_lba);
    softpc_partition_put_le32(entry + 12u, sectors);
}

static void softpc_partition_write_image(const char *path)
{
    unsigned char image[SOFTPC_PARTITION_SECTOR_BYTES * SOFTPC_PARTITION_SECTORS];
    unsigned char *mbr = image;
    unsigned char *ebr = image + 17u * SOFTPC_PARTITION_SECTOR_BYTES;
    FILE *file;
    /* Read LBA 1, then LBA 18 (CHS sectors 2 and 19 under fixed 16/63). */
    static const unsigned char boot_program[] = {
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u, 0x8eu, 0xc0u,
        0xbbu, 0x00u, 0x06u,
        0xb4u, 0x02u, 0xb0u, 0x01u, 0xb5u, 0x00u, 0xb1u, 0x02u,
        0xb6u, 0x00u, 0xb2u, 0x80u, 0xcdu, 0x13u,
        0xa0u, 0x00u, 0x06u, 0xa2u, 0x00u, 0x05u,
        0xbbu, 0x02u, 0x06u,
        0xb4u, 0x02u, 0xb0u, 0x01u, 0xb5u, 0x00u, 0xb1u, 0x13u,
        0xb6u, 0x00u, 0xb2u, 0x80u, 0xcdu, 0x13u,
        0xa0u, 0x02u, 0x06u, 0xa2u, 0x01u, 0x05u, 0xebu, 0xfeu
    };

    memset(image, 0, sizeof(image));
    memcpy(mbr, boot_program, sizeof(boot_program));
    softpc_partition_entry(mbr + 0x1beu, 0x06u, 1u, 16u);
    softpc_partition_entry(mbr + 0x1ceu, 0x05u, 17u, 32u);
    mbr[510] = 0x55u;
    mbr[511] = 0xaau;
    /* The EBR is guest metadata. The host sees only bytes at LBA 17. */
    softpc_partition_entry(ebr + 0x1beu, 0x06u, 1u, 8u);
    ebr[510] = 0x55u;
    ebr[511] = 0xaau;
    image[1u * SOFTPC_PARTITION_SECTOR_BYTES] = SOFTPC_PRIMARY_MARKER;
    image[18u * SOFTPC_PARTITION_SECTOR_BYTES] = SOFTPC_LOGICAL_MARKER;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(image, 1u, sizeof(image), file) == sizeof(image));
    assert(fclose(file) == 0);
}

int main(void)
{
    const char *path = "softpc-partition-image-smoke.img";
    softpc_machine_options options = { NULL, path,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    unsigned char markers[2] = { 0u, 0u };
    unsigned char mbr_table[32];
    unsigned char ebr_table[16];
    FILE *file;
    unsigned int slice;

    softpc_partition_write_image(path);
    options.media_mode = SOFTPC_MEDIA_OVERLAY;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    for (slice = 0u; slice < 16u; ++slice)
        assert(softpc_machine_run(machine, 6000u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, markers,
        sizeof(markers)) == SOFTPC_MACHINE_OK);
    assert(markers[0] == SOFTPC_PRIMARY_MARKER);
    assert(markers[1] == SOFTPC_LOGICAL_MARKER);
    softpc_machine_destroy(machine);

    /* Overlay writes are not committed, and the host has not modified or
       normalized the MBR/EBR records while presenting the complete disk. */
    file = fopen(path, "rb");
    assert(file != NULL);
    assert(fseek(file, 0x1beL, SEEK_SET) == 0);
    assert(fread(mbr_table, 1u, sizeof(mbr_table), file) == sizeof(mbr_table));
    assert(fseek(file, 17L * SOFTPC_PARTITION_SECTOR_BYTES + 0x1beL,
        SEEK_SET) == 0);
    assert(fread(ebr_table, 1u, sizeof(ebr_table), file) == sizeof(ebr_table));
    assert(fclose(file) == 0);
    assert(mbr_table[4] == 0x06u && mbr_table[16u + 4u] == 0x05u);
    assert(ebr_table[4] == 0x06u);
    assert(softpc_test_remove_image(path));
    return 0;
}
