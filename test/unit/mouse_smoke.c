#include "machine.h"
#include "test_cleanup.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "bios.h"
#include "ios.h"
#include "mouse.h"

extern void com_outb IPT2(io_addr, port, half_word, value);

static void make_boot_disk(const char *path)
{
    unsigned char sector[512] = {
        /* Install an IRQ9 (slave PIC line 1 / vector 71h) handler, unmask
           its cascade path, then wait.  This proves the public standalone
           mouse ingress reaches an actual guest interrupt, not merely the
           controller's readable register latch. */
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u,
        0xb8u, 0x22u, 0x7cu, 0xa3u, 0xc4u, 0x01u,
        0x0eu, 0x58u, 0xa3u, 0xc6u, 0x01u,
        0xe4u, 0x21u, 0x24u, 0xfbu, 0xe6u, 0x21u,
        0xe4u, 0xa1u, 0x24u, 0xfdu, 0xe6u, 0xa1u,
        0xfbu, 0xf4u, 0xebu, 0xfdu,
        /* 7c22: acknowledge slave and master PIC, record delivery, IRET. */
        0xb0u, 0x20u, 0xe6u, 0xa0u, 0xe6u, 0x20u,
        0xc6u, 0x06u, 0x00u, 0x05u, 0xa5u, 0xcfu
    };
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
    const char *path = "softpc-mouse-smoke.img";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    half_word value = 0u;

    make_boot_disk(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    /* Let the guest install the hardware IRQ handler before injecting an
       input transition. */
    assert(softpc_machine_run(machine, 6000u) == SOFTPC_MACHINE_OK);

    /* The original InPort mouse occupies only 23Ch--23Fh.  Keep this
       explicit: an accidental adapter-table overwrite makes virtual-8086
       I/O appear to work until a protected-mode guest probes COM1. */
    assert((unsigned char)Ios_in_adapter_table[MOUSE_PORT_0] ==
        MOUSE_ADAPTOR);
    assert((unsigned char)Ios_out_adapter_table[MOUSE_PORT_0] ==
        MOUSE_ADAPTOR);
    assert((unsigned char)Ios_in_adapter_table[0x3f8u] == COM1_ADAPTOR);
    assert((unsigned char)Ios_out_adapter_table[0x3f8u] == COM1_ADAPTOR);
    assert(Ios_outb_function[MOUSE_ADAPTOR] == mouse_outb);
    assert(Ios_outb_function[COM1_ADAPTOR] == com_outb);

    /* These are the original InPort adapter's alternating ID bytes. */
    inb(MOUSE_PORT_2, &value);
    assert(value == 0xdeu);
    inb(MOUSE_PORT_2, &value);
    assert(value == 0x10u);

    /* Exact Microsoft InPort loopback handshake issued by Windows 3.1's
       MOUSE.DRV before it commits to the IRQ9 controller path. */
    outb(MOUSE_PORT_3, 0x91u);
    outb(MOUSE_PORT_1, 0xa5u);
    outb(MOUSE_PORT_2, 0x10u);
    inb(MOUSE_PORT_1, &value);
    assert(value == 0xa5u);
    /* The diagnostic is one transfer, not an alternate controller mode. */
    outb(MOUSE_PORT_0, 0x07u);
    outb(MOUSE_PORT_1, 0x09u);
    inb(MOUSE_PORT_1, &value);
    assert(value == 0x09u);
    outb(MOUSE_PORT_0, 0x80u);

    /* Reset and select the original mode register, then round-trip it. */
    /* Windows 3.1's installed Microsoft MOUSE.DRV resets the card, selects
       mode register 7 and enables the Microsoft InPort base mode (10h).
       Its IRQ handler then raises HOLD to 30h before reading status/X/Y.
       Exercise that real driver protocol, not merely the older 20h hold
       sequence. */
    outb(MOUSE_PORT_0, 0x87u);
    outb(MOUSE_PORT_1, 0x10u);
    inb(MOUSE_PORT_1, &value);
    assert(value == 0x10u);
    assert(softpc_machine_mouse_input(machine, 5, -3, 1u, 0u) ==
        SOFTPC_MACHINE_OK);
    outb(MOUSE_PORT_1, 0x30u);
    inb(MOUSE_PORT_1, &value);
    assert(value == 0x30u);

    /* The public port reaches the original adapter; its hold transition
       latches relative movement and button state in the original registers. */
    outb(MOUSE_PORT_0, INTERNAL_MOUSE_STAT_REG);
    inb(MOUSE_PORT_1, &value);
    assert(value == (LEFT_BUTTON_DOWN | LEFT_BUTTON_CHANGE | MOVEMENT));
    outb(MOUSE_PORT_0, INTERNAL_DATA1_REG);
    inb(MOUSE_PORT_1, &value);
    assert(value == 5u);
    outb(MOUSE_PORT_0, INTERNAL_DATA2_REG);
    inb(MOUSE_PORT_1, &value);
    assert(value == 0xfdu);

    /* A second event must travel through the original InPort, slave PIC,
       CCPU interrupt delivery and guest handler.  Windows' Microsoft
       MOUSE.DRV uses this exact machine path to draw and update its own
       cursor. */
    assert(softpc_machine_mouse_input(machine, 1, 0, 1u, 0u) ==
        SOFTPC_MACHINE_OK);
    assert(softpc_machine_run(machine, 6000u) == SOFTPC_MACHINE_OK);
    {
        unsigned char marker = 0u;
        assert(softpc_machine_read_physical(machine, 0x500u, &marker,
            sizeof(marker)) == SOFTPC_MACHINE_OK);
        assert(marker == 0xa5u);
    }

    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
