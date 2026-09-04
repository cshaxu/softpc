#include "machine.h"
#include "test_cleanup.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"

static void make_boot_disk(const char *path)
{
    unsigned char sector[512] = { 0 };
    FILE *file = fopen(path, "wb");
    assert(file != NULL);
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fseek(file, 1474560L - 1L, SEEK_SET) == 0);
    assert(fputc(0, file) == 0);
    assert(fclose(file) == 0);
}

int main(void)
{
    const char *path = "softpc-printer-smoke.img";
    const char *output_path = "softpc-printer-output.bin";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    half_word status = 0u;

    make_boot_disk(path);
    assert(remove(output_path) == 0 || errno == ENOENT);
    options.printer_output_path = output_path;
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    outb(LPT1_PORT_START, 0x41u);
    outb(LPT1_PORT_START + 2u, 0x0du);
    inb(LPT1_PORT_START + 1u, &status);
    /* Original controller clears NOTBUSY while a strobed byte awaits ACK. */
    assert((status & 0x80u) == 0u);
    inb(LPT1_PORT_START + 1u, &status);
    assert((status & 0x80u) != 0u);
    softpc_machine_destroy(machine);
    {
        FILE *file = fopen(output_path, "rb");
        assert(file != NULL);
        assert(fgetc(file) == 0x41);
        assert(fgetc(file) == EOF);
        assert(fclose(file) == 0);
    }
    assert(remove(output_path) == 0);
    assert(softpc_test_remove_image(path));
    return 0;
}
