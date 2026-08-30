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
} softpc_machine_options;

softpc_machine_result softpc_machine_create(const softpc_machine_options *options,
    softpc_machine **machine_out);
softpc_machine_result softpc_machine_reset(softpc_machine *machine);
softpc_machine_result softpc_machine_run(softpc_machine *machine,
    uint64_t instruction_budget);
void softpc_machine_destroy(softpc_machine *machine);

const char *softpc_machine_result_name(softpc_machine_result result);

#endif
