#include "win32_window.h"

#ifdef _WIN32
#include "runtime.h"

#include <windows.h>
#include <stdlib.h>
#include <string.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD KeyEvent);

#define SOFTPC_TEXT_COLUMNS 80
#define SOFTPC_TEXT_ROWS 25
#define SOFTPC_TEXT_CELL_WIDTH 8
#define SOFTPC_TEXT_CELL_HEIGHT 16
#define SOFTPC_TEXT_SURFACE_WIDTH (SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_CELL_WIDTH)
#define SOFTPC_TEXT_SURFACE_HEIGHT (SOFTPC_TEXT_ROWS * SOFTPC_TEXT_CELL_HEIGHT)
#define SOFTPC_TIMER_ID 1u

static softpc_runtime *softpc_window_runtime;
static softpc_runtime_frame *softpc_window_frame;
static HFONT softpc_window_font;
static HDC softpc_window_text_dc;
static HBITMAP softpc_window_text_bitmap;
static HGDIOBJ softpc_window_text_previous_bitmap;
static unsigned char softpc_window_presented_text[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
static unsigned short softpc_window_presented_attributes[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
static int softpc_window_presented_text_valid;
static int softpc_window_result;
static int softpc_window_keydown_delivered;
static int softpc_window_mouse_x;
static int softpc_window_mouse_y;
static int softpc_window_mouse_valid;
static int softpc_window_left_button;
static int softpc_window_right_button;

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

static void softpc_window_update_text_surface(void)
{
    int row;
    if (softpc_window_frame == NULL || softpc_window_text_dc == NULL ||
        (softpc_window_presented_text_valid && memcmp(softpc_window_presented_text,
            softpc_window_frame->text, sizeof(softpc_window_presented_text)) == 0 &&
         memcmp(softpc_window_presented_attributes,
            softpc_window_frame->attributes,
            sizeof(softpc_window_presented_attributes)) == 0)) return;
    SelectObject(softpc_window_text_dc, softpc_window_font);
    SetBkMode(softpc_window_text_dc, OPAQUE);
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        int column = 0;
        while (column < SOFTPC_TEXT_COLUMNS) {
            unsigned short attribute = softpc_window_frame->attributes[
                row * SOFTPC_TEXT_COLUMNS + column];
            char line[SOFTPC_TEXT_COLUMNS];
            int length = 0;
            while (column + length < SOFTPC_TEXT_COLUMNS &&
                softpc_window_frame->attributes[row * SOFTPC_TEXT_COLUMNS +
                    column + length] == attribute) {
                unsigned char character = softpc_window_frame->text[
                    row * SOFTPC_TEXT_COLUMNS + column + length];
                line[length++] = character >= 0x20u && character < 0x7fu ?
                    (char)character : ' ';
            }
            SetTextColor(softpc_window_text_dc, softpc_window_colour(attribute));
            SetBkColor(softpc_window_text_dc, softpc_window_colour(attribute >> 4));
            TextOutA(softpc_window_text_dc, column * SOFTPC_TEXT_CELL_WIDTH,
                row * SOFTPC_TEXT_CELL_HEIGHT, line, length);
            column += length;
        }
    }
    memcpy(softpc_window_presented_text, softpc_window_frame->text,
        sizeof(softpc_window_presented_text));
    memcpy(softpc_window_presented_attributes, softpc_window_frame->attributes,
        sizeof(softpc_window_presented_attributes));
    softpc_window_presented_text_valid = 1;
}

static void softpc_window_paint(HDC dc)
{
    if (softpc_window_frame == NULL || softpc_window_frame->valid == 0u) return;
    if (softpc_window_frame->graphics != 0u) {
        StretchDIBits(dc, 8, 8, (int)softpc_window_frame->dib_width,
            (int)softpc_window_frame->dib_height, 0, 0,
            (int)softpc_window_frame->dib_width,
            (int)softpc_window_frame->dib_height,
            softpc_window_frame->dib_bits,
            (const BITMAPINFO *)softpc_window_frame->dib_info,
            DIB_RGB_COLORS, SRCCOPY);
        return;
    }
    softpc_window_update_text_surface();
    BitBlt(dc, 8, 8, SOFTPC_TEXT_SURFACE_WIDTH, SOFTPC_TEXT_SURFACE_HEIGHT,
        softpc_window_text_dc, 0, 0, SRCCOPY);
}

static void softpc_window_enqueue_key(WPARAM key, LPARAM lparam, int released)
{
    KEY_EVENT_RECORD event;
    BYTE key_number;
    ZeroMemory(&event, sizeof(event));
    event.wVirtualKeyCode = (WORD)key;
    event.wVirtualScanCode = (WORD)((lparam >> 16) & 0xffu);
    if ((lparam & 0x01000000L) != 0) event.dwControlKeyState = ENHANCED_KEY;
    key_number = KeyMsgToKeyCode(&event);
    if (key_number == 0u && event.wVirtualKeyCode != 0u) {
        UINT scan = MapVirtualKeyW(event.wVirtualKeyCode, MAPVK_VK_TO_VSC);
        event.wVirtualScanCode = (WORD)(scan & 0xffu);
        key_number = KeyMsgToKeyCode(&event);
    }
    if (key_number != 0u)
        (void)softpc_runtime_enqueue_key(softpc_window_runtime, key_number,
            (uint8_t)released);
}

static void softpc_window_char(WCHAR character)
{
    SHORT translated = VkKeyScanW(character);
    KEY_EVENT_RECORD event;
    BYTE key_number;
    if (translated == -1) return;
    ZeroMemory(&event, sizeof(event));
    event.wVirtualKeyCode = LOBYTE(translated);
    event.wVirtualScanCode = (WORD)MapVirtualKeyW(event.wVirtualKeyCode,
        MAPVK_VK_TO_VSC);
    key_number = KeyMsgToKeyCode(&event);
    if (key_number != 0u) {
        (void)softpc_runtime_enqueue_key(softpc_window_runtime, key_number, 0u);
        (void)softpc_runtime_enqueue_key(softpc_window_runtime, key_number, 1u);
    }
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
            (void)softpc_runtime_copy_frame(softpc_window_runtime,
                softpc_window_frame);
            InvalidateRect(window, NULL, FALSE);
            if (softpc_runtime_get_state(softpc_window_runtime) !=
                SOFTPC_RUNTIME_RUNNING) DestroyWindow(window);
        }
        return 0;
    case WM_PAINT:
        { PAINTSTRUCT paint; HDC dc = BeginPaint(window, &paint);
          softpc_window_paint(dc); EndPaint(window, &paint); }
        return 0;
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
        } else { softpc_window_keydown_delivered = 1; softpc_window_enqueue_key(wparam, lparam, 0); }
        return 0;
    case WM_KEYUP:
    case WM_SYSKEYUP: softpc_window_enqueue_key(wparam, lparam, 1); return 0;
    case WM_CHAR:
        if (!softpc_window_keydown_delivered && wparam >= 0x20u && wparam != 0x7fu)
            softpc_window_char((WCHAR)wparam);
        softpc_window_keydown_delivered = 0;
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
    softpc_window_mouse_valid = 0;
    softpc_window_left_button = softpc_window_right_button = 0;
    softpc_window_font = CreateFontA(16, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, "Consolas");
    window = CreateWindowExA(0, klass.lpszClassName, "SoftPC VM", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 680, 560, NULL, NULL, klass.hInstance, NULL);
    if (window == NULL) { free(softpc_window_frame); return SOFTPC_VM_FRONTEND_ERROR; }
    dc = GetDC(window);
    softpc_window_text_dc = CreateCompatibleDC(dc);
    softpc_window_text_bitmap = CreateCompatibleBitmap(dc, SOFTPC_TEXT_SURFACE_WIDTH,
        SOFTPC_TEXT_SURFACE_HEIGHT);
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
    DeleteDC(softpc_window_text_dc); DeleteObject(softpc_window_text_bitmap);
    DeleteObject(softpc_window_font); free(softpc_window_frame);
    softpc_window_frame = NULL; softpc_window_runtime = NULL;
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
