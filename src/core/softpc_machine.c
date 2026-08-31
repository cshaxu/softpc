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
extern void gfi_init(void);
extern void *setup_global_data_ptr(void);
extern void setup_vga_globals(void);
extern void softpc_ccpu_install_video_vector(void);
extern void reset(void);
extern int soft_reset;
extern unsigned long softpc_ccpu_instruction_budget;
extern int softpc_platform_write_physical(unsigned long address,
    const unsigned char *bytes, unsigned long length);
extern int softpc_platform_read_physical(unsigned long address,
    unsigned char *bytes, unsigned long length);
extern void softpc_device_bop_register_machine_services(void);
extern void softpc_device_bop_set_memory_size(unsigned long memory_bytes);
extern int softpc_platform_keyboard_scancode(unsigned char scan_code);
extern void mouse_send(int delta_x, int delta_y, int left, int right);
extern void time_strobe(void);
extern void host_timer_shutdown(void);
extern void q_event_init(void);
extern void tic_event_init(void);
extern void mouse_driver_initialisation(void);
extern void mouse_driver_termination(void);
extern int softpc_platform_hdd_attach(const char *hard_disk_path);
extern void softpc_platform_hdd_detach(void);
extern int softpc_platform_floppy_attach(const char *path);
extern void softpc_platform_floppy_detach(void);
extern int softpc_platform_video_buffers_init(void);
extern int softpc_platform_vga_mode13_frame(unsigned long *pixels,
    unsigned long pixel_count);
extern int softpc_platform_vga_mode13_active(void);
extern FILE *trace_file;

#define SOFTPC_FIXED_RAM_BYTES (16ul * 1024ul * 1024ul)
#define SOFTPC_MINIMUM_RAM_BYTES (1024ul * 1024ul)

struct softpc_machine {
    softpc_machine_options options;
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
    *machine_out = machine;
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_reset(softpc_machine *machine)
{
    if (machine == NULL) return SOFTPC_MACHINE_INVALID_ARGUMENT;
    if (!machine->hardware_initialized) {
        sas_init(machine->memory_bytes);
        softpc_device_bop_register_machine_services();
        softpc_device_bop_set_memory_size(machine->memory_bytes);
        gfi_init();
        if (!softpc_platform_video_buffers_init())
            return SOFTPC_MACHINE_IO_ERROR;
        if (setup_global_data_ptr() == NULL)
            return SOFTPC_MACHINE_IO_ERROR;
        setup_vga_globals();
        softpc_ccpu_install_video_vector();
        machine->hardware_initialized = 1;
    }
    /* CCPU owns the optional fault trace; the standalone machine owns its
       concrete stream rather than relying on a historical host logger. */
    trace_file = stderr;
    c_cpu_init();
    c_cpu_reset();
    /* The original non-NT reset path creates queues only on a soft reset.
       A standalone first boot needs them before its original FDC POST. */
    q_event_init();
    tic_event_init();
    /* The media has to exist before original CMOS, FDC and fixed-disk POST
       query their respective configuration and host controller hooks. */
    if (!softpc_platform_hdd_attach(machine->options.hard_disk_path))
        return SOFTPC_MACHINE_IO_ERROR;
    if (!softpc_platform_floppy_attach(machine->options.floppy_path))
        return SOFTPC_MACHINE_IO_ERROR;
    soft_reset = machine->reset ? 1 : 0;
    reset();
    if (!machine->mouse_driver_initialized) {
        mouse_driver_initialisation();
        machine->mouse_driver_initialized = 1;
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

softpc_machine_result softpc_machine_mouse_input(softpc_machine *machine,
    int32_t delta_x, int32_t delta_y, uint8_t left_down, uint8_t right_down)
{
    if (machine == NULL || !machine->reset)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    mouse_send((int)delta_x, (int)delta_y, left_down != 0u, right_down != 0u);
    return SOFTPC_MACHINE_OK;
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

int softpc_machine_vga_mode13_active(const softpc_machine *machine)
{
    return machine != NULL && machine->reset &&
        softpc_platform_vga_mode13_active();
}

softpc_machine_result softpc_machine_vga_mode13_frame(
    const softpc_machine *machine, uint32_t *pixels, uint32_t pixel_count)
{
    if (machine == NULL || pixels == NULL || !machine->reset)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return softpc_platform_vga_mode13_frame((unsigned long *)pixels,
        (unsigned long)pixel_count) ? SOFTPC_MACHINE_OK :
        SOFTPC_MACHINE_BACKEND_UNAVAILABLE;
}

void softpc_machine_destroy(softpc_machine *machine)
{
    if (machine != NULL && machine->hardware_initialized) {
        host_timer_shutdown();
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
