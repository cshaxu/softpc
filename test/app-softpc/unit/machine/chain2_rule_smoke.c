/* C-VID rule registers use native IUH; the CPU fixture uses guest-width IUH. */
#define CVIDC_RULE_WORD 1
#include "lib/types/types_interface.h"
#include "insignia.h"
#include "host_def.h"
#include "j_c_lang.h"
#include "cpu_vid.h"
#include "egacpu.h"
#include "egaports.h"
#undef const
#include <assert.h>

extern IHP Gdp;
extern IU8 *EGA_planes;
extern EVID_WRT_POINTERS c_ev_write_ptr, chain2_evid[];
extern EVID_READ_POINTERS read_mode0_evid[], read_mode1_evid[];

void softpc_test_chain2_continuations(void)
{
    static lib_u8 expected[4u * EGA_PLANE_SIZE];
    lib_u8 input[128], output[64], first[64];
    EVID_WRT_POINTERS saved = c_ev_write_ptr;
    lib_u32 mode, parity, slot, pass, flag, i, offset;
    for (i = 0; i < sizeof(input); ++i) input[i] = (lib_u8)(i * 17u + 0x35u);
    for (mode = 0; mode < 4; ++mode)
    for (parity = 0; parity < 4; ++parity) {
        offset = 0x100u + parity;
        c_ev_write_ptr = chain2_evid[mode];
        /* The former crash: an odd word continuation returns into dword.
           Compare with ModeX's actual two-word decomposition, stride 4.
           Copy has an independent inline four-byte algorithm; exercise its
           continuation below without assuming this ModeX decomposition.
           Upper bytes are zero: no change to legacy value-width semantics. */
        lib_memory_set(EGA_planes, 0x5a, sizeof(expected));
        r1 = (IUH)Gdp; r2 = offset; r3 = 0xa5;
        c_ev_write_ptr.d_write(offset, 0xa5);
        lib_memory_copy(expected, EGA_planes, sizeof(expected));
        lib_memory_set(EGA_planes, 0x5a, sizeof(expected));
        r1 = (IUH)Gdp; r2 = offset; r3 = 0xa5;
        c_ev_write_ptr.w_write(offset, 0xa5);
        r1 = (IUH)Gdp; r2 = offset + 4u; r3 = 0;
        c_ev_write_ptr.w_write(offset + 4u, 0);
        assert(lib_memory_compare(expected, EGA_planes, sizeof(expected)) == 0);
    }
    for (mode = 0; mode < 5; ++mode)
    for (parity = 0; parity < 4; ++parity) {
        offset = 0x100u + parity;
        c_ev_write_ptr = chain2_evid[mode];
        /* Exercise all compound entries with valid native RAM pointers.
           Replay checks stable output and a scalar sentinel checks that nested
           returns leave the next ordinary call usable. No missing video-source
           bulk callback is manufactured by this fixture. */
        for (slot = 0; slot < 12; ++slot)
        for (flag = 1; flag <= (mode == 4 && slot >= 6 ? 2u : 1u); ++flag)
        for (pass = 0; pass < 2; ++pass) {
            IBOOL in_ram = flag == 1;
            IHPE source = in_ram ? (IHPE)(input + ((slot & 1u) ? 64u : 32u) + (parity & 1u)) : (IHPE)0x700u;
            for (i = 0; i < sizeof(expected); ++i)
                EGA_planes[i] = (lib_u8)(i * 37u + (i >> 3));
            r1 = (IUH)Gdp; r2 = offset; r3 = 0xa5; r4 = 4; r5 = in_ram;
            if (slot >= 6) r3 = (IUH)source;
            switch (slot) {
            case 0: c_ev_write_ptr.b_write(offset, 0xa5); break;
            case 1: c_ev_write_ptr.w_write(offset, 0xa5); break;
            case 2: c_ev_write_ptr.d_write(offset, 0xa5); break;
            case 3: c_ev_write_ptr.b_fill(offset, 0xa5, 4); break;
            case 4: c_ev_write_ptr.w_fill(offset, 0xa5, 4); break;
            case 5: c_ev_write_ptr.d_fill(offset, 0xa5, 4); break;
            case 6: c_ev_write_ptr.b_fwd_move(offset, source, 4, in_ram); break;
            case 7: c_ev_write_ptr.b_bwd_move(offset, source, 4, in_ram); break;
            case 8: c_ev_write_ptr.w_fwd_move(offset, source, 4, in_ram); break;
            case 9: c_ev_write_ptr.w_bwd_move(offset, source, 4, in_ram); break;
            case 10: c_ev_write_ptr.d_fwd_move(offset, source, 4, in_ram); break;
            case 11: c_ev_write_ptr.d_bwd_move(offset, source, 4, in_ram); break;
            }
            r1 = (IUH)Gdp; r2 = 0x500; r3 = 0x69;
            c_ev_write_ptr.b_write(0x500, 0x69);
            if (!pass) lib_memory_copy(expected, EGA_planes, sizeof(expected));
            else assert(lib_memory_compare(expected, EGA_planes, sizeof(expected)) == 0);
        }
    }
    for (i = 0; i < sizeof(expected); ++i)
        EGA_planes[i] = (lib_u8)(i * 37u + (i >> 3));
    for (mode = 0; mode < 2; ++mode)
    for (parity = 0; parity < 4; ++parity)
    for (slot = 0; slot < 5; ++slot)
    for (flag = 0; flag < (slot >= 3 ? 2u : 1u); ++flag) {
        EVID_READ_POINTERS reader = mode ? read_mode1_evid[1] : read_mode0_evid[1];
        lib_u32 first_value = 0;
        offset = 0x100u + parity;
        for (pass = 0; pass < 2; ++pass) {
            lib_memory_set(output, 0xcc, sizeof(output));
            r1 = (IUH)Gdp; r2 = (IUH)(output + 24u);
            r3 = offset; r4 = 4; r5 = flag;
            switch (slot) {
            case 0: reader.b_read(offset); break;
            case 1: reader.w_read(offset); break;
            case 2: reader.d_read(offset); break;
            case 3: reader.str_fwd_read(output + 24u, offset, 4); break;
            case 4: reader.str_bwd_read(output + 24u, offset, 4); break;
            }
            if (!pass) {
                first_value = (lib_u32)r2;
                lib_memory_copy(first, output, sizeof(first));
            } else {
                if (slot < 3) assert(first_value == (lib_u32)r2);
                assert(lib_memory_compare(first, output, sizeof(first)) == 0);
            }
            for (i = 0; i < 8; ++i) {
                assert(output[i] == 0xcc);
                assert(output[sizeof(output) - 1u - i] == 0xcc);
            }
        }
    }
    c_ev_write_ptr = saved;
}
