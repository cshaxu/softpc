#include "softpc_machine.h"

#include <stdio.h>
#include <string.h>

#define SOFTPC_TEXT_BYTES (80u * 25u * 2u)
#define SOFTPC_BOOT_SLICES 200u
#define SOFTPC_SLICE_INSTRUCTIONS 50000u
#define SOFTPC_TRACE_INSTRUCTIONS 100u
#define SOFTPC_TRACE_1K_INSTRUCTIONS 1000u
#define SOFTPC_LONG_TRACE_SLICES 800u

static int text_has_printable_character(const unsigned char *text)
{
    unsigned int index;
    for (index = 0u; index < SOFTPC_TEXT_BYTES; index += 2u) {
        if (text[index] >= 0x20u && text[index] < 0x7fu) return 1;
    }
    return 0;
}

static void print_instruction(softpc_machine *machine, uint16_t cs,
    uint32_t eip)
{
    unsigned char instruction[8];
    uint32_t address = 0u;
    if (softpc_machine_instruction_address(machine, &address) ==
            SOFTPC_MACHINE_OK &&
        softpc_machine_read_physical(machine, address, instruction,
            sizeof(instruction)) == SOFTPC_MACHINE_OK)
        fprintf(stderr, "    %05x: %02x %02x %02x %02x %02x %02x %02x %02x\n",
            (unsigned int)address, instruction[0], instruction[1],
            instruction[2], instruction[3], instruction[4], instruction[5],
            instruction[6], instruction[7]);
}

int main(int argc, char **argv)
{
    softpc_machine_options options = { NULL, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    unsigned char text[SOFTPC_TEXT_BYTES];
    uint16_t cs = 0u;
    uint32_t eip = 0u;
    unsigned int index;
    unsigned int slices = SOFTPC_BOOT_SLICES;
    int trace = 0;
    uint64_t slice_instructions = SOFTPC_SLICE_INSTRUCTIONS;
    softpc_machine_result result;

    if ((argc != 3 && argc != 4) || (strcmp(argv[1], "--floppy") != 0 &&
        strcmp(argv[1], "--hdd") != 0) ||
        (argc == 4 && strcmp(argv[3], "--trace") != 0 &&
            strcmp(argv[3], "--trace-1k") != 0 &&
            strcmp(argv[3], "--trace-long") != 0 &&
            strcmp(argv[3], "--trace-slices") != 0)) {
        fprintf(stderr, "Usage: %s (--floppy|--hdd) image.img [--trace|--trace-1k|--trace-long|--trace-slices]\n",
            argv[0]);
        return 2;
    }
    trace = argc == 4;
    if (trace && strcmp(argv[3], "--trace") == 0)
        slice_instructions = SOFTPC_TRACE_INSTRUCTIONS;
    else if (trace && strcmp(argv[3], "--trace-1k") == 0)
        slice_instructions = SOFTPC_TRACE_1K_INSTRUCTIONS;
    else if (trace && strcmp(argv[3], "--trace-long") == 0) {
        slice_instructions = SOFTPC_TRACE_INSTRUCTIONS;
        slices = SOFTPC_LONG_TRACE_SLICES;
    }
    if (strcmp(argv[1], "--floppy") == 0) options.floppy_path = argv[2];
    else options.hard_disk_path = argv[2];
    result = softpc_machine_create(&options, &machine);
    if (result != SOFTPC_MACHINE_OK) goto failed;
    result = softpc_machine_reset(machine);
    if (result != SOFTPC_MACHINE_OK) goto failed;
    for (index = 0u; index < slices; ++index) {
        result = softpc_machine_run(machine, slice_instructions);
        if (result != SOFTPC_MACHINE_OK) goto failed;
        if (trace) {
            (void)softpc_machine_instruction_pointer(machine, &cs, &eip);
            fprintf(stderr, "%03u %04x:%08x\n", index,
                (unsigned int)cs, (unsigned int)eip);
            print_instruction(machine, cs, eip);
        }
        if (softpc_machine_read_physical(machine, 0xb8000u, text,
                sizeof(text)) == SOFTPC_MACHINE_OK &&
            text_has_printable_character(text)) {
            softpc_machine_destroy(machine);
            return 0;
        }
    }
    (void)softpc_machine_instruction_pointer(machine, &cs, &eip);
    fprintf(stderr, "softpc-real-boot-smoke: guest produced no text output at %04x:%08x\n",
        (unsigned int)cs, (unsigned int)eip);
    print_instruction(machine, cs, eip);
failed:
    if (result != SOFTPC_MACHINE_OK)
        fprintf(stderr, "softpc-real-boot-smoke: %s\n",
            softpc_machine_result_name(result));
    softpc_machine_destroy(machine);
    return 1;
}
