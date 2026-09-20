#ifndef KVM_FRAME_INTERFACE_H
#define KVM_FRAME_INTERFACE_H

#include "lib/types/types_interface.h"

/* Shared text fields only. Each stored row occupies KVM_TEXT_COLUMNS cells,
 * even when fewer columns are visible. Palette entries are 0x00RRGGBB.
 * Glyph indices select resources supplied by the receiving leaf's frame.
 * Zero dimensions are invalid; nonzero extents beyond this fixed capacity
 * are unsupported. Off-surface/hidden cursors do not invalidate a frame. */
#define KVM_TEXT_COLUMNS 80u
#define KVM_TEXT_ROWS 25u

typedef struct kvm_text_frame {
    lib_u16 text_columns;
    lib_u16 text_rows;
    lib_i32 cursor_column;
    lib_i32 cursor_row;
    lib_u8 cursor_top;
    lib_u8 cursor_bottom;
    lib_u8 cursor_visible;
    lib_u8 cursor_phase;
    lib_u8 text[KVM_TEXT_COLUMNS * KVM_TEXT_ROWS];
    lib_u16 attributes[KVM_TEXT_COLUMNS * KVM_TEXT_ROWS];
    lib_u32 text_palette[16u]; /* 0x00RRGGBB */
    lib_u32 font_height;
    lib_u32 attribute_font_select;
} kvm_text_frame;

static inline lib_status kvm_text_frame_validate(const kvm_text_frame *frame)
{
    if (frame == LIB_NULL || frame->text_columns == 0u || frame->text_rows == 0u)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (frame->text_columns > KVM_TEXT_COLUMNS || frame->text_rows > KVM_TEXT_ROWS)
        return LIB_STATUS_UNSUPPORTED;
    return LIB_STATUS_OK;
}

#endif
