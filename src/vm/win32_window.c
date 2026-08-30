#include "win32_window.h"

#ifdef _WIN32
#include <windows.h>

#define SOFTPC_TEXT_COLUMNS 80
#define SOFTPC_TEXT_ROWS 25
#define SOFTPC_TEXT_ADDRESS 0xb8000u
#define SOFTPC_TIMER_ID 1u
#define SOFTPC_RUN_SLICE 50000u

static softpc_machine *softpc_window_machine;
static HFONT softpc_window_font;

static void softpc_window_paint(HDC dc)
{
    unsigned char text[SOFTPC_TEXT_COLUMNS * SOFTPC_TEXT_ROWS * 2u];
    int row;
    if (softpc_machine_read_physical(softpc_window_machine,
            SOFTPC_TEXT_ADDRESS, text, sizeof(text)) != SOFTPC_MACHINE_OK) return;
    SelectObject(dc, softpc_window_font);
    SetBkMode(dc, OPAQUE);
    SetBkColor(dc, RGB(0, 0, 0));
    SetTextColor(dc, RGB(192, 192, 192));
    for (row = 0; row < SOFTPC_TEXT_ROWS; ++row) {
        char line[SOFTPC_TEXT_COLUMNS + 1];
        int column;
        for (column = 0; column < SOFTPC_TEXT_COLUMNS; ++column) {
            unsigned char character = text[(row * SOFTPC_TEXT_COLUMNS + column) * 2u];
            line[column] = character >= 0x20u && character < 0x7fu ?
                (char)character : ' ';
        }
        line[SOFTPC_TEXT_COLUMNS] = '\0';
        TextOutA(dc, 8, 8 + row * 16, line, SOFTPC_TEXT_COLUMNS);
    }
}

static LRESULT CALLBACK softpc_window_proc(HWND window, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_TIMER:
        if (wparam == SOFTPC_TIMER_ID) {
            (void)softpc_machine_run(softpc_window_machine, SOFTPC_RUN_SLICE);
            InvalidateRect(window, NULL, FALSE);
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
    softpc_window_font = CreateFontA(16, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, "Consolas");
    window = CreateWindowExA(0, window_class.lpszClassName, "SoftPC VM",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 680, 460,
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
    return 0;
}

#else
int softpc_vm_run_window(softpc_machine *machine)
{
    (void)machine;
    return 1;
}
#endif
