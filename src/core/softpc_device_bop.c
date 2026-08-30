#include "insignia.h"
#include "bios.h"

/*
 * The original ROM communicates with machine-resident C services through
 * C4 C4..C7 BIOS Operations.  This is deliberately only a machine device
 * table: it carries no product/session dispatcher and no fallback service.
 * A ROM call can reach a handler only after that handler has been restored
 * and registered below.
 */
IBOOL softpc_device_bop_dispatch IFN2(IU8, number, IU32, argument)
{
    void (*handler)();

    UNUSED(argument);
    handler = BIOS[number];
    if (handler == 0)
        return FALSE;
    (*handler)();
    return TRUE;
}

void softpc_device_bop_register_machine_services IFN0()
{
    extern void keyboard_int IPT0();
    extern void keyboard_io IPT0();
    extern void diskette_int IPT0();
    extern void diskette_io IPT0();
    extern void ega_video_io IPT0();
    extern void printer_io IPT0();
    extern void rs232_io IPT0();
    extern void disk_io IPT0();

    BIOS[BIOS_KB_INT] = keyboard_int;
    BIOS[BIOS_KEYBOARD_IO] = keyboard_io;
    BIOS[BIOS_DISKETTE_INT] = diskette_int;
    BIOS[BIOS_DISKETTE_IO] = diskette_io;
    BIOS[BIOS_VIDEO_IO] = ega_video_io;
    BIOS[BIOS_PRINTER_IO] = printer_io;
    BIOS[BIOS_RS232_IO] = rs232_io;
    BIOS[BIOS_DISK_IO] = disk_io;
}
