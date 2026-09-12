#include "lib/types/win32/window.h"
#include "lib/ui-window/win32/mouse.h"

void ui_win32_mouse_reset(ui_win32_mouse *mouse)
{
    if (mouse == LIB_NULL) return;
    mouse->x = 0;
    mouse->y = 0;
    mouse->valid = 0;
    mouse->captured = LIB_FALSE;
}

void ui_win32_mouse_release(ui_win32_mouse *mouse)
{
    if (mouse == LIB_NULL || !mouse->captured) return;
    lib_win32_clip_cursor(LIB_NULL);
    lib_win32_release_capture();
    lib_win32_set_cursor(lib_win32_load_cursor_a(LIB_NULL, LIB_WIN32_IDC_ARROW));
    mouse->captured = LIB_FALSE;
    mouse->valid = 0;
}

int ui_win32_mouse_capture(ui_win32_mouse *mouse,
    lib_win32_hwnd window, lib_win32_lparam position)
{
    lib_win32_rect client;
    lib_win32_point upper_left;
    lib_win32_point lower_right;
    lib_win32_rect bounds;

    if (mouse == LIB_NULL || window == LIB_NULL || !lib_win32_get_client_rect(window, &client) ||
        client.right <= client.left || client.bottom <= client.top) return 0;
    lib_win32_set_focus(window);
    if (lib_win32_get_focus() != window) return 0;
    lib_win32_set_capture(window);
    if (lib_win32_get_capture() != window) return 0;
    upper_left.x = client.left;
    upper_left.y = client.top;
    lower_right.x = client.right;
    lower_right.y = client.bottom;
    if (!lib_win32_client_to_screen(window, &upper_left) ||
        !lib_win32_client_to_screen(window, &lower_right)) {
        lib_win32_release_capture();
        return 0;
    }
    bounds.left = upper_left.x;
    bounds.top = upper_left.y;
    bounds.right = lower_right.x;
    bounds.bottom = lower_right.y;
    if (!lib_win32_clip_cursor(&bounds)) {
        lib_win32_release_capture();
        return 0;
    }
    mouse->x = (int)(short)lib_win32_loword(position);
    mouse->y = (int)(short)lib_win32_hiword(position);
    mouse->valid = 1;
    mouse->captured = LIB_TRUE;
    return 1;
}

int ui_win32_mouse_move(ui_win32_mouse *mouse,
    lib_win32_lparam position, int client_width, int client_height,
    unsigned int content_width, unsigned int content_height, int *dx, int *dy)
{
    int x;
    int y;

    if (mouse == LIB_NULL || dx == LIB_NULL || dy == LIB_NULL) return 0;
    x = (int)(short)lib_win32_loword(position);
    y = (int)(short)lib_win32_hiword(position);
    *dx = mouse->valid ? x - mouse->x : 0;
    *dy = mouse->valid ? y - mouse->y : 0;
    mouse->x = x;
    mouse->y = y;
    mouse->valid = 1;
    if (client_width > 0 && content_width != 0u)
        *dx = (int)((long long)*dx * (long long)content_width / client_width);
    if (client_height > 0 && content_height != 0u)
        *dy = (int)((long long)*dy * (long long)content_height / client_height);
    return 1;
}

int ui_win32_mouse_captured(const ui_win32_mouse *mouse)
{
    return mouse != LIB_NULL && mouse->captured;
}
