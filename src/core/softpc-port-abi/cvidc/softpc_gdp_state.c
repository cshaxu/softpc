#include "insignia.h"
#include "evid_c.h"
#include "softpc_gdp_state.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern IHP Gdp;

/* Generated C-VID reaches more distinct historical offsets during controller
 * initialization than occur in the static header.  Slot storage itself is
 * separately allocated, so growing this index never invalidates a returned
 * field address. */
#define SOFTPC_GDP_INITIAL_SLOT_CAPACITY 128u
#define SOFTPC_GDP_MAGIC 0x47504453u

typedef struct softpc_gdp_slot_record {
    unsigned int original_offset;
    size_t native_width;
    void *storage;
} softpc_gdp_slot_record;

typedef struct softpc_gdp_state {
    uint32_t magic;
    unsigned int count;
    unsigned int capacity;
    softpc_gdp_slot_record *slots;
} softpc_gdp_state;

static size_t softpc_gdp_vga_native_offset(unsigned int original_offset)
{
    switch (original_offset) {
    case 1280u: return offsetof(struct VGAGLOBALSETTINGS, latches);
    case 1284u: return offsetof(struct VGAGLOBALSETTINGS, VGA_rplane);
    case 1288u: return offsetof(struct VGAGLOBALSETTINGS, VGA_wplane);
    case 1292u: return offsetof(struct VGAGLOBALSETTINGS, scratch);
    case 1296u: return offsetof(struct VGAGLOBALSETTINGS, sr_masked_val);
    case 1300u: return offsetof(struct VGAGLOBALSETTINGS, sr_nmask);
    case 1304u: return offsetof(struct VGAGLOBALSETTINGS, data_and_mask);
    case 1308u: return offsetof(struct VGAGLOBALSETTINGS, data_xor_mask);
    case 1312u: return offsetof(struct VGAGLOBALSETTINGS, latch_xor_mask);
    case 1316u: return offsetof(struct VGAGLOBALSETTINGS, bit_prot_mask);
    case 1320u: return offsetof(struct VGAGLOBALSETTINGS, plane_enable);
    case 1324u: return offsetof(struct VGAGLOBALSETTINGS, plane_enable_mask);
    case 1328u: return offsetof(struct VGAGLOBALSETTINGS, sr_lookup);
    case 1332u: return offsetof(struct VGAGLOBALSETTINGS, fwd_str_read_addr);
    case 1336u: return offsetof(struct VGAGLOBALSETTINGS, bwd_str_read_addr);
    case 1340u: return offsetof(struct VGAGLOBALSETTINGS, dirty_total);
    case 1344u: return offsetof(struct VGAGLOBALSETTINGS, dirty_low);
    case 1348u: return offsetof(struct VGAGLOBALSETTINGS, dirty_high);
    case 1352u: return offsetof(struct VGAGLOBALSETTINGS, video_copy);
    case 1356u: return offsetof(struct VGAGLOBALSETTINGS, mark_byte);
    case 1360u: return offsetof(struct VGAGLOBALSETTINGS, mark_word);
    case 1364u: return offsetof(struct VGAGLOBALSETTINGS, mark_string);
    case 1368u: return offsetof(struct VGAGLOBALSETTINGS, read_shift_count);
    case 1372u: return offsetof(struct VGAGLOBALSETTINGS, read_mapped_plane);
    case 1376u: return offsetof(struct VGAGLOBALSETTINGS, colour_comp);
    case 1380u: return offsetof(struct VGAGLOBALSETTINGS, dont_care);
    case 1384u: return offsetof(struct VGAGLOBALSETTINGS, v7_bank_vid_copy_off);
    /* The original 32-bit C-VID name at this offset is
     * video_base_lin_addr.  The selected CCPU owns the shared aggregate and
     * represents the same slot as video_base_ls0 at native pointer width. */
    case 1388u: return offsetof(struct VGAGLOBALSETTINGS, video_base_ls0);
    case 1392u: return offsetof(struct VGAGLOBALSETTINGS, route_reg1);
    case 1396u: return offsetof(struct VGAGLOBALSETTINGS, route_reg2);
    case 1400u: return offsetof(struct VGAGLOBALSETTINGS, screen_ptr);
    case 1404u: return offsetof(struct VGAGLOBALSETTINGS, rotate);
    case 1408u: return offsetof(struct VGAGLOBALSETTINGS, calc_data_xor);
    case 1412u: return offsetof(struct VGAGLOBALSETTINGS, calc_latch_xor);
    case 1416u: return offsetof(struct VGAGLOBALSETTINGS, read_byte_addr);
    case 1420u: return offsetof(struct VGAGLOBALSETTINGS, v7_fg_latches);
    case 1424u: return offsetof(struct VGAGLOBALSETTINGS, GCRegs);
    case 1428u: return offsetof(struct VGAGLOBALSETTINGS, lastGCindex);
    case 1429u: return offsetof(struct VGAGLOBALSETTINGS, dither);
    case 1430u: return offsetof(struct VGAGLOBALSETTINGS, wrmode);
    case 1431u: return offsetof(struct VGAGLOBALSETTINGS, chain);
    case 1432u: return offsetof(struct VGAGLOBALSETTINGS, wrstate);
    default: return (size_t)-1;
    }
}

void *softpc_gdp_create(void)
{
    softpc_gdp_state *state = (softpc_gdp_state *)calloc(1u, sizeof(*state));

    if (state != NULL) state->magic = SOFTPC_GDP_MAGIC;
    return state;
}

void softpc_gdp_destroy(void *value)
{
    softpc_gdp_state *state = (softpc_gdp_state *)value;
    unsigned int index;

    if (state == NULL || state->magic != SOFTPC_GDP_MAGIC) return;
    for (index = 0u; index < state->count; ++index) free(state->slots[index].storage);
    free(state->slots);
    state->magic = 0u;
    free(state);
}

void softpc_gdp_destroy_global(void)
{
    softpc_gdp_destroy(Gdp);
    Gdp = (IHP)0;
}

void *softpc_gdp_slot(const void *value, unsigned int original_offset,
    size_t native_width)
{
    softpc_gdp_state *state = (softpc_gdp_state *)value;
    softpc_gdp_slot_record *slot;
    unsigned int index;

    if (state == NULL || state->magic != SOFTPC_GDP_MAGIC || native_width == 0u)
        return NULL;
    for (index = 0u; index < state->count; ++index) {
        slot = &state->slots[index];
        if (slot->original_offset != original_offset) continue;
        return slot->native_width == native_width ? slot->storage : NULL;
    }
    if (state->count == state->capacity) {
        unsigned int new_capacity = state->capacity == 0u
            ? SOFTPC_GDP_INITIAL_SLOT_CAPACITY : state->capacity * 2u;
        softpc_gdp_slot_record *new_slots = (softpc_gdp_slot_record *)realloc(
            state->slots, new_capacity * sizeof(*new_slots));

        if (new_slots == NULL) return NULL;
        state->slots = new_slots;
        state->capacity = new_capacity;
    }
    slot = &state->slots[state->count];
    slot->storage = calloc(1u, native_width);
    if (slot->storage == NULL) return NULL;
    slot->original_offset = original_offset;
    slot->native_width = native_width;
    ++state->count;
    return slot->storage;
}

void *softpc_gdp_rule_slot(void *state, unsigned int original_offset,
    size_t native_width)
{
    size_t member_offset = softpc_gdp_vga_native_offset(original_offset);

    if (member_offset != (size_t)-1) {
        void *vga = softpc_gdp_slot(state, 1280u,
            sizeof(struct VGAGLOBALSETTINGS));
        if (vga == NULL) return NULL;
        return (unsigned char *)vga + member_offset;
    }
    return softpc_gdp_slot(state, original_offset, native_width);
}

void *softpc_gdp_rule_address(void *state, uintptr_t original_address,
    size_t native_width)
{
    /* All source-derived GDP offsets fit below 4 KiB.  Generated C-VID rules
     * also carry native framebuffer and callback pointers in registers, which
     * must remain direct host addresses. */
    if (original_address < 4096u)
        return softpc_gdp_slot(state, (unsigned int)original_address, native_width);
    return (void *)original_address;
}
