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
    extern void equipment IPT0();
    extern void memory_size IPT0();
    extern void bootstrap IPT0();
    extern void bootstrap1 IPT0();
    extern void bootstrap2 IPT0();
    extern void bootstrap3 IPT0();
    extern void mouse_install1 IPT0();
    extern void mouse_install2 IPT0();
    extern void mouse_int1 IPT0();
    extern void mouse_int2 IPT0();
    extern void mouse_io_language IPT0();
    extern void mouse_io_interrupt IPT0();
    extern void mouse_video_io IPT0();

    BIOS[BIOS_KB_INT] = keyboard_int;
    BIOS[BIOS_KEYBOARD_IO] = keyboard_io;
    BIOS[BIOS_DISKETTE_INT] = diskette_int;
    BIOS[BIOS_DISKETTE_IO] = diskette_io;
    BIOS[BIOS_VIDEO_IO] = ega_video_io;
    BIOS[BIOS_PRINTER_IO] = printer_io;
    BIOS[BIOS_RS232_IO] = rs232_io;
    BIOS[BIOS_DISK_IO] = disk_io;
    BIOS[BIOS_EQUIPMENT] = equipment;
    BIOS[BIOS_MEMORY_SIZE] = memory_size;
    BIOS[BIOS_BOOT_STRAP] = bootstrap;
    BIOS[0x90] = bootstrap1;
    BIOS[0x91] = bootstrap2;
    BIOS[0x92] = bootstrap3;
    BIOS[BIOS_MOUSE_INSTALL1] = mouse_install1;
    BIOS[BIOS_MOUSE_INSTALL2] = mouse_install2;
    BIOS[BIOS_MOUSE_INT1] = mouse_int1;
    BIOS[BIOS_MOUSE_INT2] = mouse_int2;
    BIOS[BIOS_MOUSE_IO_LANGUAGE] = mouse_io_language;
    BIOS[BIOS_MOUSE_IO_INTERRUPT] = mouse_io_interrupt;
    BIOS[BIOS_MOUSE_VIDEO_IO] = mouse_video_io;
}
