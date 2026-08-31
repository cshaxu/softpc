#include "win32_window.h"

#ifdef _WIN32
#include <windows.h>
#include <string.h>

extern BYTE KeyMsgToKeyCode(PKEY_EVENT_RECORD KeyEvent);

#define SOFTPC_TEXT_COLUMNS 80
#define SOFTPC_TEXT_ROWS 25
#define SOFTPC_TIMER_ID 1u
/* Keep each GUI tick bounded for responsive input, but large enough that
 * POST is not delayed by the host timer granularity. */
#define SOFTPC_RUN_SLICE 1000u
#define SOFTPC_PAINT_INTERVAL_MS 16u
#define SOFTPC_INPUT_QUEUE_CAPACITY 128u
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
static int softpc_window_keydown_delivered;
static HWND softpc_window_handle;
static HANDLE softpc_window_runner;
static CRITICAL_SECTION softpc_window_machine_lock;
static CRITICAL_SECTION softpc_window_input_lock;
static volatile LONG softpc_window_runner_active;
static volatile LONG softpc_window_runner_failed;
static unsigned char softpc_window_previous_text[SOFTPC_TEXT_COLUMNS *
    SOFTPC_TEXT_ROWS];
static int softpc_window_previous_text_valid;
static unsigned char softpc_window_input_keys[SOFTPC_INPUT_QUEUE_CAPACITY];
static unsigned char softpc_window_input_releases[SOFTPC_INPUT_QUEUE_CAPACITY];
static unsigned int softpc_window_input_head;
static unsigned int softpc_window_input_tail;

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
    }
    LeaveCriticalSection(&softpc_window_input_lock);
}

static void softpc_window_apply_queued_keys(softpc_machine *machine)
{
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
    }
}

static int softpc_window_text_changed(void)
{
    const void *surface;
    const unsigned char *cells;
    uint32_t columns;
    uint32_t rows;
    uint32_t stride;
    uint32_t cell_bytes;
    unsigned char current[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS];
    uint32_t row;

    if (!softpc_machine_presentation_text(softpc_window_machine, &surface,
            &columns, &rows, &stride, &cell_bytes) || cell_bytes < 1u ||
        stride < columns)
        return 0;
    memset(current, ' ', sizeof(current));
    cells = (const unsigned char *)surface;
    if (columns > SOFTPC_TEXT_COLUMNS) columns = SOFTPC_TEXT_COLUMNS;
    if (rows > SOFTPC_TEXT_ROWS) rows = SOFTPC_TEXT_ROWS;
    for (row = 0u; row < rows; ++row) {
        uint32_t column;
        for (column = 0u; column < columns; ++column)
            current[row * SOFTPC_TEXT_COLUMNS + column] =
                cells[(row * stride + column) * cell_bytes];
    }
    if (!softpc_window_previous_text_valid || memcmp(current,
            softpc_window_previous_text, sizeof(current)) != 0) {
        memcpy(softpc_window_previous_text, current, sizeof(current));
        softpc_window_previous_text_valid = 1;
        return 1;
    }
    return 0;
}

static DWORD WINAPI softpc_window_run_machine(void *opaque)
{
    softpc_machine *machine = (softpc_machine *)opaque;

    while (InterlockedCompareExchange(&softpc_window_runner_active, 0, 0) != 0) {
        softpc_machine_result result;

        EnterCriticalSection(&softpc_window_machine_lock);
        softpc_window_apply_queued_keys(machine);
        result = softpc_machine_run(machine, SOFTPC_RUN_SLICE);
        LeaveCriticalSection(&softpc_window_machine_lock);
        if (result != SOFTPC_MACHINE_OK) {
            InterlockedExchange(&softpc_window_runner_failed, 1);
            InterlockedExchange(&softpc_window_runner_active, 0);
            if (softpc_window_handle != NULL)
                PostMessageA(softpc_window_handle, WM_CLOSE, 0, 0);
            break;
        }
        /* A real sleep prevents this detached VM from monopolising one host
           core, and gives RDP/Win32 message delivery a reliable timeslice. */
        Sleep(1u);
    }
    return 0u;
}

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
    const void *surface;
    const unsigned char *cells;
    uint32_t columns;
    uint32_t rows;
    uint32_t stride;
    uint32_t cell_bytes;
    int row;
    /* The executor may be in a long original C-VID update.  Painting must
       never wait behind it: a later timer tick will invalidate the current
       surface once the worker releases the machine lock. */
    if (!TryEnterCriticalSection(&softpc_window_machine_lock)) return;
    if (softpc_machine_presentation_is_graphics(softpc_window_machine)) {
        (void)softpc_window_paint_original_dib(dc);
        LeaveCriticalSection(&softpc_window_machine_lock);
        return;
    }
    if (!softpc_machine_presentation_text(softpc_window_machine, &surface,
            &columns, &rows, &stride, &cell_bytes) ||
        cell_bytes < 1u || stride < columns) {
        LeaveCriticalSection(&softpc_window_machine_lock);
        return;
    }
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
    LeaveCriticalSection(&softpc_window_machine_lock);
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
            int graphics;
            int changed = 0;

            if (!TryEnterCriticalSection(&softpc_window_machine_lock))
                return 0;
            graphics = softpc_machine_presentation_is_graphics(
                softpc_window_machine);
            if (!graphics) changed = softpc_window_text_changed();
            else changed = softpc_machine_presentation_take_dirty(
                softpc_window_machine, &left, &top, &right, &bottom);
            LeaveCriticalSection(&softpc_window_machine_lock);
            if (!graphics && changed) {
                InvalidateRect(window, NULL, FALSE);
            } else if (graphics && changed) {
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
        EnterCriticalSection(&softpc_window_machine_lock);
        softpc_window_mouse(softpc_window_mouse_x_from_lparam(lparam),
            softpc_window_mouse_y_from_lparam(lparam));
        LeaveCriticalSection(&softpc_window_machine_lock);
        return 0;
    case WM_LBUTTONDOWN:
        softpc_window_left_button = 1;
        SetCapture(window);
        EnterCriticalSection(&softpc_window_machine_lock);
        softpc_window_mouse(softpc_window_mouse_x_from_lparam(lparam),
            softpc_window_mouse_y_from_lparam(lparam));
        LeaveCriticalSection(&softpc_window_machine_lock);
        return 0;
    case WM_LBUTTONUP:
        softpc_window_left_button = 0;
        EnterCriticalSection(&softpc_window_machine_lock);
        softpc_window_mouse(softpc_window_mouse_x_from_lparam(lparam),
            softpc_window_mouse_y_from_lparam(lparam));
        LeaveCriticalSection(&softpc_window_machine_lock);
        if (!softpc_window_right_button) ReleaseCapture();
        return 0;
    case WM_RBUTTONDOWN:
        softpc_window_right_button = 1;
        SetCapture(window);
        EnterCriticalSection(&softpc_window_machine_lock);
        softpc_window_mouse(softpc_window_mouse_x_from_lparam(lparam),
            softpc_window_mouse_y_from_lparam(lparam));
        LeaveCriticalSection(&softpc_window_machine_lock);
        return 0;
    case WM_RBUTTONUP:
        softpc_window_right_button = 0;
        EnterCriticalSection(&softpc_window_machine_lock);
        softpc_window_mouse(softpc_window_mouse_x_from_lparam(lparam),
            softpc_window_mouse_y_from_lparam(lparam));
        LeaveCriticalSection(&softpc_window_machine_lock);
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
    softpc_window_previous_text_valid = 0;
    softpc_window_left_button = 0;
    softpc_window_right_button = 0;
    softpc_window_result = SOFTPC_VM_FRONTEND_STOPPED;
    softpc_window_handle = NULL;
    softpc_window_runner = NULL;
    softpc_window_input_head = 0u;
    softpc_window_input_tail = 0u;
    InterlockedExchange(&softpc_window_runner_active, 0);
    InterlockedExchange(&softpc_window_runner_failed, 0);
    InitializeCriticalSection(&softpc_window_machine_lock);
    InitializeCriticalSection(&softpc_window_input_lock);
    softpc_window_font = CreateFontA(16, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, "Consolas");
    window = CreateWindowExA(0, window_class.lpszClassName, "SoftPC VM",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 680, 560,
        NULL, NULL, instance, NULL);
    if (window == NULL) {
        DeleteCriticalSection(&softpc_window_input_lock);
        DeleteCriticalSection(&softpc_window_machine_lock);
        return 1;
    }
    softpc_window_handle = window;
    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);
    SetTimer(window, SOFTPC_TIMER_ID, SOFTPC_PAINT_INTERVAL_MS, NULL);
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
