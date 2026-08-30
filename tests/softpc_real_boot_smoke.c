#include "softpc_machine.h"

#include <stdio.h>
#include <string.h>

#define SOFTPC_TEXT_BYTES (80u * 25u * 2u)
#define SOFTPC_BOOT_SLICES 200u
#define SOFTPC_SLICE_INSTRUCTIONS 50000u

static int text_has_printable_character(const unsigned char *text)
{
    unsigned int index;
    for (index = 0u; index < SOFTPC_TEXT_BYTES; index += 2u) {
        if (text[index] >= 0x20u && text[index] < 0x7fu) return 1;
    }
    return 0;
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
    softpc_machine_result result;

    if (argc != 3 || (strcmp(argv[1], "--floppy") != 0 &&
        strcmp(argv[1], "--hdd") != 0)) {
        fprintf(stderr, "Usage: %s (--floppy|--hdd) image.img\n", argv[0]);
        return 2;
    }
    if (strcmp(argv[1], "--floppy") == 0) options.floppy_path = argv[2];
    else options.hard_disk_path = argv[2];
    result = softpc_machine_create(&options, &machine);
    if (result != SOFTPC_MACHINE_OK) goto failed;
    result = softpc_machine_reset(machine);
    if (result != SOFTPC_MACHINE_OK) goto failed;
    for (index = 0u; index < SOFTPC_BOOT_SLICES; ++index) {
        result = softpc_machine_run(machine, SOFTPC_SLICE_INSTRUCTIONS);
        if (result != SOFTPC_MACHINE_OK) goto failed;
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
failed:
    if (result != SOFTPC_MACHINE_OK)
        fprintf(stderr, "softpc-real-boot-smoke: %s\n",
            softpc_machine_result_name(result));
    softpc_machine_destroy(machine);
    return 1;
}
