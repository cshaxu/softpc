#include "lib/ui-base/mailbox_interface.h"
#include "lib/ui-window/win32/mouse.h"
#include <assert.h>

static ui_component_mailboxes mailbox;
static ui_frame frame, received;

static void damage(void)
{
    lib_u32 generation = 0u;
    assert(ui_component_mailboxes_create(&mailbox) == LIB_STATUS_OK);
    frame.valid = frame.graphics = 1u;
    frame.graphics_width = frame.graphics_stride = 4u;
    frame.graphics_height = 4u;
    assert(ui_component_mailboxes_publish_frame(&mailbox, &frame) == LIB_STATUS_OK);
    assert(ui_component_mailboxes_capture_frame(&mailbox, &generation, &received));
    assert(received.dirty_right == 3 && received.dirty_bottom == 3);
    frame.dirty_left = frame.dirty_top = frame.dirty_right = frame.dirty_bottom = 1;
    frame.graphics_pixels[5] = 1u;
    assert(ui_component_mailboxes_publish_frame(&mailbox, &frame) == LIB_STATUS_OK);
    frame.dirty_left = frame.dirty_top = frame.dirty_right = frame.dirty_bottom = 2;
    frame.graphics_pixels[10] = 2u;
    assert(ui_component_mailboxes_publish_frame(&mailbox, &frame) == LIB_STATUS_OK);
    assert(ui_component_mailboxes_capture_frame(&mailbox, &generation, &received));
    assert(received.dirty_left == 1 && received.dirty_top == 1 &&
        received.dirty_right == 2 && received.dirty_bottom == 2);
    assert(received.graphics_pixels[5] == 1u && received.graphics_pixels[10] == 2u);
    assert(!ui_component_mailboxes_capture_frame(&mailbox, &generation, &received));
    assert(ui_component_mailboxes_publish_frame(&mailbox, &frame) == LIB_STATUS_OK);
    assert(ui_component_mailboxes_capture_frame(&mailbox, &generation, &received));
    assert(received.dirty_left == 2 && received.dirty_top == 2);
    frame.graphics_palette[1] = 0xffu;
    assert(ui_component_mailboxes_publish_frame(&mailbox, &frame) == LIB_STATUS_OK);
    assert(ui_component_mailboxes_capture_frame(&mailbox, &generation, &received));
    assert(received.dirty_left == 0 && received.dirty_right == 3);
    frame.graphics_width = frame.graphics_stride = 3u;
    assert(ui_component_mailboxes_publish_frame(&mailbox, &frame) == LIB_STATUS_OK);
    assert(ui_component_mailboxes_capture_frame(&mailbox, &generation, &received));
    assert(received.dirty_left == 0 && received.dirty_right == 2);
    frame.graphics = 0u; frame.text_columns = 80u; frame.text_rows = 25u;
    assert(ui_component_mailboxes_publish_frame(&mailbox, &frame) == LIB_STATUS_OK);
    frame.graphics = 1u;
    assert(ui_component_mailboxes_publish_frame(&mailbox, &frame) == LIB_STATUS_OK);
    assert(ui_component_mailboxes_capture_frame(&mailbox, &generation, &received));
    assert(received.dirty_left == 0 && received.dirty_bottom == 3);
    ui_component_mailboxes_destroy(&mailbox);
}

static void motion(void)
{
    ui_win32_mouse mouse;
    int dx, dy, total_x = 0, total_y = 0;
    ui_win32_mouse_reset(&mouse);
    assert(ui_win32_mouse_move(&mouse, 0, 200, 200, 100, 100, &dx, &dy));
    for (int i = 1; i <= 10; ++i) {
        assert(ui_win32_mouse_move(&mouse, (i << 16) | i, 200, 200, 100, 100, &dx, &dy));
        total_x += dx; total_y += dy;
    }
    assert(total_x == 5 && total_y == 5);
    for (int i = 9; i >= 0; --i) {
        assert(ui_win32_mouse_move(&mouse, (i << 16) | i, 200, 200, 100, 100, &dx, &dy));
        total_x += dx; total_y += dy;
    }
    assert(total_x == 0 && total_y == 0);
    assert(ui_win32_mouse_move(&mouse, 0x10001, 200, 200, 100, 100, &dx, &dy));
    assert(dx == 0 && dy == 0);
    assert(ui_win32_mouse_move(&mouse, 0, 200, 200, 100, 100, &dx, &dy));
    assert(dx == 0 && dy == 0 && mouse.remainder_x == 0 && mouse.remainder_y == 0);
    assert(ui_win32_mouse_move(&mouse, 0x10001, 200, 200, 100, 100, &dx, &dy));
    assert(ui_win32_mouse_move(&mouse, 0x20002, 100, 100, 100, 100, &dx, &dy));
    assert(dx == 1 && dy == 1 && mouse.remainder_x == 0 && mouse.remainder_y == 0);
}

int main(void) { damage(); motion(); return 0; }
