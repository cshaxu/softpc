#include "insignia.h"
#include "host_def.h"
#include TypesH

#include "sas.h"

/* Original reset.c::setup_ivt(), retained as the POST-owned ROM vector
 * setup.  The standalone machine invokes it between ROM loading and device
 * POST; product shutdown/reset policy remains outside the machine. */
#define int_addr(int_no) ((int_no) * 4)

void softpc_bios_setup_ivt()
{
    IUH count;

    for (count = 0; count <= 0x78; count++) {
        sas_storew(int_addr(count), UNEXP_INT_OFFSET);
        sas_storew(int_addr(count) + 2, UNEXP_INT_SEGMENT);
    }
    for (count = 0x60; count <= 0x67; count++) {
        sas_storew(int_addr(count), 0);
        sas_storew(int_addr(count) + 2, 0);
    }

    sas_storew(int_addr(0x5), PRINT_SCREEN_OFFSET); sas_storew(int_addr(0x5)+2, PRINT_SCREEN_SEGMENT);
    sas_storew(int_addr(0x6), ILL_OP_INT_OFFSET); sas_storew(int_addr(0x6)+2, ILL_OP_INT_SEGMENT);
    sas_storew(int_addr(0x8), TIMER_INT_OFFSET); sas_storew(int_addr(0x8)+2, TIMER_INT_SEGMENT);
    sas_storew(int_addr(0x9), KB_INT_OFFSET); sas_storew(int_addr(0x9)+2, KB_INT_SEGMENT);
    sas_storew(int_addr(0xE), DISKETTE_INT_OFFSET); sas_storew(int_addr(0xE)+2, DISKETTE_INT_SEGMENT);
    sas_storew(int_addr(0x10), VIDEO_IO_OFFSET); sas_storew(int_addr(0x10)+2, VIDEO_IO_SEGMENT);
    sas_storew(int_addr(0x11), EQUIPMENT_OFFSET); sas_storew(int_addr(0x11)+2, EQUIPMENT_SEGMENT);
    sas_storew(int_addr(0x12), MEMORY_SIZE_OFFSET); sas_storew(int_addr(0x12)+2, MEMORY_SIZE_SEGMENT);
    sas_storew(int_addr(0x13), DISKETTE_IO_OFFSET); sas_storew(int_addr(0x13)+2, DISKETTE_IO_SEGMENT);
    sas_storew(int_addr(0x14), RS232_IO_OFFSET); sas_storew(int_addr(0x14)+2, RS232_IO_SEGMENT);
    sas_storew(int_addr(0x15), CASSETTE_IO_OFFSET); sas_storew(int_addr(0x15)+2, CASSETTE_IO_SEGMENT);
    sas_storew(int_addr(0x16), KEYBOARD_IO_OFFSET); sas_storew(int_addr(0x16)+2, KEYBOARD_IO_SEGMENT);
    sas_storew(int_addr(0x17), PRINTER_IO_OFFSET); sas_storew(int_addr(0x17)+2, PRINTER_IO_SEGMENT);
    sas_storew(int_addr(0x18), BASIC_OFFSET); sas_storew(int_addr(0x18)+2, BASIC_SEGMENT);
    sas_storew(int_addr(0x19), BOOT_STRAP_OFFSET); sas_storew(int_addr(0x19)+2, BOOT_STRAP_SEGMENT);
    sas_storew(int_addr(0x1A), TIME_OF_DAY_OFFSET); sas_storew(int_addr(0x1A)+2, TIME_OF_DAY_SEGMENT);
    sas_storew(int_addr(0x1B), DUMMY_INT_OFFSET); sas_storew(int_addr(0x1B)+2, DUMMY_INT_SEGMENT);
    sas_storew(int_addr(0x1C), DUMMY_INT_OFFSET); sas_storew(int_addr(0x1C)+2, DUMMY_INT_SEGMENT);
    sas_storew(int_addr(0x1D), VIDEO_PARM_OFFSET); sas_storew(int_addr(0x1D)+2, VIDEO_PARM_SEGMENT);
    sas_storew(int_addr(0x1E), DISKETTE_TB_OFFSET); sas_storew(int_addr(0x1E)+2, DISKETTE_TB_SEGMENT);
    sas_storew(int_addr(0x1F), EXTEND_CHAR_OFFSET); sas_storew(int_addr(0x1F)+2, EXTEND_CHAR_SEGMENT);
    sas_storew(int_addr(0x40), DUMMY_INT_OFFSET); sas_storew(int_addr(0x40)+2, DUMMY_INT_SEGMENT);
    sas_storew(int_addr(0x41), DISK_TB_OFFSET); sas_storew(int_addr(0x41)+2, DISK_TB_SEGMENT);
    sas_storew(int_addr(0x6F), DUMMY_INT_OFFSET); sas_storew(int_addr(0x6F)+2, DUMMY_INT_SEGMENT);
    sas_storew(int_addr(0x70), RTC_INT_OFFSET); sas_storew(int_addr(0x70)+2, RTC_INT_SEGMENT);
    sas_storew(int_addr(0x71), REDIRECT_INT_OFFSET); sas_storew(int_addr(0x71)+2, REDIRECT_INT_SEGMENT);
    sas_storew(int_addr(0x72), D11_INT_OFFSET); sas_storew(int_addr(0x72)+2, D11_INT_SEGMENT);
    sas_storew(int_addr(0x73), D11_INT_OFFSET); sas_storew(int_addr(0x73)+2, D11_INT_SEGMENT);
    sas_storew(int_addr(0x74), D11_INT_OFFSET); sas_storew(int_addr(0x74)+2, D11_INT_SEGMENT);
    sas_storew(int_addr(0x75), X287_INT_OFFSET); sas_storew(int_addr(0x75)+2, X287_INT_SEGMENT);
    sas_storew(int_addr(0x76), D11_INT_OFFSET); sas_storew(int_addr(0x76)+2, D11_INT_SEGMENT);
    sas_storew(int_addr(0x77), D11_INT_OFFSET); sas_storew(int_addr(0x77)+2, D11_INT_SEGMENT);
}
