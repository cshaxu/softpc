#include "lib/ux-window/internal.h"

#ifdef _WIN32
#include <windows.h>

struct ux_window_native { HANDLE wake, ready, thread; HWND hwnd; lib_u32 *pixels; lib_size pixel_capacity; int mouse_x, mouse_y; lib_bool mouse_valid, mouse_enabled; };

static lib_u32 ux_window_modifiers(void)
{
    lib_u32 result = 0u;
    if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) result |= UX_MODIFIER_CONTROL;
    if ((GetKeyState(VK_MENU) & 0x8000) != 0) result |= UX_MODIFIER_ALT;
    if ((GetKeyState(VK_SHIFT) & 0x8000) != 0) result |= UX_MODIFIER_SHIFT;
    return result;
}

static LRESULT CALLBACK ux_window_proc(HWND hwnd, UINT message, WPARAM wparam,
    LPARAM lparam)
{
    ux_window *window = (ux_window *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
    if (message == WM_NCCREATE)
        SetWindowLongPtrA(hwnd, GWLP_USERDATA,
            (LONG_PTR)((CREATESTRUCTA *)lparam)->lpCreateParams);
    window = (ux_window *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
    if (window == LIB_NULL) return DefWindowProcA(hwnd, message, wparam, lparam);
    if (message == WM_CLOSE) { (void)ux_window_request_close(window); return 0; }
    if (message == WM_KILLFOCUS) { ReleaseCapture(); return 0; }
    if (message == WM_LBUTTONDOWN) {
        if (window->native->mouse_enabled != LIB_FALSE) { SetCapture(hwnd); window->native->mouse_valid = LIB_FALSE; }
        return 0;
    }
    if (message == WM_MOUSEMOVE && GetCapture() == hwnd) {
        int x = (int)(short)LOWORD(lparam), y = (int)(short)HIWORD(lparam);
        if (window->native->mouse_valid != LIB_FALSE) {
            ux_input_event event;
            if (ux_input_make_mouse(&event, window, x - window->native->mouse_x,
                    y - window->native->mouse_y, 0, 0, 0, 0, 0u, LIB_TRUE) == LIB_STATUS_OK)
                (void)ux_window_submit_input(window, &event);
        }
        window->native->mouse_x = x; window->native->mouse_y = y; window->native->mouse_valid = LIB_TRUE;
        return 0;
    }
    if (message == WM_LBUTTONUP) { if (GetCapture() == hwnd) ReleaseCapture(); return 0; }
    if (message == WM_PAINT) {
        PAINTSTRUCT paint;
        HDC dc = BeginPaint(hwnd, &paint);
        ux_frame frame;
        lib_u32 generation;
        if (ux_window_capture_paint_state(window, &frame, &generation) ==
                LIB_STATUS_OK && ux_frame_is_valid(&frame) &&
            frame.graphics == 0u) {
            lib_u32 row;
            (void)generation;
            SetBkMode(dc, OPAQUE);
            for (row = 0u; row < frame.text_rows; ++row) {
                lib_u32 column;
                for (column = 0u; column < frame.text_columns; ++column) {
                    lib_size index = (lib_size)row * UX_TEXT_COLUMNS + column;
                    lib_u16 attribute = frame.attributes[index];
                    char character = (char)frame.text[index];
                    lib_u32 foreground = frame.text_palette[attribute & 0x0fu];
                    lib_u32 background = frame.text_palette[(attribute >> 4u) & 0x0fu];
                    SetTextColor(dc, RGB((foreground >> 16u) & 0xffu,
                        (foreground >> 8u) & 0xffu, foreground & 0xffu));
                    SetBkColor(dc, RGB((background >> 16u) & 0xffu,
                        (background >> 8u) & 0xffu, background & 0xffu));
                    TextOutA(dc, (int)column * 8, (int)row * 16, &character, 1);
                }
            }
        } else if (ux_frame_is_valid(&frame) && frame.graphics != 0u) {
            ux_window_native *native = window->native;
            lib_size count = (lib_size)frame.graphics_width * frame.graphics_height;
            if (native != LIB_NULL && count <= UX_GRAPHICS_MAX_PIXELS &&
                native->pixel_capacity < count) {
                lib_u32 *pixels = realloc(native->pixels, count * sizeof(*pixels));
                if (pixels != LIB_NULL) { native->pixels = pixels; native->pixel_capacity = count; }
            }
            if (native != LIB_NULL && native->pixels != LIB_NULL && native->pixel_capacity >= count) {
                BITMAPINFO info = { 0 }; lib_u32 y, x;
                info.bmiHeader.biSize = sizeof(info.bmiHeader);
                info.bmiHeader.biWidth = frame.graphics_width;
                info.bmiHeader.biHeight = -(LONG)frame.graphics_height;
                info.bmiHeader.biPlanes = 1; info.bmiHeader.biBitCount = 32;
                info.bmiHeader.biCompression = BI_RGB;
                for (y = 0u; y < frame.graphics_height; ++y)
                    for (x = 0u; x < frame.graphics_width; ++x)
                        native->pixels[(lib_size)y * frame.graphics_width + x] =
                            frame.graphics_palette[frame.graphics_pixels[(lib_size)y * frame.graphics_stride + x]];
                StretchDIBits(dc, 0, 0, paint.rcPaint.right - paint.rcPaint.left,
                    paint.rcPaint.bottom - paint.rcPaint.top, 0, 0, frame.graphics_width,
                    frame.graphics_height, native->pixels, &info, DIB_RGB_COLORS, SRCCOPY);
            }
        }
        EndPaint(hwnd, &paint);
        return 0;
    }
    if (message == WM_KEYDOWN || message == WM_KEYUP || message == WM_SYSKEYDOWN ||
        message == WM_SYSKEYUP) {
        ux_input_event event;
        lib_u16 scan_code = (lib_u16)((lparam >> 16) & 0xffu);
        if ((lparam & 0x01000000L) != 0) scan_code |= 0x0100u;
        if (ux_input_make_key(&event, window, scan_code,
                (lib_u32)wparam, ux_window_modifiers(),
                (message == WM_KEYDOWN || message == WM_SYSKEYDOWN) ? LIB_TRUE : LIB_FALSE) ==
            LIB_STATUS_OK) (void)ux_window_submit_input(window, &event);
        return 0;
    }
    if (message == WM_CHAR || message == WM_SYSCHAR) {
        ux_input_event event;
        if (ux_input_make_text(&event, window, (lib_u32)wparam) == LIB_STATUS_OK)
            (void)ux_window_submit_input(window, &event);
        return 0;
    }
    return DefWindowProcA(hwnd, message, wparam, lparam);
}

static lib_bool ux_window_process_controls(ux_window *window,
    ux_window_native *native)
{
    ux_control_message message;
    lib_bool has_message;

    for (;;) {
        if (ux_window_take_control(window, &message, &has_message) !=
                LIB_STATUS_OK || has_message == LIB_FALSE)
            return LIB_TRUE;
        switch (message.kind) {
        case UX_CONTROL_STOP:
            /* Stop is a FIFO cut-off: the remaining queue and frame are
             * deliberately abandoned as this worker leaves its instance. */
            return LIB_FALSE;
        case UX_CONTROL_SET_TITLE:
            SetWindowTextA(native->hwnd, message.value.title);
            break;
        case UX_CONTROL_SET_MOUSE_ENABLED:
            native->mouse_enabled = message.value.mouse_enabled;
            if (native->mouse_enabled == LIB_FALSE) ReleaseCapture();
            break;
        case UX_CONTROL_RELEASE_MOUSE:
            ReleaseCapture();
            break;
        default:
            break;
        }
    }
}

static DWORD WINAPI ux_window_worker(void *context)
{
    ux_window *window = context;
    ux_window_native *native = window->native;
    WNDCLASSA klass = { 0 };
    MSG message;
    HANDLE waits[1] = { native->wake };
    lib_bool running = LIB_TRUE;
    klass.lpfnWndProc = ux_window_proc; klass.hInstance = GetModuleHandleA(NULL);
    klass.hCursor = LoadCursorA(NULL, IDC_ARROW); klass.lpszClassName = "SoftPCUxWindow";
    if (RegisterClassA(&klass) == 0 && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        SetEvent(native->ready); return 0u;
    }
    native->hwnd = CreateWindowExA(0, klass.lpszClassName, "SoftPC",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 680, 560,
        NULL, NULL, klass.hInstance, window);
    SetEvent(native->ready);
    if (native->hwnd == NULL) return 0u;
    ShowWindow(native->hwnd, SW_SHOW);
    while (running != LIB_FALSE) {
        DWORD wait = MsgWaitForMultipleObjects(1u, waits, FALSE, INFINITE, QS_ALLINPUT);
        if (wait == WAIT_OBJECT_0) {
            if (ux_window_process_controls(window, native) == LIB_FALSE)
                break;
            InvalidateRect(native->hwnd, NULL, FALSE);
        }
        while (PeekMessageA(&message, NULL, 0u, 0u, PM_REMOVE)) {
            if (message.message == WM_QUIT) { running = LIB_FALSE; break; }
            TranslateMessage(&message); DispatchMessageA(&message);
        }
    }
    DestroyWindow(native->hwnd); native->hwnd = NULL; return 0u;
}

lib_status ux_window_native_start(ux_window *window)
{
    ux_window_native *native;
    if (window == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    native = calloc(1u, sizeof(*native)); if (native == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    native->wake = CreateEventA(NULL, FALSE, FALSE, NULL);
    native->ready = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (native->wake == NULL || native->ready == NULL) {
        if (native->wake != NULL) CloseHandle(native->wake);
        if (native->ready != NULL) CloseHandle(native->ready);
        free(native);
        return LIB_STATUS_NO_MEMORY;
    }
    window->native = native;
    native->thread = CreateThread(NULL, 0u, ux_window_worker, window, 0u, NULL);
    if (native->thread == NULL || WaitForSingleObject(native->ready, INFINITE) != WAIT_OBJECT_0 || native->hwnd == NULL) { ux_window_native_stop(window); return LIB_STATUS_IO_ERROR; }
    (void)SetEvent(native->wake); /* consume the create-time title command */
    return LIB_STATUS_OK;
}
void ux_window_native_stop(ux_window *window)
{
    ux_window_native *native = window == LIB_NULL ? LIB_NULL : window->native;
    const ux_control_message stop = { UX_CONTROL_STOP, { { 0 } } };
    if (native == LIB_NULL) return;
    (void)ux_window_push_control(window, &stop);
    if (native->thread != NULL) { WaitForSingleObject(native->thread, INFINITE); CloseHandle(native->thread); }
    CloseHandle(native->ready); CloseHandle(native->wake); free(native->pixels); window->native = LIB_NULL; free(native);
}
void ux_window_native_signal(ux_window *window)
{ if (window != LIB_NULL && window->native != LIB_NULL) SetEvent(window->native->wake); }
#endif
