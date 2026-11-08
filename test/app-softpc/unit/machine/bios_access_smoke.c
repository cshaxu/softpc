#include "lib/types/types_interface.h"
#include "machine/machine.h"
#include "compat/ccpu/abi.h"
#include "insignia.h"
#include "host_def.h"
#include "sas.h"
#undef const
#include "../../fixture_cleanup.h"
#include <assert.h>
#include <stdio.h>

extern void c_setCPL(ISM32 value);
extern void c_setVM(ISM32 value);
extern void flush_tlb(void);
extern IU8 *c_GetPhyAdd(IU32 address);
extern IBOOL __real_xtrn2phy(IU32 address, IUM8 access, IU32 *physical);
extern void (*host_simulate_func)(void);
static int probing, mapped, translations, fallback;
static IUM8 requested;
static int force_fallback;
extern void c_setLDT_SELECTOR(IU16 value);
extern void c_setLDT_BASE_LIMIT(IU32 base, IU32 limit);
extern IBOOL c_SasRegisterVirtualSelectors(IU16 data, IU16 code);

/* Permission cases observe the real mapper and substitute the nested handler.
   Width cases force one outer miss, then execute the original ROM fallback. */
IBOOL __wrap_xtrn2phy(IU32 address, IUM8 access, IU32 *physical)
{
    if (force_fallback && address == 0x6100) {
        force_fallback = 0;
        return FALSE;
    }
    IBOOL result = __real_xtrn2phy(address, access, physical);
    if (probing && address == 0x400100) {
        ++translations; requested = access; mapped = result;
    }
    return result;
}
static void simulated_guest_handler(void)
{
    assert(probing);
    ++fallback;
    assert(c_getCS() == 0xf000);
    c_setEAX(0x5a);
}

/* Force only the outer mapping attempt to fallback; the real CPU then reads
   valid memory through the unchanged embedded BIOS, without a mock EAX value. */
static unsigned verify_fallback_width(softpc_machine *machine)
{
    static const IU32 values[] = { 0x12345678ul, 0xfedcba98ul };
    unsigned mode, pattern, width, failures = 0;
    softpc_machine_executor_thread_enter(machine);
    for (mode = 0; mode < 2; ++mode) {
        c_setVM(0); c_setCPL(0); c_setEFLAGS(2);
        if (mode) {
            c_setLDT_SELECTOR(8);
            c_setLDT_BASE_LIMIT(0x10000, 0xff);
            (void)c_SasRegisterVirtualSelectors(0x0c, 0x14);
            assert(c_setCS(0x14) == 0 && c_setDS(0x0c) == 0);
        } else {
            c_setVM(1); c_setCPL(3);
            assert(c_setCS(0x1000) == 0 && c_setDS(0) == 0);
        }
        for (pattern = 0; pattern < 2; ++pattern)
        for (width = 1; width <= 4; width *= 2) {
            IU32 result, expected = values[pattern];
            IU32 mask = width == 4 ? 0xfffffffful : (1ul << (width * 8)) - 1;
            IU16 cs = c_getCS(), ds = c_getDS();
            lib_memory_copy(c_GetPhyAdd(0x6100), &expected, 4);
            c_setEAX(0xaabbccdd); c_setEDX(0x13572468);
            c_setEBP(0x24681357); c_setEIP(0x1234);
            flush_tlb(); force_fallback = 1;
            if (width == 1) result = sas_hw_at(0x6100);
            else if (width == 2) result = sas_w_at(0x6100);
            else result = sas_dw_at(0x6100);
            assert(!force_fallback);
            assert(c_getCS() == cs && c_getDS() == ds && c_getEIP() == 0x1234);
            assert(c_getEAX() == 0xaabbccdd && c_getEDX() == 0x13572468 &&
                c_getEBP() == 0x24681357);
            if (result != (expected & mask)) {
                ++failures;
                fprintf(stderr, "BIOS fallback mode=%u width=%u expected=%08lx actual=%08lx\n",
                    mode, width, expected & mask, result);
            }
        }
    }
    c_setVM(0); c_setCPL(0);
    softpc_machine_executor_thread_leave(machine);
    printf("BIOS original-ROM fallback: %u/12 passed\n", 12 - failures);
    return failures;
}

int main(void)
{
    softpc_machine_options options = { "bios-access.img", NULL };
    softpc_machine *machine = NULL;
    IU32 *directory, *pages, *target;
    unsigned state, page, write, width, i, failures = 0, cases = 0;
    unsigned long original_cr0, original_flags;
    unsigned char boot[512] = { 0xeb, 0xfe };
    FILE *file = fopen(options.floppy_path, "wb");
    assert(file != NULL);
    boot[510] = 0x55; boot[511] = 0xaa;
    assert(fwrite(boot, 1, sizeof(boot), file) == sizeof(boot));
    assert(fclose(file) == 0);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    void (*original_simulate)(void) = host_simulate_func;
    host_simulate_func = simulated_guest_handler;
    original_cr0 = c_getCR0(); original_flags = c_getEFLAGS();
    sas_disable_20_bit_wrapping();
    directory = (IU32 *)c_GetPhyAdd(0x300000);
    pages = (IU32 *)c_GetPhyAdd(0x301000);
    target = (IU32 *)c_GetPhyAdd(0x302000);
    lib_memory_set(directory, 0, 4096);
    lib_memory_set(target, 0, 4096);
    for (i = 0; i < 1024; ++i) pages[i] = i * 4096u | 7u;
    directory[0] = 0x301007;
    c_setCR3(0x300000);
    c_setCR0(original_cr0 | 0x80000001ul);
    for (state = 0; state < 5; ++state)
    for (page = 0; page < 4; ++page)
    for (write = 0; write < 2; ++write)
    for (width = 1; width <= 4; width *= 2) {
        unsigned cpl = state == 4 ? 3 : state;
        unsigned user = page >> 1, writable = page & 1;
        int allowed = cpl < 3 || (user && (!write || writable));
        IU32 result = 0, mask = width == 4 ? 0xfffffffful : (1ul << (width * 8)) - 1;
        IU8 *data = c_GetPhyAdd(0x200100);
        directory[1] = 0x302001 | (user << 2) | (writable << 1);
        target[0] = 0x200001 | (user << 2) | (writable << 1);
        flush_tlb();
        c_setEFLAGS((original_flags & ~0x20000ul) | (state == 4 ? 0x20000ul : 0));
        c_setVM(state == 4);
        c_setCPL((ISM32)cpl);
        lib_memory_set(data, 0x5a, 4);
        translations = fallback = 0; probing = 1;
        if (write) {
            if (width == 1) sas_store(0x400100, 0xa5);
            else if (width == 2) sas_storew(0x400100, 0xa5a5);
            else sas_storedw(0x400100, 0xa5a5a5a5);
        } else {
            if (width == 1) result = sas_hw_at(0x400100);
            else if (width == 2) result = sas_w_at(0x400100);
            else result = sas_dw_at(0x400100);
        }
        probing = 0; ++cases;
        if (translations != 1 || requested != (write | (cpl == 3 ? 2 : 0) |
            (state == 4 ? 4 : 0)) || mapped != allowed ||
            fallback != (state == 4 && !allowed) ||
            (!write && allowed && result != (0x5a5a5a5aul & mask)) ||
            (write && data[0] != (allowed ? 0xa5 : 0x5a))) {
            ++failures;
            fprintf(stderr, "BIOS access state=%u page=%u write=%u width=%u request=%u mapped=%d fallback=%d\n",
                state, page, write, width, requested, mapped, fallback);
        }
    }
    c_setVM(0); c_setCPL(0); c_setEFLAGS(original_flags); c_setCR0(original_cr0);
    host_simulate_func = original_simulate;
    c_setCR0(original_cr0 | 0x80000001ul);
    failures += verify_fallback_width(machine);
    c_setCR0(original_cr0);
    assert(softpc_machine_destroy(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_test_remove_image(options.floppy_path));
    printf("BIOS page access: %u cases, combined failures=%u\n", cases, failures);
    return failures != 0;
}
