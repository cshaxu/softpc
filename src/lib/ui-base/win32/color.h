#ifndef UI_BASE_WIN32_COLOR_H
#define UI_BASE_WIN32_COLOR_H

#include "lib/types/types_interface.h"

#include "lib/types/win32.h"

/* Shared native-boundary conversion for copied 0x00RRGGBB palette values. */
COLORREF ui_win32_colorref_from_rgb(lib_u32 rgb);

#endif
