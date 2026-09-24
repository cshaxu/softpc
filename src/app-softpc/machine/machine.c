#include "lib/types/types_interface.h"
#include "machine.h"
#include "compat/ccpu/lifecycle.h"
#include "compat/ccpu/abi.h"
#include "compat/cvidc/gdp_state.h"
#include "compat/platform.h"
#include "compat/dib_surface.h"
#include "compat/media_snapshot.h"
#include "lib/storage/medium_interface.h"

#include <stdio.h>

/* CCPU's standalone executor entry points.  The wrapper intentionally calls
 * the core directly instead of the historical host shim. */
extern void sas_init(unsigned long size);
extern void sas_term(void);
extern void gfi_init(void);
extern void *setup_global_data_ptr(void);
extern void setup_vga_globals(void);
extern void softpc_ccpu_install_video_vector(void);
extern void reset(void);
extern void (*ica_clear_int_func)(unsigned long adapter, unsigned long line);
extern int soft_reset;
extern unsigned long softpc_ccpu_instruction_budget;
extern int softpc_ccpu_instruction_budget_active;
extern void mouse_send(int delta_x, int delta_y, int left, int right);
extern void time_strobe(void);
extern void host_timer_shutdown(void);
extern void q_event_init(void);
extern void tic_event_init(void);
extern void host_lpt_close_all(void);
extern void host_com_close_all(void);
extern FILE *trace_file;

#define SOFTPC_FIXED_RAM_BYTES (16ul * 1024ul * 1024ul)
#define SOFTPC_MINIMUM_RAM_BYTES (1024ul * 1024ul)
#define SOFTPC_MEDIA_PATH_MAX 1024u

struct softpc_machine {
    softpc_machine_options options;
    unsigned long memory_bytes;
    int reset;
    int hardware_initialized;
    int cpu_initialized;
    char floppy_path[SOFTPC_MEDIA_PATH_MAX];
    char hard_disk_path[SOFTPC_MEDIA_PATH_MAX];
    char serial_output_path[SOFTPC_MEDIA_PATH_MAX];
    char printer_output_path[SOFTPC_MEDIA_PATH_MAX];
};


static int softpc_machine_media_exists(const char *path)
{
    lib_storage_medium *medium = NULL;
    if (path == NULL) return 1;
    if (lib_storage_medium_open(path, LIB_STORAGE_MEDIUM_READONLY, &medium) !=
        LIB_STATUS_OK) return 0;
    lib_storage_medium_destroy(&medium);
    return 1;
}

softpc_machine_result softpc_machine_create(const softpc_machine_options *options,
    softpc_machine **machine_out)
{
    softpc_machine *machine;
    if (machine_out != NULL) *machine_out = NULL;
    if (options == NULL || machine_out == NULL ||
        (options->floppy_path == NULL && options->hard_disk_path == NULL) ||
        options->floppy_mode > LIB_STORAGE_MEDIUM_OVERLAY ||
        options->hard_disk_mode > LIB_STORAGE_MEDIUM_OVERLAY ||
        !softpc_machine_media_exists(options->floppy_path) ||
        !softpc_machine_media_exists(options->hard_disk_path))
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    machine = lib_allocate_zero(1u, sizeof(*machine));
    if (machine == NULL) return SOFTPC_MACHINE_IO_ERROR;
    machine->options = *options;
    if (options->floppy_path != NULL) {
        lib_size length = lib_text_length(options->floppy_path);
        if (length >= sizeof(machine->floppy_path)) {
            lib_release(machine);
            return SOFTPC_MACHINE_INVALID_ARGUMENT;
        }
        lib_memory_copy(machine->floppy_path, options->floppy_path, length + 1u);
        machine->options.floppy_path = machine->floppy_path;
    }
    if (options->hard_disk_path != NULL) {
        lib_size length = lib_text_length(options->hard_disk_path);
        if (length >= sizeof(machine->hard_disk_path)) {
            lib_release(machine);
            return SOFTPC_MACHINE_INVALID_ARGUMENT;
        }
        lib_memory_copy(machine->hard_disk_path, options->hard_disk_path, length + 1u);
        machine->options.hard_disk_path = machine->hard_disk_path;
    }
    if (options->serial_output_path != NULL) {
        lib_size length = lib_text_length(options->serial_output_path);
        if (length >= sizeof(machine->serial_output_path)) {
            lib_release(machine);
            return SOFTPC_MACHINE_INVALID_ARGUMENT;
        }
        lib_memory_copy(machine->serial_output_path, options->serial_output_path,
            length + 1u);
        machine->options.serial_output_path = machine->serial_output_path;
    }
    if (options->printer_output_path != NULL) {
        lib_size length = lib_text_length(options->printer_output_path);
        if (length >= sizeof(machine->printer_output_path)) {
            lib_release(machine);
            return SOFTPC_MACHINE_INVALID_ARGUMENT;
        }
        lib_memory_copy(machine->printer_output_path, options->printer_output_path,
            length + 1u);
        machine->options.printer_output_path = machine->printer_output_path;
    }
    if (!softpc_host_com_set_output_path(0,
        machine->options.serial_output_path) ||
        !softpc_host_lpt_set_output_path(0,
            machine->options.printer_output_path)) {
        lib_release(machine);
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    }
    machine->memory_bytes = options->memory_bytes == 0u ?
        SOFTPC_FIXED_RAM_BYTES : (unsigned long)options->memory_bytes;
    if (machine->memory_bytes < SOFTPC_MINIMUM_RAM_BYTES) {
        lib_release(machine);
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    }
    *machine_out = machine;
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_reset(softpc_machine *machine)
{
    if (machine == NULL) return SOFTPC_MACHINE_INVALID_ARGUMENT;
    softpc_ccpu_lifecycle_clear_exit();
    /* A public cold run starts only after the executor completed the prior
       run. CCPU's private asynchronous-event map is not reset by
       c_cpu_reset(), so old-run bits must not redirect the new reset vector. */
    softpc_ccpu_lifecycle_clear_pending_interrupts();
    if (!machine->hardware_initialized) {
        sas_init(machine->memory_bytes);
        softpc_device_bop_register_machine_services();
        gfi_init();
        if (setup_global_data_ptr() == NULL)
            return SOFTPC_MACHINE_IO_ERROR;
        setup_vga_globals();
        softpc_ccpu_install_video_vector();
        softpc_platform_bind_reset_host_functions();
        /* nt_init_screen() immediately issues the original C-VID dirty
           notification.  Its generated vector and data carrier therefore
           have to exist before the renderer host is initialized. */
        if (!softpc_platform_video_buffers_init())
            return SOFTPC_MACHINE_IO_ERROR;
        /* Startup media is mounted once with the hardware.  A cold reset
           resets guest devices only; it retains the live media objects and
           their overlays, including a later monitor-driven floppy swap. */
        if (!softpc_platform_hdd_attach(machine->options.hard_disk_path,
            machine->options.hard_disk_mode))
            return SOFTPC_MACHINE_IO_ERROR;
        if (!softpc_platform_floppy_attach(machine->floppy_path[0] == '\0' ? NULL :
            machine->floppy_path, machine->options.floppy_mode)) {
            softpc_platform_hdd_detach();
            return SOFTPC_MACHINE_IO_ERROR;
        }
        machine->hardware_initialized = 1;
    }
    /* c_cpu_init creates the CCPU's per-thread simulation-stack facility.
       It is an original machine-lifetime initialization, not a reset action:
       repeating it leaks/replaces the TLS slot on every standalone start.
       The executor that performs this first reset therefore owns the original
       CPU context for the complete machine lifetime; later starts use the
       original c_cpu_reset path only. */
    if (!machine->cpu_initialized) {
        /* CCPU owns the optional fault trace; the standalone machine owns its
           concrete stream rather than relying on a historical host logger. */
        trace_file = stderr;
        c_cpu_init();
        machine->cpu_initialized = 1;
    }
    c_cpu_reset();
    /* The original non-NT reset path creates queues only on a soft reset.
       A standalone first boot needs them before its original FDC POST. */
    q_event_init();
    tic_event_init();
    /* This public standalone operation is the monitor's cold-start boundary,
       not the guest's hardware warm-reset line.  Reusing `machine` after a
       stopped run must therefore repeat the original cold initialisation
       (FDC, mouse and fixed-disk setup included).  Guest Ctrl+Alt+Del still
       reaches CCPU's independent CPU reset path and retains its original
       warm-reset semantics. */
    soft_reset = 0;
    softpc_platform_set_boot_clock(1);
    reset();
    /* The original reset recreates the keyboard controller, but an IRQ1 that
       was already asserted by the completed standalone run is an interrupt
       line, not controller state.  Drop that line at the cold-run boundary;
       the new controller will assert it again only for new input. */
    if (ica_clear_int_func != NULL)
        ica_clear_int_func(0u, 1u);
    softpc_platform_keyboard_discard_stale_output();
    /* reset() can service the old PIC line while rebuilding devices. Clear
       the CCPU delivery map after that final line drop, rather than only
       before reset(), so no old IRQ1 reaches the new boot image. */
    softpc_ccpu_lifecycle_clear_pending_interrupts();
    softpc_platform_set_boot_clock(0);
    if (!softpc_platform_executor_ready()) return SOFTPC_MACHINE_IO_ERROR;
    machine->reset = 1;
    return SOFTPC_MACHINE_OK;
}

lib_u32 softpc_machine_memory_bytes(const softpc_machine *machine)
{
    if (machine == NULL || machine->memory_bytes > UINT32_MAX) return 0u;
    return (lib_u32)machine->memory_bytes;
}

lib_status softpc_machine_prepare_media(softpc_machine *machine,
    softpc_media_archive *archive)
{
    const char *floppy_path;
    const char *hard_disk_path;
    lib_storage_medium_mode floppy_mode;
    lib_storage_medium_mode hard_disk_mode;
    lib_status status;
    if (machine == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_media_archive_prepare(archive);
    if (status != LIB_STATUS_OK) return status;
    status = softpc_media_archive_attachment(archive, 0u, &floppy_path,
        &floppy_mode);
    if (status == LIB_STATUS_OK)
        status = softpc_media_archive_attachment(archive, 2u, &hard_disk_path,
            &hard_disk_mode);
    if (status != LIB_STATUS_OK ||
        (floppy_path != NULL && lib_text_length(floppy_path) >= sizeof(machine->floppy_path)) ||
        (hard_disk_path != NULL && lib_text_length(hard_disk_path) >= sizeof(machine->hard_disk_path)))
        return status == LIB_STATUS_OK ? LIB_STATUS_LIMIT_EXCEEDED : status;
    if (floppy_path == NULL) machine->floppy_path[0] = '\0';
    else lib_memory_copy(machine->floppy_path, floppy_path, lib_text_length(floppy_path) + 1u);
    if (hard_disk_path == NULL) machine->hard_disk_path[0] = '\0';
    else lib_memory_copy(machine->hard_disk_path, hard_disk_path, lib_text_length(hard_disk_path) + 1u);
    machine->options.floppy_path = machine->floppy_path[0] == '\0' ? NULL :
        machine->floppy_path;
    machine->options.hard_disk_path = machine->hard_disk_path[0] == '\0' ? NULL :
        machine->hard_disk_path;
    machine->options.floppy_mode = floppy_mode;
    machine->options.hard_disk_mode = hard_disk_mode;
    return LIB_STATUS_OK;
}

softpc_machine_result softpc_machine_key_scancode(softpc_machine *machine,
    lib_u8 scan_code)
{
    if (machine == NULL || !machine->reset)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return softpc_platform_keyboard_scancode((unsigned char)scan_code) ?
        SOFTPC_MACHINE_OK : SOFTPC_MACHINE_IO_ERROR;
}

softpc_machine_result softpc_machine_key_number(softpc_machine *machine,
    lib_u8 key_number, lib_u8 released)
{
    if (machine == NULL || !machine->reset || key_number == 0u)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return softpc_platform_keyboard_key((int)key_number, released != 0u) ?
        SOFTPC_MACHINE_OK : SOFTPC_MACHINE_IO_ERROR;
}

softpc_machine_result softpc_machine_mouse_input(softpc_machine *machine,
    lib_i32 delta_x, lib_i32 delta_y, lib_u8 left_down, lib_u8 right_down)
{
    if (machine == NULL || !machine->reset)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    mouse_send((int)delta_x, (int)delta_y, left_down != 0u, right_down != 0u);
    /* Windows' V7 driver can update its software cursor through its virtual
       display path without an ordinary mapped VGA write.  Ask the imported
       renderer for its own complete refresh after a real InPort event. */
    softpc_platform_presentation_request_refresh();
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_set_floppy(softpc_machine *machine,
    const char *path, lib_storage_medium_mode mode)
{
    lib_size length;
    if (machine == NULL || mode > LIB_STORAGE_MEDIUM_OVERLAY)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    if (path != NULL) {
        length = lib_text_length(path);
        if (length >= sizeof(machine->floppy_path) ||
            (!machine->hardware_initialized && !softpc_machine_media_exists(path)))
            return SOFTPC_MACHINE_INVALID_ARGUMENT;
    }
    if (machine->hardware_initialized &&
        !softpc_platform_floppy_attach(path, mode))
        return SOFTPC_MACHINE_IO_ERROR;
    if (path == NULL) machine->floppy_path[0] = '\0';
    else {
        lib_memory_copy(machine->floppy_path, path, length + 1u);
    }
    machine->options.floppy_path = machine->floppy_path[0] == '\0' ?
        NULL : machine->floppy_path;
    if (path != NULL) machine->options.floppy_mode = mode;
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_read_physical(const softpc_machine *machine,
    lib_u32 address, void *buffer, lib_u32 bytes)
{
    if (machine == NULL || buffer == NULL || !machine->reset || bytes == 0u)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return softpc_platform_read_physical((unsigned long)address,
        (unsigned char *)buffer, (unsigned long)bytes) ?
        SOFTPC_MACHINE_OK : SOFTPC_MACHINE_INVALID_ARGUMENT;
}

softpc_machine_result softpc_machine_write_physical(softpc_machine *machine,
    lib_u32 address, const void *buffer, lib_u32 bytes)
{
    if (machine == NULL || buffer == NULL || !machine->reset || bytes == 0u)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return softpc_platform_write_physical((unsigned long)address,
        (const unsigned char *)buffer, (unsigned long)bytes) ?
        SOFTPC_MACHINE_OK : SOFTPC_MACHINE_INVALID_ARGUMENT;
}

softpc_machine_result softpc_machine_run(softpc_machine *machine,
    lib_u64 instruction_budget)
{
    if (machine == NULL || !machine->reset || instruction_budget == 0u)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    if (!softpc_platform_executor_ready()) return SOFTPC_MACHINE_IO_ERROR;
    /* CCPU's restored inter-instruction counter is 32-bit.  UINT64_MAX is
       the public API's continuous-execution sentinel: do not truncate it to
       0xffffffff and accidentally turn a VM run into a roughly-20-second
       slice.  Finite callers (smokes and the public slicing API) retain the
       original generated safe-point budget. */
    if (instruction_budget == UINT64_MAX) {
        softpc_ccpu_instruction_budget = 0u;
        softpc_ccpu_instruction_budget_active = 0;
    } else {
        softpc_ccpu_instruction_budget = (unsigned long)instruction_budget;
        /* The original timer subsystem starts the standalone host timer during
           reset.  Do not manufacture a second timer tick at every KVM slice. */
        softpc_ccpu_instruction_budget_active = 1;
    }
    c_cpu_simulate();
    softpc_ccpu_instruction_budget_active = 0;
    softpc_ccpu_lifecycle_clear_exit();
    return softpc_platform_executor_ready() ? SOFTPC_MACHINE_OK :
        SOFTPC_MACHINE_IO_ERROR;
}

void softpc_machine_request_wake(softpc_machine *machine)
{
    if (machine != NULL && machine->reset)
        softpc_platform_request_executor_wake();
}

void softpc_machine_request_stop(softpc_machine *machine)
{
    if (machine != NULL && machine->reset) {
        softpc_ccpu_lifecycle_request_exit();
        softpc_platform_request_executor_wake();
    }
}

void softpc_machine_executor_thread_enter(softpc_machine *machine)
{
    if (machine != NULL && machine->reset)
        ccpu386newthread();
}

void softpc_machine_executor_thread_leave(softpc_machine *machine)
{
    if (machine != NULL && machine->reset)
        ccpu386exitthread();
}

void softpc_machine_set_heartbeat(softpc_machine *machine, int enabled)
{
    if (machine == NULL || !machine->reset) return;
    softpc_platform_set_runtime_heartbeat(enabled);
}

void softpc_machine_set_executor_callback(softpc_machine *machine,
    softpc_machine_executor_callback callback, void *context)
{
    if (machine == NULL) return;
    softpc_platform_set_executor_callback((void (*)(void *))callback, context);
}

softpc_machine_result softpc_machine_instruction_pointer(
    const softpc_machine *machine, lib_u16 *cs, lib_u32 *eip)
{
    if (machine == NULL || cs == NULL || eip == NULL || !machine->reset)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    *cs = (lib_u16)c_getCS();
    *eip = (lib_u32)c_getEIP();
    return SOFTPC_MACHINE_OK;
}

softpc_machine_result softpc_machine_instruction_address(
    const softpc_machine *machine, lib_u32 *address)
{
    if (machine == NULL || address == NULL || !machine->reset)
        return SOFTPC_MACHINE_INVALID_ARGUMENT;
    *address = (lib_u32)(c_getCS_BASE() + c_getEIP());
    return SOFTPC_MACHINE_OK;
}

int softpc_machine_presentation_is_graphics(const softpc_machine *machine)
{
    return machine != NULL && machine->reset &&
        softpc_platform_presentation_is_graphics();
}

int softpc_machine_presentation_state(const softpc_machine *machine,
    lib_u32 *mode_type_out, lib_u32 *screen_state_out)
{
    if (machine == NULL || !machine->reset || mode_type_out == NULL ||
        screen_state_out == NULL) return 0;
    return softpc_platform_presentation_state(mode_type_out, screen_state_out);
}

int softpc_machine_presentation_take_dirty(const softpc_machine *machine,
    lib_i32 *left, lib_i32 *top, lib_i32 *right, lib_i32 *bottom)
{
    long native_left;
    long native_top;
    long native_right;
    long native_bottom;
    if (machine == NULL || !machine->reset || left == NULL || top == NULL ||
        right == NULL || bottom == NULL || !softpc_standalone_dib_take_dirty(
            &native_left, &native_top, &native_right, &native_bottom)) return 0;
    *left = (lib_i32)native_left;
    *top = (lib_i32)native_top;
    *right = (lib_i32)native_right;
    *bottom = (lib_i32)native_bottom;
    return 1;
}

int softpc_machine_presentation_dib(const softpc_machine *machine,
    const void **bits_out, const void **info_out, lib_u32 *width_out,
    lib_u32 *height_out)
{
    unsigned long width;
    unsigned long height;
    if (machine == NULL || bits_out == NULL || info_out == NULL ||
        width_out == NULL || height_out == NULL ||
        !softpc_standalone_dib_surface(bits_out, info_out, &width, &height))
        return 0;
    *width_out = (lib_u32)width;
    *height_out = (lib_u32)height;
    return 1;
}

int softpc_machine_presentation_text(const softpc_machine *machine,
    const void **cells_out, lib_u32 *columns_out, lib_u32 *rows_out,
    lib_u32 *stride_out, lib_u32 *cell_bytes_out)
{
    unsigned long columns;
    unsigned long rows;
    unsigned long stride;
    unsigned long cell_bytes;
    if (machine == NULL || !machine->reset || cells_out == NULL ||
        columns_out == NULL || rows_out == NULL || stride_out == NULL ||
        cell_bytes_out == NULL || !softpc_standalone_text_surface(cells_out,
            &columns, &rows, &stride, &cell_bytes) ||
        !softpc_platform_presentation_text_extent(&columns, &rows)) return 0;
    *columns_out = (lib_u32)columns;
    *rows_out = (lib_u32)rows;
    *stride_out = (lib_u32)stride;
    *cell_bytes_out = (lib_u32)cell_bytes;
    return 1;
}

int softpc_machine_presentation_cursor(const softpc_machine *machine,
    lib_i32 *column_out, lib_i32 *row_out, lib_u32 *size_out)
{
    long column;
    long row;
    unsigned long size;
    if (machine == NULL || !machine->reset || column_out == NULL ||
        row_out == NULL || size_out == NULL ||
        !softpc_platform_presentation_cursor(&column, &row, &size)) return 0;
    *column_out = (lib_i32)column;
    *row_out = (lib_i32)row;
    *size_out = (lib_u32)size;
    return 1;
}

int softpc_machine_presentation_fonts(const softpc_machine *machine,
    lib_u8 primary[256u * 16u], lib_u8 secondary[256u * 16u],
    lib_u32 *height_out, lib_u32 *attribute_select_out)
{
    unsigned long height;
    unsigned long attribute_select;

    if (machine == NULL || !machine->reset || primary == NULL ||
        secondary == NULL || height_out == NULL || attribute_select_out == NULL ||
        !softpc_platform_presentation_fonts(primary, secondary, &height,
            &attribute_select)) return 0;
    *height_out = (lib_u32)height;
    *attribute_select_out = (lib_u32)attribute_select;
    return 1;
}

void softpc_machine_destroy(softpc_machine *machine)
{
    if (machine != NULL && machine->hardware_initialized) {
        softpc_platform_set_runtime_heartbeat(0);
        softpc_platform_set_executor_callback(NULL, NULL);
        host_timer_shutdown();
        host_lpt_close_all();
        host_com_close_all();
        softpc_platform_hdd_detach();
        softpc_platform_floppy_detach();
        if (machine->cpu_initialized)
            c_cpu_terminate();
        softpc_gdp_destroy_global();
        sas_term();
    }
    lib_release(machine);
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
