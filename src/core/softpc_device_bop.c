#include "insignia.h"
#include "host_def.h"
#include "bios.h"
#include "tape_io.h"
#include "sas.h"
#include TypesH
#include CpuH


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
    if (handler == 0) {
        return FALSE;
    }
    (*handler)();
    return TRUE;
}

void softpc_device_bop_register_machine_services IFN0()
{
    extern void reset IPT0();
    extern void keyboard_int IPT0();
    extern void dummy_int IPT0();
    extern void illegal_op_int IPT0();
    extern void keyboard_io IPT0();
    extern void diskette_int IPT0();
    extern void diskette_io IPT0();
    extern void video_io IPT0();
    extern void ega_video_io IPT0();
    extern void printer_io IPT0();
    extern void rs232_io IPT0();
    extern void disk_io IPT0();
    extern void cassette_io IPT0();
    extern void equipment IPT0();
    extern void memory_size IPT0();
    extern void time_of_day IPT0();
    extern void kb_idle_poll IPT0();
    extern void rom_basic IPT0();
    extern void Get_build_id IPT0();
    extern void re_direct IPT0();
    extern void D11_int IPT0();
    extern void int_287 IPT0();
    extern void unexpected_int IPT0();
    extern void bootstrap IPT0();
    extern void bootstrap1 IPT0();
    extern void bootstrap2 IPT0();
    extern void bootstrap3 IPT0();
#ifdef WDCTRL_BOP
    /* Original diskbios.c 32-bit controller path.  Windows probes this at
       ROM BOP 04h; it is a machine disk I/O acceleration, not a DOS, DPMI,
       XMS, VDD, or NTVDM service. */
#endif

    BIOS[BIOS_RESET] = reset;
    BIOS[BIOS_DUMMY_INT] = dummy_int;
#ifdef WDCTRL_BOP
    BIOS[0x04] = wdctrl_bop;
#endif
    BIOS[0x06] = illegal_op_int;
    BIOS[BIOS_UNEXP_INT] = unexpected_int;
    BIOS[BIOS_KB_INT] = keyboard_int;
    BIOS[BIOS_KEYBOARD_IO] = keyboard_io;
    BIOS[BIOS_DISKETTE_INT] = diskette_int;
    BIOS[BIOS_DISKETTE_IO] = diskette_io;
    /* Keep the original BIOS BOP layout: the base video service occupies
       10h, while the V7 VGA ROM transfers INT 10h to the EGA extension at
       42h.  The latter is visible in v7vga.rom at C000:0898. */
    BIOS[BIOS_VIDEO_IO] = video_io;
    BIOS[0x42] = ega_video_io;
    BIOS[BIOS_PRINTER_IO] = printer_io;
    BIOS[BIOS_RS232_IO] = rs232_io;
    BIOS[BIOS_DISK_IO] = disk_io;
    BIOS[BIOS_CASSETTE_IO] = cassette_io;
    BIOS[BIOS_EQUIPMENT] = equipment;
    BIOS[BIOS_MEMORY_SIZE] = memory_size;
    BIOS[BIOS_TIME_OF_DAY] = time_of_day;
    BIOS[BIOS_BASIC] = rom_basic;
    BIOS[0x21] = Get_build_id;
    BIOS[BIOS_KEYBOARD_BREAK + 2] = kb_idle_poll;
    BIOS[0x71] = re_direct;
    BIOS[0x72] = D11_int;
    BIOS[0x73] = D11_int;
    BIOS[0x74] = D11_int;
    BIOS[0x75] = int_287;
    BIOS[0x76] = D11_int;
    BIOS[0x77] = D11_int;
    BIOS[BIOS_BOOT_STRAP] = bootstrap;
    BIOS[0x90] = bootstrap1;
    BIOS[0x91] = bootstrap2;
    BIOS[0x92] = bootstrap3;
}
