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
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u,
        0xc7u, 0x06u, 0x20u, 0x00u, 0x80u, 0x7cu,
        0xc7u, 0x06u, 0x22u, 0x00u, 0x00u, 0x00u,
        0xb0u, 0x34u, 0xe6u, 0x43u, 0xb0u, 0x01u, 0xe6u, 0x40u,
        0xb0u, 0x00u, 0xe6u, 0x40u, 0xfbu, 0xebu, 0xfeu
    };
    unsigned char handler[] = {
        0xfeu, 0x06u, 0x00u, 0x05u, 0xb0u, 0x20u, 0xe6u, 0x20u,
        0xcfu
    };
    softpc_machine_options options = { path, NULL, SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    FILE *file;

    memcpy(sector, program, sizeof(program));
    memcpy(sector + 0x80u, handler, sizeof(handler));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);

    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 100u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 100u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker != 0u);
    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
