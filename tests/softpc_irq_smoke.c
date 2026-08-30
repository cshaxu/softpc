#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *path = "softpc-machine-irq-smoke.img";
    unsigned char sector[512] = { 0 };
    unsigned char marker = 0;
    unsigned char program[] = {
        /* Program the PIT with one instruction per tick while IF is clear. */
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u,
        0xb0u, 0x34u, 0xe6u, 0x43u, 0xb0u, 0x01u, 0xe6u, 0x40u,
        0xb0u, 0x00u, 0xe6u, 0x40u,
        /* The queued IRQ0 must be delivered after STI and return through the
           fixed handler before the following store runs. */
        0xfbu, 0xb8u, 0x5au, 0x00u, 0xa3u, 0x00u, 0x05u, 0xebu, 0xfeu
    };
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    FILE *file;

    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);

    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 9u) == SOFTPC_MACHINE_OK);
    /* The second slice loads the PIT's final byte and queues IRQ0. */
    assert(softpc_machine_run(machine, 2u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 12u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == 0x5au);
    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
