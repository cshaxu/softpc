#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "bios.h"
#include "build_id.h"
#include "gfi.h"
#include "tape_io.h"

extern IBOOL softpc_device_bop_dispatch(IU8 number, IU32 argument);
extern void c_setAL(IU8 value);
extern void c_setAH(IU8 value);
extern void c_setCX(IU16 value);
extern ISM32 c_setDS(IU16 value);
extern IU16 c_getAX(void);
extern IU16 c_getBX(void);

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
    const char *path = "softpc-bop-smoke.img";
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;

    make_boot_disk(path);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);

    /* These are the original ROM's machine-service slots.  They must remain
       direct entries into restored SoftPC controller/firmware code, rather
       than silently falling back to a new VM service layer. */
    assert(BIOS[BIOS_KB_INT] != NULL);
    assert(BIOS[BIOS_RESET] != NULL);
    assert(BIOS[BIOS_DUMMY_INT] != NULL);
    assert(BIOS[BIOS_KEYBOARD_IO] != NULL);
    assert(BIOS[BIOS_DISKETTE_INT] != NULL);
    assert(BIOS[BIOS_DISKETTE_IO] != NULL);
    assert(BIOS[BIOS_VIDEO_IO] != NULL);
    assert(BIOS[0x42] != NULL); /* V7 VGA ROM EGA extension */
    assert(BIOS[BIOS_PRINTER_IO] != NULL);
    assert(BIOS[BIOS_RS232_IO] != NULL);
    assert(BIOS[BIOS_DISK_IO] != NULL);
    assert(BIOS[BIOS_CASSETTE_IO] != NULL);
    assert(BIOS[BIOS_CASSETTE_IO] == cassette_io);
    assert(BIOS[BIOS_EQUIPMENT] != NULL);
    assert(BIOS[BIOS_MEMORY_SIZE] != NULL);
    assert(BIOS[BIOS_TIMER_INT] != NULL);
    assert(BIOS[BIOS_TIME_OF_DAY] != NULL);
    assert(BIOS[BIOS_BOOT_STRAP] != NULL);
    assert(BIOS[BIOS_MOUSE_INSTALL1] != NULL);
    assert(BIOS[BIOS_MOUSE_VIDEO_IO] != NULL);

    /* BOP 01 is the original BIOS dummy interrupt, not a product service. */
    assert(softpc_device_bop_dispatch(BIOS_DUMMY_INT, 0u) == TRUE);

    /* INT 15h stays an original SoftPC BIOS handler: under the selected PM
       profile it returns the original CMOS-backed extended-memory value. */
    c_setAH(INT15_EMS_DETERMINE);
    assert(softpc_device_bop_dispatch(BIOS_CASSETTE_IO, 0u) == TRUE);
    assert(c_getAX() == 0x3c00u); /* 16 MiB fixed RAM less the first MiB */

    /* The original reset BOP reinitialises machine controllers, not a
       session/product shell.  The raw-image GFI attachment survives it. */
    assert(softpc_device_bop_dispatch(BIOS_RESET, 0u) == TRUE);
    assert(gfi_drive_type(0) == GFI_DRIVE_TYPE_144);

    /* BOP 18 is original ROM BASIC fallback firmware, unlike product
       selectors intentionally absent from the standalone machine table. */
    assert(softpc_device_bop_dispatch(BIOS_BASIC, 0u) == TRUE);

    c_setAL(1u);
    c_setDS(0u);
    c_setCX(0x600u);
    assert(softpc_device_bop_dispatch(0x21u, 0u) == TRUE);
    {
        unsigned char name[5];
        assert(softpc_machine_read_physical(machine, 0x600u, name,
            sizeof(name)) == SOFTPC_MACHINE_OK);
        assert(name[0] == 'B' && name[1] == 'a' && name[2] == 's' &&
            name[3] == 'e' && name[4] == '$');
    }
    assert(c_getAX() == 0u);
    assert(c_getBX() == BUILD_ID_CODE);

    /* Product-shell selectors must remain unreachable in the standalone
       machine even though the historical BOP instruction remains its ROM-to-C
       firmware bridge. */
    assert(softpc_device_bop_dispatch(0x25u, 0u) == FALSE); /* VDD */
    assert(softpc_device_bop_dispatch(0x2bu, 0u) == FALSE); /* DOS command */
    assert(softpc_device_bop_dispatch(0x30u, 0u) == FALSE); /* DPMI */

    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
