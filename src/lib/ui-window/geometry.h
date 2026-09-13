#ifndef UI_WINDOW_GEOMETRY_H
#define UI_WINDOW_GEOMETRY_H
#include "lib/ui-base/frame_interface.h"

typedef struct ui_window_rect { lib_i32 left, top, right, bottom; } ui_window_rect;
int ui_window_display_rect(int width, int height, lib_u32 source_width,
    lib_u32 source_height, ui_window_rect *out);
void ui_window_map_dirty_rect(const ui_window_rect *source, const ui_window_rect *display,
    lib_u32 width, lib_u32 height, ui_window_rect *out);
int ui_window_fit_outer_rect(const ui_window_rect *work, int width, int height,
    ui_window_rect *out);
int ui_window_fit_client_size(const ui_window_rect *work, int decoration_width,
    int decoration_height, int width, int height, int *out_width, int *out_height);
int ui_window_fit_aspect_size(int width, int height, lib_u32 source_width,
    lib_u32 source_height, int *out_width, int *out_height);
int ui_window_cursor_rect(const ui_frame *frame, const ui_window_rect *display,
    ui_window_rect *cursor);
#endif
