#include "win32_window.h"

#ifdef _WIN32
#include <windows.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD KeyEvent);

#define SOFTPC_TEXT_COLUMNS 80
#define SOFTPC_TEXT_ROWS 25
#define SOFTPC_TIMER_ID 1u
/* Keep each GUI tick bounded for responsive input, but large enough that
 * POST is not delayed by the host timer granularity. */
#define SOFTPC_RUN_SLICE 50000u
#define SOFTPC_VGA_MODE13_WIDTH 320
#define SOFTPC_VGA_MODE13_HEIGHT 200
#define SOFTPC_VGA_PLANAR_MAX_WIDTH 1024
#define SOFTPC_VGA_PLANAR_MAX_HEIGHT 768
#define SOFTPC_CGA_GRAPHICS_MAX_WIDTH 640
#define SOFTPC_CGA_GRAPHICS_MAX_HEIGHT 200

static softpc_machine *softpc_window_machine;
static HFONT softpc_window_font;
static int softpc_window_mouse_x;
static int softpc_window_mouse_y;
static int softpc_window_mouse_position_valid;
static int softpc_window_left_button;
static int softpc_window_right_button;
static int softpc_window_result;

static int softpc_window_paint_original_dib(HDC dc)
{
    const void *bits;
    const void *info;
    uint32_t width;
    uint32_t height;
    if (!softpc_machine_presentation_dib(softpc_window_machine, &bits, &info,
            &width, &height))
        return 0;
    StretchDIBits(dc, 8, 8, (int)width, (int)height, 0, 0, (int)width,
        (int)height, bits, (const BITMAPINFO *)info, DIB_RGB_COLORS, SRCCOPY);
    return 1;
}

static int softpc_window_mouse_x_from_lparam(LPARAM position)
{
    return (int)(short)LOWORD(position);
}

static int softpc_window_mouse_y_from_lparam(LPARAM position)
{
    return (int)(short)HIWORD(position);
}

static void softpc_window_paint(HDC dc)
{
    const void *surface;
    const unsigned char *cells;
    uint32_t columns;
    uint32_t rows;
    uint32_t stride;
    uint32_t cell_bytes;
    int row;
    if (softpc_machine_presentation_is_graphics(softpc_window_machine)) {
        (void)softpc_window_paint_original_dib(dc);
        return;
    }
    if (!softpc_machine_presentation_text(softpc_window_machine, &surface,
            &columns, &rows, &stride, &cell_bytes) ||
        cell_bytes < 1u || stride < columns) return;
    cells = (const unsigned char *)surface;
    SelectObject(dc, softpc_window_font);
    SetBkMode(dc, OPAQUE);
    SetBkColor(dc, RGB(0, 0, 0));
    SetTextColor(dc, RGB(192, 192, 192));
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        char line[SOFTPC_TEXT_COLUMNS + 1];
        int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = ' ';
            if ((uint32_t)row < rows && (uint32_t)column < columns)
                character = cells[((uint32_t)row * stride + (uint32_t)column) *
                    cell_bytes];
            line[column] = character >= 0x20u && character < 0x7fu ?
                (char)character : ' ';
        }
        line[SOFTPC_TEXT_COLUMNS] = '\0';
        TextOutA(dc, 8, 8 + row * 16, line, SOFTPC_TEXT_COLUMNS);
    }
}

static void softpc_window_key(WPARAM key, LPARAM lparam, int released)
{
    KEY_EVENT_RECORD event;
    BYTE key_number;
    ZeroMemory(&event, sizeof(event));
    event.wVirtualKeyCode = (WORD)key;
    event.wVirtualScanCode = (WORD)((lparam >> 16) & 0xffu);
    if ((lparam & 0x01000000L) != 0)
        event.dwControlKeyState |= ENHANCED_KEY;
    key_number = KeyMsgToKeyCode(&event);
    if (key_number != 0u)
        (void)softpc_machine_key_number(softpc_window_machine, key_number,
            (uint8_t)released);
}
static void softpc_window_mouse(int x, int y)
{
    int delta_x = 0;
    int delta_y = 0;
    if (softpc_window_mouse_position_valid) {
        delta_x = x - softpc_window_mouse_x;
        delta_y = y - softpc_window_mouse_y;
    }
    softpc_window_mouse_x = x;
    softpc_window_mouse_y = y;
    softpc_window_mouse_position_valid = 1;
    (void)softpc_machine_mouse_input(softpc_window_machine, delta_x, delta_y,
        (uint8_t)softpc_window_left_button,
        (uint8_t)softpc_window_right_button);
}

static LRESULT CALLBACK softpc_window_proc(HWND window, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_TIMER:
        if (wparam == SOFTPC_TIMER_ID) {
            int32_t left;
            int32_t top;
            int32_t right;
            int32_t bottom;
            (void)softpc_machine_run(softpc_window_machine, SOFTPC_RUN_SLICE);
            if (!softpc_machine_presentation_is_graphics(softpc_window_machine)) {
                InvalidateRect(window, NULL, FALSE);
            } else if (softpc_machine_presentation_take_dirty(
                    softpc_window_machine, &left, &top, &right, &bottom)) {
                RECT dirty;
                dirty.left = left + 8;
                dirty.top = top + 8;
                dirty.right = right + 9;
                dirty.bottom = bottom + 9;
                InvalidateRect(window, &dirty, FALSE);
            }
        }
        return 0;
    case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC dc = BeginPaint(window, &paint);
            softpc_window_paint(dc);
            EndPaint(window, &paint);
        }
        return 0;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (wparam == 'P' && (GetKeyState(VK_CONTROL) < 0) &&
            (GetKeyState(VK_MENU) < 0)) {
            softpc_window_result = SOFTPC_VM_FRONTEND_PAUSED;
            DestroyWindow(window);
            return 0;
        }
        if (wparam == VK_ESCAPE) {
            softpc_window_result = SOFTPC_VM_FRONTEND_STOPPED;
            DestroyWindow(window);
            return 0;
        }
        softpc_window_key(wparam, lparam, 0);
        return 0;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        softpc_window_key(wparam, lparam, 1);
        return 0;
    case WM_MOUSEMOVE:
        softpc_window_mouse(softpc_window_mouse_x_from_lparam(lparam),
            softpc_window_mouse_y_from_lparam(lparam));
        return 0;
    case WM_LBUTTONDOWN:
        softpc_window_left_button = 1;
        SetCapture(window);
        softpc_window_mouse(softpc_window_mouse_x_from_lparam(lparam),
            softpc_window_mouse_y_from_lparam(lparam));
        return 0;
    case WM_LBUTTONUP:
        softpc_window_left_button = 0;
        softpc_window_mouse(softpc_window_mouse_x_from_lparam(lparam),
            softpc_window_mouse_y_from_lparam(lparam));
        if (!softpc_window_right_button) ReleaseCapture();
        return 0;
    case WM_RBUTTONDOWN:
        softpc_window_right_button = 1;
        SetCapture(window);
        softpc_window_mouse(softpc_window_mouse_x_from_lparam(lparam),
            softpc_window_mouse_y_from_lparam(lparam));
        return 0;
    case WM_RBUTTONUP:
        softpc_window_right_button = 0;
        softpc_window_mouse(softpc_window_mouse_x_from_lparam(lparam),
            softpc_window_mouse_y_from_lparam(lparam));
        if (!softpc_window_left_button) ReleaseCapture();
        return 0;
    case WM_DESTROY:
        KillTimer(window, SOFTPC_TIMER_ID);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

int softpc_vm_run_window(softpc_machine *machine)
{
    WNDCLASSA window_class;
    MSG message;
    HWND window;
    HINSTANCE instance;
    if (machine == NULL) return 1;
    instance = GetModuleHandleA(NULL);
    ZeroMemory(&window_class, sizeof(window_class));
    window_class.lpfnWndProc = softpc_window_proc;
    window_class.hInstance = instance;
    window_class.hCursor = LoadCursorA(NULL, IDC_ARROW);
    window_class.lpszClassName = "SoftPCStandaloneWindow";
    if (RegisterClassA(&window_class) == 0 && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        return 1;
    softpc_window_machine = machine;
    softpc_window_mouse_position_valid = 0;
    softpc_window_left_button = 0;
    softpc_window_right_button = 0;
    softpc_window_result = SOFTPC_VM_FRONTEND_STOPPED;
    softpc_window_font = CreateFontA(16, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, "Consolas");
    window = CreateWindowExA(0, window_class.lpszClassName, "SoftPC VM",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 680, 560,
        NULL, NULL, instance, NULL);
    if (window == NULL) return 1;
    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);
    SetTimer(window, SOFTPC_TIMER_ID, 1u, NULL);
    while (GetMessageA(&message, NULL, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    DeleteObject(softpc_window_font);
    softpc_window_font = NULL;
    softpc_window_machine = NULL;
    return softpc_window_result;
}

#else
int softpc_vm_run_window(softpc_machine *machine)
{
    (void)machine;
    return SOFTPC_VM_FRONTEND_ERROR;
}
#endif
