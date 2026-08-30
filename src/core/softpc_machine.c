#include "softpc_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* CCPU's standalone executor entry points.  The wrapper intentionally calls
 * the core directly instead of the historical host shim. */
extern void c_cpu_init(void);
extern void c_cpu_reset(void);
extern void c_cpu_simulate(void);
extern unsigned short c_getCS(void);
extern unsigned long c_getCS_BASE(void);
extern unsigned long c_getEIP(void);
extern void sas_init(unsigned long size);
extern void sas_term(void);
extern void sas_fills(unsigned long address, unsigned char value,
    unsigned long length);
extern void io_init(void);
extern void SWPIC_init_funcptrs(void);
extern void ica0_init(void);
extern void ica1_init(void);
extern void dma_init(void);
extern void dma_post(void);
extern void gfi_init(void);
extern void fla_init(void);
extern void cmos_init(void);
extern void rom_init(void);
extern void softpc_bios_setup_ivt(void);
extern void ppi_init(void);
extern void ica0_post(void);
extern void ica1_post(void);
extern void gvi_init(unsigned char adapter);
extern void video_init(void);
extern void *setup_global_data_ptr(void);
extern void setup_vga_globals(void);
extern int soft_reset;
extern unsigned long softpc_ccpu_instruction_budget;
extern int softpc_platform_write_physical(unsigned long address,
    const unsigned char *bytes, unsigned long length);
extern int softpc_platform_read_physical(unsigned long address,
    unsigned char *bytes, unsigned long length);
extern void softpc_platform_keyboard_reset(void);
extern void softpc_device_bop_register_machine_services(void);
extern int softpc_platform_keyboard_scancode(unsigned char scan_code);
extern void SWTMR_init_funcptrs(void);
extern void timer_init(void);
extern void timer_post(void);
extern void time_of_day_init(void);
extern void time_strobe(void);
extern void q_event_init(void);
extern void tic_event_init(void);
extern void printer_init(int adapter);
extern void printer_post(int adapter);
extern void com_init(int adapter);
extern void com_post(int adapter);
extern void disk_post(void);
extern void diskette_post(void);
extern void mouse_init(void);
extern void mouse_driver_initialisation(void);
extern void mouse_driver_termination(void);
extern void softpc_platform_hdd_init(void);
extern int softpc_platform_hdd_attach(const char *floppy_path,
    const char *hard_disk_path);
extern void softpc_platform_hdd_detach(void);
extern int softpc_platform_floppy_attach(const char *path);
extern void softpc_platform_floppy_detach(void);
extern int softpc_platform_video_buffers_init(void);
extern FILE *trace_file;

#define SOFTPC_FIXED_RAM_BYTES (16ul * 1024ul * 1024ul)
#define SOFTPC_MINIMUM_RAM_BYTES (1024ul * 1024ul)
#define SOFTPC_BOOT_SECTOR_BYTES 512u

static int softpc_machine_floppy_geometry(const char *path,
    unsigned short *sectors_per_track, unsigned short *heads);
static int softpc_machine_hdd_geometry(const char *path,
    unsigned short *sectors_per_track, unsigned short *heads);

struct softpc_machine {
    softpc_machine_options options;
    unsigned short floppy_sectors_per_track;
    unsigned short floppy_heads;
    unsigned short hard_disk_sectors_per_track;
    unsigned short hard_disk_heads;
    unsigned long memory_bytes;
    int reset;
    int hardware_initialized;
    int mouse_driver_initialized;
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

static softpc_machine_result softpc_machine_install_reset_rom(
    const softpc_machine *machine)
{
    unsigned char bda_configuration[6];
    unsigned char bda_fixed_disk_count;
#if 0
    static unsigned char retired_int13_rom[] = {
        0x50u, 0x53u, 0x51u, 0x52u, 0x56u, 0x57u, 0x55u, 0x1eu,
        0x50u, 0x80u, 0xfcu, 0x00u, 0x0fu, 0x84u, 0xa0u, 0x00u,
        0x80u, 0xfcu, 0x08u, 0x0fu, 0x84u, 0xa6u, 0x00u, 0x89u,
        0xdfu, 0x80u, 0xfcu, 0x02u, 0x74u, 0x07u, 0x80u, 0xfcu,
        0x03u, 0x0fu, 0x85u, 0xb5u, 0x00u, 0x08u, 0xc0u, 0x0fu,
        0x84u, 0xafu, 0x00u, 0x3cu, 0x80u, 0x0fu, 0x87u, 0xa9u,
        0x00u, 0x89u, 0xc8u, 0x86u, 0xc4u, 0x80u, 0xe4u, 0xc0u,
        0xc0u, 0xecu, 0x06u, 0x89u, 0xd3u, 0x88u, 0xfbu, 0x30u,
        0xffu, 0x2eu, 0x8bu, 0x36u, 0xe7u, 0x01u, 0x2eu, 0x8bu,
        0x2eu, 0xe9u, 0x01u, 0xf7u, 0xe6u, 0x01u, 0xd8u, 0xf7u,
        0xe5u, 0x89u, 0xcbu, 0x80u, 0xe3u, 0x3fu, 0x4bu, 0x01u,
        0xd8u, 0x89u, 0xc3u, 0xbau, 0xf2u, 0x01u, 0x59u, 0x89u,
        0xc8u, 0xeeu, 0x42u, 0x89u, 0xd8u, 0xeeu, 0x42u, 0x88u,
        0xe0u, 0xeeu, 0x42u, 0x30u, 0xc0u, 0xeeu, 0x42u, 0xb0u,
        0xe0u, 0xeeu, 0x42u, 0x80u, 0xfdu, 0x03u, 0x74u, 0x16u,
        0xb0u, 0x20u, 0xeeu, 0xbau, 0xf0u, 0x01u, 0x30u, 0xedu,
        0x51u, 0xb9u, 0x00u, 0x01u, 0xfcu, 0xedu, 0xabu, 0xe2u,
        0xfcu, 0x59u, 0xe2u, 0xf4u, 0xebu, 0x16u, 0xb0u, 0x30u,
        0xeeu, 0xbau, 0xf0u, 0x01u, 0x30u, 0xedu, 0x06u, 0x1fu,
        0x89u, 0xfeu, 0x51u, 0xb9u, 0x00u, 0x01u, 0xfcu, 0xf3u,
        0x6fu, 0x59u, 0xe2u, 0xf6u, 0x1fu, 0x5du, 0x5fu, 0x5eu,
        0x5au, 0x59u, 0x5bu, 0x58u, 0x30u, 0xe4u, 0xf8u, 0xcfu,
        0x58u, 0x1fu, 0x5du, 0x5fu, 0x5eu, 0x5au, 0x59u, 0x5bu,
        0x58u, 0x30u, 0xe4u, 0xf8u, 0xcfu, 0x58u, 0x1fu, 0x5du,
        0x5fu, 0x5eu, 0x5au, 0x59u, 0x5bu, 0x58u, 0xb5u, 0xffu,
        0x2eu, 0x8au, 0x0eu, 0xe9u, 0x01u, 0x2eu, 0x8au, 0x36u,
        0xe7u, 0x01u, 0xfeu, 0xceu, 0xb2u, 0x01u, 0x30u, 0xe4u,
        0xf8u, 0xcfu, 0x59u, 0x1fu, 0x5du, 0x5fu, 0x5eu, 0x5au,
        0x59u, 0x5bu, 0x58u, 0xb4u, 0x01u, 0xf9u, 0xcfu, 0x10u,
        0x00u, 0x3fu, 0x00u,
    };
#endif
    /* Assembled from firmware/int10_teletype.asm; embedding keeps the normal
       C build independent of an assembler installation. */
    static const unsigned char int10_teletype_rom[] = {
        0x53u, 0x52u, 0x57u, 0x06u, 0x80u, 0xfcu, 0x0eu, 0x74u,
        0x40u, 0x80u, 0xfcu, 0x0fu, 0x74u, 0x0cu, 0x80u, 0xfcu,
        0x02u, 0x74u, 0x0eu, 0x80u, 0xfcu, 0x03u, 0x74u, 0x1fu,
        0xebu, 0x76u, 0xb8u, 0x03u, 0x50u, 0x30u, 0xffu, 0xebu,
        0x6fu, 0x31u, 0xc0u, 0x88u, 0xf0u, 0xb3u, 0x50u, 0xf6u,
        0xe3u, 0x31u, 0xdbu, 0x88u, 0xd3u, 0x01u, 0xd8u, 0xd1u,
        0xe0u, 0x2eu, 0xa3u, 0x95u, 0x02u, 0xebu, 0x59u, 0x2eu,
        0xa1u, 0x95u, 0x02u, 0xd1u, 0xe8u, 0xb3u, 0x50u, 0xf6u,
        0xf3u, 0x88u, 0xc6u, 0x88u, 0xe2u, 0x30u, 0xffu, 0xebu,
        0x47u, 0xbbu, 0x00u, 0xb8u, 0x8eu, 0xc3u, 0x2eu, 0x8bu,
        0x3eu, 0x95u, 0x02u, 0x3cu, 0x0du, 0x74u, 0x0fu, 0x3cu,
        0x0au, 0x74u, 0x1eu, 0xb4u, 0x07u, 0xabu, 0x2eu, 0x83u,
        0x06u, 0x95u, 0x02u, 0x02u, 0xebu, 0x1au, 0x2eu, 0xa1u,
        0x95u, 0x02u, 0x31u, 0xd2u, 0xbbu, 0xa0u, 0x00u, 0xf7u,
        0xf3u, 0xf7u, 0xe3u, 0x2eu, 0xa3u, 0x95u, 0x02u, 0xebu,
        0x17u, 0x2eu, 0x81u, 0x06u, 0x95u, 0x02u, 0xa0u, 0x00u,
        0x2eu, 0x81u, 0x3eu, 0x95u, 0x02u, 0xa0u, 0x0fu, 0x72u,
        0x07u, 0x2eu, 0xc7u, 0x06u, 0x95u, 0x02u, 0x00u, 0x00u,
        0x07u, 0x5fu, 0x5au, 0x5bu, 0xcfu, 0x00u, 0x00u,
    };
    static const unsigned char int10_vector[] = { 0x00u, 0x02u, 0x00u, 0xf0u };
    /* Assembled from firmware/int16_keyboard.asm. */
    static const unsigned char int16_keyboard_rom[] = {
        0x53u, 0x52u, 0x55u, 0x89u, 0xe5u, 0x80u, 0xfcu, 0x00u,
        0x74u, 0x17u, 0x80u, 0xfcu, 0x01u, 0x75u, 0x2du, 0xe4u,
        0x64u, 0xa8u, 0x01u, 0x74u, 0x06u, 0x83u, 0x66u, 0x0au,
        0xbfu, 0xebu, 0x21u, 0x83u, 0x4eu, 0x0au, 0x40u, 0xebu,
        0x1bu, 0xe4u, 0x64u, 0xa8u, 0x01u, 0x74u, 0xfau, 0xe4u,
        0x60u, 0xa8u, 0x80u, 0x75u, 0xf4u, 0x3cu, 0xe0u, 0x74u,
        0xf0u, 0x88u, 0xc4u, 0x31u, 0xdbu, 0x88u, 0xc3u, 0x2eu,
        0x8au, 0x87u, 0x40u, 0x03u, 0x5du, 0x5au, 0x5bu, 0xcfu,
        0x00u, 0x1bu, 0x31u, 0x32u, 0x33u, 0x34u, 0x35u, 0x36u,
        0x37u, 0x38u, 0x39u, 0x30u, 0x2du, 0x3du, 0x08u, 0x09u,
        0x71u, 0x77u, 0x65u, 0x72u, 0x74u, 0x79u, 0x75u, 0x69u,
        0x6fu, 0x70u, 0x5bu, 0x5du, 0x0du, 0x00u, 0x61u, 0x73u,
        0x64u, 0x66u, 0x67u, 0x68u, 0x6au, 0x6bu, 0x6cu, 0x3bu,
        0x60u, 0x00u, 0x5cu, 0x7au, 0x78u, 0x63u, 0x76u, 0x62u,
        0x6eu, 0x6du, 0x2cu, 0x2eu, 0x2fu, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
    };
    static const unsigned char int16_vector[] = { 0x00u, 0x03u, 0x00u, 0xf0u };
    /* INT 15h/AH=88h reports the fixed RAM above the first MiB. */
    static unsigned char int15_memory_rom[] = {
        0x80u, 0xfcu, 0x88u, 0x75u, 0x05u, 0xb8u, 0x00u, 0x3cu,
        0xf8u, 0xcfu, 0xb4u, 0x86u, 0xf9u, 0xcfu
    };
    static const unsigned char int15_vector[] = { 0x00u, 0x08u, 0x00u, 0xf0u };
    /* IRQ1 leaves the controller's queued scan code for INT 16h, acknowledges
       the master PIC, preserves AX, and returns to the interrupted guest. */
    static const unsigned char irq1_rom[] = {
        0x50u, 0xb0u, 0x20u, 0xe6u, 0x20u, 0x58u, 0xcfu
    };
    static const unsigned char irq1_vector[] = { 0x20u, 0x04u, 0x00u, 0xf0u };
    static const unsigned char bootstrap_vector[] = { 0x00u, 0xe7u, 0x00u, 0xf0u };
    {
        unsigned long extended_kib = (machine->memory_bytes -
            SOFTPC_MINIMUM_RAM_BYTES) / 1024ul;
        if (extended_kib > 0xfffful) extended_kib = 0xfffful;
        int15_memory_rom[6] = (unsigned char)extended_kib;
        int15_memory_rom[7] = (unsigned char)(extended_kib >> 8u);
    }
    bda_configuration[0] = machine->options.floppy_path != NULL ? 0x23u : 0x22u;
    bda_configuration[1] = 0u;
    bda_configuration[2] = 0u;
    bda_configuration[3] = 0x80u;
    bda_configuration[4] = 0x02u;
    bda_configuration[5] = 0u;
    bda_fixed_disk_count = machine->options.hard_disk_path != NULL ? 1u : 0u;
    if (!softpc_platform_write_physical(0xf0200u, int10_teletype_rom,
            sizeof(int10_teletype_rom)) ||
        !softpc_platform_write_physical(0x40u, int10_vector,
            sizeof(int10_vector)) ||
        !softpc_platform_write_physical(0xf0300u, int16_keyboard_rom,
            sizeof(int16_keyboard_rom)) ||
        !softpc_platform_write_physical(0x58u, int16_vector,
            sizeof(int16_vector)) ||
        /* BDA equipment at 0040:0010, conventional memory at 0040:0013. */
        !softpc_platform_write_physical(0x410u, bda_configuration,
            sizeof(bda_configuration)) ||
        /* BDA fixed-disk count at 0040:0075. */
        !softpc_platform_write_physical(0x475u, &bda_fixed_disk_count,
            sizeof(bda_fixed_disk_count)) ||
        !softpc_platform_write_physical(0xf0800u, int15_memory_rom,
            sizeof(int15_memory_rom)) ||
        !softpc_platform_write_physical(0x54u, int15_vector,
            sizeof(int15_vector)) ||
        !softpc_platform_write_physical(0xf0420u, irq1_rom,
            sizeof(irq1_rom)) ||
        !softpc_platform_write_physical(0x24u, irq1_vector,
            sizeof(irq1_vector)) ||
        !softpc_platform_write_physical(0x60u, bootstrap_vector,
            sizeof(bootstrap_vector)) ||
        !softpc_platform_write_physical(0x64u, bootstrap_vector,
            sizeof(bootstrap_vector))) {
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
        !softpc_machine_media_exists(options->floppy_path) ||
        !softpc_machine_media_exists(options->hard_disk_path))
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    machine = calloc(1u, sizeof(*machine));
    if (machine == NULL) return SOFTPC_MACHINE_IO_ERROR;
    machine->options = *options;
    machine->memory_bytes = options->memory_bytes == 0u ?
        SOFTPC_FIXED_RAM_BYTES : (unsigned long)options->memory_bytes;
    if (machine->memory_bytes < SOFTPC_MINIMUM_RAM_BYTES) {
        free(machine);
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    }
    machine->floppy_sectors_per_track = 18u;
    machine->floppy_heads = 2u;
    machine->hard_disk_sectors_per_track = 63u;
    machine->hard_disk_heads = 16u;
    if (options->floppy_path != NULL && !softpc_machine_floppy_geometry(
            options->floppy_path, &machine->floppy_sectors_per_track,
            &machine->floppy_heads)) {
        free(machine);
        return SOFTPC_MACHINE_IO_ERROR;
    }
    if (options->hard_disk_path != NULL && !softpc_machine_hdd_geometry(
            options->hard_disk_path, &machine->hard_disk_sectors_per_track,
            &machine->hard_disk_heads)) {
        free(machine);
        return SOFTPC_MACHINE_IO_ERROR;
    }
    *machine_out = machine;
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_reset(softpc_machine *machine)
{
    if (machine == NULL) return SOFTPC_MACHINE_INVALID_ARGUMENT;
    if (!machine->hardware_initialized) {
        sas_init(machine->memory_bytes);
        io_init();
        SWPIC_init_funcptrs();
        ica0_init();
        ica1_init();
        dma_init();
        cmos_init();
        ppi_init();
        SWTMR_init_funcptrs();
        softpc_device_bop_register_machine_services();
        softpc_platform_hdd_init();
        gfi_init();
        fla_init();
        if (!softpc_platform_video_buffers_init())
            return SOFTPC_MACHINE_IO_ERROR;
        if (setup_global_data_ptr() == NULL)
            return SOFTPC_MACHINE_IO_ERROR;
        setup_vga_globals();
        machine->hardware_initialized = 1;
    }
    /* CCPU owns the optional fault trace; the standalone machine owns its
       concrete stream rather than relying on a historical host logger. */
    trace_file = stderr;
    c_cpu_init();
    c_cpu_reset();
    /* Original reset POST clears conventional memory before rebuilding the
       IVT and BIOS data area.  Keep reset semantics independent of allocator
       state and prior guest execution. */
    sas_fills(0u, 0u, 640ul * 1024ul);
    /* Retain reset.c's complete keyboard lifecycle after conventional memory
       is cleared: BIOS ring POST followed by the original AT controller
       POST.  This is controller reset work, not a one-time host setup. */
    softpc_platform_keyboard_reset();
    /* Restore the original BIOS and V7 video ROM images first.  The current
       temporary reset overlay remains only until every original ROM BOP
       service is registered. */
    rom_init();
    softpc_bios_setup_ivt();
    /* Original reset POST establishes both event queues before device POST.
       FLA completes commands through this queue; the standalone host only
       drives the original dispatcher from the CCPU execution loop. */
    q_event_init();
    tic_event_init();
    /* Preserve the original controller lifecycle: its GVI layer installs the
       V7 VGA ports and memory mappings, then the BIOS video state is made
       from the restored firmware.  The host layer only presents its output. */
    soft_reset = machine->reset ? 1 : 0;
    gvi_init(5u);
    video_init();
    time_of_day_init();
    timer_init();
    timer_post();
    printer_init(0);
    printer_post(0);
    com_init(0);
    com_post(0);
    com_init(1);
    com_post(1);
    mouse_init();
    if (!machine->mouse_driver_initialized) {
        mouse_driver_initialisation();
        machine->mouse_driver_initialized = 1;
    }
    /* The original product's BIOS POST programmed the two 8259 PICs after
       their port glue was registered.  A standalone reset owns that hardware
       action directly; it is not a guest-service operation. */
    ica0_post();
    ica1_post();
    dma_post();
    if (!softpc_platform_hdd_attach(machine->options.floppy_path,
            machine->options.hard_disk_path))
        return SOFTPC_MACHINE_IO_ERROR;
    if (!softpc_platform_floppy_attach(machine->options.floppy_path))
        return SOFTPC_MACHINE_IO_ERROR;
    /* Original reset POST initialises the FDC BIOS state before replacing
       INT 13h with the fixed-disk dispatcher (which retains it as INT 40h). */
    diskette_post();
    disk_post();
    {
        softpc_machine_result result = softpc_machine_install_reset_rom(machine);
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
    time_strobe();
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_instruction_pointer(
    const softpc_machine *machine, uint16_t *cs, uint32_t *eip)
{
    if (machine == NULL || cs == NULL || eip == NULL || !machine->reset)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    *cs = (uint16_t)c_getCS();
    *eip = (uint32_t)c_getEIP();
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_instruction_address(
    const softpc_machine *machine, uint32_t *address)
{
    if (machine == NULL || address == NULL || !machine->reset)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    *address = (uint32_t)(c_getCS_BASE() + c_getEIP());
    return SOFTPC_MACHINE_OK;
}

static unsigned short softpc_machine_u16le(const unsigned char *bytes)
{
    return (unsigned short)(bytes[0] | ((unsigned short)bytes[1] << 8u));
}

static int softpc_machine_floppy_geometry(const char *path,
    unsigned short *sectors_per_track, unsigned short *heads)
{
    FILE *file = fopen(path, "rb");
    long bytes;
    if (file == NULL) return 0;
    if (fseek(file, 0, SEEK_END) != 0) { fclose(file); return 0; }
    bytes = ftell(file);
    fclose(file);
    *heads = 2u;
    switch (bytes) {
    case 368640L: *sectors_per_track = 9u; return 1;
    case 737280L: *sectors_per_track = 9u; return 1;
    case 1228800L: *sectors_per_track = 15u; return 1;
    case 1474560L: *sectors_per_track = 18u; return 1;
    case 2949120L: *sectors_per_track = 36u; return 1;
    default: *sectors_per_track = 18u; return 1;
    }
}

static int softpc_machine_hdd_geometry(const char *path,
    unsigned short *sectors_per_track, unsigned short *heads)
{
    unsigned char sector[SOFTPC_BOOT_SECTOR_BYTES];
    FILE *file = fopen(path, "rb");
    unsigned long partition_lba = 0u;
    unsigned int index;
    if (file == NULL) return 0;
    if (fread(sector, 1u, sizeof(sector), file) == sizeof(sector) &&
        sector[510] == 0x55u && sector[511] == 0xaau) {
        for (index = 0u; index < 4u; ++index) {
            const unsigned char *entry = sector + 446u + index * 16u;
            if (entry[4] != 0u) {
                partition_lba = (unsigned long)entry[8] |
                    ((unsigned long)entry[9] << 8u) |
                    ((unsigned long)entry[10] << 16u) |
                    ((unsigned long)entry[11] << 24u);
                break;
            }
        }
        if (partition_lba != 0u && fseek(file,
                (long)(partition_lba * SOFTPC_BOOT_SECTOR_BYTES), SEEK_SET) == 0 &&
            fread(sector, 1u, sizeof(sector), file) == sizeof(sector)) {
            unsigned short sectors = softpc_machine_u16le(sector + 24u);
            unsigned short disk_heads = softpc_machine_u16le(sector + 26u);
            if (sectors != 0u && sectors <= 63u && disk_heads != 0u) {
                *sectors_per_track = sectors;
                *heads = disk_heads;
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    *sectors_per_track = 63u;
    *heads = 16u;
    return 1;
}

void softpc_machine_destroy(softpc_machine *machine)
{
    if (machine != NULL && machine->hardware_initialized) {
        softpc_platform_hdd_detach();
        softpc_platform_floppy_detach();
        if (machine->mouse_driver_initialized)
            mouse_driver_termination();
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
