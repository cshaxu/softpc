#include "console.h"
#include "softpc_machine.h"
#include "win32_window.h"

#include <stdio.h>
#include <string.h>

static void usage(const char *program)
{
    fprintf(stderr, "Usage: %s (--floppy image.img | --hdd image.img) [--window]\n",
        program);
}

int main(int argc, char **argv)
{
    softpc_machine_options options = { 0 };
    softpc_machine *machine = NULL;
    softpc_machine_result result;
    int index;

    options.presentation = SOFTPC_PRESENTATION_CONSOLE;
    for (index = 1; index < argc; ++index) {
        if (!strcmp(argv[index], "--floppy") && index + 1 < argc)
            options.floppy_path = argv[++index];
        else if (!strcmp(argv[index], "--hdd") && index + 1 < argc)
            options.hard_disk_path = argv[++index];
        else if (!strcmp(argv[index], "--window"))
            options.presentation = SOFTPC_PRESENTATION_WINDOW;
        else {
            usage(argv[0]);
            return 2;
        }
    }

    result = softpc_machine_create(&options, &machine);
    if (result != SOFTPC_MACHINE_OK) {
        fprintf(stderr, "softpcvm: %s\n", softpc_machine_result_name(result));
        return 1;
    }
    result = softpc_machine_reset(machine);
    if (result == SOFTPC_MACHINE_OK &&
        options.presentation == SOFTPC_PRESENTATION_WINDOW) {
        if (softpc_vm_run_window(machine) != 0) result = SOFTPC_MACHINE_IO_ERROR;
    } else if (result == SOFTPC_MACHINE_OK) {
        if (softpc_vm_run_console(machine) != 0) result = SOFTPC_MACHINE_IO_ERROR;
    }
    if (result != SOFTPC_MACHINE_OK)
        fprintf(stderr, "softpcvm: %s\n", softpc_machine_result_name(result));
    softpc_machine_destroy(machine);
    return result != SOFTPC_MACHINE_OK;
}
