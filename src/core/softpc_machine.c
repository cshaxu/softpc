#include "softpc_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* CCPU's standalone executor entry points.  The wrapper intentionally calls
 * the core directly instead of the historical host shim. */
extern void c_cpu_init(void);
extern void c_cpu_reset(void);
extern void c_cpu_simulate(void);
extern void sas_init(unsigned long size);
extern void sas_term(void);
extern void io_init(void);
extern void SWPIC_init_funcptrs(void);
extern void ica0_init(void);
extern void ica1_init(void);
extern void ica0_post(void);
extern void ica1_post(void);
extern unsigned long softpc_ccpu_instruction_budget;
extern int softpc_platform_write_physical(unsigned long address,
    const unsigned char *bytes, unsigned long length);
extern int softpc_platform_read_physical(unsigned long address,
    unsigned char *bytes, unsigned long length);
extern void softpc_platform_keyboard_init(void);
extern int softpc_platform_keyboard_scancode(unsigned char scan_code);
extern void softpc_platform_timer_init(void);
extern void softpc_platform_timer_advance(unsigned long instructions);
extern void softpc_platform_hdd_init(void);
extern int softpc_platform_hdd_attach(const char *path);
extern void softpc_platform_hdd_detach(void);

#define SOFTPC_FIXED_RAM_BYTES (16ul * 1024ul * 1024ul)
#define SOFTPC_BOOT_SECTOR_BYTES 512u
#define SOFTPC_BOOT_SECTOR_ADDRESS 0x7c00u
#define SOFTPC_RESET_VECTOR_ADDRESS 0xffff0u

struct softpc_machine {
    softpc_machine_options options;
    int reset;
    int hardware_initialized;
};

static int softpc_machine_media_exists(const char *path)
{
    FILE *file;
    if (path == NULL) return 1;
    file = fopen(path, "rb");
    if (file == NULL) return 0;
    fclose(file);
    return 1;
}

static softpc_machine_result softpc_machine_load_boot_sector(
    const softpc_machine *machine)
{
    const char *path = machine->options.floppy_path != NULL ?
        machine->options.floppy_path : machine->options.hard_disk_path;
    unsigned char sector[SOFTPC_BOOT_SECTOR_BYTES];
    FILE *file = fopen(path, "rb");
    if (file == NULL) return SOFTPC_MACHINE_IO_ERROR;
    if (fread(sector, 1u, sizeof(sector), file) != sizeof(sector)) {
        fclose(file);
        return SOFTPC_MACHINE_IO_ERROR;
    }
    fclose(file);
    if (sector[510] != 0x55u || sector[511] != 0xaau)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    if (!softpc_platform_write_physical(SOFTPC_BOOT_SECTOR_ADDRESS, sector,
            sizeof(sector))) return SOFTPC_MACHINE_IO_ERROR;
    return SOFTPC_MACHINE_OK;
}

static softpc_machine_result softpc_machine_install_reset_rom(
    const softpc_machine *machine)
{
    /* Architectural reset enters at f000:fff0. HDD boot uses ATA PIO to
       fetch LBA 0; floppy remains on the temporary sector-load path. */
    static const unsigned char hdd_reset_vector[] = { 0xeau, 0x00u, 0x00u, 0x00u, 0xf0u };
    static const unsigned char hdd_boot_rom[] = {
        0xbau, 0xf2u, 0x01u, 0xb0u, 0x01u, 0xeeu,
        0x42u, 0x30u, 0xc0u, 0xeeu, 0x42u, 0xeeu, 0x42u, 0xeeu,
        0x42u, 0xb0u, 0xe0u, 0xeeu, 0x42u, 0xb0u, 0x20u, 0xeeu,
        0xbau, 0xf0u, 0x01u, 0xbfu, 0x00u, 0x7cu, 0xb9u, 0x00u, 0x01u,
        0xfcu, 0xedu, 0xabu, 0xe2u, 0xfcu, 0xeau, 0x00u, 0x7cu, 0x00u, 0x00u
    };
    /* Assembled from firmware/int13_chs.asm.  Keep the checked-in assembly
       source and this freestanding C initializer in lockstep so the normal C
       build has no assembler-toolchain dependency. */
    static const unsigned char hdd_int13_rom[] = {
        0x50u, 0x53u, 0x51u, 0x52u, 0x57u, 0x55u, 0x50u, 0x89u,
        0xdfu, 0x80u, 0xfcu, 0x02u, 0x0fu, 0x85u, 0x80u, 0x00u,
        0x08u, 0xc0u, 0x74u,
        0x7cu, 0x3cu, 0x80u, 0x77u, 0x78u, 0x89u, 0xc8u, 0x86u,
        0xc4u, 0x80u, 0xe4u, 0xc0u, 0xc0u, 0xecu, 0x06u, 0x89u,
        0xd3u, 0x88u, 0xfbu, 0x30u, 0xffu, 0x89u, 0xd5u, 0x81u,
        0xe5u, 0xffu, 0x00u, 0x81u, 0xfdu, 0x80u, 0x00u, 0x72u,
        0x05u, 0xbeu, 0x10u, 0x00u, 0xebu, 0x03u, 0xbeu,
        0x02u, 0x00u, 0xf7u, 0xe6u, 0x01u, 0xd8u, 0x81u, 0xfdu,
        0x80u, 0x00u, 0x72u, 0x05u, 0xbeu, 0x3fu, 0x00u, 0xebu, 0x03u,
        0xbeu, 0x12u, 0x00u, 0xf7u, 0xe6u, 0x89u, 0xcbu,
        0x80u, 0xe3u, 0x3fu, 0x4bu, 0x01u, 0xd8u, 0x89u, 0xc3u,
        0xbau, 0xf2u, 0x01u, 0x59u, 0x89u, 0xc8u, 0xeeu, 0x42u, 0x89u,
        0xd8u, 0xeeu, 0x42u, 0x88u, 0xe0u, 0xeeu, 0x42u, 0x30u,
        0xc0u, 0xeeu, 0x42u, 0xb0u, 0xe0u, 0xeeu, 0x42u, 0xb0u,
        0x20u, 0xeeu, 0xbau, 0xf0u, 0x01u, 0x30u,
        0xedu, 0x51u, 0xb9u, 0x00u, 0x01u, 0xfcu, 0xedu, 0xabu,
        0xe2u, 0xfcu, 0x59u, 0xe2u, 0xf4u, 0x5du, 0x5fu, 0x5au,
        0x59u, 0x5bu, 0x58u, 0x30u, 0xe4u,
        0xf8u, 0xcfu, 0x59u, 0x5du, 0x5fu, 0x5au, 0x59u, 0x5bu, 0x58u,
        0xb4u, 0x01u, 0xf9u, 0xcfu
    };
    static const unsigned char hdd_int13_vector[] = { 0x00u, 0x01u, 0x00u, 0xf0u };
    /* Assembled from firmware/int10_teletype.asm; embedding keeps the normal
       C build independent of an assembler installation. */
    static const unsigned char int10_teletype_rom[] = {
        0x50u, 0x53u, 0x52u, 0x57u, 0x06u, 0x80u, 0xfcu, 0x0eu,
        0x75u, 0x2bu, 0xbbu, 0x00u, 0xb8u, 0x8eu, 0xc3u, 0x2eu,
        0x8bu, 0x3eu, 0x3bu, 0x02u, 0x3cu, 0x0du, 0x74u, 0x1du,
        0x3cu, 0x0au, 0x74u, 0x19u, 0xb4u, 0x07u, 0xabu, 0x2eu,
        0x83u, 0x06u, 0x3bu, 0x02u, 0x02u, 0x2eu, 0x81u, 0x3eu,
        0x3bu, 0x02u, 0xa0u, 0x0fu, 0x72u, 0x07u, 0x2eu, 0xc7u,
        0x06u, 0x3bu, 0x02u, 0x00u, 0x00u, 0x07u, 0x5fu, 0x5au,
        0x5bu, 0x58u, 0xcfu, 0x00u, 0x00u
    };
    static const unsigned char int10_vector[] = { 0x00u, 0x02u, 0x00u, 0xf0u };
    static const unsigned char floppy_reset_vector[] = {
        0xeau, 0x00u, 0x7cu, 0x00u, 0x00u
    };
    if (!softpc_platform_write_physical(0xf0100u, hdd_int13_rom,
            sizeof(hdd_int13_rom)) ||
        !softpc_platform_write_physical(0x4cu, hdd_int13_vector,
            sizeof(hdd_int13_vector)) ||
        !softpc_platform_write_physical(0xf0200u, int10_teletype_rom,
            sizeof(int10_teletype_rom)) ||
        !softpc_platform_write_physical(0x40u, int10_vector,
            sizeof(int10_vector))) return SOFTPC_MACHINE_IO_ERROR;
    if (machine->options.floppy_path == NULL) {
        if (!softpc_platform_write_physical(0xf0000u, hdd_boot_rom,
                sizeof(hdd_boot_rom)) ||
            !softpc_platform_write_physical(SOFTPC_RESET_VECTOR_ADDRESS,
                hdd_reset_vector, sizeof(hdd_reset_vector)))
            return SOFTPC_MACHINE_IO_ERROR;
    } else if (!softpc_platform_write_physical(SOFTPC_RESET_VECTOR_ADDRESS,
            floppy_reset_vector, sizeof(floppy_reset_vector))) {
        return SOFTPC_MACHINE_IO_ERROR;
    }
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_create(const softpc_machine_options *options,
    softpc_machine **machine_out)
{
    softpc_machine *machine;
    if (machine_out != NULL) *machine_out = NULL;
    if (options == NULL || machine_out == NULL ||
        (options->floppy_path == NULL && options->hard_disk_path == NULL) ||
        (options->floppy_path != NULL && options->hard_disk_path != NULL) ||
        !softpc_machine_media_exists(options->floppy_path) ||
        !softpc_machine_media_exists(options->hard_disk_path))
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    machine = calloc(1u, sizeof(*machine));
    if (machine == NULL) return SOFTPC_MACHINE_IO_ERROR;
    machine->options = *options;
    *machine_out = machine;
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_reset(softpc_machine *machine)
{
    if (machine == NULL) return SOFTPC_MACHINE_INVALID_ARGUMENT;
    if (!machine->hardware_initialized) {
        sas_init(SOFTPC_FIXED_RAM_BYTES);
        io_init();
        SWPIC_init_funcptrs();
        ica0_init();
        ica1_init();
        softpc_platform_timer_init();
        softpc_platform_keyboard_init();
        softpc_platform_hdd_init();
        machine->hardware_initialized = 1;
    }
    c_cpu_init();
    c_cpu_reset();
    /* The original product's BIOS POST programmed the two 8259 PICs after
       their port glue was registered.  A standalone reset owns that hardware
       action directly; it is not a guest-service operation. */
    ica0_post();
    ica1_post();
    if (!softpc_platform_hdd_attach(machine->options.hard_disk_path != NULL ?
            machine->options.hard_disk_path : machine->options.floppy_path))
        return SOFTPC_MACHINE_IO_ERROR;
    {
        softpc_machine_result result = SOFTPC_MACHINE_OK;
        if (machine->options.floppy_path != NULL)
            result = softpc_machine_load_boot_sector(machine);
        if (result != SOFTPC_MACHINE_OK) return result;
        result = softpc_machine_install_reset_rom(machine);
        if (result != SOFTPC_MACHINE_OK) return result;
    }
    machine->reset = 1;
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_key_scancode(softpc_machine *machine,
    uint8_t scan_code)
{
    if (machine == NULL || !machine->reset)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return softpc_platform_keyboard_scancode((unsigned char)scan_code) ?
        SOFTPC_MACHINE_OK : SOFTPC_MACHINE_IO_ERROR;
}

softpc_machine_result softpc_machine_read_physical(const softpc_machine *machine,
    uint32_t address, void *buffer, uint32_t bytes)
{
    if (machine == NULL || buffer == NULL || !machine->reset || bytes == 0u)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return softpc_platform_read_physical((unsigned long)address,
        (unsigned char *)buffer, (unsigned long)bytes) ?
        SOFTPC_MACHINE_OK : SOFTPC_MACHINE_INVALID_ARGUMENT;
}

softpc_machine_result softpc_machine_run(softpc_machine *machine,
    uint64_t instruction_budget)
{
    if (machine == NULL || !machine->reset || instruction_budget == 0u)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    softpc_ccpu_instruction_budget = (unsigned long)instruction_budget;
    c_cpu_simulate();
    softpc_platform_timer_advance((unsigned long)instruction_budget -
        softpc_ccpu_instruction_budget);
    return SOFTPC_MACHINE_OK;
}

void softpc_machine_destroy(softpc_machine *machine)
{
    if (machine != NULL && machine->hardware_initialized) {
        softpc_platform_hdd_detach();
        sas_term();
    }
    free(machine);
}

const char *softpc_machine_result_name(softpc_machine_result result)
{
    switch (result) {
    case SOFTPC_MACHINE_OK: return "ok";
    case SOFTPC_MACHINE_INVALID_ARGUMENT: return "invalid argument or media";
    case SOFTPC_MACHINE_IO_ERROR: return "host I/O error";
    case SOFTPC_MACHINE_BACKEND_UNAVAILABLE: return "SoftPC executor not wired";
    }
    return "unknown result";
}
