#include "softpc_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* CCPU's standalone executor entry points.  The wrapper intentionally calls
 * the core directly instead of the historical host shim. */
extern void c_cpu_init(void);
extern void c_cpu_reset(void);
extern void c_cpu_simulate(void);
extern void c_cpu_terminate(void);
extern void a3_cpu_interrupt(int errupt, unsigned short number);
extern unsigned short c_getCS(void);
extern unsigned long c_getCS_BASE(void);
extern unsigned long c_getEIP(void);
extern void sas_init(unsigned long size);
extern void sas_term(void);
extern void gfi_init(void);
extern void *setup_global_data_ptr(void);
extern void softpc_gdp_destroy_global(void);
extern void setup_vga_globals(void);
extern void softpc_ccpu_install_video_vector(void);
extern void reset(void);
extern int soft_reset;
extern unsigned long softpc_ccpu_instruction_budget;
extern int softpc_ccpu_instruction_budget_active;
extern int softpc_platform_write_physical(unsigned long address,
    const unsigned char *bytes, unsigned long length);
extern int softpc_platform_read_physical(unsigned long address,
    unsigned char *bytes, unsigned long length);
extern void softpc_device_bop_register_machine_services(void);
extern int softpc_platform_keyboard_scancode(unsigned char scan_code);
extern int softpc_platform_keyboard_key(int key, int released);
extern void mouse_send(int delta_x, int delta_y, int left, int right);
extern void time_strobe(void);
extern void host_timer_shutdown(void);
extern void softpc_platform_set_boot_clock(int active);
extern void q_event_init(void);
extern void tic_event_init(void);
extern void mouse_driver_initialisation(void);
extern void mouse_driver_termination(void);
extern void host_lpt_close_all(void);
extern void host_com_close_all(void);
extern int softpc_platform_hdd_attach(const char *hard_disk_path,
    softpc_media_mode mode);
extern void softpc_platform_hdd_detach(void);
extern int softpc_platform_floppy_attach(const char *path, softpc_media_mode mode);
extern void softpc_platform_floppy_detach(void);
extern int softpc_platform_video_buffers_init(void);
extern int softpc_standalone_dib_take_dirty(long *left, long *top,
    long *right, long *bottom);
extern FILE *trace_file;

#define SOFTPC_FIXED_RAM_BYTES (16ul * 1024ul * 1024ul)
#define SOFTPC_MINIMUM_RAM_BYTES (1024ul * 1024ul)
#define SOFTPC_MEDIA_PATH_MAX 1024u

struct softpc_machine {
    softpc_machine_options options;
    unsigned long memory_bytes;
    int reset;
    int hardware_initialized;
    int mouse_driver_initialized;
    char floppy_path[SOFTPC_MEDIA_PATH_MAX];
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
        options->media_mode > SOFTPC_MEDIA_OVERLAY ||
        !softpc_machine_media_exists(options->floppy_path) ||
        !softpc_machine_media_exists(options->hard_disk_path))
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    machine = calloc(1u, sizeof(*machine));
    if (machine == NULL) return SOFTPC_MACHINE_IO_ERROR;
    machine->options = *options;
    if (options->floppy_path != NULL) {
        size_t length = strlen(options->floppy_path);
        if (length >= sizeof(machine->floppy_path)) {
            free(machine);
            return SOFTPC_MACHINE_INVALID_ARGUMENT;
        }
        memcpy(machine->floppy_path, options->floppy_path, length + 1u);
        machine->options.floppy_path = machine->floppy_path;
    }
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
    if (!softpc_platform_hdd_attach(machine->options.hard_disk_path,
        machine->options.media_mode))
        return SOFTPC_MACHINE_IO_ERROR;
    if (!softpc_platform_floppy_attach(machine->options.floppy_path,
        machine->options.media_mode))
        return SOFTPC_MACHINE_IO_ERROR;
    soft_reset = machine->reset ? 1 : 0;
    softpc_platform_set_boot_clock(1);
    reset();
    softpc_platform_set_boot_clock(0);
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

softpc_machine_result softpc_machine_key_number(softpc_machine *machine,
    uint8_t key_number, uint8_t released)
{
    if (machine == NULL || !machine->reset || key_number == 0u)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return softpc_platform_keyboard_key((int)key_number, released != 0u) ?
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

softpc_machine_result softpc_machine_set_floppy(softpc_machine *machine,
    const char *path)
{
    size_t length;
    if (machine == NULL) return SOFTPC_MACHINE_INVALID_ARGUMENT;
    if (path != NULL) {
        length = strlen(path);
        if (length >= sizeof(machine->floppy_path) ||
            !softpc_machine_media_exists(path))
            return SOFTPC_MACHINE_INVALID_ARGUMENT;
    }
    if (!machine->hardware_initialized) {
        if (path == NULL) machine->floppy_path[0] = '\0';
        else {
            memcpy(machine->floppy_path, path, length + 1u);
        }
        machine->options.floppy_path = machine->floppy_path[0] == '\0' ?
            NULL : machine->floppy_path;
        return SOFTPC_MACHINE_OK;
    }
    if (!softpc_platform_floppy_attach(path, machine->options.media_mode))
        return SOFTPC_MACHINE_IO_ERROR;
    if (path == NULL) machine->floppy_path[0] = '\0';
    else {
        memcpy(machine->floppy_path, path, length + 1u);
    }
    machine->options.floppy_path = machine->floppy_path[0] == '\0' ?
        NULL : machine->floppy_path;
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

softpc_machine_result softpc_machine_write_physical(softpc_machine *machine,
    uint32_t address, const void *buffer, uint32_t bytes)
{
    if (machine == NULL || buffer == NULL || !machine->reset || bytes == 0u)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return softpc_platform_write_physical((unsigned long)address,
        (const unsigned char *)buffer, (unsigned long)bytes) ?
        SOFTPC_MACHINE_OK : SOFTPC_MACHINE_INVALID_ARGUMENT;
}

softpc_machine_result softpc_machine_run(softpc_machine *machine,
    uint64_t instruction_budget)
{
    if (machine == NULL || !machine->reset || instruction_budget == 0u)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    softpc_ccpu_instruction_budget = (unsigned long)instruction_budget;
    /* The original CCPU consumes CPU_TIMER_TICK at an instruction boundary.
       A standalone machine owns that heartbeat at the public slice boundary,
       rather than using the historical NTVDM timer thread.  Nested original
       host_simulate() calls retain their own BOP-FE return path in CCPU. */
    a3_cpu_interrupt(1, 0u);
    softpc_ccpu_instruction_budget_active = 1;
    c_cpu_simulate();
    softpc_ccpu_instruction_budget_active = 0;
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

int softpc_machine_presentation_is_graphics(const softpc_machine *machine)
{
    extern int softpc_platform_presentation_is_graphics(void);
    return machine != NULL && machine->reset &&
        softpc_platform_presentation_is_graphics();
}

int softpc_machine_presentation_take_dirty(const softpc_machine *machine,
    int32_t *left, int32_t *top, int32_t *right, int32_t *bottom)
{
    long native_left;
    long native_top;
    long native_right;
    long native_bottom;
    if (machine == NULL || !machine->reset || left == NULL || top == NULL ||
        right == NULL || bottom == NULL || !softpc_standalone_dib_take_dirty(
            &native_left, &native_top, &native_right, &native_bottom)) return 0;
    *left = (int32_t)native_left;
    *top = (int32_t)native_top;
    *right = (int32_t)native_right;
    *bottom = (int32_t)native_bottom;
    return 1;
}

int softpc_machine_presentation_dib(const softpc_machine *machine,
    const void **bits_out, const void **info_out, uint32_t *width_out,
    uint32_t *height_out)
{
    unsigned long width;
    unsigned long height;
    extern int softpc_standalone_dib_surface(const void **, const void **,
        unsigned long *, unsigned long *);
    if (machine == NULL || bits_out == NULL || info_out == NULL ||
        width_out == NULL || height_out == NULL ||
        !softpc_standalone_dib_surface(bits_out, info_out, &width, &height))
        return 0;
    *width_out = (uint32_t)width;
    *height_out = (uint32_t)height;
    return 1;
}

int softpc_machine_presentation_text(const softpc_machine *machine,
    const void **cells_out, uint32_t *columns_out, uint32_t *rows_out,
    uint32_t *stride_out, uint32_t *cell_bytes_out)
{
    unsigned long columns;
    unsigned long rows;
    unsigned long stride;
    unsigned long cell_bytes;
    extern int softpc_standalone_text_surface(const void **, unsigned long *,
        unsigned long *, unsigned long *, unsigned long *);
    if (machine == NULL || !machine->reset || cells_out == NULL ||
        columns_out == NULL || rows_out == NULL || stride_out == NULL ||
        cell_bytes_out == NULL || !softpc_standalone_text_surface(cells_out,
            &columns, &rows, &stride, &cell_bytes)) return 0;
    *columns_out = (uint32_t)columns;
    *rows_out = (uint32_t)rows;
    *stride_out = (uint32_t)stride;
    *cell_bytes_out = (uint32_t)cell_bytes;
    return 1;
}

int softpc_machine_presentation_cursor(const softpc_machine *machine,
    int32_t *column_out, int32_t *row_out)
{
    long column;
    long row;
    extern int softpc_platform_presentation_cursor(long *, long *);
    if (machine == NULL || !machine->reset || column_out == NULL ||
        row_out == NULL || !softpc_platform_presentation_cursor(&column,
            &row)) return 0;
    *column_out = (int32_t)column;
    *row_out = (int32_t)row;
    return 1;
}

void softpc_machine_destroy(softpc_machine *machine)
{
    if (machine != NULL && machine->hardware_initialized) {
        host_timer_shutdown();
        host_lpt_close_all();
        host_com_close_all();
        softpc_platform_hdd_detach();
        softpc_platform_floppy_detach();
        if (machine->mouse_driver_initialized)
            mouse_driver_termination();
        c_cpu_terminate();
        softpc_gdp_destroy_global();
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
