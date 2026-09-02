#include "win32_window.h"

#ifdef _WIN32
#include "runtime.h"
#include "win32_keyboard.h"

#include <windows.h>
#include <stdlib.h>
#include <string.h>

#define SOFTPC_TEXT_COLUMNS 80
#define SOFTPC_TEXT_ROWS 25
#define SOFTPC_TEXT_CELL_WIDTH 8
#define SOFTPC_TEXT_CELL_HEIGHT 16
#define SOFTPC_TEXT_SURFACE_WIDTH (SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_CELL_WIDTH)
#define SOFTPC_TEXT_SURFACE_HEIGHT (SOFTPC_TEXT_ROWS * SOFTPC_TEXT_CELL_HEIGHT)
#define SOFTPC_TIMER_ID 1u

static softpc_runtime *softpc_window_runtime;
static softpc_runtime_frame *softpc_window_frame;
static HDC softpc_window_text_dc;
static HBITMAP softpc_window_text_bitmap;
static HGDIOBJ softpc_window_text_previous_bitmap;
static uint32_t *softpc_window_text_pixels;
static unsigned char softpc_window_presented_text[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
static unsigned short softpc_window_presented_attributes[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
static unsigned char softpc_window_presented_font[256u * 16u];
static uint32_t softpc_window_presented_font_height;
static int softpc_window_presented_text_valid;
static uint32_t softpc_window_displayed_sequence;
static int softpc_window_result;
static softpc_win32_keyboard_normalizer softpc_window_keyboard_normalizer;
static int softpc_window_mouse_x;
static int softpc_window_mouse_y;
static int softpc_window_mouse_valid;
static int softpc_window_left_button;
static int softpc_window_right_button;
static uint32_t softpc_window_surface_width;
static uint32_t softpc_window_surface_height;

static COLORREF softpc_window_colour(unsigned int colour)
{
    static const COLORREF palette[16] = {
        RGB(0, 0, 0), RGB(0, 0, 170), RGB(0, 170, 0), RGB(0, 170, 170),
        RGB(170, 0, 0), RGB(170, 0, 170), RGB(170, 85, 0), RGB(170, 170, 170),
        RGB(85, 85, 85), RGB(85, 85, 255), RGB(85, 255, 85), RGB(85, 255, 255),
        RGB(255, 85, 85), RGB(255, 85, 255), RGB(255, 255, 85), RGB(255, 255, 255)
    };
    return palette[colour & 0x0fu];
}

/* Match NXVM's display contract: the client rectangle is the current guest
 * surface, while the normal Win32 frame is outside it.  The frontend owns
 * this geometry only; the dimensions still come from a copied frame. */
static void softpc_window_resize_surface(HWND window, uint32_t width,
    uint32_t height)
{
    RECT client;
    RECT outer;
    LONG width_offset;
    LONG height_offset;

    if (window == NULL || width == 0u || height == 0u ||
        (softpc_window_surface_width == width &&
         softpc_window_surface_height == height)) return;
    GetClientRect(window, &client);
    GetWindowRect(window, &outer);
    width_offset = (outer.right - outer.left) - (client.right - client.left);
    height_offset = (outer.bottom - outer.top) - (client.bottom - client.top);
    MoveWindow(window, outer.left, outer.top, (int)width + width_offset,
        (int)height + height_offset, TRUE);
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

static void softpc_window_update_text_surface(void)
{
    int row;
    if (softpc_window_frame == NULL || softpc_window_text_dc == NULL ||
        (softpc_window_presented_text_valid && memcmp(softpc_window_presented_text,
            softpc_window_frame->text, sizeof(softpc_window_presented_text)) == 0 &&
         memcmp(softpc_window_presented_attributes,
            softpc_window_frame->attributes,
            sizeof(softpc_window_presented_attributes)) == 0 &&
         memcmp(softpc_window_presented_font, softpc_window_frame->font,
            sizeof(softpc_window_presented_font)) == 0 &&
         softpc_window_presented_font_height ==
            softpc_window_frame->font_height)) return;
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned int scan;
            size_t index = (size_t)row * SOFTPC_TEXT_COLUMNS + column;
            unsigned char character = softpc_window_frame->text[index];
            unsigned short attribute = softpc_window_frame->attributes[index];
            for (scan = 0u; scan < SOFTPC_TEXT_CELL_HEIGHT; ++scan) {
                unsigned char bits = softpc_window_frame->font[
                    (size_t)character * 16u + scan];
                unsigned int bit;
                uint32_t *pixels = softpc_window_text_pixels +
                    ((size_t)row * SOFTPC_TEXT_CELL_HEIGHT + scan) *
                    SOFTPC_TEXT_SURFACE_WIDTH + column * SOFTPC_TEXT_CELL_WIDTH;
                for (bit = 0u; bit < SOFTPC_TEXT_CELL_WIDTH; ++bit)
                    pixels[bit] = (bits & (0x80u >> bit)) ?
                        softpc_window_colour(attribute) :
                        softpc_window_colour(attribute >> 4);
            }
        }
    }
    memcpy(softpc_window_presented_text, softpc_window_frame->text,
        sizeof(softpc_window_presented_text));
    memcpy(softpc_window_presented_attributes, softpc_window_frame->attributes,
        sizeof(softpc_window_presented_attributes));
    memcpy(softpc_window_presented_font, softpc_window_frame->font,
        sizeof(softpc_window_presented_font));
    softpc_window_presented_font_height = softpc_window_frame->font_height;
    softpc_window_presented_text_valid = 1;
}

static void softpc_window_paint(HDC dc)
{
    if (softpc_window_frame == NULL || softpc_window_frame->valid == 0u) return;
    if (softpc_window_frame->graphics != 0u) {
        StretchDIBits(dc, 0, 0, (int)softpc_window_frame->dib_width,
            (int)softpc_window_frame->dib_height, 0, 0,
            (int)softpc_window_frame->dib_width,
            (int)softpc_window_frame->dib_height,
            softpc_window_frame->dib_bits,
            (const BITMAPINFO *)softpc_window_frame->dib_info,
            DIB_RGB_COLORS, SRCCOPY);
        return;
    }
    softpc_window_update_text_surface();
    BitBlt(dc, 0, 0, SOFTPC_TEXT_SURFACE_WIDTH, SOFTPC_TEXT_SURFACE_HEIGHT,
        softpc_window_text_dc, 0, 0, SRCCOPY);
}

static int softpc_window_keyboard_sink(void *context, uint8_t key_number,
    uint8_t released)
{
    (void)context;
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
    if (softpc_window_mouse_valid) {
        dx = x - softpc_window_mouse_x;
        dy = y - softpc_window_mouse_y;
    }
    softpc_window_mouse_x = x;
    softpc_window_mouse_y = y;
    softpc_window_mouse_valid = 1;
    (void)softpc_runtime_enqueue_mouse(softpc_window_runtime, dx, dy,
        (uint8_t)softpc_window_left_button,
        (uint8_t)softpc_window_right_button);
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
                softpc_window_resize_frame(window);
                InvalidateRect(window, NULL, FALSE);
            }
            if (softpc_runtime_get_state(softpc_window_runtime) !=
                SOFTPC_RUNTIME_RUNNING) DestroyWindow(window);
        }
        return 0;
    case WM_PAINT:
        { PAINTSTRUCT paint; HDC dc = BeginPaint(window, &paint);
          softpc_window_paint(dc); EndPaint(window, &paint); }
        return 0;
    case WM_ERASEBKGND:
        return 1;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (wparam == 'P' && GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_MENU) < 0) {
            softpc_window_result = SOFTPC_VM_FRONTEND_PAUSED;
            (void)softpc_runtime_pause(softpc_window_runtime);
            DestroyWindow(window);
        } else if (wparam == VK_ESCAPE) {
            softpc_window_result = SOFTPC_VM_FRONTEND_STOPPED;
            (void)softpc_runtime_stop(softpc_window_runtime);
            DestroyWindow(window);
        } else softpc_window_transition(wparam, lparam, 0);
        return 0;
    case WM_KEYUP:
    case WM_SYSKEYUP: softpc_window_transition(wparam, lparam, 1); return 0;
    case WM_CHAR:
        /* A physical WM_KEYDOWN has already been delivered.  A scan-less
           RDP text packet is normalized only when it is not that recovered
           physical key's duplicate character. */
        if (((uint32_t)lparam >> 16u & 0xffu) == 0u &&
            !softpc_win32_keyboard_consume_duplicate_character(
                &softpc_window_keyboard_normalizer, (WORD)wparam))
            (void)softpc_win32_keyboard_submit_utf16(
                &softpc_window_keyboard_normalizer, NULL,
                softpc_window_keyboard_sink, (WORD)wparam);
        return 0;
    case WM_MOUSEMOVE: softpc_window_mouse(lparam); return 0;
    case WM_LBUTTONDOWN: softpc_window_left_button = 1; SetCapture(window); softpc_window_mouse(lparam); return 0;
    case WM_LBUTTONUP: softpc_window_left_button = 0; softpc_window_mouse(lparam); if (!softpc_window_right_button) ReleaseCapture(); return 0;
    case WM_RBUTTONDOWN: softpc_window_right_button = 1; SetCapture(window); softpc_window_mouse(lparam); return 0;
    case WM_RBUTTONUP: softpc_window_right_button = 0; softpc_window_mouse(lparam); if (!softpc_window_left_button) ReleaseCapture(); return 0;
    case WM_DESTROY: KillTimer(window, SOFTPC_TIMER_ID); PostQuitMessage(0); return 0;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

int softpc_vm_run_window(softpc_runtime *runtime)
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
    softpc_window_result = SOFTPC_VM_FRONTEND_STOPPED;
    softpc_window_presented_text_valid = 0;
    softpc_window_displayed_sequence = 0u;
    ZeroMemory(&softpc_window_keyboard_normalizer,
        sizeof(softpc_window_keyboard_normalizer));
    softpc_window_mouse_valid = 0;
    softpc_window_left_button = softpc_window_right_button = 0;
    softpc_window_surface_width = 0u;
    softpc_window_surface_height = 0u;
    window = CreateWindowExA(0, klass.lpszClassName, "SoftPC VM", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 680, 560, NULL, NULL, klass.hInstance, NULL);
    if (window == NULL) { free(softpc_window_frame); return SOFTPC_VM_FRONTEND_ERROR; }
    softpc_window_resize_surface(window, SOFTPC_TEXT_SURFACE_WIDTH,
        SOFTPC_TEXT_SURFACE_HEIGHT);
    dc = GetDC(window);
    softpc_window_text_dc = CreateCompatibleDC(dc);
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
    ReleaseDC(window, dc);
    if (softpc_window_text_dc == NULL || softpc_window_text_bitmap == NULL) {
        DestroyWindow(window); free(softpc_window_frame); return SOFTPC_VM_FRONTEND_ERROR;
    }
    softpc_window_text_previous_bitmap = SelectObject(softpc_window_text_dc,
        softpc_window_text_bitmap);
    ShowWindow(window, SW_SHOW);
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
    free(softpc_window_frame);
    softpc_window_frame = NULL;
    softpc_window_runtime = NULL;
    if (softpc_window_result == SOFTPC_VM_FRONTEND_STOPPED)
        (void)softpc_runtime_stop(runtime);
    return softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_ERROR ?
        SOFTPC_VM_FRONTEND_ERROR : softpc_window_result;
}

#else
int softpc_vm_run_window(softpc_runtime *runtime)
{
    (void)runtime;
    return SOFTPC_VM_FRONTEND_ERROR;
}
#endif
