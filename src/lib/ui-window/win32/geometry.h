#ifndef UI_WIN32_GEOMETRY_H
#define UI_WIN32_GEOMETRY_H

#include "lib/types/types_interface.h"

#include "lib/types/win32/window.h"

/* Host-only geometry helpers.  They map copied source coordinates to a Win32
 * client surface and own no product rendering or lifecycle policy. */
int ui_win32_display_rect(int client_width, int client_height,
    lib_u32 source_width, lib_u32 source_height, lib_win32_rect *display);
void ui_win32_map_dirty_rect(const lib_win32_rect *source, const lib_win32_rect *display,
    lib_u32 source_width, lib_u32 source_height, lib_win32_rect *target);
int ui_win32_fit_outer_rect(const lib_win32_rect *work_area, int desired_width,
    int desired_height, lib_win32_rect *fitted);
int ui_win32_fit_client_size(const lib_win32_rect *work_area, int decoration_width,
    int decoration_height, int desired_width, int desired_height,
    int *fitted_width, int *fitted_height);
int ui_win32_fit_aspect_size(int available_width, int available_height,
    lib_u32 source_width, lib_u32 source_height, int *fitted_width,
    int *fitted_height);
int ui_win32_resize_client(lib_win32_hwnd window, lib_u32 width,
    lib_u32 height);
int ui_win32_enforce_client_aspect(lib_win32_hwnd window, lib_u32 source_width,
    lib_u32 source_height);
int ui_win32_maximize_client(lib_win32_hwnd window, lib_u32 source_width,
    lib_u32 source_height);
void ui_win32_constrain_sizing(lib_win32_hwnd window, lib_win32_wparam edge,
    lib_win32_rect *outer, lib_u32 source_width, lib_u32 source_height);
#endif
