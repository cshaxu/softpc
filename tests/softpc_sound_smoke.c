#include "softpc_machine.h"
#include "softpc_test_cleanup.h"

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
extern void host_timer2_waveform(int delay, ULONG loclocks, ULONG hiclocks,
    int lohi, int repeat);

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

    /* The original PPI port controls timer-2 gate and speaker data.
       Software enables the speaker through original ppi.c.  The original
       PIT callback then presents its waveform through the standalone host
       contract; no standalone device state is synthesized here. */
    outb(PPI_GENERAL, 0x03u);
    assert(PpiState == TRUE);
    assert(T2State == TRUE);
    host_timer2_waveform(0, 596u, 597u, 0, 1);
    assert(FreqT2 > 10u && FreqT2 < 20000u);
    assert(BeepLastFreq == FreqT2);
    assert(BeepLastDuration == INFINITE);

    /* Clearing both original PPI bits stops the same state machine. */
    outb(PPI_GENERAL, 0x00u);
    assert(PpiState == FALSE);
    assert(T2State == FALSE);
    assert(BeepLastFreq == 0u);
    assert(BeepLastDuration == 0u);

    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
