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

/* Observe the real mapper; only the nested guest handler is substituted.
   This proves selection of fallback, not a guest page-fault handler's policy. */
IBOOL __wrap_xtrn2phy(IU32 address, IUM8 access, IU32 *physical)
{
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
    assert(softpc_machine_destroy(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_test_remove_image(options.floppy_path));
    printf("BIOS page access: %u/%u passed\n", cases - failures, cases);
    return failures != 0;
}
