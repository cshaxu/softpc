#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"

extern ULONG FreqT2;
extern BOOL PpiState;
extern BOOL T2State;
extern ULONG BeepLastFreq;
extern ULONG BeepLastDuration;

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
    const char *path = "softpc-sound-smoke.img";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;

    make_boot_disk(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);

    /* Original PIT channel 2 mode 3, divisor 1193, produces a 1 kHz tone.
       The original PPI path then passes its full gate/data state to the
       restored nt_sound state machine. */
    outb(TIMER_MODE_REG, 0xb6u);
    outb(TIMER2_REG, 0xa9u);
    outb(TIMER2_REG, 0x04u);
    outb(PPI_GENERAL, 0x03u);
    assert(softpc_machine_run(machine, 64u) == SOFTPC_MACHINE_OK);
    assert(FreqT2 > 10u && FreqT2 < 20000u);
    assert(PpiState == TRUE);
    assert(T2State == TRUE);
    assert(BeepLastFreq == FreqT2);
    assert(BeepLastDuration == INFINITE);

    /* Clearing both original PPI bits stops the same state machine. */
    outb(PPI_GENERAL, 0x00u);
    assert(PpiState == FALSE);
    assert(T2State == FALSE);
    assert(BeepLastFreq == 0u);
    assert(BeepLastDuration == 0u);

    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}