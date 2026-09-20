#include "lib/kvm-window/window.h"
#include "lib/kvm-window/motion.h"
#include "lib/kvm-window/geometry.h"
#include "lib/kvm-window/render.h"
#include <assert.h>

static kvm_window window;
static kvm_window_frame frame, received;

static void damage(void)
{
    lib_u32 generation = 0u;
    assert(kvm_component_mailboxes_create(&window.base.mailboxes, &window.pending_frame,
        sizeof(window.pending_frame)) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_select_notify(&window.base.mailboxes, NULL, NULL) == LIB_STATUS_OK);
    frame.valid = frame.graphics = 1u;
    frame.image.width = frame.image.stride = 4u;
    frame.image.height = 4u;
    assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_capture_frame(&window.base.mailboxes, &generation, &received, sizeof(received)));
    assert(received.image.dirty_right == 3 && received.image.dirty_bottom == 3);
    {
        lib_u32 pending_generation = generation;
        assert(base_sync_event_wait(window.base.mailboxes.wake, 0u) == BASE_SYNC_WAIT_SIGNALED);
        frame.image.height = KVM_WINDOW_GRAPHICS_MAX_HEIGHT + 1u;
        assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_UNSUPPORTED);
        frame.image.height = 4u; frame.image.stride = 3u;
        assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_INVALID_ARGUMENT);
        frame.graphics = 0u;
        frame.text.base.text_columns = 80u; frame.text.base.text_rows = 25u;
        frame.text.base.font_height = KVM_WINDOW_FONT_HEIGHT + 1u;
        assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_UNSUPPORTED);
        assert(base_sync_event_wait(window.base.mailboxes.wake, 0u) == BASE_SYNC_WAIT_TIMED_OUT);
        assert(kvm_component_mailboxes_capture_frame(&window.base.mailboxes, &generation, &received, sizeof(received)));
        assert(generation == pending_generation && received.graphics && received.image.height == 4u);
        frame = received;
    }
    kvm_component_mailboxes_acknowledge_frame(&window.base.mailboxes, generation);
    frame.image.dirty_left = frame.image.dirty_top = frame.image.dirty_right = frame.image.dirty_bottom = 1;
    frame.image.pixels[5] = 1u;
    assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_OK);
    frame.image.dirty_left = frame.image.dirty_top = frame.image.dirty_right = frame.image.dirty_bottom = 2;
    frame.image.pixels[10] = 2u;
    assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_capture_frame(&window.base.mailboxes, &generation, &received, sizeof(received)));
    assert(received.image.dirty_left == 1 && received.image.dirty_top == 1 &&
        received.image.dirty_right == 2 && received.image.dirty_bottom == 2);
    assert(received.image.pixels[5] == 1u && received.image.pixels[10] == 2u);
    kvm_component_mailboxes_acknowledge_frame(&window.base.mailboxes, generation);
    assert(!kvm_component_mailboxes_capture_frame(&window.base.mailboxes, &generation, &received, sizeof(received)));
    assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_capture_frame(&window.base.mailboxes, &generation, &received, sizeof(received)));
    assert(received.image.dirty_left == 2 && received.image.dirty_top == 2);
    /* Failed output has no acknowledgement; capture remains available.
     * A newer publication cannot be erased by the older write's success. */
    lib_u32 old = generation;
    assert(kvm_component_mailboxes_capture_frame(&window.base.mailboxes, &generation, &received, sizeof(received)));
    assert(generation == old);
    assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_OK);
    kvm_component_mailboxes_acknowledge_frame(&window.base.mailboxes, old);
    assert(kvm_component_mailboxes_capture_frame(&window.base.mailboxes, &generation, &received, sizeof(received)));
    assert(generation != old);
    kvm_component_mailboxes_acknowledge_frame(&window.base.mailboxes, generation);
    frame.image.palette[1] = 0xffu;
    assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_capture_frame(&window.base.mailboxes, &generation, &received, sizeof(received)));
    assert(received.image.dirty_left == 0 && received.image.dirty_right == 3);
    frame.image.width = frame.image.stride = 3u;
    assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_capture_frame(&window.base.mailboxes, &generation, &received, sizeof(received)));
    assert(received.image.dirty_left == 0 && received.image.dirty_right == 2);
    frame.graphics = 0u; frame.text.base.text_columns = 80u; frame.text.base.text_rows = 25u;
    frame.text.base.font_height = 0u;
    assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_OK);
    frame.graphics = 1u;
    frame.image.width = frame.image.stride = 3u;
    frame.image.height = 4u;
    assert(kvm_window_publish_frame(&window, &frame) == LIB_STATUS_OK);
    assert(kvm_component_mailboxes_capture_frame(&window.base.mailboxes, &generation, &received, sizeof(received)));
    assert(received.image.dirty_left == 0 && received.image.dirty_bottom == 3);
    kvm_component_mailboxes_destroy(&window.base.mailboxes);
}

static void motion(void)
{
    kvm_window_motion mouse = {0};
    int dx, dy, total_x = 0, total_y = 0;
    assert(kvm_window_motion_move(&mouse, 0, 0, 200, 200, 100, 100, &dx, &dy));
    for (int i = 1; i <= 10; ++i) {
        assert(kvm_window_motion_move(&mouse, i, i, 200, 200, 100, 100, &dx, &dy));
        total_x += dx; total_y += dy;
    }
    assert(total_x == 5 && total_y == 5);
    for (int i = 9; i >= 0; --i) {
        assert(kvm_window_motion_move(&mouse, i, i, 200, 200, 100, 100, &dx, &dy));
        total_x += dx; total_y += dy;
    }
    assert(total_x == 0 && total_y == 0);
    assert(kvm_window_motion_move(&mouse, 1, 1, 200, 200, 100, 100, &dx, &dy));
    assert(dx == 0 && dy == 0);
    assert(kvm_window_motion_move(&mouse, 0, 0, 200, 200, 100, 100, &dx, &dy));
    assert(dx == 0 && dy == 0 && mouse.remainder_x == 0 && mouse.remainder_y == 0);
    assert(kvm_window_motion_move(&mouse, 1, 1, 200, 200, 100, 100, &dx, &dy));
    assert(kvm_window_motion_move(&mouse, 2, 2, 100, 100, 100, 100, &dx, &dy));
    assert(dx == 1 && dy == 1 && mouse.remainder_x == 0 && mouse.remainder_y == 0);
}

static void rendering(void)
{
    kvm_window_frame text = { 0 };
    kvm_window_rect display = { 0, 0, 640, 410 }, cursor;
    lib_u32 pixels[8 * 16];
    text.valid = 1; text.text.base.text_columns = 80; text.text.base.text_rows = 25;
    text.text.base.cursor_visible = 1; text.text.base.cursor_column = 0; text.text.base.cursor_row = 24;
    text.text.base.font_height = 16; text.text.base.cursor_top = 14; text.text.base.cursor_bottom = 15;
    assert(kvm_window_cursor_rect(&text, &display, &cursor));
    assert(cursor.bottom == 410 && cursor.top == 407);
    for (int row = 0; row < 25; ++row) {
        text.text.base.cursor_row = row;
        assert(kvm_window_cursor_rect(&text, &display, &cursor));
        assert(cursor.top >= row * 410 / 25);
        assert(cursor.bottom == (row + 1) * 410 / 25);
        assert(cursor.bottom - cursor.top <= 3);
    }
    text.text.base.text_columns = text.text.base.text_rows = 1;
    text.text.base.cursor_row = 0;
    display = (kvm_window_rect){10,20,18,36};
    for (unsigned top=0;top<16;++top) {
        text.text.base.cursor_top=(lib_u8)top; text.text.base.cursor_bottom=(lib_u8)top;
        assert(kvm_window_cursor_rect(&text,&display,&cursor));
        assert(cursor.top==20+(int)top && cursor.bottom==21+(int)top);
    }
    text.text.base.cursor_top=4; text.text.base.cursor_bottom=7;
    display.bottom=52;
    assert(kvm_window_cursor_rect(&text,&display,&cursor));
    assert(cursor.top==28 && cursor.bottom==36);
    text.text.base.cursor_top=15; text.text.base.cursor_bottom=255;
    assert(kvm_window_cursor_rect(&text,&display,&cursor) && cursor.bottom==52);
    text.text.base.cursor_top=16;
    assert(!kvm_window_cursor_rect(&text,&display,&cursor));
    text.text.base.cursor_top=9; text.text.base.cursor_bottom=8;
    assert(kvm_window_cursor_rect(&text,&display,&cursor) && cursor.top==20 && cursor.bottom==52);
    text.text.base.font_height=0;
    assert(kvm_window_cursor_rect(&text,&display,&cursor) && cursor.top==20 && cursor.bottom==52);
    text.text.font[0] = 0x80; text.text.base.attributes[0] = 0x21;
    text.text.base.text_palette[1] = 0x112233; text.text.base.text_palette[2] = 0x445566;
    kvm_window_render_text(&text, pixels, 8, 16);
    assert(pixels[0] == 0x112233 && pixels[1] == 0x445566 && pixels[127] == 0x445566);
    text.text.font[0] = 0x40;
    kvm_window_render_text(&text, pixels, 8, 16);
    assert(pixels[0] == 0x445566 && pixels[1] == 0x112233);
    text.text.base.attribute_font_select = 1u;
    text.text.base.attributes[0] = 0x29;
    text.text.base.text_palette[9] = 0x112233;
    text.text.secondary_font[0] = 0x20;
    kvm_window_render_text(&text, pixels, 8, 16);
    assert(pixels[0] == 0x445566 && pixels[2] == 0x112233);
}
int main(void) { damage(); motion(); rendering(); return 0; }
