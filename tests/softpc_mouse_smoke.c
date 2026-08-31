#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "bios.h"
#include "ios.h"
#include "mouse.h"

enum {
    SOFTPC_MOUSE_SET_GRAPHICS = 9,
    SOFTPC_MOUSE_GET_CURSOR_HOT_SPOT = 42,
    SOFTPC_MOUSE_TYPE_INPORT = 3
};

extern IBOOL softpc_device_bop_dispatch(IU8 number, IU32 argument);
extern void c_setAX(IU16 value);
extern void c_setBX(IU16 value);
extern void c_setCX(IU16 value);
extern void c_setDX(IU16 value);
extern ISM32 c_setES(IU16 value);
extern IU16 c_getBX(void);
extern IU16 c_getCX(void);
extern IU16 c_getDX(void);

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

    /* INT 33h function 9 is served by the original mouse driver through
       its ROM BOP.  The cursor bitmap is guest memory, so this exercises
       the restored driver's native-width guest-address conversion rather
       than allowing a host pointer to leak into SAS. */
    {
        const unsigned char bitmap[64] = {
            0x80u, 0x01u, 0x40u, 0x02u, 0x20u, 0x04u, 0x10u, 0x08u,
            0x08u, 0x10u, 0x04u, 0x20u, 0x02u, 0x40u, 0x01u, 0x80u,
            0xffu, 0xffu, 0x7fu, 0xfeu, 0x3fu, 0xfcu, 0x1fu, 0xf8u,
            0x0fu, 0xf0u, 0x07u, 0xe0u, 0x03u, 0xc0u, 0x01u, 0x80u,
            0x00u, 0x00u, 0x80u, 0x01u, 0x40u, 0x02u, 0x20u, 0x04u,
            0x10u, 0x08u, 0x08u, 0x10u, 0x04u, 0x20u, 0x02u, 0x40u,
            0x01u, 0x80u, 0xffu, 0xffu, 0x7fu, 0xfeu, 0x3fu, 0xfcu,
            0x1fu, 0xf8u, 0x0fu, 0xf0u, 0x07u, 0xe0u, 0x03u, 0xc0u
        };
        assert(softpc_machine_write_physical(machine, 0x700u, bitmap,
            sizeof(bitmap)) == SOFTPC_MACHINE_OK);
        c_setAX(SOFTPC_MOUSE_SET_GRAPHICS);
        c_setBX(3u);
        c_setCX(4u);
        c_setDX(0x700u);
        c_setES(0u);
        assert(softpc_device_bop_dispatch(BIOS_MOUSE_IO_INTERRUPT, 0u) ==
            TRUE);

        c_setAX(SOFTPC_MOUSE_GET_CURSOR_HOT_SPOT);
        c_setBX(0u);
        c_setCX(0u);
        c_setDX(0u);
        assert(softpc_device_bop_dispatch(BIOS_MOUSE_IO_INTERRUPT, 0u) ==
            TRUE);
        assert(c_getBX() == 3u);
        assert(c_getCX() == 4u);
        assert(c_getDX() == SOFTPC_MOUSE_TYPE_INPORT);
    }

    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
