#include "lib/kvm-window/render.h"

int kvm_window_frame_size(const kvm_window_frame *frame, lib_u32 *width, lib_u32 *height)
{
    if (kvm_window_frame_validate(frame) != LIB_STATUS_OK || !width || !height) return 0;
    *width = frame->graphics ? frame->image.width : frame->text.base.text_columns * 8u;
    *height = frame->graphics ? frame->image.height : frame->text.base.text_rows *
        (frame->text.base.font_height != 0u ? frame->text.base.font_height : KVM_WINDOW_FONT_HEIGHT);
    return 1;
}

void kvm_window_render_text(const kvm_window_frame *frame, lib_u32 *pixels, lib_u32 width, lib_u32 height)
{
    lib_u32 row;
    lib_u32 cell_height;

    if (kvm_window_frame_validate(frame) != LIB_STATUS_OK || !pixels || frame->graphics) return;
    cell_height = frame->text.base.font_height != 0u ? frame->text.base.font_height : KVM_WINDOW_FONT_HEIGHT;
    if (width != frame->text.base.text_columns * 8u || height != frame->text.base.text_rows * cell_height) return;
    for (row = 0u; row < frame->text.base.text_rows; ++row) {
        lib_u32 column;
        for (column = 0u; column < frame->text.base.text_columns; ++column) {
            const kvm_text_cell *cell = &frame->text.base.cells[(lib_size)row * KVM_TEXT_COLUMNS + column];
            const lib_u8 *font = cell->glyph_bank != 0u ?
                frame->text.secondary_font : frame->text.font;
            lib_u32 scan;
            for (scan = 0u; scan < cell_height; ++scan) {
                lib_u8 bits = font[(lib_size)cell->glyph_index * 16u + scan];
                lib_u32 *row_pixels = pixels +
                    ((lib_size)row * cell_height + scan) *
                    width + column * 8u;
                lib_u32 bit;
                for (bit = 0u; bit < 8u; ++bit)
                    row_pixels[bit] = frame->text.base.text_palette[
                        (bits & (0x80u >> bit)) != 0u ? cell->foreground : cell->background];
            }
        }
    }
}

int kvm_window_render_graphics(const kvm_window_frame *frame, lib_u32 *pixels,
    lib_u32 width, lib_u32 height, int *valid, kvm_window_rect *changed)
{
    lib_i32 left = (lib_i32)width;
    lib_i32 top = (lib_i32)height;
    lib_i32 right = 0;
    lib_i32 bottom = 0;
    lib_u32 row;

    if (!frame || !pixels || !valid || !changed || frame->graphics == 0u ||
        width != frame->image.width ||
        height != frame->image.height) return 0;
    /* The surface is the rendered baseline, independent of skipped publications.
     * Compare resolved colours so palette changes need no separate cache. */
    for (row = 0u; row < height; ++row) {
        const lib_u8 *source = frame->image.pixels + row * frame->image.stride;
        lib_u32 *destination = pixels + row * width;
        lib_u32 column;
        for (column = 0u; column < width; ++column) {
            lib_u32 colour = frame->image.palette[source[column]];
            if (!*valid || destination[column] != colour) {
                destination[column] = colour;
                if ((lib_i32)column < left) left = (lib_i32)column;
                if ((lib_i32)row < top) top = (lib_i32)row;
                if ((lib_i32)column + 1 > right) right = (lib_i32)column + 1;
                bottom = (lib_i32)row + 1;
            }
        }
    }
    *valid = 1;
    if (right == 0) return 0;
    changed->left = left;
    changed->top = top;
    changed->right = right;
    changed->bottom = bottom;
    return 1;
}
