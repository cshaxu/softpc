#include "window.h"

#ifdef _WIN32
#include "runtime.h"
#include "keyboard.h"
#include "prompt_trace.h"
#include "../lib/platform/win32/geometry.h"
#include "../lib/platform/win32/mouse.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOFTPC_TEXT_COLUMNS 80
#define SOFTPC_TEXT_ROWS 25
#define SOFTPC_TEXT_CELL_WIDTH 8
#define SOFTPC_TEXT_CELL_HEIGHT 16
#define SOFTPC_TEXT_SURFACE_WIDTH (SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_CELL_WIDTH)
#define SOFTPC_TEXT_SURFACE_HEIGHT (SOFTPC_TEXT_ROWS * SOFTPC_TEXT_CELL_HEIGHT)
#define SOFTPC_GRAPHICS_SURFACE_MAX_WIDTH 1280u
#define SOFTPC_GRAPHICS_SURFACE_MAX_HEIGHT 768u
#define SOFTPC_TIMER_ID 1u
#define SOFTPC_CURSOR_BLINK_INTERVAL_MS 250u

static app_runtime *app_window_runtime;
static app_runtime_frame *app_window_frame;
static HDC app_window_text_dc;
static HBITMAP app_window_text_bitmap;
static HGDIOBJ app_window_text_previous_bitmap;
static uint32_t *app_window_text_pixels;
static HDC app_window_graphics_dc;
static HBITMAP app_window_graphics_bitmap;
static HGDIOBJ app_window_graphics_previous_bitmap;
static uint32_t *app_window_graphics_pixels;
static uint32_t app_window_graphics_width;
static uint32_t app_window_graphics_height;
static uint8_t app_window_graphics_palette[SOFTPC_RUNTIME_DIB_INFO_BYTES];
static int app_window_graphics_valid;
static unsigned char app_window_presented_text[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
static unsigned short app_window_presented_attributes[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
static uint32_t app_window_presented_text_palette[16u];
static unsigned char app_window_presented_font[256u * 16u];
static unsigned char app_window_presented_secondary_font[256u * 16u];
static uint32_t app_window_presented_font_height;
static uint32_t app_window_presented_attribute_font_select;
static int app_window_presented_text_valid;
static uint32_t app_window_displayed_sequence;
static int app_window_result;
static int app_window_auto_switch;
static int app_window_auto_graphics_presented;
static unsigned int app_window_auto_text_frames;
static win32_presentation_keyboard_normalizer app_window_keyboard_normalizer;
static int app_window_left_button;
static int app_window_right_button;
static win32_presentation_mouse app_window_mouse_state;
static WPARAM app_window_suppressed_hotkey;
static uint32_t app_window_surface_width;
static uint32_t app_window_surface_height;
static int app_window_client_width;
static int app_window_client_height;
static int app_window_cursor_blink_visible;
static DWORD app_window_cursor_blink_due;

static int app_window_guest_running(void)
{
    return app_window_runtime != NULL &&
        app_runtime_get_state(app_window_runtime) ==
        SOFTPC_RUNTIME_RUNNING;
}

static void app_window_update_title(HWND window)
{
    app_runtime_state state;

    if (window == NULL || app_window_runtime == NULL) return;
    state = app_runtime_get_state(app_window_runtime);
    SetWindowTextA(window, state == SOFTPC_RUNTIME_PAUSED ?
        "Insignia SoftPC (Paused)" : "Insignia SoftPC (Running)");
}

static COLORREF app_window_colour(unsigned int colour)
{
    static const COLORREF palette[16] = {
        RGB(0, 0, 0), RGB(0, 0, 170), RGB(0, 170, 0), RGB(0, 170, 170),
        RGB(170, 0, 0), RGB(170, 0, 170), RGB(170, 85, 0), RGB(170, 170, 170),
        RGB(85, 85, 85), RGB(85, 85, 255), RGB(85, 255, 85), RGB(85, 255, 255),
        RGB(255, 85, 85), RGB(255, 85, 255), RGB(255, 255, 85), RGB(255, 255, 255)
    };
    unsigned int index = colour & 0x0fu;

    /* The original nt_graph text path supplies the active guest VGA palette
       with each copied frame.  The fallback is used only before a machine
       frame exists, never as a substitute for guest colour programming. */
    if (app_window_frame != NULL && app_window_frame->valid != 0u)
        return (COLORREF)app_window_frame->text_palette[index];
    return palette[index];
}

/* A COLORREF is packed for Win32 colour APIs (0x00bbggrr), whereas the
 * standalone text DIB is a 32-bit BI_RGB surface (0x00rrggbb as a DWORD).
 * The original nt_graph palette is already correct; convert only at this
 * final GDI storage boundary. */
static uint32_t app_window_dib_pixel(COLORREF colour)
{
    return win32_presentation_dib_pixel(colour);
}

/* Keep the byte-order conversion observable to the Win32 presenter smoke
 * test.  This is deliberately test-only: it does not add a VM interface or
 * alter the copied SoftPC video path. */
#ifdef SOFTPC_WINDOW_TESTING
uint32_t app_window_test_dib_pixel(COLORREF colour)
{
    return app_window_dib_pixel(colour);
}
#endif

/* The frontend has no independent canvas size. By default its client area is
 * the physical guest surface; after a user resize, the final GDI blit follows
 * that client area with no unused letterbox space. */
static int app_window_display_rect(HWND window, uint32_t source_width,
    uint32_t source_height, RECT *display)
{
    if (window == NULL) return 0;
    return win32_presentation_display_rect(app_window_client_width,
        app_window_client_height, source_width, source_height, display);
}

static void app_window_capture_client_size(HWND window)
{
    RECT client;

    if (window == NULL) return;
    GetClientRect(window, &client);
    app_window_client_width = client.right - client.left;
    app_window_client_height = client.bottom - client.top;
}

static void app_window_resize_surface(HWND window, uint32_t width,
    uint32_t height)
{
    if (window == NULL || width == 0u || height == 0u ||
        (app_window_surface_width == width &&
         app_window_surface_height == height)) return;
    /* Preserve the user's desktop position across guest mode changes. */
    if (!win32_presentation_resize_client(window, width, height)) return;
    app_window_surface_width = width;
    app_window_surface_height = height;
}

static void app_window_resize_frame(HWND window)
{
    if (app_window_frame == NULL || app_window_frame->valid == 0u)
        return;
    if (app_window_frame->graphics != 0u)
        app_window_resize_surface(window, app_window_frame->dib_width,
            app_window_frame->dib_height);
    else
        app_window_resize_surface(window, SOFTPC_TEXT_SURFACE_WIDTH,
            SOFTPC_TEXT_SURFACE_HEIGHT);
}

static void app_window_current_surface_size(uint32_t *width,
    uint32_t *height)
{
    if (width == NULL || height == NULL) return;
    if (app_window_frame != NULL && app_window_frame->valid != 0u &&
        app_window_frame->graphics != 0u) {
        *width = app_window_frame->dib_width;
        *height = app_window_frame->dib_height;
    } else {
        *width = SOFTPC_TEXT_SURFACE_WIDTH;
        *height = SOFTPC_TEXT_SURFACE_HEIGHT;
    }
}

static void app_window_constrain_sizing(HWND window, WPARAM edge,
    RECT *outer)
{
    uint32_t source_width;
    uint32_t source_height;

    if (window == NULL || outer == NULL) return;
    app_window_current_surface_size(&source_width, &source_height);
    win32_presentation_constrain_sizing(window, edge, outer, source_width,
        source_height);
}

static void app_window_update_text_surface(void)
{
    int row;
    if (app_window_frame == NULL || app_window_text_dc == NULL ||
        (app_window_presented_text_valid && memcmp(app_window_presented_text,
            app_window_frame->text, sizeof(app_window_presented_text)) == 0 &&
         memcmp(app_window_presented_attributes,
            app_window_frame->attributes,
            sizeof(app_window_presented_attributes)) == 0 &&
         memcmp(app_window_presented_text_palette,
            app_window_frame->text_palette,
            sizeof(app_window_presented_text_palette)) == 0 &&
         memcmp(app_window_presented_font, app_window_frame->font,
            sizeof(app_window_presented_font)) == 0 &&
         memcmp(app_window_presented_secondary_font,
            app_window_frame->secondary_font,
            sizeof(app_window_presented_secondary_font)) == 0 &&
         app_window_presented_font_height ==
            app_window_frame->font_height &&
         app_window_presented_attribute_font_select ==
            app_window_frame->attribute_font_select)) return;
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned int scan;
            size_t index = (size_t)row * SOFTPC_TEXT_COLUMNS + column;
            unsigned char character = app_window_frame->text[index];
            unsigned short attribute = app_window_frame->attributes[index];
            for (scan = 0u; scan < SOFTPC_TEXT_CELL_HEIGHT; ++scan) {
                const unsigned char *font =
                    app_window_frame->attribute_font_select != 0u &&
                    (attribute & 0x08u) != 0u ?
                    app_window_frame->secondary_font : app_window_frame->font;
                unsigned char bits = font[
                    (size_t)character * 16u + scan];
                unsigned int bit;
                uint32_t *pixels = app_window_text_pixels +
                    ((size_t)row * SOFTPC_TEXT_CELL_HEIGHT + scan) *
                    SOFTPC_TEXT_SURFACE_WIDTH + column * SOFTPC_TEXT_CELL_WIDTH;
                for (bit = 0u; bit < SOFTPC_TEXT_CELL_WIDTH; ++bit)
                    pixels[bit] = app_window_dib_pixel(
                        (bits & (0x80u >> bit)) ?
                        app_window_colour(attribute) :
                        app_window_colour(attribute >> 4));
            }
        }
    }
    memcpy(app_window_presented_text, app_window_frame->text,
        sizeof(app_window_presented_text));
    memcpy(app_window_presented_attributes, app_window_frame->attributes,
        sizeof(app_window_presented_attributes));
    memcpy(app_window_presented_text_palette,
        app_window_frame->text_palette,
        sizeof(app_window_presented_text_palette));
    memcpy(app_window_presented_font, app_window_frame->font,
        sizeof(app_window_presented_font));
    memcpy(app_window_presented_secondary_font,
        app_window_frame->secondary_font,
        sizeof(app_window_presented_secondary_font));
    app_window_presented_font_height = app_window_frame->font_height;
    app_window_presented_attribute_font_select =
        app_window_frame->attribute_font_select;
    app_window_presented_text_valid = 1;
}

/* The original nt_ega/nt_vga painters own the indexed DIB and palette.  Some
 * current Win32/RDP paths fail to blit that indexed, top-down DIB directly to
 * a window even though its bytes are valid (the exact same frame writes a
 * correct BMP). Convert only at the final frontend outlet to an RGB32 DIB;
 * this is the same isolated presentation boundary as the text surface. */
static int app_window_update_graphics_surface(RECT *changed)
{
    const BITMAPINFO *dib;
    uint32_t source_stride;
    uint32_t row;
    int full_refresh;
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;

    if (app_window_frame == NULL || app_window_graphics_pixels == NULL ||
        app_window_frame->graphics == 0u ||
        app_window_frame->dib_width == 0u ||
        app_window_frame->dib_height == 0u ||
        app_window_frame->dib_width > SOFTPC_GRAPHICS_SURFACE_MAX_WIDTH ||
        app_window_frame->dib_height > SOFTPC_GRAPHICS_SURFACE_MAX_HEIGHT ||
        changed == NULL)
        return 0;
    dib = (const BITMAPINFO *)app_window_frame->dib_info;
    full_refresh = !app_window_graphics_valid ||
        app_window_graphics_width != app_window_frame->dib_width ||
        app_window_graphics_height != app_window_frame->dib_height ||
        memcmp(app_window_graphics_palette, app_window_frame->dib_info,
            sizeof(app_window_graphics_palette)) != 0;
    left = full_refresh ? 0 : app_window_frame->dirty_left;
    top = full_refresh ? 0 : app_window_frame->dirty_top;
    right = full_refresh ? (int32_t)app_window_frame->dib_width - 1 :
        app_window_frame->dirty_right;
    bottom = full_refresh ? (int32_t)app_window_frame->dib_height - 1 :
        app_window_frame->dirty_bottom;
    if (left < 0) left = 0;
    if (top < 0) top = 0;
    if (right >= (int32_t)app_window_frame->dib_width)
        right = (int32_t)app_window_frame->dib_width - 1;
    if (bottom >= (int32_t)app_window_frame->dib_height)
        bottom = (int32_t)app_window_frame->dib_height - 1;
    if (right < left || bottom < top) return 0;
    source_stride = (app_window_frame->dib_width + 3u) & ~3u;
    for (row = (uint32_t)top; row <= (uint32_t)bottom; ++row) {
        const uint8_t *source = app_window_frame->dib_bits +
            row * source_stride;
        uint32_t *destination = app_window_graphics_pixels +
            row * SOFTPC_GRAPHICS_SURFACE_MAX_WIDTH;
        uint32_t column;
        for (column = (uint32_t)left; column <= (uint32_t)right; ++column) {
            const RGBQUAD *colour = &dib->bmiColors[source[column]];
            destination[column] = ((uint32_t)colour->rgbRed << 16) |
                ((uint32_t)colour->rgbGreen << 8) |
                (uint32_t)colour->rgbBlue;
        }
    }
    app_window_graphics_width = app_window_frame->dib_width;
    app_window_graphics_height = app_window_frame->dib_height;
    memcpy(app_window_graphics_palette, app_window_frame->dib_info,
        sizeof(app_window_graphics_palette));
    app_window_graphics_valid = 1;
    changed->left = left;
    changed->top = top;
    changed->right = right + 1;
    changed->bottom = bottom + 1;
    app_prompt_trace("softpc prompt rgb frame=%lu full=%d rect=%ld,%ld,%ld,%ld",
        (unsigned long)app_window_frame->sequence, full_refresh,
        (long)changed->left, (long)changed->top, (long)changed->right,
        (long)changed->bottom);
    return 1;
}

static void app_window_invalidate_graphics(HWND window, const RECT *source)
{
    RECT display;
    RECT target;
    uint32_t width;
    uint32_t height;

    if (window == NULL || source == NULL || app_window_frame == NULL ||
        !app_window_display_rect(window, app_window_frame->dib_width,
            app_window_frame->dib_height, &display)) return;
    width = app_window_frame->dib_width;
    height = app_window_frame->dib_height;
    win32_presentation_map_dirty_rect(source, &display, width, height,
        &target);
    InvalidateRect(window, &target, FALSE);
}

/* The original nt_graph endpoint gave Windows Console a real cursor.  Its
 * blink was therefore owned by the host, not by a guest timer or by C-VID.
 * Keep that boundary: this merely describes the copied-frame overlay that the
 * standalone window may invalidate between otherwise unchanged frames. */
static int app_window_cursor_rect(HWND window, RECT *cursor)
{
    RECT display;
    int width;
    int height;
    int cell_height;
    int cursor_height;
    uint32_t cursor_size;

    if (window == NULL || cursor == NULL || app_window_frame == NULL ||
        app_window_frame->valid == 0u ||
        app_window_frame->graphics != 0u ||
        app_window_frame->cursor_column < 0 ||
        app_window_frame->cursor_row < 0 ||
        app_window_frame->cursor_column >= SOFTPC_TEXT_COLUMNS ||
        app_window_frame->cursor_row >= SOFTPC_TEXT_ROWS) return 0;
    if (!app_window_display_rect(window, SOFTPC_TEXT_SURFACE_WIDTH,
            SOFTPC_TEXT_SURFACE_HEIGHT, &display)) return 0;
    width = display.right - display.left;
    height = display.bottom - display.top;
    cell_height = height / SOFTPC_TEXT_ROWS;
    if (width <= 0 || cell_height <= 0) return 0;
    cursor_size = app_window_frame->cursor_size;
    if (cursor_size == 0u || cursor_size > 100u) cursor_size = 100u;
    cursor_height = (int)((cell_height * cursor_size + 99u) / 100u);
    if (cursor_height > cell_height) cursor_height = cell_height;
    cursor->left = display.left + app_window_frame->cursor_column * width /
        SOFTPC_TEXT_COLUMNS;
    cursor->right = display.left + (app_window_frame->cursor_column + 1) * width /
        SOFTPC_TEXT_COLUMNS;
    cursor->top = display.top + (app_window_frame->cursor_row + 1) * cell_height -
        cursor_height;
    cursor->bottom = display.top + (app_window_frame->cursor_row + 1) * height /
        SOFTPC_TEXT_ROWS;
    return cursor->right > cursor->left && cursor->bottom > cursor->top;
}

static void app_window_advance_cursor_blink(HWND window)
{
    RECT cursor;
    DWORD now = GetTickCount();

    /* The text cursor is a host presentation overlay.  Once the executor is
       paused, preserve the last composited frame exactly: do not let this
       host-only blink timer alter an otherwise frozen guest display. */
    if (!app_window_guest_running()) return;
    if ((LONG)(now - app_window_cursor_blink_due) < 0) return;
    app_window_cursor_blink_visible = !app_window_cursor_blink_visible;
    app_window_cursor_blink_due = now + SOFTPC_CURSOR_BLINK_INTERVAL_MS;
    if (app_window_cursor_rect(window, &cursor))
        InvalidateRect(window, &cursor, FALSE);
}

static void app_window_paint(HWND window, HDC dc)
{
    RECT display;

    if (app_window_frame == NULL || app_window_frame->valid == 0u) return;
    if (app_window_frame->graphics != 0u) {
        if (!app_window_display_rect(window,
                app_window_frame->dib_width,
                app_window_frame->dib_height, &display)) return;
        if (app_window_graphics_dc == NULL ||
            app_window_graphics_width != app_window_frame->dib_width ||
            app_window_graphics_height != app_window_frame->dib_height)
            return;
        StretchBlt(dc, display.left, display.top,
            display.right - display.left, display.bottom - display.top,
            app_window_graphics_dc, 0, 0,
            (int)app_window_graphics_width,
            (int)app_window_graphics_height, SRCCOPY);
        return;
    }
    app_window_update_text_surface();
    if (!app_window_display_rect(window, SOFTPC_TEXT_SURFACE_WIDTH,
            SOFTPC_TEXT_SURFACE_HEIGHT, &display)) return;
    StretchBlt(dc, display.left, display.top, display.right - display.left,
        display.bottom - display.top, app_window_text_dc, 0, 0,
        SOFTPC_TEXT_SURFACE_WIDTH, SOFTPC_TEXT_SURFACE_HEIGHT, SRCCOPY);
    if (app_window_cursor_blink_visible) {
        RECT cursor;
        /* nt_graph publishes the original controller-selected text cursor
           through the compatibility Console endpoint.  Draw it only after
           the copied text DIB reaches the window, so this remains a pure
           frontend overlay and never changes guest video memory. */
        if (app_window_cursor_rect(window, &cursor))
            InvertRect(dc, &cursor);
    }
}

static void app_window_transition(WPARAM key, LPARAM lparam, int released)
{
    WORD scan = (WORD)((lparam >> 16) & 0xffu);
    DWORD control_state = (lparam & 0x01000000L) != 0 ? ENHANCED_KEY : 0u;
    if (scan == 0u && !released)
        win32_presentation_keyboard_note_recovered_key(
            &app_window_keyboard_normalizer, (WORD)key);
    if (scan == 0u && released)
        win32_presentation_keyboard_release_recovered_key(
            &app_window_keyboard_normalizer, (WORD)key);
    (void)win32_presentation_keyboard_submit_transition(app_window_runtime,
        app_keyboard_enqueue_win32_event, scan, (WORD)key, control_state,
        !released);
}

static void app_window_mouse(LPARAM position)
{
    int dx = 0, dy = 0;
    uint32_t guest_width;
    uint32_t guest_height;

    if (!app_window_guest_running()) return;
    /* Mouse counters belong to the native guest surface, whereas WM_MOUSE
       reports pixels in the current (possibly user-scaled) client area.
       Preserve the same physical InPort controller while making an enlarged
       or reduced window describe the same guest movement. */
    app_window_current_surface_size(&guest_width, &guest_height);
    if (!win32_presentation_mouse_move(&app_window_mouse_state, position,
            app_window_client_width, app_window_client_height, guest_width,
            guest_height, &dx, &dy)) return;
    (void)app_runtime_enqueue_mouse(app_window_runtime, dx, dy,
        (uint8_t)app_window_left_button,
        (uint8_t)app_window_right_button);
}

/* Windowed mouse input is a relative guest device.  Once the user explicitly
 * clicks the guest surface, keep the host pointer in that surface so it cannot
 * accidentally operate the desktop while its deltas are being delivered to
 * the original Bus Mouse controller.  Ctrl+Alt+M (and loss of window focus)
 * releases this purely frontend capture; no guest controller state changes. */
static void app_window_release_mouse_capture(void)
{
    win32_presentation_mouse_release(&app_window_mouse_state);
}

static void app_window_capture_mouse(HWND window, LPARAM position)
{
    if (window == NULL || !app_window_guest_running()) return;
    (void)win32_presentation_mouse_capture(&app_window_mouse_state, window,
        position);
    /* The guest owns the visible pointer after an explicit click.  Returning
       NULL from WM_SETCURSOR keeps the desktop arrow out of the guest DIB
       without changing any SoftPC device or guest cursor state. */
    SetCursor(NULL);
}

static LRESULT CALLBACK app_window_proc(HWND window, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_TIMER:
        if (wparam == SOFTPC_TIMER_ID) {
            if (app_runtime_published_frame_sequence(
                    app_window_runtime) != app_window_displayed_sequence &&
                app_runtime_copy_frame(app_window_runtime,
                    app_window_frame)) {
                app_window_displayed_sequence = app_window_frame->sequence;
                if (app_window_frame->graphics != 0u) {
                    RECT changed;
                    int graphics_changed = app_window_update_graphics_surface(
                        &changed);
                    app_window_auto_graphics_presented = 1;
                    app_window_auto_text_frames = 0u;
                    app_window_resize_frame(window);
                    if (graphics_changed)
                        app_window_invalidate_graphics(window, &changed);
                } else if (app_window_auto_switch &&
                    app_window_auto_graphics_presented) {
                    /* C-VID can publish a transient text snapshot around a
                       graphics transition.  Do not tear down a just-opened
                       window until the original renderer has demonstrated a
                       stable return to text mode. */
                    if (++app_window_auto_text_frames >= 3u) {
                        app_prompt_trace("softpc prompt route window->console frame=%lu",
                            (unsigned long)app_window_frame->sequence);
                        app_window_result = SOFTPC_VM_FRONTEND_SWITCH_CONSOLE;
                        DestroyWindow(window);
                        return 0;
                    }
                }
                if (app_window_frame->graphics == 0u) {
                    app_window_resize_frame(window);
                    InvalidateRect(window, NULL, FALSE);
                }
            }
            app_window_advance_cursor_blink(window);
            app_window_update_title(window);
            if (app_runtime_get_state(app_window_runtime) ==
                SOFTPC_RUNTIME_STOPPED ||
                app_runtime_get_state(app_window_runtime) ==
                SOFTPC_RUNTIME_ERROR) DestroyWindow(window);
        }
        return 0;
    case WM_PAINT:
        { PAINTSTRUCT paint; HDC dc = BeginPaint(window, &paint);
          app_window_paint(window, dc); EndPaint(window, &paint); }
        return 0;
    case WM_SIZE:
        app_window_capture_client_size(window);
        return 0;
    case WM_ERASEBKGND:
        return 1;
    case WM_NCLBUTTONDBLCLK:
        if (wparam == HTCAPTION) {
            if (IsZoomed(window)) ShowWindow(window, SW_RESTORE);
            /* A user resize deliberately does not alter the guest-surface
               cache.  Double-click is an explicit request to override that
               host geometry and restore the current native guest size. */
            app_window_surface_width = 0u;
            app_window_surface_height = 0u;
            app_window_resize_frame(window);
            return 0;
        }
        break;
    case WM_SIZING:
        app_window_constrain_sizing(window, wparam, (RECT *)lparam);
        return TRUE;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (wparam == 'P' && GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_MENU) < 0) {
            if (app_runtime_get_state(app_window_runtime) ==
                SOFTPC_RUNTIME_RUNNING) {
                /* A paused guest cannot consume further pointer traffic.
                   Keep the final frame visible, release the host mouse, and
                   release the Ctrl/Alt makes which led to this host chord. */
                app_window_release_mouse_capture();
                (void)win32_presentation_keyboard_release_ctrl_alt(app_window_runtime,
                    app_keyboard_enqueue_win32_event);
                (void)app_runtime_pause(app_window_runtime);
            } else if (app_runtime_get_state(app_window_runtime) ==
                SOFTPC_RUNTIME_PAUSED) {
                (void)app_runtime_resume(app_window_runtime);
            }
            app_window_update_title(window);
            app_window_suppressed_hotkey = wparam;
        } else if (wparam == 'D' && GetKeyState(VK_CONTROL) < 0 &&
            GetKeyState(VK_MENU) < 0) {
            (void)win32_presentation_keyboard_release_ctrl_alt(app_window_runtime,
                app_keyboard_enqueue_win32_event);
            (void)win32_presentation_keyboard_submit_ctrl_alt_del(app_window_runtime,
                app_keyboard_enqueue_win32_event);
            app_window_suppressed_hotkey = wparam;
        } else if (wparam == 'F' && GetKeyState(VK_CONTROL) < 0 &&
            GetKeyState(VK_MENU) < 0) {
            (void)win32_presentation_keyboard_submit_alt_enter(app_window_runtime,
                app_keyboard_enqueue_win32_event);
            app_window_suppressed_hotkey = wparam;
        } else if (wparam == 'M' && GetKeyState(VK_CONTROL) < 0 &&
            GetKeyState(VK_MENU) < 0) {
            (void)win32_presentation_keyboard_release_ctrl_alt(app_window_runtime,
                app_keyboard_enqueue_win32_event);
            app_window_release_mouse_capture();
            app_window_suppressed_hotkey = wparam;
        } else if (app_window_guest_running())
            app_window_transition(wparam, lparam, 0);
        return 0;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        if (wparam == app_window_suppressed_hotkey) {
            app_window_suppressed_hotkey = 0u;
            return 0;
        }
        if (app_window_guest_running())
            app_window_transition(wparam, lparam, 1);
        return 0;
    case WM_CHAR:
        /* A physical WM_KEYDOWN has already been delivered.  A scan-less
           RDP text packet is normalized only when it is not that recovered
           physical key's duplicate character. */
        if (app_window_guest_running() &&
            ((uint32_t)lparam >> 16u & 0xffu) == 0u &&
            !win32_presentation_keyboard_consume_duplicate_character(
                &app_window_keyboard_normalizer, (WORD)wparam))
            (void)win32_presentation_keyboard_submit_utf16(
                &app_window_keyboard_normalizer, app_window_runtime,
                app_keyboard_enqueue_win32_event, (WORD)wparam);
        return 0;
    case WM_MOUSEMOVE:
        if (app_window_guest_running() &&
            win32_presentation_mouse_captured(&app_window_mouse_state))
            app_window_mouse(lparam);
        return 0;
    case WM_SETCURSOR:
        if (win32_presentation_mouse_hides_host_cursor(
                &app_window_mouse_state) && LOWORD(lparam) == HTCLIENT) {
            SetCursor(NULL);
            return TRUE;
        }
        break;
    case WM_LBUTTONDOWN:
        if (!app_window_guest_running()) return 0;
        app_window_left_button = 1;
        app_window_capture_mouse(window, lparam);
        app_window_mouse(lparam);
        return 0;
    case WM_LBUTTONUP:
        if (!app_window_guest_running()) return 0;
        app_window_left_button = 0;
        if (win32_presentation_mouse_captured(&app_window_mouse_state))
            app_window_mouse(lparam);
        return 0;
    case WM_RBUTTONDOWN:
        if (!app_window_guest_running()) return 0;
        app_window_right_button = 1;
        app_window_capture_mouse(window, lparam);
        app_window_mouse(lparam);
        return 0;
    case WM_RBUTTONUP:
        if (!app_window_guest_running()) return 0;
        app_window_right_button = 0;
        if (win32_presentation_mouse_captured(&app_window_mouse_state))
            app_window_mouse(lparam);
        return 0;
    case WM_KILLFOCUS:
        app_window_release_mouse_capture();
        return 0;
    case WM_CLOSE:
        /* Closing the presentation must not power off the machine.  The
           monitor owns its lifetime, so retain the paused executor and let a
           later `resume` create a fresh window for the same guest. */
        app_window_release_mouse_capture();
        app_window_result = SOFTPC_VM_FRONTEND_PAUSED;
        (void)app_runtime_pause(app_window_runtime);
        DestroyWindow(window);
        return 0;
    case WM_DESTROY:
        app_window_release_mouse_capture();
        KillTimer(window, SOFTPC_TIMER_ID);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

static int app_vm_run_window_mode(app_runtime *runtime,
    int auto_switch)
{
    WNDCLASSA klass;
    MSG message;
    HWND window;
    HDC dc;
    if (runtime == NULL) return SOFTPC_VM_FRONTEND_ERROR;
    ZeroMemory(&klass, sizeof(klass));
    klass.lpfnWndProc = app_window_proc;
    klass.hInstance = GetModuleHandleA(NULL);
    klass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    klass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    klass.lpszClassName = "SoftPCStandaloneWindow";
    if (RegisterClassA(&klass) == 0 && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        return SOFTPC_VM_FRONTEND_ERROR;
    app_window_frame = (app_runtime_frame *)calloc(1u, sizeof(*app_window_frame));
    if (app_window_frame == NULL) return SOFTPC_VM_FRONTEND_ERROR;
    app_window_runtime = runtime;
    app_window_auto_switch = auto_switch;
    app_window_auto_graphics_presented = 0;
    app_window_auto_text_frames = 0u;
    app_window_result = SOFTPC_VM_FRONTEND_STOPPED;
    app_window_presented_text_valid = 0;
    app_window_graphics_valid = 0;
    ZeroMemory(app_window_graphics_palette,
        sizeof(app_window_graphics_palette));
    app_window_displayed_sequence = 0u;
    ZeroMemory(&app_window_keyboard_normalizer,
        sizeof(app_window_keyboard_normalizer));
    app_window_left_button = app_window_right_button = 0;
    win32_presentation_mouse_reset(&app_window_mouse_state);
    app_window_surface_width = 0u;
    app_window_surface_height = 0u;
    app_window_client_width = 0;
    app_window_client_height = 0;
    app_window_cursor_blink_visible = 1;
    app_window_cursor_blink_due = GetTickCount() +
        SOFTPC_CURSOR_BLINK_INTERVAL_MS;
    /* Begin at the actual 80x25 guest client dimensions. */
    window = CreateWindowExA(0, klass.lpszClassName, "Insignia SoftPC (Running)",
        WS_THICKFRAME | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
        WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        CW_USEDEFAULT, 0, 680, 560, NULL, NULL, klass.hInstance, NULL);
    if (window == NULL) { free(app_window_frame); return SOFTPC_VM_FRONTEND_ERROR; }
    app_window_resize_surface(window, SOFTPC_TEXT_SURFACE_WIDTH,
        SOFTPC_TEXT_SURFACE_HEIGHT);
    app_window_capture_client_size(window);
    dc = GetDC(window);
    app_window_text_dc = CreateCompatibleDC(dc);
    app_window_graphics_dc = CreateCompatibleDC(dc);
    {
        BITMAPINFO info;
        ZeroMemory(&info, sizeof(info));
        info.bmiHeader.biSize = sizeof(info.bmiHeader);
        info.bmiHeader.biWidth = SOFTPC_TEXT_SURFACE_WIDTH;
        info.bmiHeader.biHeight = -SOFTPC_TEXT_SURFACE_HEIGHT;
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 32;
        info.bmiHeader.biCompression = BI_RGB;
        app_window_text_bitmap = CreateDIBSection(dc, &info, DIB_RGB_COLORS,
            (void **)&app_window_text_pixels, NULL, 0u);
    }
    {
        BITMAPINFO info;
        ZeroMemory(&info, sizeof(info));
        info.bmiHeader.biSize = sizeof(info.bmiHeader);
        info.bmiHeader.biWidth = SOFTPC_GRAPHICS_SURFACE_MAX_WIDTH;
        info.bmiHeader.biHeight = -(LONG)SOFTPC_GRAPHICS_SURFACE_MAX_HEIGHT;
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 32;
        info.bmiHeader.biCompression = BI_RGB;
        app_window_graphics_bitmap = CreateDIBSection(dc, &info,
            DIB_RGB_COLORS, (void **)&app_window_graphics_pixels, NULL, 0u);
    }
    ReleaseDC(window, dc);
    if (app_window_text_dc == NULL || app_window_text_bitmap == NULL ||
        app_window_graphics_dc == NULL || app_window_graphics_bitmap == NULL) {
        DestroyWindow(window); free(app_window_frame); return SOFTPC_VM_FRONTEND_ERROR;
    }
    app_window_text_previous_bitmap = SelectObject(app_window_text_dc,
        app_window_text_bitmap);
    app_window_graphics_previous_bitmap = SelectObject(app_window_graphics_dc,
        app_window_graphics_bitmap);
    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);
    SetForegroundWindow(window);
    SetFocus(window);
    for (;;) {
        HANDLE frame_event = (HANDLE)app_runtime_frame_event(runtime);
        DWORD count = frame_event == NULL ? 0u : 1u;
        DWORD wait = MsgWaitForMultipleObjects(count, &frame_event, FALSE,
            SOFTPC_CURSOR_BLINK_INTERVAL_MS, QS_ALLINPUT);

        /* The presentation event replaces the former 16 ms polling timer.
           The bounded deadline is solely for cursor/title/state maintenance;
           normal window and input messages still wake immediately. */
        if (wait == WAIT_TIMEOUT || (count != 0u && wait == WAIT_OBJECT_0))
            SendMessageA(window, WM_TIMER, SOFTPC_TIMER_ID, 0);
        else if (wait == WAIT_FAILED) {
            app_window_result = SOFTPC_VM_FRONTEND_ERROR;
            DestroyWindow(window);
        }
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) goto window_loop_done;
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
window_loop_done:
    SelectObject(app_window_text_dc, app_window_text_previous_bitmap);
    DeleteDC(app_window_text_dc);
    app_window_text_dc = NULL;
    DeleteObject(app_window_text_bitmap);
    app_window_text_bitmap = NULL;
    app_window_text_pixels = NULL;
    SelectObject(app_window_graphics_dc, app_window_graphics_previous_bitmap);
    DeleteDC(app_window_graphics_dc);
    app_window_graphics_dc = NULL;
    DeleteObject(app_window_graphics_bitmap);
    app_window_graphics_bitmap = NULL;
    app_window_graphics_pixels = NULL;
    free(app_window_frame);
    app_window_frame = NULL;
    app_window_runtime = NULL;
    app_window_auto_switch = 0;
    if (app_window_result == SOFTPC_VM_FRONTEND_STOPPED)
        (void)app_runtime_stop(runtime);
    return app_runtime_get_state(runtime) == SOFTPC_RUNTIME_ERROR ?
        SOFTPC_VM_FRONTEND_ERROR : app_window_result;
}

int app_vm_run_window(app_runtime *runtime)
{
    return app_vm_run_window_mode(runtime, 0);
}

int app_vm_run_console_window(app_runtime *runtime)
{
    return app_vm_run_window_mode(runtime, 1);
}

#else
int app_vm_run_window(app_runtime *runtime)
{
    (void)runtime;
    return SOFTPC_VM_FRONTEND_ERROR;
}

int app_vm_run_console_window(app_runtime *runtime)
{
    (void)runtime;
    return SOFTPC_VM_FRONTEND_ERROR;
}
#endif
