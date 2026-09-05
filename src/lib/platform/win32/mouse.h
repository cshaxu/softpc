#ifndef WIN32_PRESENTATION_MOUSE_H
#define WIN32_PRESENTATION_MOUSE_H

#ifdef _WIN32
#include <windows.h>

/* Host-only relative-pointer state.  The caller owns guest-device buttons and
 * lifecycle policy; this component owns explicit Win32 client capture only. */
typedef struct win32_presentation_mouse {
    int x;
    int y;
    int valid;
    int captured;
    int host_cursor_hidden;
} win32_presentation_mouse;

void win32_presentation_mouse_reset(win32_presentation_mouse *mouse);
void win32_presentation_mouse_release(win32_presentation_mouse *mouse);
int win32_presentation_mouse_capture(win32_presentation_mouse *mouse,
    HWND window, LPARAM position);
int win32_presentation_mouse_move(win32_presentation_mouse *mouse,
    LPARAM position, int client_width, int client_height,
    unsigned int guest_width, unsigned int guest_height, int *dx, int *dy);
int win32_presentation_mouse_captured(const win32_presentation_mouse *mouse);
int win32_presentation_mouse_hides_host_cursor(
    const win32_presentation_mouse *mouse);
#endif

#endif
