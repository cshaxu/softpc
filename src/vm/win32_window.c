#include "win32_window.h"

#ifdef _WIN32
#include <windows.h>
#include <string.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD KeyEvent);

#define SOFTPC_TEXT_COLUMNS 80
#define SOFTPC_TEXT_ROWS 25
#define SOFTPC_TEXT_CELL_WIDTH 8
#define SOFTPC_TEXT_CELL_HEIGHT 16
#define SOFTPC_TEXT_SURFACE_WIDTH (SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_CELL_WIDTH)
#define SOFTPC_TEXT_SURFACE_HEIGHT (SOFTPC_TEXT_ROWS * SOFTPC_TEXT_CELL_HEIGHT)
#define SOFTPC_TIMER_ID 1u
/* Match the standalone console's executor quantum.  The window has its own
 * UI thread and queued input, so it must not turn every 1,000 instructions
 * into a Windows timer sleep: on normal timer resolution that throttles the
 * guest to tens of thousands of instructions per second. */
#define SOFTPC_RUN_SLICE 50000u
#define SOFTPC_DISPLAY_CADENCE_MS 50u
#define SOFTPC_INPUT_QUEUE_CAPACITY 128u
#define SOFTPC_VGA_MODE13_WIDTH 320
#define SOFTPC_VGA_MODE13_HEIGHT 200
#define SOFTPC_VGA_PLANAR_MAX_WIDTH 1024
#define SOFTPC_VGA_PLANAR_MAX_HEIGHT 768
#define SOFTPC_CGA_GRAPHICS_MAX_WIDTH 640
#define SOFTPC_CGA_GRAPHICS_MAX_HEIGHT 200

static softpc_machine *softpc_window_machine;
static HFONT softpc_window_font;
static HDC softpc_window_text_dc;
static HBITMAP softpc_window_text_bitmap;
static HGDIOBJ softpc_window_text_previous_bitmap;
static int softpc_window_mouse_x;
static int softpc_window_mouse_y;
static int softpc_window_mouse_position_valid;
static int softpc_window_left_button;
static int softpc_window_right_button;
static int softpc_window_result;
static int softpc_window_keydown_delivered;
static HWND softpc_window_handle;
static HANDLE softpc_window_runner;
static CRITICAL_SECTION softpc_window_machine_lock;
static CRITICAL_SECTION softpc_window_input_lock;
static CRITICAL_SECTION softpc_window_snapshot_lock;
static volatile LONG softpc_window_runner_active;
static volatile LONG softpc_window_runner_failed;
static unsigned char softpc_window_snapshot_text[SOFTPC_TEXT_COLUMNS *
    SOFTPC_TEXT_ROWS];
static unsigned short softpc_window_snapshot_attributes[SOFTPC_TEXT_COLUMNS *
    SOFTPC_TEXT_ROWS];
static int softpc_window_snapshot_graphics;
static int softpc_window_snapshot_valid;
static unsigned char softpc_window_presented_text[SOFTPC_TEXT_COLUMNS *
    SOFTPC_TEXT_ROWS];
static unsigned short softpc_window_presented_attributes[SOFTPC_TEXT_COLUMNS *
    SOFTPC_TEXT_ROWS];
static int softpc_window_presented_text_valid;
static unsigned char softpc_window_input_keys[SOFTPC_INPUT_QUEUE_CAPACITY];
static unsigned char softpc_window_input_releases[SOFTPC_INPUT_QUEUE_CAPACITY];
static unsigned int softpc_window_input_head;
static unsigned int softpc_window_input_tail;
static int softpc_window_pending_mouse_dx;
static int softpc_window_pending_mouse_dy;
static uint8_t softpc_window_pending_mouse_left;
static uint8_t softpc_window_pending_mouse_right;
static int softpc_window_mouse_pending;

static void softpc_window_queue_key(BYTE key_number, int released)
{
    unsigned int next;
    if (key_number == 0u) return;
    EnterCriticalSection(&softpc_window_input_lock);
    next = (softpc_window_input_head + 1u) % SOFTPC_INPUT_QUEUE_CAPACITY;
    if (next != softpc_window_input_tail) {
        softpc_window_input_keys[softpc_window_input_head] = key_number;
        softpc_window_input_releases[softpc_window_input_head] =
            (unsigned char)(released != 0);
        softpc_window_input_head = next;
        softpc_machine_request_wake(softpc_window_machine);
    }
    LeaveCriticalSection(&softpc_window_input_lock);
}

static int softpc_window_apply_queued_input(softpc_machine *machine)
{
    int applied = 0;
    for (;;) {
        BYTE key_number;
        unsigned char released;
        EnterCriticalSection(&softpc_window_input_lock);
        if (softpc_window_input_tail == softpc_window_input_head) {
            LeaveCriticalSection(&softpc_window_input_lock);
            break;
        }
        key_number = softpc_window_input_keys[softpc_window_input_tail];
        released = softpc_window_input_releases[softpc_window_input_tail];
        softpc_window_input_tail = (softpc_window_input_tail + 1u) %
            SOFTPC_INPUT_QUEUE_CAPACITY;
        LeaveCriticalSection(&softpc_window_input_lock);
        (void)softpc_machine_key_number(machine, key_number, released);
        applied = 1;
    }

    EnterCriticalSection(&softpc_window_input_lock);
    if (softpc_window_mouse_pending) {
        int delta_x = softpc_window_pending_mouse_dx;
        int delta_y = softpc_window_pending_mouse_dy;
        uint8_t left = softpc_window_pending_mouse_left;
        uint8_t right = softpc_window_pending_mouse_right;
        softpc_window_pending_mouse_dx = 0;
        softpc_window_pending_mouse_dy = 0;
        softpc_window_mouse_pending = 0;
        LeaveCriticalSection(&softpc_window_input_lock);
        (void)softpc_machine_mouse_input(machine, delta_x, delta_y, left, right);
        applied = 1;
    } else {
        LeaveCriticalSection(&softpc_window_input_lock);
    }
    return applied;
}

/* NXVM-style publication boundary: the worker alone reads SoftPC state;
 * the UI sees only this copied frame and never contends for machine state. */
static void softpc_window_publish_snapshot(softpc_machine *machine)
{
    const void *surface;
    const unsigned char *cells;
    uint32_t columns;
    uint32_t rows;
    uint32_t stride;
    uint32_t cell_bytes;
    uint32_t row;

    EnterCriticalSection(&softpc_window_snapshot_lock);
    if (softpc_machine_presentation_is_graphics(machine)) {
        softpc_window_snapshot_graphics = 1;
        softpc_window_snapshot_valid = 1;
    } else if (softpc_machine_presentation_text(machine, &surface, &columns,
            &rows, &stride, &cell_bytes) && cell_bytes >= 1u && stride >= columns) {
        memset(softpc_window_snapshot_text, ' ', sizeof(softpc_window_snapshot_text));
        {
            size_t cell;
            for (cell = 0u; cell < SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS;
                    ++cell)
                softpc_window_snapshot_attributes[cell] = 0x07u;
        }
        cells = (const unsigned char *)surface;
        if (columns > SOFTPC_TEXT_COLUMNS) columns = SOFTPC_TEXT_COLUMNS;
        if (rows > SOFTPC_TEXT_ROWS) rows = SOFTPC_TEXT_ROWS;
        for (row = 0u; row < rows; ++row) {
            uint32_t column;
            for (column = 0u; column < columns; ++column) {
                size_t source = ((size_t)row * stride + column) * cell_bytes;
                size_t destination = (size_t)row * SOFTPC_TEXT_COLUMNS + column;
                softpc_window_snapshot_text[destination] = cells[source];
                /* The imported nt_cga renderer retains the original x86
                   layout: character, attribute, then two padding bytes.
                   (The four-byte cell is an ABI alignment detail, not a
                   16-bit attribute.) */
                if (cell_bytes >= 2u)
                    softpc_window_snapshot_attributes[destination] =
                        (unsigned short)cells[source + 1u];
            }
        }
        softpc_window_snapshot_graphics = 0;
        softpc_window_snapshot_valid = 1;
    }
    LeaveCriticalSection(&softpc_window_snapshot_lock);
}

static DWORD WINAPI softpc_window_run_machine(void *opaque)
{
    softpc_machine *machine = (softpc_machine *)opaque;
    DWORD next_snapshot = 0u;

    while (InterlockedCompareExchange(&softpc_window_runner_active, 0, 0) != 0) {
        softpc_machine_result result;
        int input_applied;

        EnterCriticalSection(&softpc_window_machine_lock);
        input_applied = softpc_window_apply_queued_input(machine);
        result = softpc_machine_run(machine, SOFTPC_RUN_SLICE);
        LeaveCriticalSection(&softpc_window_machine_lock);
        if (result != SOFTPC_MACHINE_OK) {
            InterlockedExchange(&softpc_window_runner_failed, 1);
            InterlockedExchange(&softpc_window_runner_active, 0);
            if (softpc_window_handle != NULL)
                PostMessageA(softpc_window_handle, WM_CLOSE, 0, 0);
            break;
        }
        if (input_applied || (LONG)(GetTickCount() - next_snapshot) >= 0) {
            EnterCriticalSection(&softpc_window_machine_lock);
            softpc_window_publish_snapshot(machine);
            LeaveCriticalSection(&softpc_window_machine_lock);
            next_snapshot = GetTickCount() + SOFTPC_DISPLAY_CADENCE_MS;
            if (input_applied && softpc_window_handle != NULL)
                PostMessageA(softpc_window_handle, WM_APP + 1u, 0, 0);
        }
    }
    return 0u;
}

/* Keep the original DIB presentation path intact.  The UI uses a try-lock,
 * so an in-flight C-VID update merely drops this repaint rather than blocking
 * input or the Windows message queue. */
static int softpc_window_paint_original_dib(HDC dc, softpc_machine *machine)
{
    const void *bits;
    const void *info;
    uint32_t width;
    uint32_t height;
    if (!softpc_machine_presentation_dib(machine, &bits, &info, &width,
            &height)) return 0;
    StretchDIBits(dc, 8, 8, (int)width, (int)height, 0, 0, (int)width,
        (int)height, bits, (const BITMAPINFO *)info, DIB_RGB_COLORS, SRCCOPY);
    return 1;
}

static COLORREF softpc_window_text_colour(unsigned int colour)
{
    static const COLORREF palette[16] = {
        RGB(0, 0, 0), RGB(0, 0, 170), RGB(0, 170, 0), RGB(0, 170, 170),
        RGB(170, 0, 0), RGB(170, 0, 170), RGB(170, 85, 0), RGB(170, 170, 170),
        RGB(85, 85, 85), RGB(85, 85, 255), RGB(85, 255, 85), RGB(85, 255, 255),
        RGB(255, 85, 85), RGB(255, 85, 255), RGB(255, 255, 85), RGB(255, 255, 255)
    };
    return palette[colour & 0x0fu];
}

/* Match NXVM's display ownership: GDI builds a local backing surface only
 * when the published text frame changes; each paint then sends one BitBlt to
 * the window/RDP transport rather than a sequence of remote TextOut calls. */
static void softpc_window_update_text_surface(void)
{
    int row;

    if (softpc_window_text_dc == NULL || softpc_window_font == NULL ||
        (softpc_window_presented_text_valid && memcmp(
            softpc_window_presented_text, softpc_window_snapshot_text,
            sizeof(softpc_window_presented_text)) == 0 && memcmp(
            softpc_window_presented_attributes,
            softpc_window_snapshot_attributes,
            sizeof(softpc_window_presented_attributes)) == 0)) return;
    SelectObject(softpc_window_text_dc, softpc_window_font);
    SetBkMode(softpc_window_text_dc, OPAQUE);
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        int column = 0;
        while (column < SOFTPC_TEXT_COLUMNS) {
            unsigned short attribute = softpc_window_snapshot_attributes[
                row * SOFTPC_TEXT_COLUMNS + column];
            char line[SOFTPC_TEXT_COLUMNS + 1];
            int length = 0;
            while (column + length < SOFTPC_TEXT_COLUMNS &&
                softpc_window_snapshot_attributes[row * SOFTPC_TEXT_COLUMNS +
                column + length] == attribute) {
                unsigned char character = softpc_window_snapshot_text[
                    row * SOFTPC_TEXT_COLUMNS + column + length];
                line[length++] = character >= 0x20u && character < 0x7fu ?
                    (char)character : ' ';
            }
            SetTextColor(softpc_window_text_dc,
                softpc_window_text_colour(attribute));
            SetBkColor(softpc_window_text_dc,
                softpc_window_text_colour(attribute >> 4));
            TextOutA(softpc_window_text_dc,
                column * SOFTPC_TEXT_CELL_WIDTH,
                row * SOFTPC_TEXT_CELL_HEIGHT, line, length);
            column += length;
        }
    }
    memcpy(softpc_window_presented_text, softpc_window_snapshot_text,
        sizeof(softpc_window_presented_text));
    memcpy(softpc_window_presented_attributes,
        softpc_window_snapshot_attributes,
        sizeof(softpc_window_presented_attributes));
    softpc_window_presented_text_valid = 1;
}

static void softpc_window_deliver_virtual_key(WORD virtual_key,
    int released)
{
    KEY_EVENT_RECORD event;
    BYTE key_number;
    UINT scan_code = MapVirtualKeyW(virtual_key, MAPVK_VK_TO_VSC);
    if (scan_code == 0u) return;
    ZeroMemory(&event, sizeof(event));
    event.wVirtualKeyCode = virtual_key;
    event.wVirtualScanCode = (WORD)(scan_code & 0xffu);
    key_number = KeyMsgToKeyCode(&event);
    if (key_number != 0u)
        softpc_window_queue_key(key_number, released);
}

static void softpc_window_deliver_unicode(WCHAR character)
{
    SHORT translated = VkKeyScanW(character);
    BYTE modifiers;
    if (translated == -1) return;
    modifiers = HIBYTE(translated);
    /* The same mobile-RDP packet form handled by the console frontend can
       arrive as WM_CHAR when the standalone DIB window has focus.  Retain
       the original SoftPC key-number mapper; synthesize only the missing
       host key transition. */
    if ((modifiers & 1u) != 0u) softpc_window_deliver_virtual_key(VK_SHIFT, 0);
    if ((modifiers & 2u) != 0u) softpc_window_deliver_virtual_key(VK_CONTROL, 0);
    if ((modifiers & 4u) != 0u) softpc_window_deliver_virtual_key(VK_MENU, 0);
    softpc_window_deliver_virtual_key(LOBYTE(translated), 0);
    softpc_window_deliver_virtual_key(LOBYTE(translated), 1);
    if ((modifiers & 4u) != 0u) softpc_window_deliver_virtual_key(VK_MENU, 1);
    if ((modifiers & 2u) != 0u) softpc_window_deliver_virtual_key(VK_CONTROL, 1);
    if ((modifiers & 1u) != 0u) softpc_window_deliver_virtual_key(VK_SHIFT, 1);
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
    EnterCriticalSection(&softpc_window_snapshot_lock);
    if (softpc_window_snapshot_graphics) {
        LeaveCriticalSection(&softpc_window_snapshot_lock);
        if (TryEnterCriticalSection(&softpc_window_machine_lock)) {
            (void)softpc_window_paint_original_dib(dc, softpc_window_machine);
            LeaveCriticalSection(&softpc_window_machine_lock);
        }
        return;
    }
    softpc_window_update_text_surface();
    LeaveCriticalSection(&softpc_window_snapshot_lock);
    if (softpc_window_text_dc != NULL)
        BitBlt(dc, 8, 8, SOFTPC_TEXT_SURFACE_WIDTH, SOFTPC_TEXT_SURFACE_HEIGHT,
            softpc_window_text_dc, 0, 0, SRCCOPY);
}

static void softpc_window_key(WPARAM key, LPARAM lparam, int released)
{
    KEY_EVENT_RECORD event;
    BYTE key_number;
    ZeroMemory(&event, sizeof(event));
    if (!released) softpc_window_keydown_delivered = 0;
    event.wVirtualKeyCode = (WORD)key;
    event.wVirtualScanCode = (WORD)((lparam >> 16) & 0xffu);
    if ((lparam & 0x01000000L) != 0)
        event.dwControlKeyState |= ENHANCED_KEY;
    key_number = KeyMsgToKeyCode(&event);
    if (key_number == 0u && event.wVirtualKeyCode != 0u) {
        UINT scan_code = MapVirtualKeyW(event.wVirtualKeyCode, MAPVK_VK_TO_VSC);
        if (scan_code != 0u) {
            event.wVirtualScanCode = (WORD)(scan_code & 0xffu);
            key_number = KeyMsgToKeyCode(&event);
        }
    }
    if (key_number != 0u)
        softpc_window_queue_key(key_number, released);
    if (!released && key_number != 0u) softpc_window_keydown_delivered = 1;
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
    EnterCriticalSection(&softpc_window_input_lock);
    softpc_window_pending_mouse_dx += delta_x;
    softpc_window_pending_mouse_dy += delta_y;
    softpc_window_pending_mouse_left = (uint8_t)softpc_window_left_button;
    softpc_window_pending_mouse_right = (uint8_t)softpc_window_right_button;
    softpc_window_mouse_pending = 1;
    softpc_machine_request_wake(softpc_window_machine);
    LeaveCriticalSection(&softpc_window_input_lock);
}

static LRESULT CALLBACK softpc_window_proc(HWND window, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_TIMER:
        if (wparam == SOFTPC_TIMER_ID) {
            InvalidateRect(window, NULL, FALSE);
        }
        return 0;
    case WM_APP + 1u:
        InvalidateRect(window, NULL, FALSE);
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
            InterlockedExchange(&softpc_window_runner_active, 0);
            DestroyWindow(window);
            return 0;
        }
        if (wparam == VK_ESCAPE) {
            softpc_window_result = SOFTPC_VM_FRONTEND_STOPPED;
            InterlockedExchange(&softpc_window_runner_active, 0);
            DestroyWindow(window);
            return 0;
        }
        softpc_window_key(wparam, lparam, 0);
        return 0;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        softpc_window_key(wparam, lparam, 1);
        return 0;
    case WM_CHAR:
        if (!softpc_window_keydown_delivered && wparam >= 0x20u && wparam != 0x7fu)
            softpc_window_deliver_unicode((WCHAR)wparam);
        softpc_window_keydown_delivered = 0;
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
        InterlockedExchange(&softpc_window_runner_active, 0);
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
    window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    window_class.lpszClassName = "SoftPCStandaloneWindow";
    if (RegisterClassA(&window_class) == 0 && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        return 1;
    softpc_window_machine = machine;
    softpc_window_mouse_position_valid = 0;
    softpc_window_left_button = 0;
    softpc_window_right_button = 0;
    softpc_window_result = SOFTPC_VM_FRONTEND_STOPPED;
    softpc_window_handle = NULL;
    softpc_window_runner = NULL;
    softpc_window_input_head = 0u;
    softpc_window_input_tail = 0u;
    softpc_window_pending_mouse_dx = 0;
    softpc_window_pending_mouse_dy = 0;
    softpc_window_pending_mouse_left = 0u;
    softpc_window_pending_mouse_right = 0u;
    softpc_window_mouse_pending = 0;
    InterlockedExchange(&softpc_window_runner_active, 0);
    InterlockedExchange(&softpc_window_runner_failed, 0);
    InitializeCriticalSection(&softpc_window_machine_lock);
    InitializeCriticalSection(&softpc_window_input_lock);
    InitializeCriticalSection(&softpc_window_snapshot_lock);
    softpc_window_snapshot_valid = 0;
    softpc_window_snapshot_graphics = 0;
    softpc_window_presented_text_valid = 0;
    softpc_window_font = CreateFontA(16, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, "Consolas");
    window = CreateWindowExA(0, window_class.lpszClassName, "SoftPC VM",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 680, 560,
        NULL, NULL, instance, NULL);
    if (window == NULL) {
        DeleteCriticalSection(&softpc_window_snapshot_lock);
        DeleteCriticalSection(&softpc_window_input_lock);
        DeleteCriticalSection(&softpc_window_machine_lock);
        return 1;
    }
    {
        HDC window_dc = GetDC(window);
        if (window_dc != NULL) {
            softpc_window_text_dc = CreateCompatibleDC(window_dc);
            softpc_window_text_bitmap = CreateCompatibleBitmap(window_dc,
                SOFTPC_TEXT_SURFACE_WIDTH, SOFTPC_TEXT_SURFACE_HEIGHT);
            ReleaseDC(window, window_dc);
        }
        if (softpc_window_text_dc == NULL || softpc_window_text_bitmap == NULL) {
            if (softpc_window_text_dc != NULL) DeleteDC(softpc_window_text_dc);
            if (softpc_window_text_bitmap != NULL) DeleteObject(softpc_window_text_bitmap);
            softpc_window_text_dc = NULL;
            softpc_window_text_bitmap = NULL;
            DestroyWindow(window);
            DeleteCriticalSection(&softpc_window_snapshot_lock);
            DeleteCriticalSection(&softpc_window_input_lock);
            DeleteCriticalSection(&softpc_window_machine_lock);
            return 1;
        }
        softpc_window_text_previous_bitmap = SelectObject(softpc_window_text_dc,
            softpc_window_text_bitmap);
    }
    softpc_window_handle = window;
    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);
    SetTimer(window, SOFTPC_TIMER_ID, SOFTPC_DISPLAY_CADENCE_MS, NULL);
    InterlockedExchange(&softpc_window_runner_active, 1);
    softpc_window_runner = CreateThread(NULL, 0u, softpc_window_run_machine,
        machine, 0u, NULL);
    if (softpc_window_runner == NULL) {
        InterlockedExchange(&softpc_window_runner_active, 0);
        DestroyWindow(window);
    }
    while (GetMessageA(&message, NULL, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    if (softpc_window_runner != NULL) {
        InterlockedExchange(&softpc_window_runner_active, 0);
        WaitForSingleObject(softpc_window_runner, INFINITE);
        CloseHandle(softpc_window_runner);
        softpc_window_runner = NULL;
    }
    if (InterlockedCompareExchange(&softpc_window_runner_failed, 0, 0) != 0)
        softpc_window_result = SOFTPC_VM_FRONTEND_ERROR;
    softpc_window_handle = NULL;
    DeleteCriticalSection(&softpc_window_machine_lock);
    DeleteCriticalSection(&softpc_window_input_lock);
    DeleteCriticalSection(&softpc_window_snapshot_lock);
    if (softpc_window_text_dc != NULL) {
        SelectObject(softpc_window_text_dc, softpc_window_text_previous_bitmap);
        DeleteDC(softpc_window_text_dc);
    }
    DeleteObject(softpc_window_text_bitmap);
    softpc_window_text_dc = NULL;
    softpc_window_text_bitmap = NULL;
    softpc_window_text_previous_bitmap = NULL;
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
