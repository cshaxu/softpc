#include "lib/types/win32/window.h"
#include "lib/kvm-window/win32/mouse.h"

static int same_bounds(const lib_win32_rect *a, const lib_win32_rect *b)
{
    return a->left == b->left && a->top == b->top &&
        a->right == b->right && a->bottom == b->bottom;
}

void kvm_win32_mouse_reset(kvm_win32_mouse *mouse)
{
    if (mouse == LIB_NULL) return;
    lib_memory_set(mouse, 0, sizeof(*mouse));
}

lib_status kvm_win32_mouse_release(kvm_win32_mouse *mouse)
{
    lib_status status = LIB_STATUS_OK;
    lib_win32_rect clipped;
    if (mouse == LIB_NULL || !mouse->captured) return LIB_STATUS_OK;
    /* Clear ownership before ReleaseCapture synchronously notifies the window. */
    mouse->captured = LIB_FALSE;
    /* Do not remove another window's replacement clipping rectangle. */
    if (!lib_win32_get_clip_cursor(&clipped) ||
        (same_bounds(&clipped, &mouse->bounds) && !lib_win32_clip_cursor(LIB_NULL)))
        status = LIB_STATUS_IO_ERROR;
    if (lib_win32_get_capture() == mouse->window && !lib_win32_release_capture())
        status = LIB_STATUS_IO_ERROR;
    mouse->window = LIB_NULL;
    lib_win32_set_cursor(lib_win32_load_cursor_a(LIB_NULL, LIB_WIN32_IDC_ARROW));
    mouse->motion.valid = 0;
    mouse->motion.remainder_x = mouse->motion.remainder_y = 0;
    return status;
}

int kvm_win32_mouse_refresh_bounds(kvm_win32_mouse *mouse)
{
    lib_win32_rect client, bounds;
    lib_win32_point upper_left, lower_right;
    if (mouse == LIB_NULL) return 0;
    if (!mouse->captured) return 1;
    if (lib_win32_get_capture() != mouse->window ||
        !lib_win32_get_client_rect(mouse->window, &client) ||
        client.right <= client.left || client.bottom <= client.top) return 0;
    upper_left.x = client.left; upper_left.y = client.top;
    lower_right.x = client.right; lower_right.y = client.bottom;
    if (!lib_win32_client_to_screen(mouse->window, &upper_left) ||
        !lib_win32_client_to_screen(mouse->window, &lower_right)) return 0;
    bounds.left = upper_left.x; bounds.top = upper_left.y;
    bounds.right = lower_right.x; bounds.bottom = lower_right.y;
    if (!lib_win32_clip_cursor(&bounds)) return 0;
    mouse->bounds = bounds;
    if (!lib_win32_set_cursor_pos(bounds.left + (bounds.right - bounds.left) / 2,
        bounds.top + (bounds.bottom - bounds.top) / 2)) return 0;
    mouse->motion.x = bounds.left + (bounds.right - bounds.left) / 2;
    mouse->motion.y = bounds.top + (bounds.bottom - bounds.top) / 2;
    mouse->motion.valid = 1;
    mouse->motion.remainder_x = mouse->motion.remainder_y = 0;
    return 1;
}

lib_status kvm_win32_mouse_capture(kvm_win32_mouse *mouse,
    lib_win32_hwnd window)
{
    if (mouse == LIB_NULL || window == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    lib_win32_set_focus(window);
    if (lib_win32_get_focus() != window) return LIB_STATUS_INVALID_STATE;
    lib_win32_set_capture(window);
    if (lib_win32_get_capture() != window) return LIB_STATUS_INVALID_STATE;
    mouse->window = window;
    mouse->captured = LIB_TRUE;
    if (!kvm_win32_mouse_refresh_bounds(mouse)) {
        /* Capture failed; release is attempted once, without hiding failure. */
        (void)kvm_win32_mouse_release(mouse);
        return LIB_STATUS_IO_ERROR;
    }
    return LIB_STATUS_OK;
}

int kvm_win32_mouse_move(kvm_win32_mouse *mouse,
    int client_width, int client_height,
    unsigned int content_width, unsigned int content_height, int *dx, int *dy)
{
    lib_win32_point position;
    lib_win32_rect clipped;
    int x, y;
    if (mouse == LIB_NULL || !mouse->captured || dx == LIB_NULL || dy == LIB_NULL ||
        lib_win32_get_capture() != mouse->window ||
        !lib_win32_get_clip_cursor(&clipped) ||
        !same_bounds(&clipped, &mouse->bounds) ||
        !lib_win32_get_cursor_pos(&position)) return 0;
    x = mouse->bounds.left + (mouse->bounds.right - mouse->bounds.left) / 2;
    y = mouse->bounds.top + (mouse->bounds.bottom - mouse->bounds.top) / 2;
    /* Sample current position: queued pre-warp coordinates must never turn
       our own recenter into a reverse content movement. */
    if (!kvm_window_motion_move(&mouse->motion, position.x, position.y,
        client_width, client_height, content_width, content_height, dx, dy)) return 0;
    if ((position.x != x || position.y != y) && !lib_win32_set_cursor_pos(x, y))
        return 0;
    mouse->motion.x = x;
    mouse->motion.y = y;
    return 1;
}

int kvm_win32_mouse_captured(const kvm_win32_mouse *mouse)
{
    return mouse != LIB_NULL && mouse->captured;
}
