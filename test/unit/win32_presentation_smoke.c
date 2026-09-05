#include "frame.h"
#include "geometry.h"
#include "mouse.h"

#include <assert.h>

#ifdef _WIN32
int main(void)
{
    RECT display;
    RECT source = { 10, 20, 30, 40 };
    RECT target = { 0, 0, 0, 0 };
    win32_presentation_frame frame = { 0 };
    win32_presentation_mouse mouse;

    frame.sequence = 7u;
    frame.dib_width = 640u;
    frame.dib_height = 480u;
    assert(frame.sequence == 7u);
    assert(win32_presentation_display_rect(1280, 960, frame.dib_width,
        frame.dib_height, &display));
    assert(display.left == 0 && display.top == 0 &&
        display.right == 1280 && display.bottom == 960);
    win32_presentation_map_dirty_rect(&source, &display, frame.dib_width,
        frame.dib_height, &target);
    assert(target.left == 20 && target.top == 40 &&
        target.right == 60 && target.bottom == 80);
    assert(win32_presentation_dib_pixel(RGB(0, 0, 168)) == 0x000000a8u);
    mouse.x = 3;
    mouse.y = 4;
    mouse.valid = mouse.captured = mouse.host_cursor_hidden = 1;
    win32_presentation_mouse_reset(&mouse);
    assert(mouse.valid == 0 && mouse.captured == 0 &&
        mouse.host_cursor_hidden == 0);
    return 0;
}
#else
int main(void) { return 0; }
#endif
