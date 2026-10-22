#ifndef KVM_FRAME_INTERFACE_H
#define KVM_FRAME_INTERFACE_H

#include "lib/types/types_interface.h"

/* Shared text fields only. Capacity is 80x50, not a default visible size.
 * Each stored row occupies KVM_TEXT_COLUMNS cells,
 * even when fewer columns are visible. Palette entries are 0x00RRGGBB.
 * Glyph indices select resources supplied by the receiving leaf's frame.
 * Zero dimensions are invalid; nonzero extents beyond this fixed capacity
 * are unsupported. Off-surface/hidden cursors do not invalidate a frame.
 * font_height == 0 means 16 scanlines. Underline uses the cell foreground at
 * underline_row; an out-of-height position draws nothing. Cursor spans are
 * [start, start+height), clipped to the font; zero height is empty. Their union
 * is one cursor. DISABLED hides; HOST delegates blinking; SOURCE supplies phase.
 * Window uses exactly one phase owner. Console approximates shape and uses its
 * native host blinking for both enabled modes, ignoring source phase. */
#define KVM_TEXT_COLUMNS 80u
#define KVM_TEXT_ROWS 50u
#define KVM_TEXT_FONT_DEFAULT_HEIGHT 16u

typedef struct kvm_text_cell {
    lib_u8 glyph_index;
    lib_u8 glyph_bank; /* 0 primary, 1 secondary */
    lib_u8 foreground; /* palette index 0..15 */
    lib_u8 background; /* palette index 0..15 */
    lib_u8 underline; /* 0 disabled, 1 enabled */
} kvm_text_cell;

_Static_assert(sizeof(kvm_text_cell) == 5u, "Text cells must have no padding");

typedef struct kvm_text_cursor_span {
    lib_u32 start;
    lib_u32 height;
} kvm_text_cursor_span;

typedef enum kvm_cursor_mode {
    KVM_CURSOR_DISABLED = 0,
    KVM_CURSOR_HOST = 1,
    KVM_CURSOR_SOURCE = 2
} kvm_cursor_mode;

typedef struct kvm_text_frame {
    lib_u16 text_columns;
    lib_u16 text_rows;
    lib_i32 cursor_column;
    lib_i32 cursor_row;
    kvm_text_cursor_span cursor[2];
    lib_u32 underline_row;
    kvm_cursor_mode cursor_mode;
    lib_bool cursor_phase; /* SOURCE only; HOST owns its own phase. */
    kvm_text_cell cells[KVM_TEXT_COLUMNS * KVM_TEXT_ROWS];
    lib_u32 text_palette[16u]; /* 0x00RRGGBB */
    lib_u32 font_height;
} kvm_text_frame;

/* Sorted, clipped disjoint spans; adjacent/overlapping spans merge. Leaves own
 * pixel/percentage conversion and phase. Return 0, 1 or 2 resolved spans. */
static inline lib_u32 kvm_text_cursor_shape(const kvm_text_frame *frame,
    lib_u32 *height, kvm_text_cursor_span spans[2])
{
    lib_u32 count = 0u;
    if (frame == LIB_NULL || height == LIB_NULL || spans == LIB_NULL) return 0u;
    *height = frame->font_height ? frame->font_height : KVM_TEXT_FONT_DEFAULT_HEIGHT;
    if (frame->cursor_mode == KVM_CURSOR_DISABLED || frame->cursor_column < 0 ||
        frame->cursor_row < 0 || frame->cursor_column >= frame->text_columns ||
        frame->cursor_row >= frame->text_rows) return 0u;
    for (lib_u32 i = 0u; i < 2u; ++i) {
        kvm_text_cursor_span span = frame->cursor[i];
        if (span.height == 0u || span.start >= *height) continue;
        if (span.height > *height - span.start) span.height = *height - span.start;
        spans[count++] = span;
    }
    if (count == 2u) {
        if (spans[0].start > spans[1].start) {
            kvm_text_cursor_span swap = spans[0];
            spans[0] = spans[1]; spans[1] = swap;
        }
        if (spans[1].start <= spans[0].start + spans[0].height) {
            lib_u32 end = spans[1].start + spans[1].height;
            if (end > spans[0].start + spans[0].height)
                spans[0].height = end - spans[0].start;
            count = 1u;
        }
    }
    return count;
}

static inline lib_status kvm_text_frame_validate(const kvm_text_frame *frame)
{
    if (frame == LIB_NULL || frame->text_columns == 0u || frame->text_rows == 0u)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (frame->cursor_mode != KVM_CURSOR_DISABLED &&
        frame->cursor_mode != KVM_CURSOR_HOST && frame->cursor_mode != KVM_CURSOR_SOURCE)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (frame->text_columns > KVM_TEXT_COLUMNS || frame->text_rows > KVM_TEXT_ROWS)
        return LIB_STATUS_UNSUPPORTED;
    for (lib_size row = 0u; row < frame->text_rows; ++row) {
        for (lib_size column = 0u; column < frame->text_columns; ++column) {
            const kvm_text_cell *cell = &frame->cells[row * KVM_TEXT_COLUMNS + column];
            if (cell->foreground > 15u || cell->background > 15u ||
                cell->glyph_bank > 1u || cell->underline > 1u) return LIB_STATUS_INVALID_ARGUMENT;
        }
    }
    return LIB_STATUS_OK;
}

#endif
