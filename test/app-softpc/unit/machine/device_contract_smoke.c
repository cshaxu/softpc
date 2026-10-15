#include "lib/types/types_interface.h"
#include "machine/machine.h"
#include "../../fixture_cleanup.h"
#include "insignia.h"
#include "host_def.h"
#include "xt.h"
#include "ios.h"
#include "dma.h"
#include "bios.h"
#include "cmos.h"
#include "sas.h"

#undef const
#include <assert.h>
#include <stdio.h>

extern void disk_io(void);
extern void c_setAX(IU16 value);
extern void c_setCX(IU16 value);
extern void c_setDX(IU16 value);
extern void c_setCF(IBOOL value);
extern IU16 c_getAX(void);
extern IU16 c_getCX(void);
extern IU16 c_getDX(void);
extern ISM32 c_getCF(void);
extern ISM32 c_setSS(IU16 value);
extern ISM32 c_setDS(IU16 value);
extern void c_setSP(IU16 value);
extern void search_for_roms(void);
extern IU8 *c_GetPhyAdd(IU32 address);
extern ISM32 c_setES(IU16 value);
extern void c_setSI(IU16 value);
extern ISM32 c_getZF(void);
extern ISM32 c_getIF(void);
extern void retrieve_descr_fields(half_word *ar, sys_addr *base,
    sys_addr *limit, sys_addr descriptor_address);

static void verify_descriptor(softpc_machine *machine)
{
    static const struct {
        unsigned char bytes[8];
        sys_addr base, limit;
    } cases[] = {
        { { 0xff, 0xff, 0, 0, 0x10, 0x92, 0, 0 }, 0x100000, 0xffff },
        { { 0x45, 0x23, 0, 0, 0x10, 0x92, 1, 0 }, 0x100000, 0x12345 },
        { { 0xff, 0xff, 0, 0, 0, 0x92, 0x8f, 0 }, 0, 0xffffffff }
    };
    unsigned index, wrapping;
    for (index = 0; index != sizeof(cases) / sizeof(cases[0]); ++index) {
        struct { sys_addr before, value, after; } limit = {
            0x13572468, 0xa5a5a5a5, 0x24681357
        };
        sys_addr base = 0;
        half_word ar = 0;
        assert(softpc_machine_write_physical(machine, 0x600,
            cases[index].bytes, 8) == SOFTPC_MACHINE_OK);
        retrieve_descr_fields(&ar, &base, &limit.value, 0x600);
        assert(ar == 0x92 && base == cases[index].base);
        assert(limit.value == cases[index].limit);
        assert(limit.before == 0x13572468 && limit.after == 0x24681357);
    }
    for (wrapping = 0; wrapping != 2; ++wrapping) {
        unsigned char table[32] = { 0 }, source[32], result[32];
        lib_memory_copy(table + 16, cases[0].bytes, 8);
        lib_memory_copy(table + 24, cases[0].bytes, 8);
        table[27] = 0x10; /* destination base 0x101000 */
        for (index = 0; index != sizeof(source); ++index)
            source[index] = (unsigned char)(index + 0x61);
        sas_disable_20_bit_wrapping();
        assert(softpc_machine_write_physical(machine, 0x600, table, 32) ==
            SOFTPC_MACHINE_OK);
        assert(softpc_machine_write_physical(machine, 0x100000, source, 32) ==
            SOFTPC_MACHINE_OK);
        lib_memory_set(result, 0, sizeof(result));
        assert(softpc_machine_write_physical(machine, 0x101000, result, 32) ==
            SOFTPC_MACHINE_OK);
        if (wrapping) sas_enable_20_bit_wrapping();
        c_setES(0);
        c_setSI(0x600);
        c_setAX(0x8700);
        c_setCX(16);
        c_setCF(1);
        cassette_io();
        assert(c_getAX() == 0 && c_getCF() == 0);
        assert(c_getZF() == 1 && c_getIF() == 1);
        assert(!!sas_twenty_bit_wrapping_enabled() == (int)wrapping);
        sas_disable_20_bit_wrapping();
        assert(softpc_machine_read_physical(machine, 0x101000, result, 32) ==
            SOFTPC_MACHINE_OK);
        assert(lib_memory_compare(source, result, 32) == 0);
    }
}

static void verify_cmos(void)
{
    half_word low, high, value;
    unsigned corrupt;
    /* The fixture has more than 256 KiB of extended memory: comparing a
       byte with the unmasked full KiB value must not be the contract. */
    assert((sas_memory_size() - 0x100000) / 1024 > 255);
    cmos_post();
    assert(cmos_read_byte(CMOS_E_M_S_LO, &low) == 0);
    assert(cmos_read_byte(CMOS_E_M_S_HI, &high) == 0);
    assert((unsigned)low + 256u * high ==
        (sas_memory_size() - 0x100000) / 1024);
    for (corrupt = 0; corrupt != 4; ++corrupt) {
        assert(cmos_write_byte(CMOS_E_M_S_LO,
            (half_word)(low ^ ((corrupt & 1) ? 0x5a : 0))) == 0);
        assert(cmos_write_byte(CMOS_E_M_S_HI,
            (half_word)(high ^ ((corrupt & 2) ? 0xa5 : 0))) == 0);
        cmos_post();
        assert(cmos_read_byte(CMOS_E_M_S_LO, &value) == 0 && value == low);
        assert(cmos_read_byte(CMOS_E_M_S_HI, &value) == 0 && value == high);
    }
}

static void dma_word(io_addr port, IU16 value)
{
    outb(port, (half_word)value);
    outb(port, (half_word)(value >> 8));
}

static void verify_dma(softpc_machine *machine)
{
    unsigned controller, decrement, automatic, round, index;
    for (controller = 0; controller != 2; ++controller)
    for (decrement = 0; decrement != 2; ++decrement)
    for (automatic = 0; automatic != 2; ++automatic) {
        const half_word channel = controller ? 5 : 1;
        const io_addr address_port = controller ? 0xc4 : 0x02;
        const io_addr count_port = controller ? 0xc6 : 0x03;
        const io_addr clear_port = controller ? 0xd8 : 0x0c;
        const unsigned unit = controller ? 2 : 1;
        const IU16 initial = 0x2100;
        const lib_u32 physical = (lib_u32)initial * unit;
        unsigned char memory[32], output[8];
        sys_addr address;
        word count;

        for (index = 0; index < sizeof(memory); ++index)
            memory[index] = (unsigned char)(index + 0x31);
        assert(softpc_machine_write_physical(machine, physical - 16,
            memory, sizeof(memory)) == SOFTPC_MACHINE_OK);
        dma_post();
        outb(clear_port, 0);
        dma_word(address_port, initial);
        dma_word(count_port, 3);
        outb(controller ? 0x8b : 0x83, 0);
        /* Single transfer; decrement uses the controller's VERIFY operation
           to isolate reload from the separate legacy reverse-copy helper. */
        outb(controller ? 0xd6 : 0x0b,
            (half_word)((decrement ? 0x41 : 0x49) |
                (decrement << 5) | (automatic << 4)));
        outb(controller ? 0xd4 : 0x0a, 1);
        for (round = 0; round != 2; ++round) {
            lib_memory_set(output, 0, sizeof(output));
            assert(dma_request(channel, (char *)output, 4) == FALSE);
            for (index = 0; index != 4 && !decrement; ++index) {
                unsigned byte;
                for (byte = 0; byte != unit; ++byte)
                    assert(output[index * unit + byte] ==
                        memory[16 + index * unit + byte]);
            }
            dma_enquire(channel, &address, &count);
            if (automatic) {
                assert(address == physical);
                assert(count == 3);
            } else {
                assert(address == physical + (decrement ? -4 * unit : 4 * unit));
                assert(count == 0xffff);
                lib_memory_set(output, 0x5a, sizeof(output));
                /* Terminal count masks the channel: no second transfer. */
                assert(dma_request(channel, (char *)output, 4) == TRUE);
                for (index = 0; index < sizeof(output); ++index)
                    assert(output[index] == 0x5a);
                break;
            }
        }
    }
}

static void verify_disk(softpc_machine *machine)
{
    /* Guest-owned INT 41h parameter table: no real disk or assets required.
       Keep the original reserved-cylinder convention; test only register split. */
    static const struct {
        IU16 cylinders;
        IU8 heads, sectors;
        IU16 high, low;
    } cases[] = {
        { 18, 4, 17, 0, 1156 },
        { 513, 4, 32, 1, 0 },
        { 1024, 16, 63, 15, 0xbc10 }
    };
    const unsigned char vector[4] = { 0, 6, 0, 0 };
    unsigned index;
    assert(softpc_machine_write_physical(machine, 0x104, vector, 4) ==
        SOFTPC_MACHINE_OK);
    for (index = 0; index < sizeof(cases) / sizeof(cases[0]); ++index) {
        unsigned char parameters[16] = { 0 };
        parameters[0] = (unsigned char)cases[index].cylinders;
        parameters[1] = (unsigned char)(cases[index].cylinders >> 8);
        parameters[2] = cases[index].heads;
        parameters[14] = cases[index].sectors;
        assert(softpc_machine_write_physical(machine, 0x600, parameters, 16) ==
            SOFTPC_MACHINE_OK);
        c_setAX(0x1500);
        c_setCX(0xffff);
        c_setDX(0x0080);
        c_setCF(1);
        assert(BIOS[BIOS_DISK_IO] == disk_io);
        BIOS[BIOS_DISK_IO]();
        assert((c_getAX() >> 8) == 3 && c_getCF() == 0);
        assert(c_getCX() == cases[index].high);
        assert(c_getDX() == cases[index].low);
    }
    /* The public dispatcher rejects absent D: before read_dasd is called. */
    c_setAX(0x1500);
    c_setCX(0x5a5a);
    c_setDX(0x0081);
    BIOS[BIOS_DISK_IO]();
    assert(c_getCF() == 1);
    assert(c_getAX() == 0x1500);
    assert(c_getCX() == 0x5a5a && c_getDX() == 0x0081);
}

static void verify_rom(softpc_machine *machine)
{
    /* Minimal option ROM: mov ax,cs; mov [0500],ax; retf. The original
       scanner calls it and returns via the existing BIOS BOP FE trampoline. */
    unsigned char rom[512] = { 0x55, 0xaa, 1, 0x8c, 0xc8,
        0xa3, 0x00, 0x05, 0xcb };
    unsigned char observed[2];
    unsigned sum = 0, index;
    for (index = 0; index != sizeof(rom) - 1; ++index)
        sum += rom[index];
    rom[511] = (unsigned char)(0u - sum);
    /* Firmware installation is a host write, not a guest bus write to ROM. */
    assert(c_GetPhyAdd(0xc8000) != NULL);
    lib_memory_copy(c_GetPhyAdd(0xc8000), rom, sizeof(rom));
    c_setSS(0);
    c_setSP(0x7000);
    c_setDS(0);
    search_for_roms();
    assert(softpc_machine_read_physical(machine, 0x500, observed, 2) ==
        SOFTPC_MACHINE_OK);
    assert(observed[0] == 0 && observed[1] == 0xc0);
}

int main(int argc, char **argv)
{
    const char *path = "device-contract.img";
    softpc_machine_options options = { path, NULL };
    softpc_machine *machine = NULL;
    unsigned char boot[512] = { 0xeb, 0xfe };
    FILE *file;
    assert(argc == 2);
    boot[510] = 0x55;
    boot[511] = 0xaa;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(boot, 1, sizeof(boot), file) == sizeof(boot));
    assert(fclose(file) == 0);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    if (lib_text_compare(argv[1], "dma") == 0)
        verify_dma(machine);
    else if (lib_text_compare(argv[1], "rom") == 0)
        verify_rom(machine);
    else if (lib_text_compare(argv[1], "descriptor") == 0)
        verify_descriptor(machine);
    else if (lib_text_compare(argv[1], "cmos") == 0)
        verify_cmos();
    else {
        assert(lib_text_compare(argv[1], "disk") == 0);
        verify_disk(machine);
    }
    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(path));
    puts("App SoftPC device contract: OK");
    return 0;
}
