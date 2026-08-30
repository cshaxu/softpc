#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void write_boot_image(const char *path, unsigned char marker)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* mov byte ptr [0500],42; jmp $ */
    sector[0] = 0xc6u; sector[1] = 0x06u; sector[2] = 0x00u;
    sector[3] = 0x05u; sector[4] = marker; sector[5] = 0xebu; sector[6] = 0xfeu;
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_keyboard_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* in al,60h; mov [0500],al; jmp $ */
    sector[0] = 0xe4u; sector[1] = 0x60u; sector[2] = 0xa2u;
    sector[3] = 0x00u; sector[4] = 0x05u; sector[5] = 0xebu; sector[6] = 0xfeu;
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_int16_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* xor ah,ah; int 16h; mov al,ah; mov [0500],al; jmp $ */
    unsigned char program[] = {
        0x30u, 0xe4u, 0xcdu, 0x16u, 0x88u, 0xe0u, 0xa2u,
        0x00u, 0x05u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_int15_memory_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* AH=88h; INT 15h; store AX at 0500h; jmp $. */
    unsigned char program[] = {
        0xb4u, 0x88u, 0xcdu, 0x15u, 0xa3u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_int16_ascii_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* xor ah,ah; int 16h; mov [0500],al; jmp $ */
    unsigned char program[] = {
        0x30u, 0xe4u, 0xcdu, 0x16u, 0xa2u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_int16_check_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* AH=01 checks without consuming; AH=00 must still return scan 1Eh. */
    unsigned char program[] = {
        0xb4u, 0x01u, 0xcdu, 0x16u, 0x30u, 0xe4u, 0xcdu, 0x16u,
        0x88u, 0xe0u, 0xa2u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_timer_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* program PIT channel 0 with 1234h; read its low byte into [0500]. */
    unsigned char program[] = {
        0xb0u, 0x34u, 0xe6u, 0x43u, 0xb0u, 0x34u, 0xe6u, 0x40u,
        0xb0u, 0x12u, 0xe6u, 0x40u, 0xe4u, 0x40u, 0xa2u, 0x00u,
        0x05u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_text_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* mov ax,b800; mov es,ax; mov word es:[0],0756; jmp $ */
    unsigned char program[] = {
        0xb8u, 0x00u, 0xb8u, 0x8eu, 0xc0u, 0x26u, 0xc7u, 0x06u,
        0x00u, 0x00u, 0x56u, 0x07u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_int10_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* mov ah,0e; mov al,'V'; int 10; jmp $ */
    unsigned char program[] = {
        0xb4u, 0x0eu, 0xb0u, 0x56u, 0xcdu, 0x10u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_int12_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* int 12h; mov [0500],ax; jmp $ */
    unsigned char program[] = {
        0xcdu, 0x12u, 0xa3u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_int11_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* INT 11h; store equipment word at 0500h; jmp $. */
    unsigned char program[] = {
        0xcdu, 0x11u, 0xa3u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_int1a_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* Set BDA ticks to 1234h; AH=00h; INT 1Ah; store DX:CX at 0500h. */
    unsigned char program[] = {
        0xc7u, 0x06u, 0x6cu, 0x04u, 0x34u, 0x12u, 0xb4u, 0x00u,
        0xcdu, 0x1au, 0x89u, 0x16u, 0x00u, 0x05u, 0x89u, 0x0eu,
        0x02u, 0x05u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_int1a_tick_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* Program PIT for every instruction, then repeatedly read its BDA tick
       through INT 1Ah into 0500h. */
    unsigned char program[] = {
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u, 0xb0u, 0x34u, 0xe6u,
        0x43u, 0xb0u, 0x01u, 0xe6u, 0x40u, 0xb0u, 0x00u, 0xe6u,
        0x40u, 0xfbu, 0xb4u, 0x00u, 0xcdu, 0x1au, 0x89u, 0x16u,
        0x00u, 0x05u, 0xebu, 0xf6u
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_hdd_pio_boot_image(const char *path)
{
    unsigned char image[1024] = { 0 };
    FILE *file;
    unsigned char program[] = {
        0xbau, 0xf2u, 0x01u, 0xb0u, 0x01u, 0xeeu,
        0x42u, 0xb0u, 0x01u, 0xeeu, 0x42u, 0x30u, 0xc0u, 0xeeu,
        0x42u, 0xeeu, 0x42u, 0xb0u, 0xe0u, 0xeeu,
        0x42u, 0xb0u, 0x20u, 0xeeu, 0xbau, 0xf0u, 0x01u,
        0xedu, 0xa3u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    memcpy(image, program, sizeof(program));
    image[510] = 0x55u; image[511] = 0xaau;
    image[512] = 0x5au;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(image, 1u, sizeof(image), file) == sizeof(image));
    assert(fclose(file) == 0);
}

static void write_int13_boot_image(const char *path, unsigned char drive,
    unsigned char head, unsigned char sector, unsigned char marker)
{
    unsigned char image[64u * 512u] = { 0 };
    FILE *file;
    unsigned int lba;
    unsigned int bytes;
    unsigned char program[] = {
        0xb4u, 0x02u, 0xb0u, 0x01u, 0xb5u, 0x00u, 0xb1u, 0x02u,
        0xb6u, 0x00u, 0xb2u, 0x00u, 0xbbu, 0x00u, 0x06u, 0xcdu,
        0x13u, 0xa0u, 0x00u, 0x06u, 0xa2u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    program[7] = sector;
    program[9] = head;
    program[11] = drive;
    lba = (unsigned int)head * (drive < 0x80u ? 18u : 63u) +
        (unsigned int)sector - 1u;
    memcpy(image, program, sizeof(program));
    image[510] = 0x55u; image[511] = 0xaau;
    image[lba * 512u] = marker;
    bytes = (lba + 1u) * 512u;
    if (bytes < 1024u) bytes = 1024u;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(image, 1u, bytes, file) == bytes);
    assert(fclose(file) == 0);
}

static void write_hdd_pio_write_boot_image(const char *path)
{
    unsigned char image[1024] = { 0 };
    FILE *file;
    /* Select LBA 1 and write the boot sector's 512 bytes through REP OUTSW. */
    unsigned char program[] = {
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u,
        0xbau, 0xf2u, 0x01u, 0xb0u, 0x01u, 0xeeu,
        0x42u, 0xb0u, 0x01u, 0xeeu, 0x42u, 0x30u, 0xc0u, 0xeeu,
        0x42u, 0xeeu, 0x42u, 0xb0u, 0xe0u, 0xeeu,
        0x42u, 0xb0u, 0x30u, 0xeeu, 0xbau, 0xf0u, 0x01u,
        0xbeu, 0x00u, 0x7du, 0xb9u, 0x00u, 0x01u, 0xfcu, 0xf3u,
        0x6fu, 0xebu, 0xfeu
    };
    memcpy(image, program, sizeof(program));
    image[0x100u] = 0x5au;
    image[0x101u] = 0xa5u;
    image[510] = 0x55u; image[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(image, 1u, sizeof(image), file) == sizeof(image));
    assert(fclose(file) == 0);
}

static void write_hdd_identify_boot_image(const char *path)
{
    unsigned char image[1024] = { 0 };
    FILE *file;
    /* IDENTIFY DEVICE; copy word 0 and LBA sector-count word 60 to RAM. */
    unsigned char program[] = {
        0xbau, 0xf7u, 0x01u, 0xb0u, 0xecu, 0xeeu, 0xbau, 0xf0u,
        0x01u, 0xedu, 0xa3u, 0x00u, 0x05u, 0xb9u, 0x3bu, 0x00u,
        0xedu, 0xe2u, 0xfdu, 0xedu, 0xa3u, 0x02u, 0x05u, 0xebu,
        0xfeu
    };
    memcpy(image, program, sizeof(program));
    image[510] = 0x55u; image[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(image, 1u, sizeof(image), file) == sizeof(image));
    assert(fclose(file) == 0);
}

static void write_int13_multi_boot_image(const char *path, unsigned char drive,
    unsigned char first_marker, unsigned char second_marker)
{
    unsigned char image[64u * 512u] = { 0 };
    FILE *file;
    /* Read CHS 0/0/sectors 2-3 into 0000:0600, then expose both bytes. */
    unsigned char program[] = {
        0xb4u, 0x02u, 0xb0u, 0x02u, 0xb5u, 0x00u, 0xb1u, 0x02u,
        0xb6u, 0x00u, 0xb2u, 0x00u, 0xbbu, 0x00u, 0x06u, 0xcdu,
        0x13u, 0xa0u, 0x00u, 0x06u, 0xa2u, 0x00u, 0x05u, 0xa0u,
        0x00u, 0x08u, 0xa2u, 0x01u, 0x05u, 0xebu, 0xfeu
    };
    program[11] = drive;
    memcpy(image, program, sizeof(program));
    image[510] = 0x55u; image[511] = 0xaau;
    image[1u * 512u] = first_marker;
    image[2u * 512u] = second_marker;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(image, 1u, sizeof(image), file) == sizeof(image));
    assert(fclose(file) == 0);
}

static void write_int13_360k_boot_image(const char *path)
{
    unsigned char *image = calloc(1u, 368640u);
    FILE *file;
    /* Read CHS 0/1/1.  On 360 KiB media this is LBA 9, not LBA 18. */
    unsigned char program[] = {
        0xb4u, 0x02u, 0xb0u, 0x01u, 0xb5u, 0x00u, 0xb1u, 0x01u,
        0xb6u, 0x01u, 0xb2u, 0x00u, 0xbbu, 0x00u, 0x06u, 0xcdu,
        0x13u, 0xa0u, 0x00u, 0x06u, 0xa2u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    assert(image != NULL);
    memcpy(image, program, sizeof(program));
    image[510] = 0x55u; image[511] = 0xaau;
    image[9u * 512u] = 0x74u;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(image, 1u, 368640u, file) == 368640u);
    assert(fclose(file) == 0);
    free(image);
}

static void write_int13_bpb_hdd_boot_image(const char *path)
{
    unsigned char image[64u * 512u] = { 0 };
    FILE *file;
    /* Read CHS 0/1/1.  The partition BPB declares 17 sectors/track. */
    unsigned char program[] = {
        0xb4u, 0x02u, 0xb0u, 0x01u, 0xb5u, 0x00u, 0xb1u, 0x01u,
        0xb6u, 0x01u, 0xb2u, 0x80u, 0xbbu, 0x00u, 0x06u, 0xcdu,
        0x13u, 0xa0u, 0x00u, 0x06u, 0xa2u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    memcpy(image, program, sizeof(program));
    image[446u + 4u] = 0x06u;
    image[446u + 8u] = 1u;
    image[446u + 12u] = 63u;
    image[510] = 0x55u; image[511] = 0xaau;
    image[512u + 24u] = 17u;
    image[512u + 26u] = 4u;
    image[17u * 512u] = 0x75u;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(image, 1u, sizeof(image), file) == sizeof(image));
    assert(fclose(file) == 0);
}

static void write_int13_reset_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* xor ah,ah; int 13h; mov byte ptr [0500],76h; jmp $ */
    unsigned char program[] = {
        0x30u, 0xe4u, 0xcdu, 0x13u, 0xc6u, 0x06u, 0x00u,
        0x05u, 0x76u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void write_int13_write_boot_image(const char *path)
{
    unsigned char image[1024] = { 0 };
    FILE *file;
    /* Write byte 5Ah at 0000:0600 through INT 13h CHS 0/0/2. */
    unsigned char program[] = {
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u, 0x8eu, 0xc0u,
        0xc6u, 0x06u, 0x00u, 0x06u, 0x5au, 0xb4u, 0x03u,
        0xb0u, 0x01u, 0xb5u, 0x00u, 0xb1u, 0x02u, 0xb6u,
        0x00u, 0xb2u, 0x00u, 0xbbu, 0x00u, 0x06u, 0xcdu,
        0x13u, 0xebu, 0xfeu
    };
    memcpy(image, program, sizeof(program));
    image[510] = 0x55u; image[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(image, 1u, sizeof(image), file) == sizeof(image));
    assert(fclose(file) == 0);
}

static void write_int13_parameters_boot_image(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file;
    /* AH=08h; store CL(sectors/track) and DH(max head). */
    unsigned char program[] = {
        0xb4u, 0x08u, 0xcdu, 0x13u, 0x88u, 0xc8u, 0xa2u, 0x00u,
        0x05u, 0x88u, 0xf0u, 0xa2u, 0x01u, 0x05u, 0xebu, 0xfeu
    };
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
}

static void run_boot_image(const char *path, int floppy, unsigned char expected,
    uint64_t instruction_budget)
{
    unsigned char marker = 0;
    softpc_machine_options options = { NULL, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    if (floppy) options.floppy_path = path;
    else options.hard_disk_path = path;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, instruction_budget) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == expected);
    softpc_machine_destroy(machine);
}

static void run_keyboard_boot_image(const char *path)
{
    unsigned char marker = 0;
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_key_scancode(machine, 0x1eu) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 4u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == 0x1eu);
    softpc_machine_destroy(machine);
}

static void run_int16_boot_image(const char *path)
{
    unsigned char marker = 0;
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_key_scancode(machine, 0x1eu) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 64u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == 0x1eu);
    softpc_machine_destroy(machine);
}

static void run_int15_memory_boot_image(const char *path)
{
    unsigned char memory_kib[2] = { 0, 0 };
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 64u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, memory_kib,
        sizeof(memory_kib)) == SOFTPC_MACHINE_OK);
    assert(memory_kib[0] == 0u && memory_kib[1] == 0x3cu);
    softpc_machine_destroy(machine);
}

static void run_int16_ascii_boot_image(const char *path)
{
    unsigned char marker = 0;
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_key_scancode(machine, 0x1eu) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_key_scancode(machine, 0x9eu) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 64u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == 'a');
    softpc_machine_destroy(machine);
}

static void run_int16_check_boot_image(const char *path)
{
    unsigned char marker = 0;
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_key_scancode(machine, 0x1eu) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_key_scancode(machine, 0x9eu) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 64u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == 0x1eu);
    softpc_machine_destroy(machine);
}

static void run_int13_parameters_boot_image(const char *path)
{
    unsigned char values[2] = { 0, 0 };
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 64u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, values, sizeof(values)) == SOFTPC_MACHINE_OK);
    assert(values[0] == 18u && values[1] == 1u);
    softpc_machine_destroy(machine);
}

static void run_timer_boot_image(const char *path)
{
    unsigned char marker = 0;
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 16u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == 0x34u);
    softpc_machine_destroy(machine);
}

static void run_text_boot_image(const char *path)
{
    unsigned char cell[2] = { 0, 0 };
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 16u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0xb8000u, cell,
        sizeof(cell)) == SOFTPC_MACHINE_OK);
    assert(cell[0] == 'V');
    assert(cell[1] == 0x07u);
    softpc_machine_destroy(machine);
}

static void run_int10_boot_image(const char *path)
{
    unsigned char cell[2] = { 0, 0 };
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 64u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0xb8000u, cell,
        sizeof(cell)) == SOFTPC_MACHINE_OK);
    assert(cell[0] == 'V');
    assert(cell[1] == 0x07u);
    softpc_machine_destroy(machine);
}

static void run_int12_boot_image(const char *path)
{
    unsigned char memory_kib[2] = { 0, 0 };
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 64u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, memory_kib,
        sizeof(memory_kib)) == SOFTPC_MACHINE_OK);
    assert(memory_kib[0] == 0x80u && memory_kib[1] == 0x02u);
    softpc_machine_destroy(machine);
}

static void run_int11_boot_image(const char *path, int floppy,
    unsigned char expected)
{
    unsigned char equipment[2] = { 0, 0 };
    softpc_machine_options options = { NULL, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    if (floppy) options.floppy_path = path;
    else options.hard_disk_path = path;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, floppy ? 64u : 1000u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, equipment,
        sizeof(equipment)) == SOFTPC_MACHINE_OK);
    assert(equipment[0] == expected && equipment[1] == 0u);
    softpc_machine_destroy(machine);
}

static void run_int1a_boot_image(const char *path)
{
    unsigned char ticks[4] = { 0xffu, 0xffu, 0xffu, 0xffu };
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 64u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, ticks,
        sizeof(ticks)) == SOFTPC_MACHINE_OK);
    assert(ticks[0] == 0x34u && ticks[1] == 0x12u && ticks[2] == 0u &&
        ticks[3] == 0u);
    softpc_machine_destroy(machine);
}

static void run_int1a_tick_boot_image(const char *path)
{
    unsigned char bda_tick_low = 0u;
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 100u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 1000u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 100u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x46cu, &bda_tick_low,
        1u) == SOFTPC_MACHINE_OK);
    assert(bda_tick_low != 0u);
    softpc_machine_destroy(machine);
}

static void run_hdd_pio_boot_image(const char *path)
{
    unsigned char marker = 0;
    softpc_machine_options options = { NULL, path, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 1100u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == 0x5au);
    softpc_machine_destroy(machine);
}

static void run_int13_boot_image(const char *path, int floppy, unsigned char expected)
{
    unsigned char marker = 0;
    softpc_machine_options options = { NULL, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    if (floppy) options.floppy_path = path;
    else options.hard_disk_path = path;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 3000u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == expected);
    softpc_machine_destroy(machine);
}

static void run_hdd_pio_write_boot_image(const char *path)
{
    unsigned char written[2] = { 0, 0 };
    softpc_machine_options options = { NULL, path, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    FILE *file;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 2000u) == SOFTPC_MACHINE_OK);
    softpc_machine_destroy(machine);
    file = fopen(path, "rb");
    assert(file != NULL);
    assert(fseek(file, 512L, SEEK_SET) == 0);
    assert(fread(written, 1u, sizeof(written), file) == sizeof(written));
    assert(fclose(file) == 0);
    assert(written[0] == 0x5au && written[1] == 0xa5u);
}

static void run_hdd_identify_boot_image(const char *path)
{
    unsigned char identify[4] = { 0, 0, 0, 0 };
    softpc_machine_options options = { NULL, path, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 2000u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, identify,
        sizeof(identify)) == SOFTPC_MACHINE_OK);
    assert(identify[0] == 0x40u && identify[1] == 0u);
    assert(identify[2] == 2u && identify[3] == 0u);
    softpc_machine_destroy(machine);
}

static void run_int13_multi_boot_image(const char *path, int floppy,
    unsigned char first_expected, unsigned char second_expected)
{
    unsigned char markers[2] = { 0, 0 };
    softpc_machine_options options = { NULL, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    if (floppy) options.floppy_path = path;
    else options.hard_disk_path = path;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 6000u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, markers,
        sizeof(markers)) == SOFTPC_MACHINE_OK);
    assert(markers[0] == first_expected);
    assert(markers[1] == second_expected);
    softpc_machine_destroy(machine);
}

static void run_int13_write_boot_image(const char *path)
{
    unsigned char marker = 0u;
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    FILE *file;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 4000u) == SOFTPC_MACHINE_OK);
    softpc_machine_destroy(machine);
    file = fopen(path, "rb");
    assert(file != NULL);
    assert(fseek(file, 512L, SEEK_SET) == 0);
    assert(fread(&marker, 1u, 1u, file) == 1u);
    assert(fclose(file) == 0);
    assert(marker == 0x5au);
}

int main(void)
{
    const char *floppy = "softpc-machine-floppy-smoke.img";
    const char *hdd = "softpc-machine-hdd-smoke.img";
    const char *keyboard = "softpc-machine-keyboard-smoke.img";
    const char *int16 = "softpc-machine-int16-smoke.img";
    const char *int15_memory = "softpc-machine-int15-memory-smoke.img";
    const char *int16_ascii = "softpc-machine-int16-ascii-smoke.img";
    const char *int16_check = "softpc-machine-int16-check-smoke.img";
    const char *timer = "softpc-machine-timer-smoke.img";
    const char *text = "softpc-machine-text-smoke.img";
    const char *int10 = "softpc-machine-int10-smoke.img";
    const char *int12 = "softpc-machine-int12-smoke.img";
    const char *int11 = "softpc-machine-int11-smoke.img";
    const char *int1a = "softpc-machine-int1a-smoke.img";
    const char *int1a_tick = "softpc-machine-int1a-tick-smoke.img";
    const char *hdd_pio = "softpc-machine-hdd-pio-smoke.img";
    const char *hdd_pio_write = "softpc-machine-hdd-pio-write-smoke.img";
    const char *hdd_identify = "softpc-machine-hdd-identify-smoke.img";
    const char *hdd_int13 = "softpc-machine-hdd-int13-smoke.img";
    const char *floppy_int13 = "softpc-machine-floppy-int13-smoke.img";
    const char *hdd_int13_head = "softpc-machine-hdd-int13-head-smoke.img";
    const char *floppy_int13_multi = "softpc-machine-floppy-int13-multi-smoke.img";
    const char *hdd_int13_multi = "softpc-machine-hdd-int13-multi-smoke.img";
    const char *floppy_int13_360k = "softpc-machine-floppy-int13-360k-smoke.img";
    const char *hdd_int13_bpb = "softpc-machine-hdd-int13-bpb-smoke.img";
    const char *int13_reset = "softpc-machine-int13-reset-smoke.img";
    const char *int13_write = "softpc-machine-int13-write-smoke.img";
    const char *int13_parameters = "softpc-machine-int13-parameters-smoke.img";
    softpc_machine_options conflicting_media = { floppy, hdd,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *conflicting_machine = NULL;
    write_boot_image(floppy, 0x42u);
    write_boot_image(hdd, 0x77u);
    write_keyboard_boot_image(keyboard);
    write_int16_boot_image(int16);
    write_int15_memory_boot_image(int15_memory);
    write_int16_ascii_boot_image(int16_ascii);
    write_int16_check_boot_image(int16_check);
    write_timer_boot_image(timer);
    write_text_boot_image(text);
    write_int10_boot_image(int10);
    write_int12_boot_image(int12);
    write_int11_boot_image(int11);
    write_int1a_boot_image(int1a);
    write_int1a_tick_boot_image(int1a_tick);
    write_hdd_pio_boot_image(hdd_pio);
    write_hdd_pio_write_boot_image(hdd_pio_write);
    write_hdd_identify_boot_image(hdd_identify);
    write_int13_boot_image(hdd_int13, 0x80u, 0u, 2u, 0x6bu);
    write_int13_boot_image(floppy_int13, 0x00u, 0u, 2u, 0x6cu);
    write_int13_boot_image(hdd_int13_head, 0x80u, 1u, 1u, 0x6du);
    write_int13_multi_boot_image(floppy_int13_multi, 0x00u, 0x71u, 0x72u);
    write_int13_multi_boot_image(hdd_int13_multi, 0x80u, 0x73u, 0x74u);
    write_int13_360k_boot_image(floppy_int13_360k);
    write_int13_bpb_hdd_boot_image(hdd_int13_bpb);
    write_int13_reset_boot_image(int13_reset);
    write_int13_write_boot_image(int13_write);
    write_int13_parameters_boot_image(int13_parameters);
    assert(softpc_machine_create(&conflicting_media, &conflicting_machine) ==
        SOFTPC_MACHINE_INVALID_ARGUMENT);
    assert(conflicting_machine == NULL);
    run_boot_image(floppy, 1, 0x42u, 4u);
    run_boot_image(hdd, 0, 0x77u, 1000u);
    run_keyboard_boot_image(keyboard);
    run_int16_boot_image(int16);
    run_int15_memory_boot_image(int15_memory);
    run_int16_ascii_boot_image(int16_ascii);
    run_int16_check_boot_image(int16_check);
    run_timer_boot_image(timer);
    run_text_boot_image(text);
    run_int10_boot_image(int10);
    run_int12_boot_image(int12);
    run_int11_boot_image(int11, 1, 0x23u);
    run_int11_boot_image(int11, 0, 0x22u);
    run_int1a_boot_image(int1a);
    run_int1a_tick_boot_image(int1a_tick);
    run_hdd_pio_boot_image(hdd_pio);
    run_hdd_pio_write_boot_image(hdd_pio_write);
    run_hdd_identify_boot_image(hdd_identify);
    run_int13_boot_image(hdd_int13, 0, 0x6bu);
    run_int13_boot_image(floppy_int13, 1, 0x6cu);
    run_int13_boot_image(hdd_int13_head, 0, 0x6du);
    run_int13_multi_boot_image(floppy_int13_multi, 1, 0x71u, 0x72u);
    run_int13_multi_boot_image(hdd_int13_multi, 0, 0x73u, 0x74u);
    run_int13_boot_image(floppy_int13_360k, 1, 0x74u);
    run_int13_boot_image(hdd_int13_bpb, 0, 0x75u);
    run_boot_image(int13_reset, 1, 0x76u, 64u);
    run_int13_write_boot_image(int13_write);
    run_int13_parameters_boot_image(int13_parameters);
    assert(remove(floppy) == 0);
    assert(remove(hdd) == 0);
    assert(remove(keyboard) == 0);
    assert(remove(int16) == 0);
    assert(remove(int15_memory) == 0);
    assert(remove(int16_ascii) == 0);
    assert(remove(int16_check) == 0);
    assert(remove(timer) == 0);
    assert(remove(text) == 0);
    assert(remove(int10) == 0);
    assert(remove(int12) == 0);
    assert(remove(int11) == 0);
    assert(remove(int1a) == 0);
    assert(remove(int1a_tick) == 0);
    assert(remove(hdd_pio) == 0);
    assert(remove(hdd_pio_write) == 0);
    assert(remove(hdd_identify) == 0);
    assert(remove(hdd_int13) == 0);
    assert(remove(floppy_int13) == 0);
    assert(remove(hdd_int13_head) == 0);
    assert(remove(floppy_int13_multi) == 0);
    assert(remove(hdd_int13_multi) == 0);
    assert(remove(floppy_int13_360k) == 0);
    assert(remove(hdd_int13_bpb) == 0);
    assert(remove(int13_reset) == 0);
    assert(remove(int13_write) == 0);
    assert(remove(int13_parameters) == 0);
    return 0;
}
