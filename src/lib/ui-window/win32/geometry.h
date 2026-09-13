#ifndef UI_WIN32_GEOMETRY_H
#define UI_WIN32_GEOMETRY_H

#include "lib/types/types_interface.h"

#include "lib/types/win32/window.h"

#include "lib/ui-window/geometry.h"

/* Native rectangle marshalling only; all arithmetic belongs to geometry.c. */
static inline ui_window_rect ui_win32_rect_value(const lib_win32_rect *r)
{ return (ui_window_rect) { r->left, r->top, r->right, r->bottom }; }
static inline void ui_win32_rect_store(lib_win32_rect *to, const ui_window_rect *r)
{ to->left=r->left; to->top=r->top; to->right=r->right; to->bottom=r->bottom; }
static inline int ui_win32_display_rect(int w, int h, lib_u32 sw, lib_u32 sh,
    lib_win32_rect *out)
{
    ui_window_rect r;
    if (out == LIB_NULL || !ui_window_display_rect(w,h,sw,sh,&r)) return 0;
    ui_win32_rect_store(out,&r); return 1;
}
static inline void ui_win32_map_dirty_rect(const lib_win32_rect *src,
    const lib_win32_rect *display, lib_u32 w, lib_u32 h, lib_win32_rect *out)
{
    ui_window_rect a,b,r;
    if (!src || !display || !out || !w || !h) return;
    a=ui_win32_rect_value(src); b=ui_win32_rect_value(display);
    ui_window_map_dirty_rect(&a,&b,w,h,&r); ui_win32_rect_store(out,&r);
}
static inline int ui_win32_fit_outer_rect(const lib_win32_rect *work,
    int w, int h, lib_win32_rect *out)
{
    ui_window_rect a,r;
    if (!work || !out) return 0;
    a=ui_win32_rect_value(work);
    if (!ui_window_fit_outer_rect(&a,w,h,&r)) return 0;
    ui_win32_rect_store(out,&r); return 1;
}
static inline int ui_win32_fit_client_size(const lib_win32_rect *work,
    int dw, int dh, int w, int h, int *ow, int *oh)
{
    ui_window_rect r;
    if (!work) return 0;
    r=ui_win32_rect_value(work);
    return ui_window_fit_client_size(&r,dw,dh,w,h,ow,oh);
}
int ui_win32_resize_client(lib_win32_hwnd window, lib_u32 width,
    lib_u32 height);
int ui_win32_enforce_client_aspect(lib_win32_hwnd window, lib_u32 source_width,
    lib_u32 source_height);
int ui_win32_maximize_client(lib_win32_hwnd window, lib_u32 source_width,
    lib_u32 source_height);
void ui_win32_constrain_sizing(lib_win32_hwnd window, lib_win32_wparam edge,
    lib_win32_rect *outer, lib_u32 source_width, lib_u32 source_height);
#endif
