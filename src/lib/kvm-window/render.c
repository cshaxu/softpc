#include "lib/kvm-window/render.h"

int kvm_window_frame_size(const kvm_window_frame *frame, lib_u32 *width, lib_u32 *height)
{
    if (kvm_window_frame_validate(frame) != LIB_STATUS_OK || !width || !height) return 0;
    *width = frame->graphics ? frame->image.width : frame->text.base.text_columns * 8u;
    *height = frame->graphics ? frame->image.height : frame->text.base.text_rows *
        (frame->text.base.font_height != 0u ? frame->text.base.font_height : KVM_WINDOW_FONT_HEIGHT);
    return 1;
}

static void kvm_window_text_row(const kvm_window_text_frame *text,
    lib_u32 y, lib_u32 cell_height, lib_u32 *pixels)
{
    lib_u32 column;
    const kvm_text_cell *cells = text->base.cells + (y / cell_height) * KVM_TEXT_COLUMNS;
    for (column = 0u; column < text->base.text_columns; ++column) {
        const kvm_text_cell *cell = &cells[column];
        const lib_u8 *font = cell->glyph_bank ? text->secondary_font : text->font;
        lib_u8 bits = font[(lib_size)cell->glyph_index * KVM_WINDOW_FONT_HEIGHT + y % cell_height];
        lib_u32 bit;
        for (bit = 0u; bit < 8u; ++bit)
            pixels[column * 8u + bit] = text->base.text_palette[
                (bits & (0x80u >> bit)) ? cell->foreground : cell->background];
    }
}

int kvm_window_render_frame(const kvm_window_frame *frame, lib_u32 *pixels,
    lib_u32 width, lib_u32 height, int *valid, kvm_window_rect *changed)
{
    lib_i32 left = (lib_i32)width;
    lib_i32 top = (lib_i32)height;
    lib_i32 right = 0;
    lib_i32 bottom = 0;
    lib_u32 row;
    lib_u32 frame_width, frame_height, cell_height = KVM_WINDOW_FONT_HEIGHT;
    lib_u32 colours[KVM_WINDOW_GRAPHICS_MAX_WIDTH];

    if (!pixels || !valid || !changed ||
        !kvm_window_frame_size(frame, &frame_width, &frame_height) ||
        width != frame_width || height != frame_height) return 0;
    if (!frame->graphics && frame->text.base.font_height)
        cell_height = frame->text.base.font_height;
    /* The surface is the rendered baseline, independent of skipped publications.
     * Compare resolved colours so palette changes need no separate cache. */
    for (row = 0u; row < height; ++row) {
        lib_u32 *destination = pixels + row * width;
        lib_u32 column;
        if (frame->graphics) {
            const lib_u8 *source = frame->image.pixels + row * frame->image.stride;
            for (column = 0u; column < width; ++column)
                colours[column] = frame->image.palette[source[column]];
        } else {
            kvm_window_text_row(&frame->text, row, cell_height, colours);
        }
        for (column = 0u; column < width; ++column) {
            lib_u32 colour = colours[column];
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
