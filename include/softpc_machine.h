#ifndef SOFTPC_MACHINE_H
#define SOFTPC_MACHINE_H

#include <stdint.h>

typedef struct softpc_machine softpc_machine;

typedef enum softpc_presentation {
    SOFTPC_PRESENTATION_CONSOLE,
    SOFTPC_PRESENTATION_WINDOW
} softpc_presentation;

/* Source image policy selected by the fixed launcher configuration. */
typedef enum softpc_media_mode {
    SOFTPC_MEDIA_DIRECT = 0,
    SOFTPC_MEDIA_READONLY,
    SOFTPC_MEDIA_OVERLAY
} softpc_media_mode;

typedef enum softpc_machine_result {
    SOFTPC_MACHINE_OK,
    SOFTPC_MACHINE_INVALID_ARGUMENT,
    SOFTPC_MACHINE_IO_ERROR,
    SOFTPC_MACHINE_BACKEND_UNAVAILABLE
} softpc_machine_result;

typedef struct softpc_machine_options {
    const char *floppy_path;
    const char *hard_disk_path;
    softpc_presentation presentation;
    uint32_t memory_bytes;
    softpc_media_mode media_mode;
} softpc_machine_options;

softpc_machine_result softpc_machine_create(const softpc_machine_options *options,
    softpc_machine **machine_out);
softpc_machine_result softpc_machine_reset(softpc_machine *machine);
softpc_machine_result softpc_machine_run(softpc_machine *machine,
    uint64_t instruction_budget);
/* Read-only execution progress for a host scheduler.  This is a machine
 * facade observation; it neither advances guest time nor exposes CCPU state. */
uint64_t softpc_machine_executed_instructions(const softpc_machine *machine);
softpc_machine_result softpc_machine_read_physical(const softpc_machine *machine,
    uint32_t address, void *buffer, uint32_t bytes);
/* Copy bytes into guest physical RAM through the machine boundary.  This is
 * intended for firmware/media integration and test fixtures, not a host RAM
 * alias; ROM and out-of-range writes are rejected by the machine. */
softpc_machine_result softpc_machine_write_physical(softpc_machine *machine,
    uint32_t address, const void *buffer, uint32_t bytes);
softpc_machine_result softpc_machine_instruction_pointer(
    const softpc_machine *machine, uint16_t *cs, uint32_t *eip);
softpc_machine_result softpc_machine_instruction_address(
    const softpc_machine *machine, uint32_t *address);
softpc_machine_result softpc_machine_key_scancode(softpc_machine *machine,
    uint8_t scan_code);
/* Deliver an original SoftPC keyboard key number after host mapping. */
softpc_machine_result softpc_machine_key_number(softpc_machine *machine,
    uint8_t key_number, uint8_t released);
/* Inject relative host-pointer movement into the original Microsoft Bus
 * Mouse adapter. Button values are zero (up) or nonzero (down). */
softpc_machine_result softpc_machine_mouse_input(softpc_machine *machine,
    int32_t delta_x, int32_t delta_y, uint8_t left_down, uint8_t right_down);

/* Replace drive A's removable medium through the original GFI/FDC host-media
 * boundary. Passing NULL ejects it. Callers must stop or pause execution
 * before changing media. */
softpc_machine_result softpc_machine_set_floppy(softpc_machine *machine,
    const char *path);
int softpc_machine_presentation_is_graphics(const softpc_machine *machine);

/* Consume the original host renderer's pending dirty rectangle. */
int softpc_machine_presentation_take_dirty(const softpc_machine *machine,
    int32_t *left, int32_t *top, int32_t *right, int32_t *bottom);

/* Borrow the original host renderer's indexed DIB.  The caller must not
 * retain the pointers after machine destruction or mutate either surface. */
int softpc_machine_presentation_dib(const softpc_machine *machine,
    const void **bits_out, const void **info_out, uint32_t *width_out,
    uint32_t *height_out);

/* Borrow the original nt_cga text-presenter surface.  Cells are four bytes
 * wide in this standalone build: the character and attribute occupy the
 * first two bytes, followed by the original renderer's padding.  The caller
 * must not mutate the surface or retain it after machine destruction. */
int softpc_machine_presentation_text(const softpc_machine *machine,
    const void **cells_out, uint32_t *columns_out, uint32_t *rows_out,
    uint32_t *stride_out, uint32_t *cell_bytes_out);

/* Read the most recent text-cursor position emitted by the original video
 * controller. The standalone host records this callback; frontends own the
 * actual cursor drawing. */
int softpc_machine_presentation_cursor(const softpc_machine *machine,
    int32_t *column_out, int32_t *row_out);

void softpc_machine_destroy(softpc_machine *machine);

const char *softpc_machine_result_name(softpc_machine_result result);

#endif
