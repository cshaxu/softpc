/* Exercise the actual producer conversion and the actual lib geometry together. */
#include "app/runtime.c"
#include "lib/ui-window/geometry.h"
#include <assert.h>

int main(void)
{
    static ui_frame frame = { .valid=1, .text_columns=80, .text_rows=25,
        .cursor_column=3, .cursor_row=7, .cursor_visible=1 };
    const unsigned heights[] = {8,14,16};
    const unsigned sizes[] = {0,1,12,20,50,100,101};
    for (unsigned h=0;h<3;++h) for (unsigned s=0;s<7;++s)
    for (unsigned scale=1;scale<=2;++scale) {
        ui_window_rect display={0,0,640*(int)scale,25*(int)heights[h]*(int)scale};
        ui_window_rect cursor;
        unsigned percent=sizes[s]==0 || sizes[s]>100 ? 100 : sizes[s];
        unsigned lines=(heights[h]*percent+99)/100;
        frame.font_height=heights[h];
        app_runtime_cursor_shape(&frame,sizes[s]);
        assert(frame.cursor_top==heights[h]-lines);
        assert(frame.cursor_bottom==heights[h]-1);
        assert(ui_window_cursor_rect(&frame,&display,&cursor));
        assert(cursor.bottom==(frame.cursor_row+1)*(int)(heights[h]*scale));
        assert(cursor.bottom-cursor.top==(int)(lines*scale));
    }
    return 0;
}
