/* Exercise the copied cursor conversion contract and lib geometry together. */
#include "vm/driver.h"
#include "lib/kvm-window/geometry.h"
#include "lib/kvm-window/render.h"
#include <assert.h>

int main(void)
{
    static kvm_frame frame = { .valid=1, .text_columns=80, .text_rows=25,
        .cursor_column=3, .cursor_row=7, .cursor_visible=1 };
    const unsigned heights[] = {8,14,16};
    const unsigned sizes[] = {0,1,12,20,50,100,101};
    static lib_u32 pixels[640 * 400];
    for (unsigned h=0;h<3;++h) for (unsigned s=0;s<7;++s)
    for (unsigned scale=1;scale<=2;++scale) {
        kvm_window_rect display={0,0,640*(int)scale,25*(int)heights[h]*(int)scale};
        kvm_window_rect cursor;
        unsigned percent=sizes[s]==0 || sizes[s]>100 ? 100 : sizes[s];
        unsigned lines=(heights[h]*percent+99)/100;
        frame.font_height=heights[h];
        lib_u32 width, height;
        assert(kvm_window_frame_size(&frame, &width, &height));
        assert(width == 640 && height == 25 * heights[h]);
        frame.text[7 * KVM_TEXT_COLUMNS + 3] = 'A';
        frame.attributes[7 * KVM_TEXT_COLUMNS + 3] = 1;
        frame.text_palette[1] = 0xffffff;
        frame.font['A' * 16 + heights[h] - 1] = 0xff;
        kvm_window_render_text(&frame, pixels, width, height);
        assert(pixels[((7 + 1) * heights[h] - 1) * width + 3 * 8] == 0xffffff);
        vm_driver_cursor_shape(&frame,sizes[s]);
        assert(frame.cursor_top==heights[h]-lines);
        assert(frame.cursor_bottom==heights[h]-1);
        assert(kvm_window_cursor_rect(&frame,&display,&cursor));
        assert(cursor.bottom==(frame.cursor_row+1)*(int)(heights[h]*scale));
        assert(cursor.bottom-cursor.top==(int)(lines*scale));
    }
    return 0;
}
