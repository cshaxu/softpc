#include "core/machine/machine.h"
#include "core/compat/devices/snapshot.h"
#include "../lib/cleanup.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"
#include "timer.h"

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
    softpc_machine_options options = { path, NULL };
    softpc_machine *machine = NULL;
    softpc_device_pit_state pit;

    make_boot_disk(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_device_snapshot_capture_pit(&pit));
    assert(pit.counter[2].gate == GATE_SIGNAL_LOW);

    /* These are AUDIO.COM's PIT writes: channel 2, square wave, divisor
       0x0a98. The later PPI write is therefore its first guest-visible gate
       transition, not a test-only direct host callback. */
    outb(TIMER_MODE_REG, 0xb6u);
    outb(TIMER2_REG, 0x98u);
    outb(TIMER2_REG, 0x0au);
    outb(PPI_GENERAL, 0x03u);
    assert(softpc_device_snapshot_capture_pit(&pit));
    assert(pit.counter[2].gate == GATE_SIGNAL_RISE);
    assert(PpiState == TRUE);
    assert(T2State == TRUE);
    assert(FreqT2 > 10u && FreqT2 < 20000u);
    assert(BeepLastFreq == FreqT2);
    assert(BeepLastDuration == INFINITE);

    /* A second original PIT waveform changes only the requested playback
       tone; the host worker remains a presentation sink and never feeds a
       synthesized timing event back into the guest. */
    host_timer2_waveform(0, 298u, 299u, 0, 1);
    assert(FreqT2 > 10u && FreqT2 < 20000u);
    assert(BeepLastFreq == FreqT2);
    assert(BeepLastDuration == INFINITE);

    /* Clearing both original PPI bits stops the same state machine. */
    outb(PPI_GENERAL, 0x00u);
    assert(PpiState == FALSE);
    assert(T2State == FALSE);
    assert(BeepLastFreq == 0u);
    assert(BeepLastDuration == 0u);

    /* Original reset.c clears the same Timer-2/PPI path. A standalone audio
       worker must therefore have no tone or guest state to retain across a
       cold reset. */
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(BeepLastFreq == 0u);
    assert(BeepLastDuration == 0u);

    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
