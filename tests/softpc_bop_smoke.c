#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "bios.h"
#include "build_id.h"
#include "gfi.h"
#include "ios.h"
#include "tape_io.h"

enum {
    SOFTPC_COM1_PORT = 0x3f8,
    SOFTPC_RS232_LSR = 5,
    SOFTPC_RS232_MSR = 6,
    SOFTPC_LPT1_PORT = 0x378,
    SOFTPC_EQUIPMENT_BDA = 0x410,
    SOFTPC_TIMER_LOW_BDA = 0x46c,
    SOFTPC_TIMER_HIGH_BDA = 0x46e
};

extern IBOOL softpc_device_bop_dispatch(IU8 number, IU32 argument);
extern void c_setAL(IU8 value);
extern void c_setAH(IU8 value);
extern void c_setCX(IU16 value);
extern void c_setDX(IU16 value);
extern ISM32 c_setDS(IU16 value);
extern IU16 c_getAX(void);
extern IU16 c_getBX(void);
extern void reset(void);
extern void keyboard_int(void);
extern void keyboard_io(void);
extern void diskette_int(void);
extern void diskette_io(void);
extern void video_io(void);
extern void ega_video_io(void);
extern void printer_io(void);
extern void rs232_io(void);
extern void disk_io(void);
extern void equipment(void);
extern void memory_size(void);
extern void time_of_day(void);
extern void bootstrap(void);

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
    /* CPU40's original BOP 08 is illegal; timer IRQ handling stays in the
       original controller path rather than acquiring a new BIOS bridge. */
    assert(BIOS[BIOS_TIMER_INT] == NULL);
    assert(BIOS[BIOS_TIME_OF_DAY] != NULL);
    assert(BIOS[BIOS_BOOT_STRAP] != NULL);
    assert(BIOS[BIOS_MOUSE_INSTALL1] != NULL);
    assert(BIOS[BIOS_MOUSE_VIDEO_IO] != NULL);

    /* The fixed-profile BOP table must remain a direct routing table to the
       original machine code.  A wrapper at any of these slots would be a
       replacement controller/firmware layer, rather than the permitted
       ROM-to-C bridge. */
    assert(BIOS[BIOS_RESET] == reset);
    assert(BIOS[BIOS_KB_INT] == keyboard_int);
    assert(BIOS[BIOS_KEYBOARD_IO] == keyboard_io);
    assert(BIOS[BIOS_DISKETTE_INT] == diskette_int);
    assert(BIOS[BIOS_DISKETTE_IO] == diskette_io);
    assert(BIOS[BIOS_VIDEO_IO] == video_io);
    assert(BIOS[0x42] == ega_video_io);
    assert(BIOS[BIOS_PRINTER_IO] == printer_io);
    assert(BIOS[BIOS_RS232_IO] == rs232_io);
    assert(BIOS[BIOS_DISK_IO] == disk_io);
    assert(BIOS[BIOS_EQUIPMENT] == equipment);
    assert(BIOS[BIOS_MEMORY_SIZE] == memory_size);
    assert(BIOS[BIOS_TIME_OF_DAY] == time_of_day);
    assert(BIOS[BIOS_BOOT_STRAP] == bootstrap);

    /* BOP 01 is the original BIOS dummy interrupt, not a product service. */
    assert(softpc_device_bop_dispatch(BIOS_DUMMY_INT, 0u) == TRUE);
    assert(softpc_device_bop_dispatch(BIOS_TIMER_INT, 0u) == FALSE);

    /* INT 15h stays an original SoftPC BIOS handler: under the selected PM
       profile it returns the original CMOS-backed extended-memory value. */
    c_setAH(INT15_EMS_DETERMINE);
    assert(softpc_device_bop_dispatch(BIOS_CASSETTE_IO, 0u) == TRUE);
    assert(c_getAX() == 0x3c00u); /* 16 MiB fixed RAM less the first MiB */

    /* The original reset BOP reinitialises machine controllers, not a
       session/product shell.  The raw-image GFI attachment survives it. */
    assert(softpc_device_bop_dispatch(BIOS_RESET, 0u) == TRUE);
    assert(gfi_drive_type(0) == GFI_DRIVE_TYPE_144);

    /* INT 11h and INT 12h are the original BIOS-data-area handlers.  The
       values must remain guest-owned and writable through the machine RAM
       boundary, rather than being rebuilt from standalone host options. */
    {
        const unsigned char equipment_word[2] = { 0x5au, 0xa5u };
        const unsigned char memory_word[2] = { 0x80u, 0x02u };
        assert(softpc_machine_write_physical(machine, SOFTPC_EQUIPMENT_BDA,
            equipment_word, sizeof(equipment_word)) == SOFTPC_MACHINE_OK);
        assert(softpc_machine_write_physical(machine, MEMORY_VAR,
            memory_word, sizeof(memory_word)) == SOFTPC_MACHINE_OK);
    }
    assert(softpc_device_bop_dispatch(BIOS_EQUIPMENT, 0u) == TRUE);
    assert(c_getAX() == 0xa55au);
    assert(softpc_device_bop_dispatch(BIOS_MEMORY_SIZE, 0u) == TRUE);
    assert(c_getAX() == 0x0280u);

    /* INT 1Ah's original set-time service owns the BIOS timer fields and
       updates its host-clock port; it must not be a product/session clock. */
    c_setAH(0x01u);
    c_setCX(0x1234u);
    c_setDX(0x5678u);
    assert(softpc_device_bop_dispatch(BIOS_TIME_OF_DAY, 0u) == TRUE);
    {
        const unsigned char expected_low[2] = { 0x78u, 0x56u };
        const unsigned char expected_high[2] = { 0x34u, 0x12u };
        unsigned char timer_low[2];
        unsigned char timer_high[2];
        assert(softpc_machine_read_physical(machine, SOFTPC_TIMER_LOW_BDA,
            timer_low, sizeof(timer_low)) == SOFTPC_MACHINE_OK);
        assert(softpc_machine_read_physical(machine, SOFTPC_TIMER_HIGH_BDA,
            timer_high, sizeof(timer_high)) == SOFTPC_MACHINE_OK);
        assert(memcmp(timer_low, expected_low, sizeof(timer_low)) == 0);
        assert(memcmp(timer_high, expected_high, sizeof(timer_high)) == 0);
    }

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

    /* INT 16h remains the original non-NTVDM keyboard BIOS service.  Its
       XT-SFD insertion function feeds the same BIOS ring that the standard
       status and read functions consume. */
    c_setAH(0x05u);
    c_setCX(0x1e61u); /* scan code 1Eh, ASCII 'a' */
    assert(softpc_device_bop_dispatch(BIOS_KEYBOARD_IO, 0u) == TRUE);
    assert(c_getAX() == 0u);
    c_setAH(0x01u);
    assert(softpc_device_bop_dispatch(BIOS_KEYBOARD_IO, 0u) == TRUE);
    assert(c_getAX() == 0x1e61u);
    c_setAH(0x00u);
    assert(softpc_device_bop_dispatch(BIOS_KEYBOARD_IO, 0u) == TRUE);
    assert(c_getAX() == 0x1e61u);

    /* INT 14h remains the original ROM BOP service over the original UART
       controller. Its status result must agree with the direct UART ports. */
    c_setDX(0u);
    c_setAH(3u);
    assert(softpc_device_bop_dispatch(BIOS_RS232_IO, 0u) == TRUE);
    {
        half_word line_status;
        half_word modem_status;
        inb(SOFTPC_COM1_PORT + SOFTPC_RS232_LSR, &line_status);
        inb(SOFTPC_COM1_PORT + SOFTPC_RS232_MSR, &modem_status);
        /* Reading the original MSR clears its delta bits, so only compare
           the non-destructive LSR return and the stable CTS state. */
        assert((c_getAX() >> 8u) == line_status);
        assert((c_getAX() & 0x10u) != 0u);
        assert((modem_status & 0x10u) != 0u);
    }
    c_setAH(1u);
    c_setAL(0x41u);
    assert(softpc_device_bop_dispatch(BIOS_RS232_IO, 0u) == TRUE);
    assert((c_getAX() & 0x6000u) == 0x6000u);

    /* INT 17h likewise remains a ROM BOP over the original parallel
       controller. BIOS formatting of the status byte must match the port. */
    c_setDX(0u);
    c_setAH(2u);
    assert(softpc_device_bop_dispatch(BIOS_PRINTER_IO, 0u) == TRUE);
    {
        half_word raw_status;
        inb(SOFTPC_LPT1_PORT + 1u, &raw_status);
        assert((c_getAX() >> 8u) == ((raw_status & 0xf8u) ^ 0x48u));
    }
    c_setAH(0u);
    c_setAL(0x41u);
    assert(softpc_device_bop_dispatch(BIOS_PRINTER_IO, 0u) == TRUE);

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
