#ifndef SOFTPC_MACHINE_H
#define SOFTPC_MACHINE_H

#include <stdint.h>

typedef struct softpc_machine softpc_machine;

typedef enum softpc_presentation {
    SOFTPC_PRESENTATION_CONSOLE,
    SOFTPC_PRESENTATION_WINDOW
} softpc_presentation;

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
} softpc_machine_options;

softpc_machine_result softpc_machine_create(const softpc_machine_options *options,
    softpc_machine **machine_out);
softpc_machine_result softpc_machine_reset(softpc_machine *machine);
softpc_machine_result softpc_machine_run(softpc_machine *machine,
    uint64_t instruction_budget);
softpc_machine_result softpc_machine_read_physical(const softpc_machine *machine,
    uint32_t address, void *buffer, uint32_t bytes);
softpc_machine_result softpc_machine_instruction_pointer(
    const softpc_machine *machine, uint16_t *cs, uint32_t *eip);
softpc_machine_result softpc_machine_instruction_address(
    const softpc_machine *machine, uint32_t *address);
softpc_machine_result softpc_machine_key_scancode(softpc_machine *machine,
    uint8_t scan_code);
/* Inject relative host-pointer movement into the original Microsoft Bus
 * Mouse adapter. Button values are zero (up) or nonzero (down). */
softpc_machine_result softpc_machine_mouse_input(softpc_machine *machine,
    int32_t delta_x, int32_t delta_y, uint8_t left_down, uint8_t right_down);
/* Copy the original VGA controller's mode 13h (320x200, 256-colour) output
 * into a caller-owned RGB32 surface. Pixels are 0x00RRGGBB. This does not
 * emulate VGA; it is a presentation readout of the original controller's
 * planes and DAC. */
int softpc_machine_vga_mode13_active(const softpc_machine *machine);
softpc_machine_result softpc_machine_vga_mode13_frame(
    const softpc_machine *machine, uint32_t *pixels, uint32_t pixel_count);
/* Copy the original VGA controller's BIOS mode 12h (640x480, 16-colour)
 * output into a caller-owned RGB32 surface. */
int softpc_machine_vga_mode12_active(const softpc_machine *machine);
softpc_machine_result softpc_machine_vga_mode12_frame(
    const softpc_machine *machine, uint32_t *pixels, uint32_t pixel_count);
void softpc_machine_destroy(softpc_machine *machine);

const char *softpc_machine_result_name(softpc_machine_result result);

#endif
