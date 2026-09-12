#include "lib/types/win32/scalar.h"
#include "lib/types/win32/sync.h"
#include "lib/types/win32/window.h"
#include "lib/ui-window/win32/geometry.h"

int ui_win32_display_rect(int client_width, int client_height,
    lib_u32 source_width, lib_u32 source_height, lib_win32_rect *display)
{
    if (display == LIB_NULL || source_width == 0u || source_height == 0u ||
        client_width <= 0 || client_height <= 0) return 0;
    display->left = 0;
    display->top = 0;
    display->right = client_width;
    display->bottom = client_height;
    return 1;
}

void ui_win32_map_dirty_rect(const lib_win32_rect *source, const lib_win32_rect *display,
    lib_u32 source_width, lib_u32 source_height, lib_win32_rect *target)
{
    int width;
    int height;

    if (source == LIB_NULL || display == LIB_NULL || target == LIB_NULL ||
        source_width == 0u || source_height == 0u) return;
    width = display->right - display->left;
    height = display->bottom - display->top;
    target->left = display->left + source->left * width / (int)source_width;
    target->top = display->top + source->top * height / (int)source_height;
    target->right = display->left + (source->right * width +
        (int)source_width - 1) / (int)source_width;
    target->bottom = display->top + (source->bottom * height +
        (int)source_height - 1) / (int)source_height;
    if (target->right <= target->left) target->right = target->left + 1;
    if (target->bottom <= target->top) target->bottom = target->top + 1;
}

int ui_win32_fit_outer_rect(const lib_win32_rect *work_area, int desired_width,
    int desired_height, lib_win32_rect *fitted)
{
    int available_width;
    int available_height;
    int width;
    int height;

    if (work_area == LIB_NULL || fitted == LIB_NULL || desired_width <= 0 ||
        desired_height <= 0) return 0;
    available_width = work_area->right - work_area->left;
    available_height = work_area->bottom - work_area->top;
    if (available_width <= 0 || available_height <= 0) return 0;
    width = desired_width;
    height = desired_height;
    if (width > available_width || height > available_height) {
        if ((lib_u64)available_width * (lib_u64)desired_height <=
            (lib_u64)available_height * (lib_u64)desired_width) {
            width = available_width;
            height = (int)((lib_u64)width * (lib_u64)desired_height /
                (lib_u64)desired_width);
        } else {
            height = available_height;
            width = (int)((lib_u64)height * (lib_u64)desired_width /
                (lib_u64)desired_height);
        }
        if (width <= 0 || height <= 0) return 0;
    }
    fitted->left = work_area->left + (available_width - width) / 2;
    fitted->top = work_area->top + (available_height - height) / 2;
    fitted->right = fitted->left + width;
    fitted->bottom = fitted->top + height;
    return 1;
}

int ui_win32_fit_client_size(const lib_win32_rect *work_area, int decoration_width,
    int decoration_height, int desired_width, int desired_height,
    int *fitted_width, int *fitted_height)
{
    int available_width;
    int available_height;

    if (work_area == LIB_NULL || fitted_width == LIB_NULL || fitted_height == LIB_NULL ||
        decoration_width < 0 || decoration_height < 0 || desired_width <= 0 ||
        desired_height <= 0) return 0;
    available_width = (work_area->right - work_area->left) - decoration_width;
    available_height = (work_area->bottom - work_area->top) - decoration_height;
    if (available_width <= 0 || available_height <= 0) return 0;
    if (desired_width <= available_width && desired_height <= available_height) {
        *fitted_width = desired_width;
        *fitted_height = desired_height;
        return 1;
    }
    return ui_win32_fit_aspect_size(available_width, available_height,
        (lib_u32)desired_width, (lib_u32)desired_height, fitted_width,
        fitted_height);
}

int ui_win32_fit_aspect_size(int available_width, int available_height,
    lib_u32 source_width, lib_u32 source_height, int *fitted_width,
    int *fitted_height)
{
    int width;
    int height;

    if (available_width <= 0 || available_height <= 0 || source_width == 0u ||
        source_height == 0u || fitted_width == LIB_NULL || fitted_height == LIB_NULL)
        return 0;
    if ((lib_u64)available_width * source_height <=
        (lib_u64)available_height * source_width) {
        width = available_width;
        height = (int)((lib_u64)width * source_height / source_width);
    } else {
        height = available_height;
        width = (int)((lib_u64)height * source_width / source_height);
    }
    if (width <= 0 || height <= 0) return 0;
    *fitted_width = width;
    *fitted_height = height;
    return 1;
}

static int ui_win32_window_decoration(lib_win32_hwnd window, int *width, int *height)
{
    lib_win32_rect outer;
    lib_win32_rect client;

    if (window == LIB_NULL || width == LIB_NULL || height == LIB_NULL ||
        !lib_win32_get_window_rect(window, &outer) || !lib_win32_get_client_rect(window, &client))
        return 0;
    *width = (outer.right - outer.left) - (client.right - client.left);
    *height = (outer.bottom - outer.top) - (client.bottom - client.top);
    return *width >= 0 && *height >= 0;
}

int ui_win32_enforce_client_aspect(lib_win32_hwnd window, lib_u32 source_width,
    lib_u32 source_height)
{
    lib_win32_rect client;
    int decoration_width;
    int decoration_height;
    int target_width;
    int target_height;

    if (window == LIB_NULL || source_width == 0u || source_height == 0u ||
        !lib_win32_get_client_rect(window, &client) || !ui_win32_window_decoration(window,
            &decoration_width, &decoration_height) ||
        !ui_win32_fit_aspect_size(client.right - client.left,
            client.bottom - client.top, source_width, source_height,
            &target_width, &target_height)) return 0;
    if (target_width == client.right - client.left &&
        target_height == client.bottom - client.top) return 1;
    return lib_win32_set_window_pos(window, LIB_NULL, 0, 0, target_width + decoration_width,
        target_height + decoration_height,
        LIB_WIN32_SWP_NOMOVE | LIB_WIN32_SWP_NOZORDER | LIB_WIN32_SWP_NOACTIVATE) != LIB_WIN32_FALSE;
}

int ui_win32_maximize_client(lib_win32_hwnd window, lib_u32 source_width,
    lib_u32 source_height)
{
    lib_win32_monitorinfo monitor_info;
    lib_win32_hmonitor monitor;
    int decoration_width;
    int decoration_height;
    int client_width;
    int client_height;
    int outer_width;
    int outer_height;
    int available_width;
    int available_height;

    if (window == LIB_NULL || source_width == 0u || source_height == 0u ||
        !ui_win32_window_decoration(window, &decoration_width,
            &decoration_height)) return 0;
    monitor = lib_win32_monitor_from_window(window, LIB_WIN32_MONITOR_DEFAULTTONEAREST);
    lib_win32_zero_memory(&monitor_info, sizeof(monitor_info));
    monitor_info.cbSize = sizeof(monitor_info);
    if (monitor == LIB_NULL || !lib_win32_get_monitor_info_a(monitor, &monitor_info)) return 0;
    available_width = (monitor_info.rcWork.right - monitor_info.rcWork.left) -
        decoration_width;
    available_height = (monitor_info.rcWork.bottom - monitor_info.rcWork.top) -
        decoration_height;
    if (!ui_win32_fit_aspect_size(available_width, available_height,
            source_width, source_height, &client_width, &client_height)) return 0;
    outer_width = client_width + decoration_width;
    outer_height = client_height + decoration_height;
    return lib_win32_set_window_pos(window, LIB_NULL,
        monitor_info.rcWork.left + ((monitor_info.rcWork.right -
            monitor_info.rcWork.left) - outer_width) / 2,
        monitor_info.rcWork.top + ((monitor_info.rcWork.bottom -
            monitor_info.rcWork.top) - outer_height) / 2,
        outer_width, outer_height, LIB_WIN32_SWP_NOZORDER | LIB_WIN32_SWP_NOACTIVATE) != LIB_WIN32_FALSE;
}

int ui_win32_resize_client(lib_win32_hwnd window, lib_u32 width,
    lib_u32 height)
{
    lib_win32_rect outer;
    lib_win32_monitorinfo monitor_info;
    lib_win32_hmonitor monitor;
    lib_win32_dword style;
    lib_win32_dword extended_style;

    if (window == LIB_NULL || width == 0u || height == 0u) return 0;
    lib_win32_set_rect(&outer, 0, 0, (int)width, (int)height);
    style = (lib_win32_dword)lib_win32_get_window_long_ptr_a(window, LIB_WIN32_GWL_STYLE);
    extended_style = (lib_win32_dword)lib_win32_get_window_long_ptr_a(window, LIB_WIN32_GWL_EXSTYLE);
    if (!lib_win32_adjust_window_rect_ex(&outer, style, LIB_WIN32_FALSE, extended_style)) return 0;
    monitor = lib_win32_monitor_from_window(window, LIB_WIN32_MONITOR_DEFAULTTONEAREST);
    lib_win32_zero_memory(&monitor_info, sizeof(monitor_info));
    monitor_info.cbSize = sizeof(monitor_info);
    if (monitor != LIB_NULL && lib_win32_get_monitor_info_a(monitor, &monitor_info)) {
        int decoration_width = (outer.right - outer.left) - (int)width;
        int decoration_height = (outer.bottom - outer.top) - (int)height;
        int fitted_width;
        int fitted_height;
        if (ui_win32_fit_client_size(&monitor_info.rcWork, decoration_width,
                decoration_height, (int)width, (int)height, &fitted_width,
                &fitted_height))
            lib_win32_set_rect(&outer, 0, 0, fitted_width + decoration_width,
                fitted_height + decoration_height);
    }
    lib_win32_set_window_pos(window, LIB_NULL, 0, 0, outer.right - outer.left,
        outer.bottom - outer.top, LIB_WIN32_SWP_NOMOVE | LIB_WIN32_SWP_NOZORDER | LIB_WIN32_SWP_NOACTIVATE);
    return 1;
}

void ui_win32_constrain_sizing(lib_win32_hwnd window, lib_win32_wparam edge,
    lib_win32_rect *outer, lib_u32 source_width, lib_u32 source_height)
{
    lib_win32_rect current_window;
    lib_win32_rect current_client;
    int frame_width;
    int frame_height;
    int client_width;
    int client_height;
    int target_width;
    int target_height;

    if (window == LIB_NULL || outer == LIB_NULL || source_width == 0u ||
        source_height == 0u) return;
    lib_win32_get_window_rect(window, &current_window);
    lib_win32_get_client_rect(window, &current_client);
    frame_width = (current_window.right - current_window.left) -
        (current_client.right - current_client.left);
    frame_height = (current_window.bottom - current_window.top) -
        (current_client.bottom - current_client.top);
    target_width = outer->right - outer->left;
    target_height = outer->bottom - outer->top;
    client_width = target_width - frame_width;
    client_height = target_height - frame_height;
    if (client_width <= 0 || client_height <= 0) return;
    if (edge == LIB_WIN32_WMSZ_LEFT || edge == LIB_WIN32_WMSZ_RIGHT) {
        client_height = (int)((lib_u64)client_width * source_height /
            source_width);
    } else if (edge == LIB_WIN32_WMSZ_TOP || edge == LIB_WIN32_WMSZ_BOTTOM) {
        client_width = (int)((lib_u64)client_height * source_width /
            source_height);
    } else if ((lib_u64)client_width * source_height >=
        (lib_u64)client_height * source_width) {
        client_height = (int)((lib_u64)client_width * source_height /
            source_width);
    } else {
        client_width = (int)((lib_u64)client_height * source_width /
            source_height);
    }
    target_width = client_width + frame_width;
    target_height = client_height + frame_height;
    if (edge == LIB_WIN32_WMSZ_LEFT || edge == LIB_WIN32_WMSZ_TOPLEFT || edge == LIB_WIN32_WMSZ_BOTTOMLEFT)
        outer->left = outer->right - target_width;
    else
        outer->right = outer->left + target_width;
    if (edge == LIB_WIN32_WMSZ_TOP || edge == LIB_WIN32_WMSZ_TOPLEFT || edge == LIB_WIN32_WMSZ_TOPRIGHT)
        outer->top = outer->bottom - target_height;
    else
        outer->bottom = outer->top + target_height;
}
