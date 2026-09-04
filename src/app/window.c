#include "window.h"

#ifdef _WIN32
#include "runtime.h"
#include "keyboard.h"

#include <windows.h>
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

static softpc_runtime *softpc_window_runtime;
static softpc_runtime_frame *softpc_window_frame;
static HDC softpc_window_text_dc;
static HBITMAP softpc_window_text_bitmap;
static HGDIOBJ softpc_window_text_previous_bitmap;
static uint32_t *softpc_window_text_pixels;
static HDC softpc_window_graphics_dc;
static HBITMAP softpc_window_graphics_bitmap;
static HGDIOBJ softpc_window_graphics_previous_bitmap;
static uint32_t *softpc_window_graphics_pixels;
static uint32_t softpc_window_graphics_width;
static uint32_t softpc_window_graphics_height;
static unsigned char softpc_window_presented_text[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
static unsigned short softpc_window_presented_attributes[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
static uint32_t softpc_window_presented_text_palette[16u];
static unsigned char softpc_window_presented_font[256u * 16u];
static unsigned char softpc_window_presented_secondary_font[256u * 16u];
static uint32_t softpc_window_presented_font_height;
static uint32_t softpc_window_presented_attribute_font_select;
static int softpc_window_presented_text_valid;
static uint32_t softpc_window_displayed_sequence;
static int softpc_window_result;
static int softpc_window_auto_switch;
static int softpc_window_auto_graphics_presented;
static unsigned int softpc_window_auto_text_frames;
static softpc_win32_keyboard_normalizer softpc_window_keyboard_normalizer;
static int softpc_window_mouse_x;
static int softpc_window_mouse_y;
static int softpc_window_mouse_valid;
static int softpc_window_left_button;
static int softpc_window_right_button;
static int softpc_window_mouse_captured;
static int softpc_window_host_cursor_hidden;
static WPARAM softpc_window_suppressed_hotkey;
static uint32_t softpc_window_surface_width;
static uint32_t softpc_window_surface_height;
static int softpc_window_client_width;
static int softpc_window_client_height;
static int softpc_window_cursor_blink_visible;
static DWORD softpc_window_cursor_blink_due;

static int softpc_window_guest_running(void)
{
    return softpc_window_runtime != NULL &&
        softpc_runtime_get_state(softpc_window_runtime) ==
        SOFTPC_RUNTIME_RUNNING;
}

static void softpc_window_update_title(HWND window)
{
    softpc_runtime_state state;

    if (window == NULL || softpc_window_runtime == NULL) return;
    state = softpc_runtime_get_state(softpc_window_runtime);
    SetWindowTextA(window, state == SOFTPC_RUNTIME_PAUSED ?
        "Insignia SoftPC (Paused)" : "Insignia SoftPC (Running)");
}

static COLORREF softpc_window_colour(unsigned int colour)
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
    if (softpc_window_frame != NULL && softpc_window_frame->valid != 0u)
        return (COLORREF)softpc_window_frame->text_palette[index];
    return palette[index];
}

/* A COLORREF is packed for Win32 colour APIs (0x00bbggrr), whereas the
 * standalone text DIB is a 32-bit BI_RGB surface (0x00rrggbb as a DWORD).
 * The original nt_graph palette is already correct; convert only at this
 * final GDI storage boundary. */
static uint32_t softpc_window_dib_pixel(COLORREF colour)
{
    return ((uint32_t)GetRValue(colour) << 16) |
        ((uint32_t)GetGValue(colour) << 8) |
        (uint32_t)GetBValue(colour);
}

/* Keep the byte-order conversion observable to the Win32 presenter smoke
 * test.  This is deliberately test-only: it does not add a VM interface or
 * alter the copied SoftPC video path. */
#ifdef SOFTPC_WINDOW_TESTING
uint32_t softpc_window_test_dib_pixel(COLORREF colour)
{
    return softpc_window_dib_pixel(colour);
}
#endif

/* The frontend has no independent canvas size. By default its client area is
 * the physical guest surface; after a user resize, the final GDI blit follows
 * that client area with no unused letterbox space. */
static int softpc_window_display_rect(HWND window, uint32_t source_width,
    uint32_t source_height, RECT *display)
{
    if (window == NULL || display == NULL || source_width == 0u ||
        source_height == 0u) return 0;
    display->left = 0;
    display->top = 0;
    display->right = softpc_window_client_width;
    display->bottom = softpc_window_client_height;
    return display->right > 0 && display->bottom > 0;
}

static void softpc_window_capture_client_size(HWND window)
{
    RECT client;

    if (window == NULL) return;
    GetClientRect(window, &client);
    softpc_window_client_width = client.right - client.left;
    softpc_window_client_height = client.bottom - client.top;
}

static void softpc_window_resize_surface(HWND window, uint32_t width,
    uint32_t height)
{
    RECT outer;
    DWORD style;
    DWORD extended_style;

    if (window == NULL || width == 0u || height == 0u ||
        (softpc_window_surface_width == width &&
         softpc_window_surface_height == height)) return;
    SetRect(&outer, 0, 0, (int)width, (int)height);
    style = (DWORD)GetWindowLongPtrA(window, GWL_STYLE);
    extended_style = (DWORD)GetWindowLongPtrA(window, GWL_EXSTYLE);
    if (!AdjustWindowRectEx(&outer, style, FALSE, extended_style)) return;
    /* Preserve the user's desktop position across guest mode changes. */
    SetWindowPos(window, NULL, 0, 0, outer.right - outer.left,
        outer.bottom - outer.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    softpc_window_surface_width = width;
    softpc_window_surface_height = height;
}

static void softpc_window_resize_frame(HWND window)
{
    if (softpc_window_frame == NULL || softpc_window_frame->valid == 0u)
        return;
    if (softpc_window_frame->graphics != 0u)
        softpc_window_resize_surface(window, softpc_window_frame->dib_width,
            softpc_window_frame->dib_height);
    else
        softpc_window_resize_surface(window, SOFTPC_TEXT_SURFACE_WIDTH,
            SOFTPC_TEXT_SURFACE_HEIGHT);
}

static void softpc_window_current_surface_size(uint32_t *width,
    uint32_t *height)
{
    if (width == NULL || height == NULL) return;
    if (softpc_window_frame != NULL && softpc_window_frame->valid != 0u &&
        softpc_window_frame->graphics != 0u) {
        *width = softpc_window_frame->dib_width;
        *height = softpc_window_frame->dib_height;
    } else {
        *width = SOFTPC_TEXT_SURFACE_WIDTH;
        *height = SOFTPC_TEXT_SURFACE_HEIGHT;
    }
}

static void softpc_window_constrain_sizing(HWND window, WPARAM edge,
    RECT *outer)
{
    RECT current_window;
    RECT current_client;
    uint32_t source_width;
    uint32_t source_height;
    int frame_width;
    int frame_height;
    int client_width;
    int client_height;
    int target_width;
    int target_height;

    if (window == NULL || outer == NULL) return;
    softpc_window_current_surface_size(&source_width, &source_height);
    if (source_width == 0u || source_height == 0u) return;
    GetWindowRect(window, &current_window);
    GetClientRect(window, &current_client);
    frame_width = (current_window.right - current_window.left) -
        (current_client.right - current_client.left);
    frame_height = (current_window.bottom - current_window.top) -
        (current_client.bottom - current_client.top);
    target_width = outer->right - outer->left;
    target_height = outer->bottom - outer->top;
    client_width = target_width - frame_width;
    client_height = target_height - frame_height;
    if (client_width <= 0 || client_height <= 0) return;
    if (edge == WMSZ_LEFT || edge == WMSZ_RIGHT) {
        client_height = (int)((uint64_t)client_width * source_height /
            source_width);
    } else if (edge == WMSZ_TOP || edge == WMSZ_BOTTOM) {
        client_width = (int)((uint64_t)client_height * source_width /
            source_height);
    } else if ((uint64_t)client_width * source_height >=
        (uint64_t)client_height * source_width) {
        client_height = (int)((uint64_t)client_width * source_height /
            source_width);
    } else {
        client_width = (int)((uint64_t)client_height * source_width /
            source_height);
    }
    target_width = client_width + frame_width;
    target_height = client_height + frame_height;
    if (edge == WMSZ_LEFT || edge == WMSZ_TOPLEFT || edge == WMSZ_BOTTOMLEFT)
        outer->left = outer->right - target_width;
    else
        outer->right = outer->left + target_width;
    if (edge == WMSZ_TOP || edge == WMSZ_TOPLEFT || edge == WMSZ_TOPRIGHT)
        outer->top = outer->bottom - target_height;
    else
        outer->bottom = outer->top + target_height;
}

static void softpc_window_update_text_surface(void)
{
    int row;
    if (softpc_window_frame == NULL || softpc_window_text_dc == NULL ||
        (softpc_window_presented_text_valid && memcmp(softpc_window_presented_text,
            softpc_window_frame->text, sizeof(softpc_window_presented_text)) == 0 &&
         memcmp(softpc_window_presented_attributes,
            softpc_window_frame->attributes,
            sizeof(softpc_window_presented_attributes)) == 0 &&
         memcmp(softpc_window_presented_text_palette,
            softpc_window_frame->text_palette,
            sizeof(softpc_window_presented_text_palette)) == 0 &&
         memcmp(softpc_window_presented_font, softpc_window_frame->font,
            sizeof(softpc_window_presented_font)) == 0 &&
         memcmp(softpc_window_presented_secondary_font,
            softpc_window_frame->secondary_font,
            sizeof(softpc_window_presented_secondary_font)) == 0 &&
         softpc_window_presented_font_height ==
            softpc_window_frame->font_height &&
         softpc_window_presented_attribute_font_select ==
            softpc_window_frame->attribute_font_select)) return;
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned int scan;
            size_t index = (size_t)row * SOFTPC_TEXT_COLUMNS + column;
            unsigned char character = softpc_window_frame->text[index];
            unsigned short attribute = softpc_window_frame->attributes[index];
            for (scan = 0u; scan < SOFTPC_TEXT_CELL_HEIGHT; ++scan) {
                const unsigned char *font =
                    softpc_window_frame->attribute_font_select != 0u &&
                    (attribute & 0x08u) != 0u ?
                    softpc_window_frame->secondary_font : softpc_window_frame->font;
                unsigned char bits = font[
                    (size_t)character * 16u + scan];
                unsigned int bit;
                uint32_t *pixels = softpc_window_text_pixels +
                    ((size_t)row * SOFTPC_TEXT_CELL_HEIGHT + scan) *
                    SOFTPC_TEXT_SURFACE_WIDTH + column * SOFTPC_TEXT_CELL_WIDTH;
                for (bit = 0u; bit < SOFTPC_TEXT_CELL_WIDTH; ++bit)
                    pixels[bit] = softpc_window_dib_pixel(
                        (bits & (0x80u >> bit)) ?
                        softpc_window_colour(attribute) :
                        softpc_window_colour(attribute >> 4));
            }
        }
    }
    memcpy(softpc_window_presented_text, softpc_window_frame->text,
        sizeof(softpc_window_presented_text));
    memcpy(softpc_window_presented_attributes, softpc_window_frame->attributes,
        sizeof(softpc_window_presented_attributes));
    memcpy(softpc_window_presented_text_palette,
        softpc_window_frame->text_palette,
        sizeof(softpc_window_presented_text_palette));
    memcpy(softpc_window_presented_font, softpc_window_frame->font,
        sizeof(softpc_window_presented_font));
    memcpy(softpc_window_presented_secondary_font,
        softpc_window_frame->secondary_font,
        sizeof(softpc_window_presented_secondary_font));
    softpc_window_presented_font_height = softpc_window_frame->font_height;
    softpc_window_presented_attribute_font_select =
        softpc_window_frame->attribute_font_select;
    softpc_window_presented_text_valid = 1;
}

/* The original nt_ega/nt_vga painters own the indexed DIB and palette.  Some
 * current Win32/RDP paths fail to blit that indexed, top-down DIB directly to
 * a window even though its bytes are valid (the exact same frame writes a
 * correct BMP). Convert only at the final frontend outlet to an RGB32 DIB;
 * this is the same isolated presentation boundary as the text surface. */
static void softpc_window_update_graphics_surface(void)
{
    const BITMAPINFO *dib;
    uint32_t source_stride;
    uint32_t row;

    if (softpc_window_frame == NULL || softpc_window_graphics_pixels == NULL ||
        softpc_window_frame->graphics == 0u ||
        softpc_window_frame->dib_width == 0u ||
        softpc_window_frame->dib_height == 0u ||
        softpc_window_frame->dib_width > SOFTPC_GRAPHICS_SURFACE_MAX_WIDTH ||
        softpc_window_frame->dib_height > SOFTPC_GRAPHICS_SURFACE_MAX_HEIGHT)
        return;
    dib = (const BITMAPINFO *)softpc_window_frame->dib_info;
    source_stride = (softpc_window_frame->dib_width + 3u) & ~3u;
    for (row = 0u; row < softpc_window_frame->dib_height; ++row) {
        const uint8_t *source = softpc_window_frame->dib_bits +
            row * source_stride;
        uint32_t *destination = softpc_window_graphics_pixels +
            row * SOFTPC_GRAPHICS_SURFACE_MAX_WIDTH;
        uint32_t column;
        for (column = 0u; column < softpc_window_frame->dib_width; ++column) {
            const RGBQUAD *colour = &dib->bmiColors[source[column]];
            destination[column] = ((uint32_t)colour->rgbRed << 16) |
                ((uint32_t)colour->rgbGreen << 8) |
                (uint32_t)colour->rgbBlue;
        }
    }
    softpc_window_graphics_width = softpc_window_frame->dib_width;
    softpc_window_graphics_height = softpc_window_frame->dib_height;
}

/* The original nt_graph endpoint gave Windows Console a real cursor.  Its
 * blink was therefore owned by the host, not by a guest timer or by C-VID.
 * Keep that boundary: this merely describes the copied-frame overlay that the
 * standalone window may invalidate between otherwise unchanged frames. */
static int softpc_window_cursor_rect(HWND window, RECT *cursor)
{
    RECT display;
    int width;
    int height;
    int cell_height;
    int cursor_height;
    uint32_t cursor_size;

    if (window == NULL || cursor == NULL || softpc_window_frame == NULL ||
        softpc_window_frame->valid == 0u ||
        softpc_window_frame->graphics != 0u ||
        softpc_window_frame->cursor_column < 0 ||
        softpc_window_frame->cursor_row < 0 ||
        softpc_window_frame->cursor_column >= SOFTPC_TEXT_COLUMNS ||
        softpc_window_frame->cursor_row >= SOFTPC_TEXT_ROWS) return 0;
    if (!softpc_window_display_rect(window, SOFTPC_TEXT_SURFACE_WIDTH,
            SOFTPC_TEXT_SURFACE_HEIGHT, &display)) return 0;
    width = display.right - display.left;
    height = display.bottom - display.top;
    cell_height = height / SOFTPC_TEXT_ROWS;
    if (width <= 0 || cell_height <= 0) return 0;
    cursor_size = softpc_window_frame->cursor_size;
    if (cursor_size == 0u || cursor_size > 100u) cursor_size = 100u;
    cursor_height = (int)((cell_height * cursor_size + 99u) / 100u);
    if (cursor_height > cell_height) cursor_height = cell_height;
    cursor->left = display.left + softpc_window_frame->cursor_column * width /
        SOFTPC_TEXT_COLUMNS;
    cursor->right = display.left + (softpc_window_frame->cursor_column + 1) * width /
        SOFTPC_TEXT_COLUMNS;
    cursor->top = display.top + (softpc_window_frame->cursor_row + 1) * cell_height -
        cursor_height;
    cursor->bottom = display.top + (softpc_window_frame->cursor_row + 1) * height /
        SOFTPC_TEXT_ROWS;
    return cursor->right > cursor->left && cursor->bottom > cursor->top;
}

static void softpc_window_advance_cursor_blink(HWND window)
{
    RECT cursor;
    DWORD now = GetTickCount();

    /* The text cursor is a host presentation overlay.  Once the executor is
       paused, preserve the last composited frame exactly: do not let this
       host-only blink timer alter an otherwise frozen guest display. */
    if (!softpc_window_guest_running()) return;
    if ((LONG)(now - softpc_window_cursor_blink_due) < 0) return;
    softpc_window_cursor_blink_visible = !softpc_window_cursor_blink_visible;
    softpc_window_cursor_blink_due = now + SOFTPC_CURSOR_BLINK_INTERVAL_MS;
    if (softpc_window_cursor_rect(window, &cursor))
        InvalidateRect(window, &cursor, FALSE);
}

static void softpc_window_paint(HWND window, HDC dc)
{
    RECT display;

    if (softpc_window_frame == NULL || softpc_window_frame->valid == 0u) return;
    if (softpc_window_frame->graphics != 0u) {
        if (!softpc_window_display_rect(window,
                softpc_window_frame->dib_width,
                softpc_window_frame->dib_height, &display)) return;
        if (softpc_window_graphics_dc == NULL ||
            softpc_window_graphics_width != softpc_window_frame->dib_width ||
            softpc_window_graphics_height != softpc_window_frame->dib_height)
            return;
        StretchBlt(dc, display.left, display.top,
            display.right - display.left, display.bottom - display.top,
            softpc_window_graphics_dc, 0, 0,
            (int)softpc_window_graphics_width,
            (int)softpc_window_graphics_height, SRCCOPY);
        return;
    }
    softpc_window_update_text_surface();
    if (!softpc_window_display_rect(window, SOFTPC_TEXT_SURFACE_WIDTH,
            SOFTPC_TEXT_SURFACE_HEIGHT, &display)) return;
    StretchBlt(dc, display.left, display.top, display.right - display.left,
        display.bottom - display.top, softpc_window_text_dc, 0, 0,
        SOFTPC_TEXT_SURFACE_WIDTH, SOFTPC_TEXT_SURFACE_HEIGHT, SRCCOPY);
    if (softpc_window_cursor_blink_visible) {
        RECT cursor;
        /* nt_graph publishes the original controller-selected text cursor
           through the compatibility Console endpoint.  Draw it only after
           the copied text DIB reaches the window, so this remains a pure
           frontend overlay and never changes guest video memory. */
        if (softpc_window_cursor_rect(window, &cursor))
            InvertRect(dc, &cursor);
    }
}

static int softpc_window_keyboard_sink(void *context, uint8_t key_number,
    uint8_t released)
{
    (void)context;
    if (!softpc_window_guest_running()) return 0;
    return softpc_runtime_enqueue_key(softpc_window_runtime, key_number,
        released);
}

static void softpc_window_transition(WPARAM key, LPARAM lparam, int released)
{
    WORD scan = (WORD)((lparam >> 16) & 0xffu);
    DWORD control_state = (lparam & 0x01000000L) != 0 ? ENHANCED_KEY : 0u;
    if (scan == 0u && !released)
        softpc_win32_keyboard_note_recovered_key(
            &softpc_window_keyboard_normalizer, (WORD)key);
    if (scan == 0u && released)
        softpc_win32_keyboard_release_recovered_key(
            &softpc_window_keyboard_normalizer, (WORD)key);
    (void)softpc_win32_keyboard_submit_transition(NULL,
        softpc_window_keyboard_sink, scan, (WORD)key, control_state,
        !released);
}

static void softpc_window_mouse(LPARAM position)
{
    int x = (int)(short)LOWORD(position);
    int y = (int)(short)HIWORD(position);
    int dx = 0, dy = 0;
    if (!softpc_window_guest_running()) return;
    if (softpc_window_mouse_valid) {
        dx = x - softpc_window_mouse_x;
        dy = y - softpc_window_mouse_y;
    }
    softpc_window_mouse_x = x;
    softpc_window_mouse_y = y;
    softpc_window_mouse_valid = 1;

    /* Mouse counters belong to the native guest surface, whereas WM_MOUSE
       reports pixels in the current (possibly user-scaled) client area.
       Preserve the same physical InPort controller while making an enlarged
       or reduced window describe the same guest movement. */
    if (softpc_window_client_width > 0 && softpc_window_client_height > 0) {
        uint32_t guest_width;
        uint32_t guest_height;
        softpc_window_current_surface_size(&guest_width, &guest_height);
        dx = (int)((int64_t)dx * (int64_t)guest_width /
            softpc_window_client_width);
        dy = (int)((int64_t)dy * (int64_t)guest_height /
            softpc_window_client_height);
    }
    (void)softpc_runtime_enqueue_mouse(softpc_window_runtime, dx, dy,
        (uint8_t)softpc_window_left_button,
        (uint8_t)softpc_window_right_button);
}

/* Windowed mouse input is a relative guest device.  Once the user explicitly
 * clicks the guest surface, keep the host pointer in that surface so it cannot
 * accidentally operate the desktop while its deltas are being delivered to
 * the original Bus Mouse controller.  Ctrl+Alt+M (and loss of window focus)
 * releases this purely frontend capture; no guest controller state changes. */
static void softpc_window_release_mouse_capture(void)
{
    if (!softpc_window_mouse_captured) return;
    ClipCursor(NULL);
    ReleaseCapture();
    softpc_window_host_cursor_hidden = 0;
    SetCursor(LoadCursorA(NULL, IDC_ARROW));
    softpc_window_mouse_captured = 0;
    softpc_window_mouse_valid = 0;
}

static void softpc_window_capture_mouse(HWND window, LPARAM position)
{
    RECT client;
    POINT upper_left;
    POINT lower_right;

    if (window == NULL || !softpc_window_guest_running()) return;
    SetFocus(window);
    SetCapture(window);
    GetClientRect(window, &client);
    upper_left.x = client.left;
    upper_left.y = client.top;
    lower_right.x = client.right;
    lower_right.y = client.bottom;
    if (ClientToScreen(window, &upper_left) &&
        ClientToScreen(window, &lower_right)) {
        RECT bounds;
        bounds.left = upper_left.x;
        bounds.top = upper_left.y;
        bounds.right = lower_right.x;
        bounds.bottom = lower_right.y;
        (void)ClipCursor(&bounds);
    }
    softpc_window_mouse_x = (int)(short)LOWORD(position);
    softpc_window_mouse_y = (int)(short)HIWORD(position);
    softpc_window_mouse_valid = 1;
    softpc_window_mouse_captured = 1;
    /* The guest owns the visible pointer after an explicit click.  Returning
       NULL from WM_SETCURSOR keeps the desktop arrow out of the guest DIB
       without changing any SoftPC device or guest cursor state. */
    softpc_window_host_cursor_hidden = 1;
    SetCursor(NULL);
}

static LRESULT CALLBACK softpc_window_proc(HWND window, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_TIMER:
        if (wparam == SOFTPC_TIMER_ID) {
            if (softpc_runtime_published_frame_sequence(
                    softpc_window_runtime) != softpc_window_displayed_sequence &&
                softpc_runtime_copy_frame(softpc_window_runtime,
                    softpc_window_frame)) {
                softpc_window_displayed_sequence = softpc_window_frame->sequence;
                if (softpc_window_frame->graphics != 0u) {
                    softpc_window_update_graphics_surface();
                    softpc_window_auto_graphics_presented = 1;
                    softpc_window_auto_text_frames = 0u;
                } else if (softpc_window_auto_switch &&
                    softpc_window_auto_graphics_presented) {
                    /* C-VID can publish a transient text snapshot around a
                       graphics transition.  Do not tear down a just-opened
                       window until the original renderer has demonstrated a
                       stable return to text mode. */
                    if (++softpc_window_auto_text_frames >= 3u) {
                        softpc_window_result = SOFTPC_VM_FRONTEND_SWITCH_CONSOLE;
                        DestroyWindow(window);
                        return 0;
                    }
                }
                softpc_window_resize_frame(window);
                InvalidateRect(window, NULL, FALSE);
            }
            softpc_window_advance_cursor_blink(window);
            softpc_window_update_title(window);
            if (softpc_runtime_get_state(softpc_window_runtime) ==
                SOFTPC_RUNTIME_STOPPED ||
                softpc_runtime_get_state(softpc_window_runtime) ==
                SOFTPC_RUNTIME_ERROR) DestroyWindow(window);
        }
        return 0;
    case WM_PAINT:
        { PAINTSTRUCT paint; HDC dc = BeginPaint(window, &paint);
          softpc_window_paint(window, dc); EndPaint(window, &paint); }
        return 0;
    case WM_SIZE:
        softpc_window_capture_client_size(window);
        return 0;
    case WM_ERASEBKGND:
        return 1;
    case WM_NCLBUTTONDBLCLK:
        if (wparam == HTCAPTION) {
            if (IsZoomed(window)) ShowWindow(window, SW_RESTORE);
            /* A user resize deliberately does not alter the guest-surface
               cache.  Double-click is an explicit request to override that
               host geometry and restore the current native guest size. */
            softpc_window_surface_width = 0u;
            softpc_window_surface_height = 0u;
            softpc_window_resize_frame(window);
            return 0;
        }
        break;
    case WM_SIZING:
        softpc_window_constrain_sizing(window, wparam, (RECT *)lparam);
        return TRUE;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (wparam == 'P' && GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_MENU) < 0) {
            if (softpc_runtime_get_state(softpc_window_runtime) ==
                SOFTPC_RUNTIME_RUNNING) {
                /* A paused guest cannot consume further pointer traffic.
                   Keep the final frame visible, but release the host mouse. */
                softpc_window_release_mouse_capture();
                (void)softpc_runtime_pause(softpc_window_runtime);
            } else if (softpc_runtime_get_state(softpc_window_runtime) ==
                SOFTPC_RUNTIME_PAUSED) {
                (void)softpc_runtime_resume(softpc_window_runtime);
            }
            softpc_window_update_title(window);
            softpc_window_suppressed_hotkey = wparam;
        } else if (wparam == 'D' && GetKeyState(VK_CONTROL) < 0 &&
            GetKeyState(VK_MENU) < 0) {
            (void)softpc_win32_keyboard_submit_ctrl_alt_del(NULL,
                softpc_window_keyboard_sink);
            softpc_window_suppressed_hotkey = wparam;
        } else if (wparam == 'M' && GetKeyState(VK_CONTROL) < 0 &&
            GetKeyState(VK_MENU) < 0) {
            softpc_window_release_mouse_capture();
            softpc_window_suppressed_hotkey = wparam;
        } else if (softpc_window_guest_running())
            softpc_window_transition(wparam, lparam, 0);
        return 0;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        if (wparam == softpc_window_suppressed_hotkey) {
            softpc_window_suppressed_hotkey = 0u;
            return 0;
        }
        if (softpc_window_guest_running())
            softpc_window_transition(wparam, lparam, 1);
        return 0;
    case WM_CHAR:
        /* A physical WM_KEYDOWN has already been delivered.  A scan-less
           RDP text packet is normalized only when it is not that recovered
           physical key's duplicate character. */
        if (softpc_window_guest_running() &&
            ((uint32_t)lparam >> 16u & 0xffu) == 0u &&
            !softpc_win32_keyboard_consume_duplicate_character(
                &softpc_window_keyboard_normalizer, (WORD)wparam))
            (void)softpc_win32_keyboard_submit_utf16(
                &softpc_window_keyboard_normalizer, NULL,
                softpc_window_keyboard_sink, (WORD)wparam);
        return 0;
    case WM_MOUSEMOVE:
        if (softpc_window_guest_running() && softpc_window_mouse_captured)
            softpc_window_mouse(lparam);
        return 0;
    case WM_SETCURSOR:
        if (softpc_window_host_cursor_hidden && LOWORD(lparam) == HTCLIENT) {
            SetCursor(NULL);
            return TRUE;
        }
        break;
    case WM_LBUTTONDOWN:
        if (!softpc_window_guest_running()) return 0;
        softpc_window_left_button = 1;
        softpc_window_capture_mouse(window, lparam);
        softpc_window_mouse(lparam);
        return 0;
    case WM_LBUTTONUP:
        if (!softpc_window_guest_running()) return 0;
        softpc_window_left_button = 0;
        if (softpc_window_mouse_captured) softpc_window_mouse(lparam);
        return 0;
    case WM_RBUTTONDOWN:
        if (!softpc_window_guest_running()) return 0;
        softpc_window_right_button = 1;
        softpc_window_capture_mouse(window, lparam);
        softpc_window_mouse(lparam);
        return 0;
    case WM_RBUTTONUP:
        if (!softpc_window_guest_running()) return 0;
        softpc_window_right_button = 0;
        if (softpc_window_mouse_captured) softpc_window_mouse(lparam);
        return 0;
    case WM_KILLFOCUS:
        softpc_window_release_mouse_capture();
        return 0;
    case WM_CLOSE:
        /* Closing the presentation must not power off the machine.  The
           monitor owns its lifetime, so retain the paused executor and let a
           later `resume` create a fresh window for the same guest. */
        softpc_window_release_mouse_capture();
        softpc_window_result = SOFTPC_VM_FRONTEND_PAUSED;
        (void)softpc_runtime_pause(softpc_window_runtime);
        DestroyWindow(window);
        return 0;
    case WM_DESTROY:
        softpc_window_release_mouse_capture();
        KillTimer(window, SOFTPC_TIMER_ID);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

static int softpc_vm_run_window_mode(softpc_runtime *runtime,
    int auto_switch)
{
    WNDCLASSA klass;
    MSG message;
    HWND window;
    HDC dc;
    if (runtime == NULL) return SOFTPC_VM_FRONTEND_ERROR;
    ZeroMemory(&klass, sizeof(klass));
    klass.lpfnWndProc = softpc_window_proc;
    klass.hInstance = GetModuleHandleA(NULL);
    klass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    klass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    klass.lpszClassName = "SoftPCStandaloneWindow";
    if (RegisterClassA(&klass) == 0 && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        return SOFTPC_VM_FRONTEND_ERROR;
    softpc_window_frame = (softpc_runtime_frame *)calloc(1u, sizeof(*softpc_window_frame));
    if (softpc_window_frame == NULL) return SOFTPC_VM_FRONTEND_ERROR;
    softpc_window_runtime = runtime;
    softpc_window_auto_switch = auto_switch;
    softpc_window_auto_graphics_presented = 0;
    softpc_window_auto_text_frames = 0u;
    softpc_window_result = SOFTPC_VM_FRONTEND_STOPPED;
    softpc_window_presented_text_valid = 0;
    softpc_window_displayed_sequence = 0u;
    ZeroMemory(&softpc_window_keyboard_normalizer,
        sizeof(softpc_window_keyboard_normalizer));
    softpc_window_mouse_valid = 0;
    softpc_window_left_button = softpc_window_right_button = 0;
    softpc_window_mouse_captured = 0;
    softpc_window_host_cursor_hidden = 0;
    softpc_window_surface_width = 0u;
    softpc_window_surface_height = 0u;
    softpc_window_client_width = 0;
    softpc_window_client_height = 0;
    softpc_window_cursor_blink_visible = 1;
    softpc_window_cursor_blink_due = GetTickCount() +
        SOFTPC_CURSOR_BLINK_INTERVAL_MS;
    /* Begin at the actual 80x25 guest client dimensions. */
    window = CreateWindowExA(0, klass.lpszClassName, "Insignia SoftPC (Running)",
        WS_THICKFRAME | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
        WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        CW_USEDEFAULT, 0, 680, 560, NULL, NULL, klass.hInstance, NULL);
    if (window == NULL) { free(softpc_window_frame); return SOFTPC_VM_FRONTEND_ERROR; }
    softpc_window_resize_surface(window, SOFTPC_TEXT_SURFACE_WIDTH,
        SOFTPC_TEXT_SURFACE_HEIGHT);
    softpc_window_capture_client_size(window);
    dc = GetDC(window);
    softpc_window_text_dc = CreateCompatibleDC(dc);
    softpc_window_graphics_dc = CreateCompatibleDC(dc);
    {
        BITMAPINFO info;
        ZeroMemory(&info, sizeof(info));
        info.bmiHeader.biSize = sizeof(info.bmiHeader);
        info.bmiHeader.biWidth = SOFTPC_TEXT_SURFACE_WIDTH;
        info.bmiHeader.biHeight = -SOFTPC_TEXT_SURFACE_HEIGHT;
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 32;
        info.bmiHeader.biCompression = BI_RGB;
        softpc_window_text_bitmap = CreateDIBSection(dc, &info, DIB_RGB_COLORS,
            (void **)&softpc_window_text_pixels, NULL, 0u);
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
        softpc_window_graphics_bitmap = CreateDIBSection(dc, &info,
            DIB_RGB_COLORS, (void **)&softpc_window_graphics_pixels, NULL, 0u);
    }
    ReleaseDC(window, dc);
    if (softpc_window_text_dc == NULL || softpc_window_text_bitmap == NULL ||
        softpc_window_graphics_dc == NULL || softpc_window_graphics_bitmap == NULL) {
        DestroyWindow(window); free(softpc_window_frame); return SOFTPC_VM_FRONTEND_ERROR;
    }
    softpc_window_text_previous_bitmap = SelectObject(softpc_window_text_dc,
        softpc_window_text_bitmap);
    softpc_window_graphics_previous_bitmap = SelectObject(softpc_window_graphics_dc,
        softpc_window_graphics_bitmap);
    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);
    SetForegroundWindow(window);
    SetFocus(window);
    SetTimer(window, SOFTPC_TIMER_ID, 16u, NULL);
    while (GetMessageA(&message, NULL, 0, 0) > 0) {
        TranslateMessage(&message); DispatchMessageA(&message);
    }
    SelectObject(softpc_window_text_dc, softpc_window_text_previous_bitmap);
    DeleteDC(softpc_window_text_dc);
    softpc_window_text_dc = NULL;
    DeleteObject(softpc_window_text_bitmap);
    softpc_window_text_bitmap = NULL;
    softpc_window_text_pixels = NULL;
    SelectObject(softpc_window_graphics_dc, softpc_window_graphics_previous_bitmap);
    DeleteDC(softpc_window_graphics_dc);
    softpc_window_graphics_dc = NULL;
    DeleteObject(softpc_window_graphics_bitmap);
    softpc_window_graphics_bitmap = NULL;
    softpc_window_graphics_pixels = NULL;
    free(softpc_window_frame);
    softpc_window_frame = NULL;
    softpc_window_runtime = NULL;
    softpc_window_auto_switch = 0;
    if (softpc_window_result == SOFTPC_VM_FRONTEND_STOPPED)
        (void)softpc_runtime_stop(runtime);
    return softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_ERROR ?
        SOFTPC_VM_FRONTEND_ERROR : softpc_window_result;
}

int softpc_vm_run_window(softpc_runtime *runtime)
{
    return softpc_vm_run_window_mode(runtime, 0);
}

int softpc_vm_run_console_window(softpc_runtime *runtime)
{
    return softpc_vm_run_window_mode(runtime, 1);
}

#else
int softpc_vm_run_window(softpc_runtime *runtime)
{
    (void)runtime;
    return SOFTPC_VM_FRONTEND_ERROR;
}

int softpc_vm_run_console_window(softpc_runtime *runtime)
{
    (void)runtime;
    return SOFTPC_VM_FRONTEND_ERROR;
}
#endif
