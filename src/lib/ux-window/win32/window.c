#include "lib/ux-window/internal.h"

#ifdef _WIN32
#include <windows.h>

struct ux_window_native {
    HANDLE wake;
    HANDLE ready;
    HANDLE thread;
    HWND hwnd;
    ux_frame displayed_frame;
    lib_u32 displayed_generation;
    lib_u32 source_width;
    lib_u32 source_height;
    lib_u32 client_surface_width;
    lib_u32 client_surface_height;
    lib_u32 *pixels;
    lib_size pixel_capacity;
    int mouse_x;
    int mouse_y;
    lib_bool mouse_valid;
    lib_bool mouse_enabled;
    lib_bool modifier_down[6u];
    lib_u16 recovered_virtual_key;
    lib_u16 pending_high_surrogate;
};

#define UX_WINDOW_TEXT_CELL_WIDTH 8u
#define UX_WINDOW_TEXT_CELL_HEIGHT 16u

static int ux_window_modifier_slot(lib_u32 virtual_key, lib_u16 scan_code)
{
    if (virtual_key == VK_CONTROL) return (scan_code & 0x0100u) != 0u ? 1 : 0;
    if (virtual_key == VK_MENU) return (scan_code & 0x0100u) != 0u ? 3 : 2;
    if (virtual_key == VK_SHIFT) return (scan_code & 0xffu) == 0x36u ? 5 : 4;
    return -1;
}

static lib_u32 ux_window_update_modifiers(ux_window_native *native,
    lib_u32 virtual_key, lib_u16 scan_code, lib_bool pressed)
{
    lib_u32 result = 0u;
    int slot = ux_window_modifier_slot(virtual_key, scan_code);

    if (slot >= 0) native->modifier_down[slot] = pressed != LIB_FALSE;
    if (native->modifier_down[0] != LIB_FALSE ||
        native->modifier_down[1] != LIB_FALSE) result |= UX_MODIFIER_CONTROL;
    if (native->modifier_down[2] != LIB_FALSE ||
        native->modifier_down[3] != LIB_FALSE) result |= UX_MODIFIER_ALT;
    if (native->modifier_down[4] != LIB_FALSE ||
        native->modifier_down[5] != LIB_FALSE) result |= UX_MODIFIER_SHIFT;
    return result;
}

static void ux_window_note_recovered_key(ux_window_native *native,
    lib_u32 virtual_key, lib_bool released)
{
    if (native == LIB_NULL || virtual_key == 0u) return;
    if (released != LIB_FALSE) {
        if (native->recovered_virtual_key == (lib_u16)virtual_key)
            native->recovered_virtual_key = 0u;
    } else if (MapVirtualKeyExW((UINT)virtual_key, MAPVK_VK_TO_VSC_EX,
            GetKeyboardLayout(0u)) != 0u) {
        native->recovered_virtual_key = (lib_u16)virtual_key;
    }
}

static lib_bool ux_window_consume_duplicate_text(ux_window_native *native,
    lib_u32 scalar)
{
    SHORT mapped;
    lib_bool duplicate;

    if (native == LIB_NULL || scalar == 0u || scalar > 0xffffu)
        return LIB_FALSE;
    if (scalar >= 0xd800u && scalar <= 0xdfffu) {
        native->recovered_virtual_key = 0u;
        return LIB_FALSE;
    }
    if (native->recovered_virtual_key == 0u) return LIB_FALSE;
    mapped = VkKeyScanExW((WCHAR)scalar, GetKeyboardLayout(0u));
    duplicate = mapped != -1 && (lib_u16)(mapped & 0xffu) ==
        native->recovered_virtual_key;
    native->recovered_virtual_key = 0u;
    return duplicate;
}

static lib_bool ux_window_take_text_scalar(ux_window_native *native,
    lib_u16 code_unit, lib_u32 *out_scalar)
{
    if (native == LIB_NULL || out_scalar == LIB_NULL) return LIB_FALSE;
    if (code_unit >= 0xd800u && code_unit <= 0xdbffu) {
        native->pending_high_surrogate = code_unit;
        return LIB_FALSE;
    }
    if (code_unit >= 0xdc00u && code_unit <= 0xdfffu) {
        if (native->pending_high_surrogate == 0u) return LIB_FALSE;
        *out_scalar = 0x10000u +
            ((lib_u32)native->pending_high_surrogate - 0xd800u) * 0x400u +
            ((lib_u32)code_unit - 0xdc00u);
        native->pending_high_surrogate = 0u;
        return LIB_TRUE;
    }
    native->pending_high_surrogate = 0u;
    *out_scalar = code_unit;
    return code_unit != 0u;
}

static void ux_window_native_release_mouse(ux_window_native *native)
{
    if (native == LIB_NULL) return;
    ClipCursor(NULL);
    ReleaseCapture();
    SetCursor(LoadCursorA(NULL, IDC_ARROW));
    native->mouse_valid = LIB_FALSE;
}

static void ux_window_native_capture_mouse(ux_window_native *native,
    HWND hwnd, LPARAM position)
{
    RECT client;
    POINT upper_left;
    POINT lower_right;
    RECT bounds;

    if (native == LIB_NULL || hwnd == NULL ||
        native->mouse_enabled == LIB_FALSE) return;
    SetFocus(hwnd);
    SetCapture(hwnd);
    GetClientRect(hwnd, &client);
    upper_left.x = client.left;
    upper_left.y = client.top;
    lower_right.x = client.right;
    lower_right.y = client.bottom;
    if (ClientToScreen(hwnd, &upper_left) && ClientToScreen(hwnd, &lower_right)) {
        bounds.left = upper_left.x;
        bounds.top = upper_left.y;
        bounds.right = lower_right.x;
        bounds.bottom = lower_right.y;
        (void)ClipCursor(&bounds);
    }
    native->mouse_x = (int)(short)LOWORD(position);
    native->mouse_y = (int)(short)HIWORD(position);
    native->mouse_valid = LIB_TRUE;
    SetCursor(NULL);
}

static void ux_window_native_mouse_move(ux_window *window, LPARAM position,
    lib_u32 buttons)
{
    ux_window_native *native = window->native;
    int x = (int)(short)LOWORD(position);
    int y = (int)(short)HIWORD(position);
    RECT client;
    ux_input_event event;

    if (native == LIB_NULL || GetCapture() != native->hwnd) return;
    if (native->mouse_valid != LIB_FALSE) {
        lib_i32 delta_x = x - native->mouse_x;
        lib_i32 delta_y = y - native->mouse_y;
        GetClientRect(native->hwnd, &client);
        if (client.right > client.left && native->source_width != 0u)
            delta_x = delta_x * (lib_i32)native->source_width /
                (client.right - client.left);
        if (client.bottom > client.top && native->source_height != 0u)
            delta_y = delta_y * (lib_i32)native->source_height /
                (client.bottom - client.top);
        if (ux_input_make_mouse(&event, window, delta_x, delta_y, 0, 0,
                0, 0, buttons, LIB_TRUE) == LIB_STATUS_OK)
            (void)ux_window_submit_input(window, &event);
    }
    native->mouse_x = x;
    native->mouse_y = y;
    native->mouse_valid = LIB_TRUE;
}

static lib_bool ux_window_native_ensure_pixels(ux_window_native *native,
    lib_u32 width, lib_u32 height)
{
    lib_size count = (lib_size)width * height;
    lib_u32 *pixels;

    if (native == LIB_NULL || width == 0u || height == 0u ||
        count > UX_GRAPHICS_MAX_PIXELS) return LIB_FALSE;
    if (native->pixel_capacity >= count) return LIB_TRUE;
    pixels = realloc(native->pixels, count * sizeof(*pixels));
    if (pixels == LIB_NULL) return LIB_FALSE;
    native->pixels = pixels;
    native->pixel_capacity = count;
    return LIB_TRUE;
}

static void ux_window_native_resize_client(ux_window_native *native)
{
    RECT outer;
    DWORD style;
    DWORD extended_style;

    if (native == LIB_NULL || native->hwnd == NULL ||
        native->source_width == 0u || native->source_height == 0u ||
        (native->client_surface_width == native->source_width &&
         native->client_surface_height == native->source_height)) return;
    SetRect(&outer, 0, 0, (int)native->source_width,
        (int)native->source_height);
    style = (DWORD)GetWindowLongPtrA(native->hwnd, GWL_STYLE);
    extended_style = (DWORD)GetWindowLongPtrA(native->hwnd, GWL_EXSTYLE);
    if (AdjustWindowRectEx(&outer, style, FALSE, extended_style)) {
        SetWindowPos(native->hwnd, NULL, 0, 0, outer.right - outer.left,
            outer.bottom - outer.top,
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        native->client_surface_width = native->source_width;
        native->client_surface_height = native->source_height;
    }
}

static void ux_window_native_constrain_sizing(ux_window_native *native,
    WPARAM edge, RECT *outer)
{
    RECT current_window;
    RECT current_client;
    int frame_width;
    int frame_height;
    int client_width;
    int client_height;
    int target_width;
    int target_height;

    if (native == LIB_NULL || native->hwnd == NULL || outer == LIB_NULL ||
        native->source_width == 0u || native->source_height == 0u) return;
    GetWindowRect(native->hwnd, &current_window);
    GetClientRect(native->hwnd, &current_client);
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
        client_height = (int)((lib_u64)client_width * native->source_height /
            native->source_width);
    } else if (edge == WMSZ_TOP || edge == WMSZ_BOTTOM) {
        client_width = (int)((lib_u64)client_height * native->source_width /
            native->source_height);
    } else if ((lib_u64)client_width * native->source_height >=
        (lib_u64)client_height * native->source_width) {
        client_height = (int)((lib_u64)client_width * native->source_height /
            native->source_width);
    } else {
        client_width = (int)((lib_u64)client_height * native->source_width /
            native->source_height);
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

static void ux_window_native_render_text(ux_window_native *native,
    const ux_frame *frame)
{
    lib_u32 row;

    for (row = 0u; row < frame->text_rows; ++row) {
        lib_u32 column;
        for (column = 0u; column < frame->text_columns; ++column) {
            lib_size cell = (lib_size)row * UX_TEXT_COLUMNS + column;
            lib_u16 attribute = frame->attributes[cell];
            const lib_u8 *font = frame->attribute_font_select != 0u &&
                (attribute & 0x08u) != 0u ? frame->secondary_font : frame->font;
            lib_u32 scan;

            for (scan = 0u; scan < UX_WINDOW_TEXT_CELL_HEIGHT; ++scan) {
                lib_u8 bits = font[(lib_size)frame->text[cell] *
                    UX_WINDOW_TEXT_CELL_HEIGHT + scan];
                lib_u32 *destination = native->pixels +
                    ((lib_size)row * UX_WINDOW_TEXT_CELL_HEIGHT + scan) *
                    native->source_width + column * UX_WINDOW_TEXT_CELL_WIDTH;
                lib_u32 bit;
                for (bit = 0u; bit < UX_WINDOW_TEXT_CELL_WIDTH; ++bit)
                    destination[bit] = frame->text_palette[
                        (bits & (0x80u >> bit)) != 0u ? attribute & 0x0fu :
                            (attribute >> 4u) & 0x0fu];
            }
        }
    }
}

static lib_bool ux_window_native_render_frame(ux_window *window,
    ux_window_native *native)
{
    ux_frame frame;
    lib_u32 generation;
    lib_u32 width;
    lib_u32 height;

    if (ux_window_capture_paint_state(window, &frame, &generation) !=
            LIB_STATUS_OK || generation == native->displayed_generation ||
        ux_frame_is_valid(&frame) == LIB_FALSE)
        return LIB_FALSE;
    width = frame.graphics != 0u ? frame.graphics_width :
        (lib_u32)frame.text_columns * UX_WINDOW_TEXT_CELL_WIDTH;
    height = frame.graphics != 0u ? frame.graphics_height :
        (lib_u32)frame.text_rows * UX_WINDOW_TEXT_CELL_HEIGHT;
    if (ux_window_native_ensure_pixels(native, width, height) == LIB_FALSE)
        return LIB_FALSE;
    native->displayed_frame = frame;
    native->displayed_generation = generation;
    native->source_width = width;
    native->source_height = height;
    if (frame.graphics != 0u) {
        lib_u32 y;
        for (y = 0u; y < height; ++y) {
            const lib_u8 *source = frame.graphics_pixels +
                (lib_size)y * frame.graphics_stride;
            lib_u32 *destination = native->pixels + (lib_size)y * width;
            lib_u32 x;
            for (x = 0u; x < width; ++x)
                destination[x] = frame.graphics_palette[source[x]];
        }
    } else {
        memset(native->pixels, 0, (lib_size)width * height *
            sizeof(*native->pixels));
        ux_window_native_render_text(native, &frame);
    }
    ux_window_native_resize_client(native);
    return LIB_TRUE;
}

static lib_bool ux_window_native_display_rect(const ux_window_native *native,
    const RECT *client, RECT *out_display)
{
    lib_i32 width;
    lib_i32 height;
    lib_i32 display_width;
    lib_i32 display_height;

    if (native == LIB_NULL || client == LIB_NULL || out_display == LIB_NULL ||
        native->source_width == 0u || native->source_height == 0u) return LIB_FALSE;
    width = client->right - client->left;
    height = client->bottom - client->top;
    if (width <= 0 || height <= 0) return LIB_FALSE;
    if ((lib_u64)width * native->source_height <=
        (lib_u64)height * native->source_width) {
        display_width = width;
        display_height = (lib_i32)((lib_u64)width * native->source_height /
            native->source_width);
    } else {
        display_height = height;
        display_width = (lib_i32)((lib_u64)height * native->source_width /
            native->source_height);
    }
    out_display->left = client->left + (width - display_width) / 2;
    out_display->top = client->top + (height - display_height) / 2;
    out_display->right = out_display->left + display_width;
    out_display->bottom = out_display->top + display_height;
    return LIB_TRUE;
}

static void ux_window_native_paint(ux_window_native *native, HDC dc)
{
    BITMAPINFO info = { 0 };
    RECT client;
    RECT display;
    const ux_frame *frame;

    if (native == LIB_NULL || native->pixels == LIB_NULL ||
        native->source_width == 0u || native->source_height == 0u) return;
    GetClientRect(native->hwnd, &client);
    if (ux_window_native_display_rect(native, &client, &display) == LIB_FALSE)
        return;
    FillRect(dc, &client, (HBRUSH)GetStockObject(BLACK_BRUSH));
    info.bmiHeader.biSize = sizeof(info.bmiHeader);
    info.bmiHeader.biWidth = (LONG)native->source_width;
    info.bmiHeader.biHeight = -(LONG)native->source_height;
    info.bmiHeader.biPlanes = 1u;
    info.bmiHeader.biBitCount = 32u;
    info.bmiHeader.biCompression = BI_RGB;
    (void)StretchDIBits(dc, display.left, display.top,
        display.right - display.left, display.bottom - display.top, 0, 0,
        native->source_width,
        native->source_height, native->pixels, &info, DIB_RGB_COLORS, SRCCOPY);
    frame = &native->displayed_frame;
    if (frame->graphics == 0u && frame->cursor_visible != 0u &&
        frame->cursor_phase != 0u && frame->cursor_column >= 0 &&
        frame->cursor_row >= 0 && frame->cursor_column < frame->text_columns &&
        frame->cursor_row < frame->text_rows) {
        RECT cursor;
        lib_u32 cursor_percent = frame->font_height != 0u &&
            frame->cursor_bottom >= frame->cursor_top ?
            (frame->cursor_bottom - frame->cursor_top + 1u) * 100u /
                frame->font_height : 100u;
        int cell_height;
        int cursor_height;
        if (cursor_percent == 0u || cursor_percent > 100u) cursor_percent = 100u;
        cursor.left = display.left + frame->cursor_column *
            (display.right - display.left) / frame->text_columns;
        cursor.right = display.left + (frame->cursor_column + 1) *
            (display.right - display.left) / frame->text_columns;
        cell_height = (display.bottom - display.top) / frame->text_rows;
        cursor_height = (int)((cell_height * cursor_percent + 99u) / 100u);
        if (cursor_height > cell_height) cursor_height = cell_height;
        cursor.bottom = display.top + (frame->cursor_row + 1) *
            (display.bottom - display.top) / frame->text_rows;
        cursor.top = cursor.bottom - cursor_height;
        if (cursor.right > cursor.left && cursor.bottom > cursor.top)
            InvertRect(dc, &cursor);
    }
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
    if (message == WM_KILLFOCUS) {
        ux_window_native_release_mouse(window->native);
        return 0;
    }
    if (message == WM_DESTROY) {
        /* Destruction can bypass focus loss while capture is active.  Native
         * capture/clip state belongs to this Window instance and must not
         * survive its terminal control message. */
        ux_window_native_release_mouse(window->native);
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, 0);
        return 0;
    }
    if (message == WM_LBUTTONDOWN) {
        ux_window_native_capture_mouse(window->native, hwnd, lparam);
        ux_window_native_mouse_move(window, lparam, 0x01u);
        return 0;
    }
    if (message == WM_RBUTTONDOWN) {
        ux_window_native_capture_mouse(window->native, hwnd, lparam);
        ux_window_native_mouse_move(window, lparam, 0x02u);
        return 0;
    }
    if (message == WM_MOUSEMOVE) {
        lib_u32 buttons = 0u;
        if ((wparam & MK_LBUTTON) != 0u) buttons |= 0x01u;
        if ((wparam & MK_RBUTTON) != 0u) buttons |= 0x02u;
        ux_window_native_mouse_move(window, lparam, buttons);
        return 0;
    }
    if (message == WM_LBUTTONUP) {
        ux_window_native_mouse_move(window, lparam, 0u);
        return 0;
    }
    if (message == WM_RBUTTONUP) {
        ux_window_native_mouse_move(window, lparam, 0u);
        return 0;
    }
    if (message == WM_SETCURSOR && GetCapture() == hwnd &&
        LOWORD(lparam) == HTCLIENT) {
        SetCursor(NULL);
        return TRUE;
    }
    if (message == WM_SIZING) {
        ux_window_native_constrain_sizing(window->native, wparam,
            (RECT *)lparam);
        return TRUE;
    }
    if (message == WM_NCLBUTTONDBLCLK && wparam == HTCAPTION &&
        IsZoomed(hwnd)) {
        ShowWindow(hwnd, SW_RESTORE);
        window->native->client_surface_width = 0u;
        window->native->client_surface_height = 0u;
        ux_window_native_resize_client(window->native);
        return 0;
    }
    if (message == WM_ERASEBKGND) return 1;
    if (message == WM_PAINT) {
        PAINTSTRUCT paint;
        HDC dc = BeginPaint(hwnd, &paint);
        ux_window_native_paint(window->native, dc);
        EndPaint(hwnd, &paint);
        return 0;
    }
    if (message == WM_KEYDOWN || message == WM_KEYUP || message == WM_SYSKEYDOWN ||
        message == WM_SYSKEYUP) {
        ux_input_event event;
        lib_u16 scan_code = (lib_u16)((lparam >> 16) & 0xffu);
        lib_bool pressed = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN) ?
            LIB_TRUE : LIB_FALSE;
        if ((lparam & 0x01000000L) != 0) scan_code |= 0x0100u;
        if ((scan_code & 0xffu) == 0u)
            ux_window_note_recovered_key(window->native, (lib_u32)wparam,
                pressed == LIB_FALSE);
        if (ux_input_make_key(&event, window, scan_code,
                (lib_u32)wparam, ux_window_update_modifiers(window->native,
                    (lib_u32)wparam, scan_code, pressed), pressed) ==
            LIB_STATUS_OK) (void)ux_window_submit_input(window, &event);
        return 0;
    }
    if (message == WM_CHAR || message == WM_SYSCHAR) {
        ux_input_event event;
        lib_u32 scalar;
        /* Physical keys already arrived as key events.  Only a scan-less
         * character (IME/RDP recovery) needs the text fallback path. */
        if (((lparam >> 16u) & 0xffu) == 0u &&
            ux_window_consume_duplicate_text(window->native,
                (lib_u32)wparam) == LIB_FALSE &&
            ux_window_take_text_scalar(window->native, (lib_u16)wparam,
                &scalar) != LIB_FALSE &&
            ux_input_make_text(&event, window, scalar) == LIB_STATUS_OK)
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
            if (native->mouse_enabled == LIB_FALSE)
                ux_window_native_release_mouse(native);
            break;
        case UX_CONTROL_RELEASE_MOUSE:
            ux_window_native_release_mouse(native);
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
    klass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    klass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    klass.lpszClassName = "SoftPCUxWindow";
    if (RegisterClassA(&klass) == 0 && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        SetEvent(native->ready); return 0u;
    }
    native->hwnd = CreateWindowExA(0, klass.lpszClassName, "SoftPC",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 680, 560,
        NULL, NULL, klass.hInstance, window);
    SetEvent(native->ready);
    if (native->hwnd == NULL) return 0u;
    ShowWindow(native->hwnd, SW_SHOW);
    UpdateWindow(native->hwnd);
    SetForegroundWindow(native->hwnd);
    SetFocus(native->hwnd);
    while (running != LIB_FALSE) {
        DWORD wait = MsgWaitForMultipleObjects(1u, waits, FALSE, INFINITE, QS_ALLINPUT);
        if (wait == WAIT_OBJECT_0) {
            if (ux_window_process_controls(window, native) == LIB_FALSE)
                break;
            if (ux_window_native_render_frame(window, native) != LIB_FALSE)
                InvalidateRect(native->hwnd, NULL, FALSE);
        } else if (wait == WAIT_FAILED) {
            break;
        }
        while (PeekMessageA(&message, NULL, 0u, 0u, PM_REMOVE)) {
            if (message.message == WM_QUIT) { running = LIB_FALSE; break; }
            TranslateMessage(&message); DispatchMessageA(&message);
        }
    }
    if (native->hwnd != NULL) {
        ux_window_native_release_mouse(native);
        DestroyWindow(native->hwnd);
        native->hwnd = NULL;
    }
    return 0u;
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
