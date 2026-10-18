/* Exercise the copied cursor conversion contract and lib geometry together. */
#include "machine/driver.h"
#include "lib/kvm-window/geometry.h"
#include "lib/kvm-window/render.h"
#include <assert.h>

int main(void)
{
    static kvm_window_frame_storage frame = { .data.text.base = { .text_columns = 80, .text_rows = 25,
        .cursor_column=3, .cursor_row=7, .cursor_mode=KVM_CURSOR_HOST } };
    (void)kvm_window_frame_begin_text(&frame);
    const unsigned heights[] = {0,8,14,16,24,32};
    const unsigned sizes[] = {0,1,12,20,50,100,101};
    static lib_u32 pixels[640 * 25 * KVM_WINDOW_FONT_HEIGHT];
    for (unsigned h=0;h<sizeof(heights)/sizeof(heights[0]);++h) for (unsigned s=0;s<7;++s)
    for (unsigned scale=1;scale<=2;++scale) {
        unsigned cell_height = heights[h] ? heights[h] : KVM_TEXT_FONT_DEFAULT_HEIGHT;
        kvm_window_rect display={0,0,640*(int)scale,25*(int)cell_height*(int)scale};
        kvm_window_rect cursor;
        unsigned percent=sizes[s]==0 || sizes[s]>100 ? 100 : sizes[s];
        unsigned lines=(cell_height*percent+99)/100;
        frame.data.text.base.font_height=heights[h];
        lib_u32 width, height;
        assert(kvm_window_frame_size(&frame.frame, &width, &height));
        assert(width == 640 && height == 25 * cell_height);
        frame.data.text.base.cells[7 * KVM_TEXT_COLUMNS + 3].glyph_index = 'A';
        frame.data.text.base.cells[7 * KVM_TEXT_COLUMNS + 3].foreground = 1;
        frame.data.text.base.text_palette[1] = 0xffffff;
        frame.data.text.font['A' * KVM_WINDOW_FONT_HEIGHT + cell_height - 1] = 0xff;
        lib_bool valid = LIB_FALSE;
        kvm_window_rect changed;
        assert(kvm_window_render_frame(&frame.frame, pixels, width, height, &valid, &changed));
        assert(pixels[((7 + 1) * cell_height - 1) * width + 3 * 8] == 0xffffff);
        vm_driver_cursor_shape(&frame.data.text.base,sizes[s]);
        assert(frame.data.text.base.cursor_top==cell_height-lines);
        assert(frame.data.text.base.cursor_bottom==cell_height-1);
        assert(kvm_window_cursor_rect(&frame.frame,&display,&cursor));
        assert(cursor.bottom==(frame.data.text.base.cursor_row+1)*(int)(cell_height*scale));
        assert(cursor.bottom-cursor.top==(int)(lines*scale));
    }
    return 0;
}
