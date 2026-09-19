#include "vm/machine.h"
#include "../lib/cleanup.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"
#include "ica.h"
#include "compat/ccpu/abi.h"
#include "compat/ccpu/lifecycle.h"
#include "../../src/mvdm/softpc.new/base/ccpu386/c_intr.h"

extern void reboot(void);
extern void host_set_hw_int(void);

static void verify_rejected_interrupt(softpc_machine *machine)
{
    IU8 master_mask, slave_mask;
    IS32 rejected;
    unsigned short cs = c_getCS();
    unsigned long ip = c_getEIP(), sp = c_getESP(), flags = c_getEFLAGS();
    unsigned long stack = c_getSS_BASE() + ((sp - 6u) & 0xffffu);
    unsigned char before[6], after[6];

    inb(0x21u, &master_mask);
    inb(0xa1u, &slave_mask);
    outb(0x21u, 0xffu);
    outb(0xa1u, 0xffu);
    rejected = ica_intack();
    assert(rejected == -1);
    assert(softpc_machine_read_physical(machine, stack, before,
        sizeof(before)) == SOFTPC_MACHINE_OK);

    /* Exercise the real PIC result through the original CPU entry, not a
       duplicate predicate. Rejection must not push an interrupt frame. */
    do_intrupt((IU16)rejected, FALSE, FALSE, 0u);
    assert(c_getCS() == cs && c_getEIP() == ip);
    assert(c_getESP() == sp && c_getEFLAGS() == flags);
    assert(softpc_machine_read_physical(machine, stack, after,
        sizeof(after)) == SOFTPC_MACHINE_OK);
    assert(memcmp(before, after, sizeof(before)) == 0);

    /* A stale CPU request must also survive the actual instruction loop.
       The boot program is parked on JMP $, with interrupts enabled. */
    assert((flags & 0x200u) != 0u);
    host_set_hw_int();
    assert(softpc_machine_run(machine, 32u) == SOFTPC_MACHINE_OK);
    assert((*softpc_ccpu_interrupt_map_address() & 1u) == 0u);
    assert(c_getCS() == cs && c_getEIP() == ip);
    assert(c_getESP() == sp && c_getEFLAGS() == flags);
    outb(0xa1u, slave_mask);
    outb(0x21u, master_mask);
}

static void verify_keyboard_read_releases_irq(void)
{
    half_word irr = 0;
    half_word response = 0;

    /* A keyboard-device response raises IRQ1.  Reading its one-byte output
       buffer must lower the same line even if the guest polls the PIC while
       IRQ1 is masked instead of accepting the interrupt. */
    outb(0x20u, 0x0au);
    outb(0x60u, 0xf2u);
    inb(0x20u, &irr);
    assert((irr & 0x02u) != 0u);
    inb(0x60u, &response);
    assert(response == 0xfau);
    inb(0x20u, &irr);
    assert((irr & 0x02u) == 0u);
}

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
    softpc_machine_options options = { path, NULL };
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
    verify_rejected_interrupt(machine);
    verify_keyboard_read_releases_irq();

    /* The original 8042 output-port pulse requests a CPU reset through the
       original keyboard controller; it is not a standalone reset shortcut. */
    outb(0x64u, reset_command);
    assert(softpc_machine_run(machine, 1u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_instruction_pointer(machine, &cs, &eip) ==
        SOFTPC_MACHINE_OK);
    assert(cs == 0xf000u);
    assert(eip == 0xfff0u);

    /* Ctrl+Alt+Del is decoded by the original BIOS keyboard service, which
       calls the historical reboot host callback. The standalone callback
       must assert that same CCPU reset line; it must not merely set the
       soft-reset classification flag and leave execution in DOS. */
    reboot();
    assert(softpc_machine_run(machine, 1u) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_instruction_pointer(machine, &cs, &eip) ==
        SOFTPC_MACHINE_OK);
    assert(cs == 0xf000u);
    assert(eip == 0xfff0u);

    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
