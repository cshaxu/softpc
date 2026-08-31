#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"

int main(void)
{
    const char *path = "softpc-machine-irq-smoke.img";
    unsigned char sector[512] = { 0 };
    unsigned char marker = 0;
    unsigned char ticks[4] = { 0, 0, 0, 0 };
    half_word reset_command = 0xfeu;
    uint16_t cs = 0u;
    uint32_t eip = 0u;
    unsigned char program[] = {
        /* Re-enter the same boot sector through a nonzero CS. */
        0xeau, 0x10u, 0x00u, 0xc0u, 0x07u,
        0x90u, 0x90u, 0x90u, 0x90u, 0x90u, 0x90u, 0x90u, 0x90u,
        0x90u, 0x90u, 0x90u,
        /* Program the PIT with one instruction per tick while IF is clear. */
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u,
        /* Exercise the nonzero, odd stack shape used by the real loader. */
        0xb8u, 0x84u, 0x9fu, 0x8eu, 0xd0u, 0xbcu, 0xe3u, 0x00u,
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
    /* The original ROM POST is part of the machine path.  A slice measured
       in the old replacement core's bootstrap instructions cannot reach a
       boot sector after that POST, so use a bounded machine slice instead. */
    assert(softpc_machine_run(machine, 6000u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker, 1u) == SOFTPC_MACHINE_OK);
    assert(marker == 0x5au);
    assert(softpc_machine_read_physical(machine, 0x46cu, ticks,
        sizeof(ticks)) == SOFTPC_MACHINE_OK);
    assert(ticks[0] != 0u || ticks[1] != 0u || ticks[2] != 0u || ticks[3] != 0u);

    /* The original 8042 output-port pulse requests a CPU reset through the
       original keyboard controller; it is not a standalone reset shortcut. */
    outb(0x64u, reset_command);
    assert(softpc_machine_run(machine, 1u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_instruction_pointer(machine, &cs, &eip) ==
        SOFTPC_MACHINE_OK);
    assert(cs == 0xf000u);
    assert(eip == 0xfff0u);

    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
