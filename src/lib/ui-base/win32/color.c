#include "lib/types/win32/window.h"
#include "lib/ui-base/win32/color.h"

lib_win32_colorref ui_win32_colorref_from_rgb(lib_u32 rgb)
{
    return lib_win32_rgb((rgb >> 16u) & 0xffu, (rgb >> 8u) & 0xffu, rgb & 0xffu);
}
