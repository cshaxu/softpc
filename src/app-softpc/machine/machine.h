#include "../../lib/types/types_interface.h"
#ifndef SOFTPC_MACHINE_H
#define SOFTPC_MACHINE_H

#include "lib/storage/medium_interface.h"

typedef struct softpc_machine softpc_machine;
typedef struct softpc_media_archive softpc_media_archive;

lib_status softpc_machine_prepare_media(softpc_machine *machine,
    softpc_media_archive *archive);
typedef void (*softpc_machine_executor_callback)(void *context);

typedef enum softpc_machine_result {
    SOFTPC_MACHINE_OK,
    SOFTPC_MACHINE_INVALID_ARGUMENT,
    SOFTPC_MACHINE_IO_ERROR,
    SOFTPC_MACHINE_BACKEND_UNAVAILABLE
} softpc_machine_result;

typedef struct softpc_machine_options {
    const char *floppy_path;
    const char *hard_disk_path;
    lib_u32 memory_bytes;
    lib_storage_medium_mode floppy_mode;
    lib_storage_medium_mode hard_disk_mode;
    /* Optional standalone host endpoints for the original COM1/LPT1 host
       contracts. NULL retains their bounded virtual sinks. */
    const char *serial_output_path;
    const char *printer_output_path;
} softpc_machine_options;

softpc_machine_result softpc_machine_create(const softpc_machine_options *options,
    softpc_machine **machine_out);
softpc_machine_result softpc_machine_reset(softpc_machine *machine);
lib_u32 softpc_machine_memory_bytes(const softpc_machine *machine);
softpc_machine_result softpc_machine_run(softpc_machine *machine,
    lib_u64 instruction_budget);
softpc_machine_result softpc_machine_read_physical(const softpc_machine *machine,
    lib_u32 address, void *buffer, lib_u32 bytes);
/* Copy bytes into guest physical RAM through the machine boundary.  This is
 * intended for firmware/media integration and test fixtures, not a host RAM
 * alias; ROM and out-of-range writes are rejected by the machine. */
softpc_machine_result softpc_machine_write_physical(softpc_machine *machine,
    lib_u32 address, const void *buffer, lib_u32 bytes);
softpc_machine_result softpc_machine_instruction_pointer(
    const softpc_machine *machine, lib_u16 *cs, lib_u32 *eip);
softpc_machine_result softpc_machine_instruction_address(
    const softpc_machine *machine, lib_u32 *address);
softpc_machine_result softpc_machine_key_scancode(softpc_machine *machine,
    lib_u8 scan_code);
/* Deliver an original SoftPC keyboard key number after host mapping. */
softpc_machine_result softpc_machine_key_number(softpc_machine *machine,
    lib_u8 key_number, lib_u8 released);
/* Wake an executor that is halted in guest idle without manufacturing a
 * timer/device tick. This is safe to call from a frontend input thread. */
void softpc_machine_request_wake(softpc_machine *machine);
/* Request an orderly return from the outer original CCPU invocation.  The
 * request is observed only at the original timer rendezvous, never by an
 * asynchronous host thread or a nested host_simulate frame. */
void softpc_machine_request_stop(softpc_machine *machine);
/* Register the single VM executor with the original CCPU TLS simulation
 * stack. These calls belong to that executor's lifetime, never to a KVM
 * thread. */
void softpc_machine_executor_thread_enter(softpc_machine *machine);
void softpc_machine_executor_thread_leave(softpc_machine *machine);
/* The standalone runtime owns the original host heartbeat.  Legacy bounded
 * test calls keep it disabled; a continuous executor enables it only after
 * reset has returned to the public machine boundary. */
void softpc_machine_set_heartbeat(softpc_machine *machine, int enabled);
/* Called by the original CCPU host-timer event on the executor thread.  The
 * callback must copy/consume host records only; it must not retain machine
 * surfaces or invoke nested execution. */
void softpc_machine_set_executor_callback(softpc_machine *machine,
    softpc_machine_executor_callback callback, void *context);
/* Inject relative host-pointer movement into the original Microsoft Bus
 * Mouse adapter. Button values are zero (up) or nonzero (down). */
softpc_machine_result softpc_machine_mouse_input(softpc_machine *machine,
    lib_i32 delta_x, lib_i32 delta_y, lib_u8 left_down, lib_u8 right_down);

/* Replace drive A's removable medium through the original GFI/FDC host-media
 * boundary. Passing NULL ejects it. Callers must stop or pause execution
 * before changing media. */
softpc_machine_result softpc_machine_set_floppy(softpc_machine *machine,
    const char *path, lib_storage_medium_mode mode);
int softpc_machine_presentation_is_graphics(const softpc_machine *machine);
/* Read-only original renderer state for opt-in standalone diagnostics. */
int softpc_machine_presentation_state(const softpc_machine *machine,
    lib_u32 *mode_type_out, lib_u32 *screen_state_out);

/* Consume the original host renderer's pending dirty rectangle. */
int softpc_machine_presentation_take_dirty(const softpc_machine *machine,
    lib_i32 *left, lib_i32 *top, lib_i32 *right, lib_i32 *bottom);

/* Borrow the original host renderer's indexed DIB.  The caller must not
 * retain the pointers after machine destruction or mutate either surface. */
int softpc_machine_presentation_dib(const softpc_machine *machine,
    const void **bits_out, const void **info_out, lib_u32 *width_out,
    lib_u32 *height_out);

/* Borrow the original nt_cga text-presenter surface.  Cells are four bytes
 * wide in this standalone build: the character and attribute occupy the
 * first two bytes, followed by the original renderer's padding.  The caller
 * must not mutate the surface or retain it after machine destruction. */
int softpc_machine_presentation_text(const softpc_machine *machine,
    const void **cells_out, lib_u32 *columns_out, lib_u32 *rows_out,
    lib_u32 *stride_out, lib_u32 *cell_bytes_out);

/* Read the most recent text-cursor position emitted by the original video
 * controller. The standalone host records this callback; frontends own the
 * actual cursor drawing. */
int softpc_machine_presentation_cursor(const softpc_machine *machine,
    lib_i32 *column_out, lib_i32 *row_out, lib_u32 *size_out);

/* The original VGA attribute controller may select a second character map
   through attribute bit 3.  Presentation consumers need both loaded maps;
   they must not substitute a host font for either one. */
int softpc_machine_presentation_fonts(const softpc_machine *machine,
    lib_u8 primary[256u * 16u], lib_u8 secondary[256u * 16u],
    lib_u32 *height_out, lib_u32 *attribute_select_out);

void softpc_machine_destroy(softpc_machine *machine);

const char *softpc_machine_result_name(softpc_machine_result result);

/* Paused-executor debugger preflight; no partial write on invalid ranges. */
int softpc_machine_debug_memory(unsigned long address,
    unsigned char *data, unsigned long bytes, int write);

#endif
