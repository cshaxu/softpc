#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"
#include "mouse.h"

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
    const char *path = "softpc-mouse-smoke.img";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    half_word value = 0u;

    make_boot_disk(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);

    /* These are the original InPort adapter's alternating ID bytes. */
    inb(MOUSE_PORT_2, &value);
    assert(value == 0xdeu);
    inb(MOUSE_PORT_2, &value);
    assert(value == 0x10u);

    /* Reset and select the original mode register, then round-trip it. */
    outb(MOUSE_PORT_0, 0x87u);
    inb(MOUSE_PORT_1, &value);
    assert(value == 0u);
    assert(softpc_machine_mouse_input(machine, 5, -3, 1u, 0u) ==
        SOFTPC_MACHINE_OK);
    outb(MOUSE_PORT_1, 0x20u);
    inb(MOUSE_PORT_1, &value);
    assert(value == 0x20u);

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

    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
