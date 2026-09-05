#include "presenter.h"

#ifdef _WIN32
#include "geometry.h"
#include "input.h"
#include "mouse.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_WINDOW_TEXT_CELL_WIDTH 8
#define WIN32_WINDOW_TEXT_CELL_HEIGHT 16
#define WIN32_WINDOW_TEXT_SURFACE_WIDTH (WIN32_PRESENTATION_TEXT_COLUMNS * WIN32_WINDOW_TEXT_CELL_WIDTH)
#define WIN32_WINDOW_TEXT_SURFACE_HEIGHT (WIN32_PRESENTATION_TEXT_ROWS * WIN32_WINDOW_TEXT_CELL_HEIGHT)
#define WIN32_WINDOW_GRAPHICS_SURFACE_MAX_WIDTH 1280u
#define WIN32_WINDOW_GRAPHICS_SURFACE_MAX_HEIGHT 768u
#define WIN32_WINDOW_TIMER_ID 1u
#define WIN32_WINDOW_CURSOR_BLINK_INTERVAL_MS 250u

static const win32_presentation_binding *win32_window_binding;
static win32_presentation_frame *win32_window_frame;
static HDC win32_window_text_dc;
static HBITMAP win32_window_text_bitmap;
static HGDIOBJ win32_window_text_previous_bitmap;
static uint32_t *win32_window_text_pixels;
static HDC win32_window_graphics_dc;
static HBITMAP win32_window_graphics_bitmap;
static HGDIOBJ win32_window_graphics_previous_bitmap;
static uint32_t *win32_window_graphics_pixels;
static uint32_t win32_window_graphics_width;
static uint32_t win32_window_graphics_height;
static uint8_t win32_window_graphics_palette[WIN32_PRESENTATION_DIB_INFO_BYTES];
static int win32_window_graphics_valid;
static unsigned char win32_window_presented_text[WIN32_PRESENTATION_TEXT_COLUMNS * WIN32_PRESENTATION_TEXT_ROWS];
static unsigned short win32_window_presented_attributes[WIN32_PRESENTATION_TEXT_COLUMNS * WIN32_PRESENTATION_TEXT_ROWS];
static uint32_t win32_window_presented_text_palette[16u];
static unsigned char win32_window_presented_font[256u * 16u];
static unsigned char win32_window_presented_secondary_font[256u * 16u];
static uint32_t win32_window_presented_font_height;
static uint32_t win32_window_presented_attribute_font_select;
static int win32_window_presented_text_valid;
static uint32_t win32_window_displayed_sequence;
static int win32_window_result;
static win32_presentation_keyboard_normalizer win32_window_keyboard_normalizer;
static int win32_window_left_button;
static int win32_window_right_button;
static win32_presentation_mouse win32_window_mouse_state;
static WPARAM win32_window_suppressed_hotkey;
static uint32_t win32_window_surface_width;
static uint32_t win32_window_surface_height;
static int win32_window_client_width;
static int win32_window_client_height;
static int win32_window_cursor_blink_visible;
static DWORD win32_window_cursor_blink_due;

static int win32_window_guest_running(void)
{
    return win32_window_binding != NULL &&
        win32_window_binding->get_state(win32_window_binding->context) ==
        WIN32_PRESENTATION_RUN_RUNNING;
}

static void win32_window_update_title(HWND window)
{
    char title[128];

    if (window == NULL || win32_window_binding == NULL ||
        win32_window_binding->get_window_title == NULL) return;
    win32_window_binding->get_window_title(win32_window_binding->context, title,
        sizeof(title));
    SetWindowTextA(window, title);
}

static COLORREF win32_window_colour(unsigned int colour)
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
    if (win32_window_frame != NULL && win32_window_frame->valid != 0u)
        return (COLORREF)win32_window_frame->text_palette[index];
    return palette[index];
}

/* A COLORREF is packed for Win32 colour APIs (0x00bbggrr), whereas the
 * standalone text DIB is a 32-bit BI_RGB surface (0x00rrggbb as a DWORD).
 * The original nt_graph palette is already correct; convert only at this
 * final GDI storage boundary. */
static uint32_t win32_window_dib_pixel(COLORREF colour)
{
    return win32_presentation_dib_pixel(colour);
}

/* The frontend has no independent canvas size. By default its client area is
 * the physical guest surface; after a user resize, the final GDI blit follows
 * that client area with no unused letterbox space. */
static int win32_window_display_rect(HWND window, uint32_t source_width,
    uint32_t source_height, RECT *display)
{
    if (window == NULL) return 0;
    return win32_presentation_display_rect(win32_window_client_width,
        win32_window_client_height, source_width, source_height, display);
}

static void win32_window_capture_client_size(HWND window)
{
    RECT client;

    if (window == NULL) return;
    GetClientRect(window, &client);
    win32_window_client_width = client.right - client.left;
    win32_window_client_height = client.bottom - client.top;
}

static void win32_window_resize_surface(HWND window, uint32_t width,
    uint32_t height)
{
    if (window == NULL || width == 0u || height == 0u ||
        (win32_window_surface_width == width &&
         win32_window_surface_height == height)) return;
    /* Preserve the user's desktop position across guest mode changes. */
    if (!win32_presentation_resize_client(window, width, height)) return;
    win32_window_surface_width = width;
    win32_window_surface_height = height;
}

static void win32_window_resize_frame(HWND window)
{
    if (win32_window_frame == NULL || win32_window_frame->valid == 0u)
        return;
    if (win32_window_frame->graphics != 0u)
        win32_window_resize_surface(window, win32_window_frame->dib_width,
            win32_window_frame->dib_height);
    else
        win32_window_resize_surface(window, WIN32_WINDOW_TEXT_SURFACE_WIDTH,
            WIN32_WINDOW_TEXT_SURFACE_HEIGHT);
}

static void win32_window_current_surface_size(uint32_t *width,
    uint32_t *height)
{
    if (width == NULL || height == NULL) return;
    if (win32_window_frame != NULL && win32_window_frame->valid != 0u &&
        win32_window_frame->graphics != 0u) {
        *width = win32_window_frame->dib_width;
        *height = win32_window_frame->dib_height;
    } else {
        *width = WIN32_WINDOW_TEXT_SURFACE_WIDTH;
        *height = WIN32_WINDOW_TEXT_SURFACE_HEIGHT;
    }
}

static void win32_window_constrain_sizing(HWND window, WPARAM edge,
    RECT *outer)
{
    uint32_t source_width;
    uint32_t source_height;

    if (window == NULL || outer == NULL) return;
    win32_window_current_surface_size(&source_width, &source_height);
    win32_presentation_constrain_sizing(window, edge, outer, source_width,
        source_height);
}

static void win32_window_update_text_surface(void)
{
    int row;
    if (win32_window_frame == NULL || win32_window_text_dc == NULL ||
        (win32_window_presented_text_valid && memcmp(win32_window_presented_text,
            win32_window_frame->text, sizeof(win32_window_presented_text)) == 0 &&
         memcmp(win32_window_presented_attributes,
            win32_window_frame->attributes,
            sizeof(win32_window_presented_attributes)) == 0 &&
         memcmp(win32_window_presented_text_palette,
            win32_window_frame->text_palette,
            sizeof(win32_window_presented_text_palette)) == 0 &&
         memcmp(win32_window_presented_font, win32_window_frame->font,
            sizeof(win32_window_presented_font)) == 0 &&
         memcmp(win32_window_presented_secondary_font,
            win32_window_frame->secondary_font,
            sizeof(win32_window_presented_secondary_font)) == 0 &&
         win32_window_presented_font_height ==
            win32_window_frame->font_height &&
         win32_window_presented_attribute_font_select ==
            win32_window_frame->attribute_font_select)) return;
    for (row = 0; row < WIN32_PRESENTATION_TEXT_ROWS; ++row) {
        int column;
        for (column = 0; column < WIN32_PRESENTATION_TEXT_COLUMNS; ++column) {
            unsigned int scan;
            size_t index = (size_t)row * WIN32_PRESENTATION_TEXT_COLUMNS + column;
            unsigned char character = win32_window_frame->text[index];
            unsigned short attribute = win32_window_frame->attributes[index];
            for (scan = 0u; scan < WIN32_WINDOW_TEXT_CELL_HEIGHT; ++scan) {
                const unsigned char *font =
                    win32_window_frame->attribute_font_select != 0u &&
                    (attribute & 0x08u) != 0u ?
                    win32_window_frame->secondary_font : win32_window_frame->font;
                unsigned char bits = font[
                    (size_t)character * 16u + scan];
                unsigned int bit;
                uint32_t *pixels = win32_window_text_pixels +
                    ((size_t)row * WIN32_WINDOW_TEXT_CELL_HEIGHT + scan) *
                    WIN32_WINDOW_TEXT_SURFACE_WIDTH + column * WIN32_WINDOW_TEXT_CELL_WIDTH;
                for (bit = 0u; bit < WIN32_WINDOW_TEXT_CELL_WIDTH; ++bit)
                    pixels[bit] = win32_window_dib_pixel(
                        (bits & (0x80u >> bit)) ?
                        win32_window_colour(attribute) :
                        win32_window_colour(attribute >> 4));
            }
        }
    }
    memcpy(win32_window_presented_text, win32_window_frame->text,
        sizeof(win32_window_presented_text));
    memcpy(win32_window_presented_attributes, win32_window_frame->attributes,
        sizeof(win32_window_presented_attributes));
    memcpy(win32_window_presented_text_palette,
        win32_window_frame->text_palette,
        sizeof(win32_window_presented_text_palette));
    memcpy(win32_window_presented_font, win32_window_frame->font,
        sizeof(win32_window_presented_font));
    memcpy(win32_window_presented_secondary_font,
        win32_window_frame->secondary_font,
        sizeof(win32_window_presented_secondary_font));
    win32_window_presented_font_height = win32_window_frame->font_height;
    win32_window_presented_attribute_font_select =
        win32_window_frame->attribute_font_select;
    win32_window_presented_text_valid = 1;
}

/* The original nt_ega/nt_vga painters own the indexed DIB and palette.  Some
 * current Win32/RDP paths fail to blit that indexed, top-down DIB directly to
 * a window even though its bytes are valid (the exact same frame writes a
 * correct BMP). Convert only at the final frontend outlet to an RGB32 DIB;
 * this is the same isolated presentation boundary as the text surface. */
static int win32_window_update_graphics_surface(RECT *changed)
{
    const BITMAPINFO *dib;
    uint32_t source_stride;
    uint32_t row;
    int full_refresh;
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;

    if (win32_window_frame == NULL || win32_window_graphics_pixels == NULL ||
        win32_window_frame->graphics == 0u ||
        win32_window_frame->dib_width == 0u ||
        win32_window_frame->dib_height == 0u ||
        win32_window_frame->dib_width > WIN32_WINDOW_GRAPHICS_SURFACE_MAX_WIDTH ||
        win32_window_frame->dib_height > WIN32_WINDOW_GRAPHICS_SURFACE_MAX_HEIGHT ||
        changed == NULL)
        return 0;
    dib = (const BITMAPINFO *)win32_window_frame->dib_info;
    full_refresh = !win32_window_graphics_valid ||
        win32_window_graphics_width != win32_window_frame->dib_width ||
        win32_window_graphics_height != win32_window_frame->dib_height ||
        memcmp(win32_window_graphics_palette, win32_window_frame->dib_info,
            sizeof(win32_window_graphics_palette)) != 0;
    left = full_refresh ? 0 : win32_window_frame->dirty_left;
    top = full_refresh ? 0 : win32_window_frame->dirty_top;
    right = full_refresh ? (int32_t)win32_window_frame->dib_width - 1 :
        win32_window_frame->dirty_right;
    bottom = full_refresh ? (int32_t)win32_window_frame->dib_height - 1 :
        win32_window_frame->dirty_bottom;
    if (left < 0) left = 0;
    if (top < 0) top = 0;
    if (right >= (int32_t)win32_window_frame->dib_width)
        right = (int32_t)win32_window_frame->dib_width - 1;
    if (bottom >= (int32_t)win32_window_frame->dib_height)
        bottom = (int32_t)win32_window_frame->dib_height - 1;
    if (right < left || bottom < top) return 0;
    source_stride = (win32_window_frame->dib_width + 3u) & ~3u;
    for (row = (uint32_t)top; row <= (uint32_t)bottom; ++row) {
        const uint8_t *source = win32_window_frame->dib_bits +
            row * source_stride;
        uint32_t *destination = win32_window_graphics_pixels +
            row * WIN32_WINDOW_GRAPHICS_SURFACE_MAX_WIDTH;
        uint32_t column;
        for (column = (uint32_t)left; column <= (uint32_t)right; ++column) {
            const RGBQUAD *colour = &dib->bmiColors[source[column]];
            destination[column] = ((uint32_t)colour->rgbRed << 16) |
                ((uint32_t)colour->rgbGreen << 8) |
                (uint32_t)colour->rgbBlue;
        }
    }
    win32_window_graphics_width = win32_window_frame->dib_width;
    win32_window_graphics_height = win32_window_frame->dib_height;
    memcpy(win32_window_graphics_palette, win32_window_frame->dib_info,
        sizeof(win32_window_graphics_palette));
    win32_window_graphics_valid = 1;
    changed->left = left;
    changed->top = top;
    changed->right = right + 1;
    changed->bottom = bottom + 1;
    return 1;
}

static void win32_window_invalidate_graphics(HWND window, const RECT *source)
{
    RECT display;
    RECT target;
    uint32_t width;
    uint32_t height;

    if (window == NULL || source == NULL || win32_window_frame == NULL ||
        !win32_window_display_rect(window, win32_window_frame->dib_width,
            win32_window_frame->dib_height, &display)) return;
    width = win32_window_frame->dib_width;
    height = win32_window_frame->dib_height;
    win32_presentation_map_dirty_rect(source, &display, width, height,
        &target);
    InvalidateRect(window, &target, FALSE);
}

/* The original nt_graph endpoint gave Windows Console a real cursor.  Its
 * blink was therefore owned by the host, not by a guest timer or by C-VID.
 * Keep that boundary: this merely describes the copied-frame overlay that the
 * standalone window may invalidate between otherwise unchanged frames. */
static int win32_window_cursor_rect(HWND window, RECT *cursor)
{
    RECT display;
    int width;
    int height;
    int cell_height;
    int cursor_height;
    uint32_t cursor_size;

    if (window == NULL || cursor == NULL || win32_window_frame == NULL ||
        win32_window_frame->valid == 0u ||
        win32_window_frame->graphics != 0u ||
        win32_window_frame->cursor_column < 0 ||
        win32_window_frame->cursor_row < 0 ||
        win32_window_frame->cursor_column >= WIN32_PRESENTATION_TEXT_COLUMNS ||
        win32_window_frame->cursor_row >= WIN32_PRESENTATION_TEXT_ROWS) return 0;
    if (!win32_window_display_rect(window, WIN32_WINDOW_TEXT_SURFACE_WIDTH,
            WIN32_WINDOW_TEXT_SURFACE_HEIGHT, &display)) return 0;
    width = display.right - display.left;
    height = display.bottom - display.top;
    cell_height = height / WIN32_PRESENTATION_TEXT_ROWS;
    if (width <= 0 || cell_height <= 0) return 0;
    cursor_size = win32_window_frame->cursor_size;
    if (cursor_size == 0u || cursor_size > 100u) cursor_size = 100u;
    cursor_height = (int)((cell_height * cursor_size + 99u) / 100u);
    if (cursor_height > cell_height) cursor_height = cell_height;
    cursor->left = display.left + win32_window_frame->cursor_column * width /
        WIN32_PRESENTATION_TEXT_COLUMNS;
    cursor->right = display.left + (win32_window_frame->cursor_column + 1) * width /
        WIN32_PRESENTATION_TEXT_COLUMNS;
    cursor->top = display.top + (win32_window_frame->cursor_row + 1) * cell_height -
        cursor_height;
    cursor->bottom = display.top + (win32_window_frame->cursor_row + 1) * height /
        WIN32_PRESENTATION_TEXT_ROWS;
    return cursor->right > cursor->left && cursor->bottom > cursor->top;
}

static void win32_window_advance_cursor_blink(HWND window)
{
    RECT cursor;
    DWORD now = GetTickCount();

    /* The text cursor is a host presentation overlay.  Once the executor is
       paused, preserve the last composited frame exactly: do not let this
       host-only blink timer alter an otherwise frozen guest display. */
    if (!win32_window_guest_running()) return;
    if ((LONG)(now - win32_window_cursor_blink_due) < 0) return;
    win32_window_cursor_blink_visible = !win32_window_cursor_blink_visible;
    win32_window_cursor_blink_due = now + WIN32_WINDOW_CURSOR_BLINK_INTERVAL_MS;
    if (win32_window_cursor_rect(window, &cursor))
        InvalidateRect(window, &cursor, FALSE);
}

static void win32_window_paint(HWND window, HDC dc)
{
    RECT display;

    if (win32_window_frame == NULL || win32_window_frame->valid == 0u) return;
    if (win32_window_frame->graphics != 0u) {
        if (!win32_window_display_rect(window,
                win32_window_frame->dib_width,
                win32_window_frame->dib_height, &display)) return;
        if (win32_window_graphics_dc == NULL ||
            win32_window_graphics_width != win32_window_frame->dib_width ||
            win32_window_graphics_height != win32_window_frame->dib_height)
            return;
        StretchBlt(dc, display.left, display.top,
            display.right - display.left, display.bottom - display.top,
            win32_window_graphics_dc, 0, 0,
            (int)win32_window_graphics_width,
            (int)win32_window_graphics_height, SRCCOPY);
        return;
    }
    win32_window_update_text_surface();
    if (!win32_window_display_rect(window, WIN32_WINDOW_TEXT_SURFACE_WIDTH,
            WIN32_WINDOW_TEXT_SURFACE_HEIGHT, &display)) return;
    StretchBlt(dc, display.left, display.top, display.right - display.left,
        display.bottom - display.top, win32_window_text_dc, 0, 0,
        WIN32_WINDOW_TEXT_SURFACE_WIDTH, WIN32_WINDOW_TEXT_SURFACE_HEIGHT, SRCCOPY);
    if (win32_window_cursor_blink_visible) {
        RECT cursor;
        /* nt_graph publishes the original controller-selected text cursor
           through the compatibility Console endpoint.  Draw it only after
           the copied text DIB reaches the window, so this remains a pure
           frontend overlay and never changes guest video memory. */
        if (win32_window_cursor_rect(window, &cursor))
            InvertRect(dc, &cursor);
    }
}

static void win32_window_transition(WPARAM key, LPARAM lparam, int released)
{
    WORD scan = (WORD)((lparam >> 16) & 0xffu);
    DWORD control_state = (lparam & 0x01000000L) != 0 ? ENHANCED_KEY : 0u;
    if (scan == 0u && !released)
        win32_presentation_keyboard_note_recovered_key(
            &win32_window_keyboard_normalizer, (WORD)key);
    if (scan == 0u && released)
        win32_presentation_keyboard_release_recovered_key(
            &win32_window_keyboard_normalizer, (WORD)key);
    (void)win32_presentation_keyboard_submit_transition(
        win32_window_binding->context, win32_window_binding->input_sink,
        scan, (WORD)key, control_state,
        !released);
}

static void win32_window_mouse(LPARAM position)
{
    int dx = 0, dy = 0;
    uint32_t guest_width;
    uint32_t guest_height;

    if (!win32_window_guest_running()) return;
    /* Mouse counters belong to the native guest surface, whereas WM_MOUSE
       reports pixels in the current (possibly user-scaled) client area.
       Preserve the same physical InPort controller while making an enlarged
       or reduced window describe the same guest movement. */
    win32_window_current_surface_size(&guest_width, &guest_height);
    if (!win32_presentation_mouse_move(&win32_window_mouse_state, position,
            win32_window_client_width, win32_window_client_height, guest_width,
            guest_height, &dx, &dy)) return;
    {
        win32_presentation_event event = { 0 };
        event.type = WIN32_PRESENTATION_EVENT_MOUSE;
        event.data.mouse.delta_x = dx;
        event.data.mouse.delta_y = dy;
        event.data.mouse.left_down = (uint8_t)win32_window_left_button;
        event.data.mouse.right_down = (uint8_t)win32_window_right_button;
        (void)win32_window_binding->input_sink(win32_window_binding->context,
            &event);
    }
}

/* Windowed mouse input is a relative guest device.  Once the user explicitly
 * clicks the guest surface, keep the host pointer in that surface so it cannot
 * accidentally operate the desktop while its deltas are being delivered to
 * the original Bus Mouse controller.  Ctrl+Alt+M (and loss of window focus)
 * releases this purely frontend capture; no guest controller state changes. */
static void win32_window_release_mouse_capture(void)
{
    win32_presentation_mouse_release(&win32_window_mouse_state);
}

static void win32_window_capture_mouse(HWND window, LPARAM position)
{
    if (window == NULL || !win32_window_guest_running()) return;
    (void)win32_presentation_mouse_capture(&win32_window_mouse_state, window,
        position);
    /* The guest owns the visible pointer after an explicit click.  Returning
       NULL from WM_SETCURSOR keeps the desktop arrow out of the guest DIB
       without changing any SoftPC device or guest cursor state. */
    SetCursor(NULL);
}

static LRESULT CALLBACK win32_window_proc(HWND window, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_TIMER:
        if (wparam == WIN32_WINDOW_TIMER_ID) {
            if (win32_presentation_mailbox_sequence(
                    win32_window_binding->mailbox) != win32_window_displayed_sequence &&
                win32_presentation_mailbox_copy(win32_window_binding->mailbox,
                    win32_window_frame)) {
                win32_window_displayed_sequence = win32_window_frame->sequence;
                if (win32_presentation_router_observe(win32_window_binding->router,
                        win32_window_frame) == WIN32_PRESENTATION_TARGET_CONSOLE) {
                    win32_window_result = WIN32_PRESENTATION_RUN_SWITCH_CONSOLE;
                    DestroyWindow(window);
                    return 0;
                }
                if (win32_window_frame->graphics != 0u) {
                    RECT changed;
                    int graphics_changed = win32_window_update_graphics_surface(
                        &changed);
                    win32_window_resize_frame(window);
                    if (graphics_changed)
                        win32_window_invalidate_graphics(window, &changed);
                }
                if (win32_window_frame->graphics == 0u) {
                    win32_window_resize_frame(window);
                    InvalidateRect(window, NULL, FALSE);
                }
            }
            win32_window_advance_cursor_blink(window);
            win32_window_update_title(window);
            if (win32_window_binding->get_state(win32_window_binding->context) ==
                WIN32_PRESENTATION_RUN_STOPPED ||
                win32_window_binding->get_state(win32_window_binding->context) ==
                WIN32_PRESENTATION_RUN_ERROR) DestroyWindow(window);
        }
        return 0;
    case WM_PAINT:
        { PAINTSTRUCT paint; HDC dc = BeginPaint(window, &paint);
          win32_window_paint(window, dc); EndPaint(window, &paint); }
        return 0;
    case WM_SIZE:
        win32_window_capture_client_size(window);
        return 0;
    case WM_ERASEBKGND:
        return 1;
    case WM_NCLBUTTONDBLCLK:
        if (wparam == HTCAPTION) {
            if (IsZoomed(window)) ShowWindow(window, SW_RESTORE);
            /* A user resize deliberately does not alter the guest-surface
               cache.  Double-click is an explicit request to override that
               host geometry and restore the current native guest size. */
            win32_window_surface_width = 0u;
            win32_window_surface_height = 0u;
            win32_window_resize_frame(window);
            return 0;
        }
        break;
    case WM_SIZING:
        win32_window_constrain_sizing(window, wparam, (RECT *)lparam);
        return TRUE;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: {
        win32_presentation_action action = win32_presentation_actions_match(
            win32_window_binding->actions, (WORD)wparam,
            win32_presentation_actions_modifiers_from_win32());
        if (action != WIN32_PRESENTATION_ACTION_NONE) {
            win32_presentation_run_result action_result;
            if (action == WIN32_PRESENTATION_ACTION_PAUSE_TOGGLE ||
                action == WIN32_PRESENTATION_ACTION_RELEASE_MOUSE)
                win32_window_release_mouse_capture();
            action_result = win32_window_binding->handle_action(
                win32_window_binding->context, action,
                win32_window_binding->input_sink);
            if (action_result != WIN32_PRESENTATION_RUN_CONTINUE)
                win32_window_result = action_result;
            win32_window_update_title(window);
            win32_window_suppressed_hotkey = wparam;
        } else if (win32_window_guest_running())
            win32_window_transition(wparam, lparam, 0);
        return 0;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP:
        if (wparam == win32_window_suppressed_hotkey) {
            win32_window_suppressed_hotkey = 0u;
            return 0;
        }
        if (win32_window_guest_running())
            win32_window_transition(wparam, lparam, 1);
        return 0;
    case WM_CHAR:
        /* A physical WM_KEYDOWN has already been delivered.  A scan-less
           RDP text packet is normalized only when it is not that recovered
           physical key's duplicate character. */
        if (win32_window_guest_running() &&
            ((uint32_t)lparam >> 16u & 0xffu) == 0u &&
            !win32_presentation_keyboard_consume_duplicate_character(
                &win32_window_keyboard_normalizer, (WORD)wparam))
            (void)win32_presentation_keyboard_submit_utf16(
                &win32_window_keyboard_normalizer, win32_window_binding->context,
                win32_window_binding->input_sink, (WORD)wparam);
        return 0;
    case WM_MOUSEMOVE:
        if (win32_window_guest_running() &&
            win32_presentation_mouse_captured(&win32_window_mouse_state))
            win32_window_mouse(lparam);
        return 0;
    case WM_SETCURSOR:
        if (win32_presentation_mouse_hides_host_cursor(
                &win32_window_mouse_state) && LOWORD(lparam) == HTCLIENT) {
            SetCursor(NULL);
            return TRUE;
        }
        break;
    case WM_LBUTTONDOWN:
        if (!win32_window_guest_running()) return 0;
        win32_window_left_button = 1;
        win32_window_capture_mouse(window, lparam);
        win32_window_mouse(lparam);
        return 0;
    case WM_LBUTTONUP:
        if (!win32_window_guest_running()) return 0;
        win32_window_left_button = 0;
        if (win32_presentation_mouse_captured(&win32_window_mouse_state))
            win32_window_mouse(lparam);
        return 0;
    case WM_RBUTTONDOWN:
        if (!win32_window_guest_running()) return 0;
        win32_window_right_button = 1;
        win32_window_capture_mouse(window, lparam);
        win32_window_mouse(lparam);
        return 0;
    case WM_RBUTTONUP:
        if (!win32_window_guest_running()) return 0;
        win32_window_right_button = 0;
        if (win32_presentation_mouse_captured(&win32_window_mouse_state))
            win32_window_mouse(lparam);
        return 0;
    case WM_KILLFOCUS:
        win32_window_release_mouse_capture();
        return 0;
    case WM_CLOSE:
        /* Closing the presentation must not power off the machine.  The
           monitor owns its lifetime, so retain the paused executor and let a
           later `resume` create a fresh window for the same guest. */
        win32_window_release_mouse_capture();
        win32_window_result = win32_window_binding->handle_close(
            win32_window_binding->context,
            win32_window_binding->input_sink);
        DestroyWindow(window);
        return 0;
    case WM_DESTROY:
        win32_window_release_mouse_capture();
        KillTimer(window, WIN32_WINDOW_TIMER_ID);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

win32_presentation_run_result win32_presentation_run_window(
    const win32_presentation_binding *binding)
{
    WNDCLASSA klass;
    MSG message;
    HWND window;
    HDC dc;
    if (!win32_presentation_binding_valid(binding))
        return WIN32_PRESENTATION_RUN_ERROR_RESULT;
    ZeroMemory(&klass, sizeof(klass));
    klass.lpfnWndProc = win32_window_proc;
    klass.hInstance = GetModuleHandleA(NULL);
    klass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    klass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    klass.lpszClassName = "Win32PresentationWindow";
    if (RegisterClassA(&klass) == 0 && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        return WIN32_PRESENTATION_RUN_ERROR_RESULT;
    win32_window_frame = (win32_presentation_frame *)calloc(1u, sizeof(*win32_window_frame));
    if (win32_window_frame == NULL) return WIN32_PRESENTATION_RUN_ERROR_RESULT;
    win32_window_binding = binding;
    win32_window_result = WIN32_PRESENTATION_RUN_STOPPED_RESULT;
    win32_window_presented_text_valid = 0;
    win32_window_graphics_valid = 0;
    ZeroMemory(win32_window_graphics_palette,
        sizeof(win32_window_graphics_palette));
    win32_window_displayed_sequence = 0u;
    ZeroMemory(&win32_window_keyboard_normalizer,
        sizeof(win32_window_keyboard_normalizer));
    win32_window_left_button = win32_window_right_button = 0;
    win32_presentation_mouse_reset(&win32_window_mouse_state);
    win32_window_surface_width = 0u;
    win32_window_surface_height = 0u;
    win32_window_client_width = 0;
    win32_window_client_height = 0;
    win32_window_cursor_blink_visible = 1;
    win32_window_cursor_blink_due = GetTickCount() +
        WIN32_WINDOW_CURSOR_BLINK_INTERVAL_MS;
    /* Begin at the actual 80x25 guest client dimensions. */
    {
        char title[128] = "Presentation";
        if (binding->get_window_title != NULL)
            binding->get_window_title(binding->context, title, sizeof(title));
        window = CreateWindowExA(0, klass.lpszClassName, title,
        WS_THICKFRAME | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
        WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        CW_USEDEFAULT, 0, 680, 560, NULL, NULL, klass.hInstance, NULL);
    }
    if (window == NULL) { free(win32_window_frame); return WIN32_PRESENTATION_RUN_ERROR_RESULT; }
    win32_window_resize_surface(window, WIN32_WINDOW_TEXT_SURFACE_WIDTH,
        WIN32_WINDOW_TEXT_SURFACE_HEIGHT);
    win32_window_capture_client_size(window);
    dc = GetDC(window);
    win32_window_text_dc = CreateCompatibleDC(dc);
    win32_window_graphics_dc = CreateCompatibleDC(dc);
    {
        BITMAPINFO info;
        ZeroMemory(&info, sizeof(info));
        info.bmiHeader.biSize = sizeof(info.bmiHeader);
        info.bmiHeader.biWidth = WIN32_WINDOW_TEXT_SURFACE_WIDTH;
        info.bmiHeader.biHeight = -WIN32_WINDOW_TEXT_SURFACE_HEIGHT;
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 32;
        info.bmiHeader.biCompression = BI_RGB;
        win32_window_text_bitmap = CreateDIBSection(dc, &info, DIB_RGB_COLORS,
            (void **)&win32_window_text_pixels, NULL, 0u);
    }
    {
        BITMAPINFO info;
        ZeroMemory(&info, sizeof(info));
        info.bmiHeader.biSize = sizeof(info.bmiHeader);
        info.bmiHeader.biWidth = WIN32_WINDOW_GRAPHICS_SURFACE_MAX_WIDTH;
        info.bmiHeader.biHeight = -(LONG)WIN32_WINDOW_GRAPHICS_SURFACE_MAX_HEIGHT;
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 32;
        info.bmiHeader.biCompression = BI_RGB;
        win32_window_graphics_bitmap = CreateDIBSection(dc, &info,
            DIB_RGB_COLORS, (void **)&win32_window_graphics_pixels, NULL, 0u);
    }
    ReleaseDC(window, dc);
    if (win32_window_text_dc == NULL || win32_window_text_bitmap == NULL ||
        win32_window_graphics_dc == NULL || win32_window_graphics_bitmap == NULL) {
        DestroyWindow(window); free(win32_window_frame); return WIN32_PRESENTATION_RUN_ERROR_RESULT;
    }
    win32_window_text_previous_bitmap = SelectObject(win32_window_text_dc,
        win32_window_text_bitmap);
    win32_window_graphics_previous_bitmap = SelectObject(win32_window_graphics_dc,
        win32_window_graphics_bitmap);
    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);
    SetForegroundWindow(window);
    SetFocus(window);
    for (;;) {
        HANDLE frame_event = (HANDLE)win32_presentation_mailbox_event(
            binding->mailbox);
        DWORD count = frame_event == NULL ? 0u : 1u;
        DWORD wait = MsgWaitForMultipleObjects(count, &frame_event, FALSE,
            WIN32_WINDOW_CURSOR_BLINK_INTERVAL_MS, QS_ALLINPUT);

        /* The presentation event replaces the former 16 ms polling timer.
           The bounded deadline is solely for cursor/title/state maintenance;
           normal window and input messages still wake immediately. */
        if (wait == WAIT_TIMEOUT || (count != 0u && wait == WAIT_OBJECT_0))
            SendMessageA(window, WM_TIMER, WIN32_WINDOW_TIMER_ID, 0);
        else if (wait == WAIT_FAILED) {
            win32_window_result = WIN32_PRESENTATION_RUN_ERROR_RESULT;
            DestroyWindow(window);
        }
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) goto window_loop_done;
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
window_loop_done:
    SelectObject(win32_window_text_dc, win32_window_text_previous_bitmap);
    DeleteDC(win32_window_text_dc);
    win32_window_text_dc = NULL;
    DeleteObject(win32_window_text_bitmap);
    win32_window_text_bitmap = NULL;
    win32_window_text_pixels = NULL;
    SelectObject(win32_window_graphics_dc, win32_window_graphics_previous_bitmap);
    DeleteDC(win32_window_graphics_dc);
    win32_window_graphics_dc = NULL;
    DeleteObject(win32_window_graphics_bitmap);
    win32_window_graphics_bitmap = NULL;
    win32_window_graphics_pixels = NULL;
    free(win32_window_frame);
    win32_window_frame = NULL;
    win32_window_binding = NULL;
    return win32_window_result;
}
#endif
