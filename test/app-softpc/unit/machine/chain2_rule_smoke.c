/* C-VID rule registers use native IUH; the CPU fixture uses guest-width IUH. */
#define CVIDC_RULE_WORD 1
#include "lib/types/types_interface.h"
#include "insignia.h"
#include "host_def.h"
#include "j_c_lang.h"
#include "cpu_vid.h"
#include "egacpu.h"
#include "egaports.h"
#include "cvidc/gdp_rule_access.h"
#undef const
#include <assert.h>
#include <stdio.h>

extern IHP Gdp;
extern IU8 *EGA_planes;
extern EVID_WRT_POINTERS c_ev_write_ptr, chain2_evid[];
extern EVID_READ_POINTERS read_mode0_evid[], read_mode1_evid[];

static void check_gdp_subfields(void)
{
    const lib_u32 masks[][2] = {
        { 0, 0 }, { 0xffffffffu, 0 }, { 0, 0xffffffffu },
        { 0x12345678u, 0x87654321u }
    };
    IU32 compare = getVideocolour_comp(), care = getVideodont_care();
    IU32 rotate = getVideorotate();
    lib_u32 i;
    for (i = 0; i < sizeof(masks) / sizeof(masks[0]); ++i) {
        lib_u32 expected = masks[i][0] | masks[i][1];
        setVideocolour_comp(masks[i][0]);
        setVideodont_care(masks[i][1]);
        /* Historical subfield offsets address the low-order native value. */
        assert(softpc_gdp_rule_slot(Gdp, 1378, sizeof(IU16)) ==
            softpc_gdp_rule_slot(Gdp, 1376, sizeof(IU32)));
        assert(*(IU8 *)softpc_gdp_rule_slot(Gdp, 1379, sizeof(IU8)) ==
            (IU8)masks[i][0]);
        assert(softpc_gdp_rule_slot(Gdp, 1382, sizeof(IU16)) ==
            softpc_gdp_rule_slot(Gdp, 1380, sizeof(IU32)));
        assert(*(IU8 *)softpc_gdp_rule_slot(Gdp, 1383, sizeof(IU8)) ==
            (IU8)masks[i][1]);
        lib_memory_set(EGA_planes, 0, 0x1000);
        r1 = (IUH)Gdp; r3 = 0x100;
        read_mode1_evid[1].b_read(0x100);
        assert((lib_u8)r2 == (lib_u8)expected);
        r1 = (IUH)Gdp; r3 = 0x100;
        read_mode1_evid[2].w_read(0x100);
        assert((lib_u16)r2 == (lib_u16)expected);
    }
    for (i = 0; i < 8; ++i) {
        setVideorotate(i);
        assert(*(IU8 *)softpc_gdp_rule_slot(Gdp, 1407, sizeof(IU8)) == i);
    }
    setVideocolour_comp(compare); setVideodont_care(care);
    setVideorotate(rotate);
}

static void check_unchained_dword_reads(void)
{
    const lib_u32 offsets[] = { 0x100, 0x101, 0x103, 0x107, 0x3fff };
    IU32 shift = getVideoread_shift_count();
    lib_u32 plane, entry, i;
    for (plane = 0; plane < 4; ++plane)
    for (entry = 0; entry < sizeof(offsets) / sizeof(offsets[0]); ++entry) {
        lib_u32 offset = offsets[entry];
        setVideoread_shift_count(plane * 8u);
        /* Byte 8/9 differ from byte 2/3, exposing the old +8 guest offset. */
        for (i = 0; i < 12; ++i)
            EGA_planes[4u * (offset + i) + plane] = (lib_u8)(0x11u * (i + 1u));
        r1 = (IUH)Gdp; r3 = offset;
        read_mode0_evid[0].w_read(offset);
        assert((lib_u16)r2 == 0x2211u);
        r1 = (IUH)Gdp; r3 = offset; r2 = 0xa5a5a5a5u;
        read_mode0_evid[0].d_read(offset);
        assert((lib_u32)r2 == 0x44332211u);
    }
    setVideoread_shift_count(shift);
}

static void check_chain2_dword_reads(void)
{
    const lib_u32 offsets[] = { 0x100, 0x101, 0x103, 0x107, 0x3fff, 0xfffd };
    const lib_u8 bytes[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
    IU32 compare = getVideocolour_comp(), care = getVideodont_care();
    lib_u32 mode, entry, i;
    for (mode = 0; mode < 2; ++mode)
    for (entry = 0; entry < sizeof(offsets) / sizeof(offsets[0]); ++entry) {
        lib_u32 offset = offsets[entry];
        lib_u32 expected = mode ? 0x40302010u : 0x44332211u;
        EVID_READ_POINTERS reader = mode ? read_mode1_evid[1] : read_mode0_evid[1];
        setVideocolour_comp(0);
        setVideodont_care(0);
        /* Independent odd/even plane locations; later bytes catch a +4 skip.
           Mode 1 intersects each byte with its paired plane (0xf0). */
        for (i = 0; i < sizeof(bytes); ++i) {
            lib_u32 address = offset + i;
            lib_u32 physical = 4u * (address & ~1u) + (address & 1u);
            EGA_planes[physical] = bytes[i];
            EGA_planes[physical + 2u] = 0xf0;
        }
        r1 = (IUH)Gdp; r2 = 0xa5a5a5a5u; r3 = offset;
        reader.d_read(offset);
        if ((lib_u32)r2 != expected)
            fprintf(stderr, "CHAIN2 dword mode=%u offset=%x expected=%08x actual=%08x\n",
                mode, offset, expected, (lib_u32)r2);
        assert((lib_u32)r2 == expected);
    }
    setVideocolour_comp(compare);
    setVideodont_care(care);
}

void softpc_test_chain2_continuations(void)
{
    static lib_u8 expected[4u * EGA_PLANE_SIZE];
    lib_u8 input[128], output[64], first[64];
    EVID_WRT_POINTERS saved = c_ev_write_ptr;
    lib_u32 mode, parity, slot, pass, flag, i, offset;
    check_gdp_subfields();
    check_unchained_dword_reads();
    check_chain2_dword_reads();
    /* Odd/even selects a plane without compacting its address. Independent
       physical offsets distinguish adjacent cells and preserve font planes. */
    lib_memory_set(EGA_planes, 0x5a, sizeof(expected));
    c_ev_write_ptr = chain2_evid[4];
    for (i = 0; i < 8; ++i) {
        r1 = (IUH)Gdp; r2 = 0x100u + i; r3 = 0x30u + i;
        c_ev_write_ptr.b_write(0x100u + i, 0x30u + i);
    }
    for (i = 0; i < 4; ++i) {
        assert(EGA_planes[0x400u + i * 8u] == 0x30u + i * 2u);
        assert(EGA_planes[0x401u + i * 8u] == 0x31u + i * 2u);
        assert(EGA_planes[0x402u + i * 8u] == 0x5a);
        assert(EGA_planes[0x404u + i * 8u] == 0x5a);
        r1 = (IUH)Gdp; r3 = 0x100u + i * 2u;
        read_mode0_evid[1].w_read(0x100u + i * 2u);
        assert((lib_u16)r2 == (lib_u16)((0x31u + i * 2u) * 256u + 0x30u + i * 2u));
    }
    for (i = 0; i < 7; ++i) {
        r1 = (IUH)Gdp; r3 = 0x100u + i;
        read_mode0_evid[1].w_read(0x100u + i);
        assert((lib_u16)r2 == (lib_u16)((0x31u + i) * 256u + 0x30u + i));
    }
    /* String readers must traverse the same cells as scalar reads. */
    for (mode = 0; mode < 2; ++mode)
    for (parity = 0; parity < 2; ++parity)
    for (slot = 0; slot < 2; ++slot) {
        EVID_READ_POINTERS reader = mode ? read_mode1_evid[1] : read_mode0_evid[1];
        offset = slot ? 0x107u - parity : 0x100u + parity;
        for (i = 0; i < 4; ++i) {
            lib_u32 address = slot ? offset - i : offset + i;
            r1 = (IUH)Gdp; r3 = address;
            reader.b_read(address);
            first[i] = (lib_u8)r2;
        }
        lib_memory_set(output, 0xcc, sizeof(output));
        r1 = (IUH)Gdp; r2 = (IUH)(output + 24); r3 = offset; r4 = 4; r5 = slot;
        if (slot) reader.str_bwd_read(output + 24, offset, 4);
        else reader.str_fwd_read(output + 24, offset, 4);
        if (lib_memory_compare(first, output + 24, 4) != 0)
            fprintf(stderr, "string read mode=%u parity=%u backward=%u expected=%02x actual=%02x\n", mode, parity, slot, first[0], output[24]);
        assert(lib_memory_compare(first, output + 24, 4) == 0);
    }
    /* Independent physical-plane oracle for the registered byte/word Copy fills.
       Legacy odd dword fill value assembly is tracked separately in S33 evidence. */
    for (parity = 0; parity < 2; ++parity)
    for (slot = 0; slot < 2; ++slot) {
        lib_u32 width = 1u << slot;
        lib_memory_set(EGA_planes, 0x5a, sizeof(expected));
        r1 = (IUH)Gdp; r2 = 0x100u + parity; r3 = 0x11111111u; r4 = 8u / width;
        if (slot == 0) c_ev_write_ptr.b_fill(0x100u + parity, 0x11, 8);
        else c_ev_write_ptr.w_fill(0x100u + parity, 0x1111, 4);
        for (i = 0; i < 8; ++i) {
            lib_u32 address = 0x100u + parity + i;
            if (EGA_planes[4u * (address & ~1u) + (address & 1u)] != (lib_u8)(0x11111111u >> ((i % width) * 8u)))
                fprintf(stderr, "copy fill parity=%u width=%u byte=%u actual=%02x\n", parity, width, i, EGA_planes[4u * (address & ~1u) + (address & 1u)]);
            assert(EGA_planes[4u * (address & ~1u) + (address & 1u)] ==
                (lib_u8)(0x11111111u >> ((i % width) * 8u)));
        }
    }
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
