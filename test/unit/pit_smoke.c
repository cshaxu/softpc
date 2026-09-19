#include "vm/machine.h"
#include "compat/devices/snapshot.h"
#include "../lib/cleanup.h"

#include <assert.h>
#include <stdio.h>
#include "insignia.h"
#include "host_def.h"
#include "ios.h"
#include "ica.h"
#include "timer.h"

static unsigned int irq_requests;

static void count_irq(IU32 adapter, IU32 line, IS32 count)
{
    if (adapter == ICA_MASTER && line == CPU_TIMER_INT)
        irq_requests += (unsigned int)count;
}

static void verify_read_only_time(softpc_machine *machine,
    unsigned int divisor, unsigned int mode)
{
    softpc_device_pit_state before, after;
    void (*original_irq)(IU32, IU32, IS32);
    half_word value;
    unsigned int index;

    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    original_irq = ica_hw_interrupt_func;
    ica_hw_interrupt_func = count_irq;
    irq_requests = 0u;
    outb(0x43u, (half_word)(0x30u | (mode << 1u)));
    outb(0x40u, (half_word)divisor);
    outb(0x40u, (half_word)(divisor >> 8u));
    assert(softpc_device_snapshot_capture_pit(&before));

    /* No CPU execution consumes host ticks here. Reading a latched counter
       may interpolate its visible count, but cannot create elapsed time. */
    for (index = 0u; index < 1000u; ++index) {
        outb(0x43u, 0u);
        inb(0x40u, &value);
        inb(0x40u, &value);
    }
    assert(softpc_device_snapshot_capture_pit(&after));
    assert(irq_requests == 0u);
    assert(!after.active_interrupt_event);
    assert(after.more_timer_multiple == 0u);
    assert(after.counter[0].activation_age_microseconds ==
        before.counter[0].activation_age_microseconds);

    /* Actual timer heartbeats still wrap and request interrupts. */
    time_tick();
    time_tick();
    assert(irq_requests != 0u);
    ica_hw_interrupt_func = original_irq;
}

int main(void)
{
    const char *path = "softpc-pit-smoke.img";
    unsigned char sector[512] = { 0 };
    softpc_machine_options options = { path, NULL };
    softpc_machine *machine = NULL;
    const unsigned int divisors[] = { 1u, 16384u, 65536u };
    const unsigned int modes[] = { 0u, 2u, 3u };
    FILE *file = fopen(path, "wb");
    unsigned int index, mode;

    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    for (mode = 0u; mode < sizeof(modes) / sizeof(modes[0]); ++mode)
        for (index = 0u; index < sizeof(divisors) / sizeof(divisors[0]); ++index)
            verify_read_only_time(machine, divisors[index], modes[mode]);
    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
