#ifndef WIN32_PRESENTATION_GEOMETRY_H
#define WIN32_PRESENTATION_GEOMETRY_H

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>

/* Host-only geometry helpers.  They map copied source coordinates to a Win32
 * client surface and own no guest rendering or lifecycle policy. */
int win32_presentation_display_rect(int client_width, int client_height,
    uint32_t source_width, uint32_t source_height, RECT *display);
void win32_presentation_map_dirty_rect(const RECT *source, const RECT *display,
    uint32_t source_width, uint32_t source_height, RECT *target);
uint32_t win32_presentation_dib_pixel(COLORREF colour);
int win32_presentation_resize_client(HWND window, uint32_t width,
    uint32_t height);
void win32_presentation_constrain_sizing(HWND window, WPARAM edge,
    RECT *outer, uint32_t source_width, uint32_t source_height);
#endif

#endif
