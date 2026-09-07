#include "lib/ux-window/internal.h"

#ifdef _WIN32
#include <windows.h>

struct ux_window_native { HANDLE wake, stop, ready, thread; HWND hwnd; };

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
    if (message == WM_KEYDOWN || message == WM_KEYUP || message == WM_SYSKEYDOWN ||
        message == WM_SYSKEYUP) {
        ux_input_event event;
        if (ux_input_make_key(&event, window, (lib_u16)((lparam >> 16) & 0xffu),
                (lib_u32)wparam, ux_window_modifiers(),
                (message == WM_KEYDOWN || message == WM_SYSKEYDOWN) ? LIB_TRUE : LIB_FALSE) ==
            LIB_STATUS_OK) (void)ux_window_submit_input(window, &event);
        return 0;
    }
    return DefWindowProcA(hwnd, message, wparam, lparam);
}

static DWORD WINAPI ux_window_worker(void *context)
{
    ux_window *window = context;
    ux_window_native *native = window->native;
    WNDCLASSA klass = { 0 };
    MSG message;
    HANDLE waits[2] = { native->stop, native->wake };
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
    for (;;) {
        DWORD wait = MsgWaitForMultipleObjects(2u, waits, FALSE, INFINITE, QS_ALLINPUT);
        if (wait == WAIT_OBJECT_0) break;
        if (wait == WAIT_OBJECT_0 + 1u) {
            ux_frame frame; lib_u32 generation; char title[UX_WINDOW_TITLE_CAPACITY];
            lib_bool mouse_enabled, release_mouse;
            if (ux_window_capture_state(window, &frame, &generation, title,
                    &mouse_enabled, &release_mouse) == LIB_STATUS_OK) {
                (void)frame; (void)generation; (void)mouse_enabled; (void)release_mouse;
                SetWindowTextA(native->hwnd, title);
            }
        }
        while (PeekMessageA(&message, NULL, 0u, 0u, PM_REMOVE)) {
            if (message.message == WM_QUIT) { SetEvent(native->stop); break; }
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
    native->stop = CreateEventA(NULL, TRUE, FALSE, NULL);
    native->ready = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (native->wake == NULL || native->stop == NULL || native->ready == NULL) { free(native); return LIB_STATUS_NO_MEMORY; }
    window->native = native;
    native->thread = CreateThread(NULL, 0u, ux_window_worker, window, 0u, NULL);
    if (native->thread == NULL || WaitForSingleObject(native->ready, INFINITE) != WAIT_OBJECT_0 || native->hwnd == NULL) { ux_window_native_stop(window); return LIB_STATUS_IO_ERROR; }
    return LIB_STATUS_OK;
}
void ux_window_native_stop(ux_window *window)
{
    ux_window_native *native = window == LIB_NULL ? LIB_NULL : window->native;
    if (native == LIB_NULL) return; SetEvent(native->stop);
    if (native->thread != NULL) { WaitForSingleObject(native->thread, INFINITE); CloseHandle(native->thread); }
    CloseHandle(native->ready); CloseHandle(native->stop); CloseHandle(native->wake); window->native = LIB_NULL; free(native);
}
void ux_window_native_signal(ux_window *window)
{ if (window != LIB_NULL && window->native != LIB_NULL) SetEvent(window->native->wake); }
#endif
