#include "runtime.h"
#include "win32_window.h"
#include "softpc_test_cleanup.h"

#include <assert.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

extern uint32_t softpc_window_test_dib_pixel(COLORREF colour);

typedef struct softpc_window_smoke_context {
    softpc_runtime *runtime;
    int result;
} softpc_window_smoke_context;

static DWORD WINAPI softpc_window_smoke_run(void *opaque)
{
    softpc_window_smoke_context *context =
        (softpc_window_smoke_context *)opaque;
    context->result = softpc_vm_run_window(context->runtime);
    return 0u;
}

static int softpc_window_wait_for_runtime(softpc_runtime *runtime,
    softpc_runtime_state expected)
{
    DWORD deadline = GetTickCount() + 5000u;
    do {
        if (softpc_runtime_get_state(runtime) == expected) return 1;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

static HWND softpc_window_smoke_find(void)
{
    HWND window = FindWindowA("SoftPCStandaloneWindow", NULL);
    DWORD process_id = 0u;
    if (window == NULL) return NULL;
    (void)GetWindowThreadProcessId(window, &process_id);
    return process_id == GetCurrentProcessId() ? window : NULL;
}

int main(void)
{
    const char *path = "softpc-win32-window-smoke.img";
    unsigned char sector[512] = { 0 };
    FILE *file;
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_WINDOW };
    softpc_machine *machine = NULL;
    softpc_runtime *runtime = NULL;
    softpc_window_smoke_context context = { NULL, SOFTPC_VM_FRONTEND_ERROR };
    HANDLE thread;
    HWND window = NULL;
    DWORD deadline;
    DWORD window_thread;
    DWORD key_queued_at;
    softpc_runtime_frame frame = { 0 };
    uint8_t ready = 0u;
    uint8_t marker = 0u;
    static const unsigned char program[] = {
        0xfau, 0x31u, 0xc0u, 0x8eu, 0xd8u,
        0xc6u, 0x06u, 0x01u, 0x05u, 0x55u,
        /* IVT[9] = CS:7c21; IRQ1 records delivery and acknowledges PIC. */
        0xb8u, 0x21u, 0x7cu, 0xa3u, 0x24u, 0x00u,
        0x0eu, 0x58u, 0xa3u, 0x26u, 0x00u,
        0xfbu, 0x90u, 0xf4u,
        0xebu, 0xfdu,
        0xc6u, 0x06u, 0x00u, 0x05u, 0xa5u,
        0xb0u, 0x20u, 0xe6u, 0x20u, 0xcfu
    };

    /* COLORREF is 0x00bbggrr while a 32-bit BI_RGB DIB stores a DWORD as
       0x00rrggbb.  Blue Setup text backgrounds previously reached this DIB
       with the COLORREF packing unchanged and therefore painted red. */
    assert(softpc_window_test_dib_pixel(RGB(0, 0, 168)) == 0x000000a8u);
    assert(softpc_window_test_dib_pixel(RGB(168, 0, 0)) == 0x00a80000u);

    options.media_mode = SOFTPC_MEDIA_OVERLAY;
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);

    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_runtime_create(machine, &runtime));
    assert(softpc_runtime_start(runtime));
    context.runtime = runtime;
    thread = CreateThread(NULL, 0u, softpc_window_smoke_run, &context, 0u,
        NULL);
    assert(thread != NULL);
    deadline = GetTickCount() + 5000u;
    do {
        window = softpc_window_smoke_find();
        if (window != NULL) break;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    assert(window != NULL);
    window_thread = GetWindowThreadProcessId(window, NULL);
    assert(window_thread != 0u);
    {
        RECT client;
        LONG style = GetWindowLongA(window, GWL_STYLE);
        /* The endpoint requests a literal 80x25 CP437 8x16 client surface.
           A remote desktop may clamp an overlarge fixed window to its work
           area; presentation then scales into that valid client rectangle.
           Do not mistake this host constraint for a guest mode change. */
        deadline = GetTickCount() + 5000u;
        do {
            GetClientRect(window, &client);
            if (client.right > client.left && client.bottom > client.top) break;
            Sleep(10u);
        } while ((LONG)(GetTickCount() - deadline) < 0);
        assert(client.right > client.left && client.bottom > client.top);
        /* The window is an ordinary NXVM-sized shell; presentation scales
           inside its resizable client area instead of guest mode changes
           resizing the native frame. */
        assert((style & WS_THICKFRAME) != 0);
    }
    /* A click claims the relative guest pointer.  It must be a real Win32
       capture rather than merely setting a frontend flag, and a focus loss
       must always release it so the host desktop is never trapped. */
    assert(PostMessageA(window, WM_LBUTTONDOWN, MK_LBUTTON,
        MAKELPARAM(20, 20)));
    deadline = GetTickCount() + 1000u;
    do {
        GUITHREADINFO gui = { sizeof(gui) };
        if (GetGUIThreadInfo(window_thread, &gui) && gui.hwndCapture == window)
            break;
        Sleep(1u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    {
        GUITHREADINFO gui = { sizeof(gui) };
        assert(GetGUIThreadInfo(window_thread, &gui));
        assert(gui.hwndCapture == window);
    }
    assert(PostMessageA(window, WM_KILLFOCUS, 0u, 0));
    deadline = GetTickCount() + 1000u;
    do {
        GUITHREADINFO gui = { sizeof(gui) };
        if (GetGUIThreadInfo(window_thread, &gui) && gui.hwndCapture != window)
            break;
        Sleep(1u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    {
        GUITHREADINFO gui = { sizeof(gui) };
        assert(GetGUIThreadInfo(window_thread, &gui));
        assert(gui.hwndCapture != window);
    }
    /* Do not merely assert that PostMessage succeeds.  This guest stops at
       HLT until the exact Win32-keyboard-normalizer -> runtime queue ->
       original 8042/PIC/CCPU path delivers IRQ1. */
    deadline = GetTickCount() + 5000u;
    do {
        assert(softpc_machine_read_physical(machine, 0x501u, &ready,
            sizeof(ready)) == SOFTPC_MACHINE_OK);
        if (ready == 0x55u) break;
        Sleep(1u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    assert(ready == 0x55u);
    /* The fixture is now halted, so this client-pixel transition can only be
       the standalone window's host-owned cursor blink; no later guest frame
       is needed to make the copied text cursor alternate. */
    deadline = GetTickCount() + 5000u;
    do {
        /* The mailbox is deliberately non-blocking: the frontend thread may
           own its short copy window, so wait for a complete snapshot instead
           of turning that expected contention into a test failure. */
        if (softpc_runtime_copy_frame(runtime, &frame) &&
            frame.valid != 0u && frame.graphics == 0u &&
            frame.cursor_column >= 0 && frame.cursor_row >= 0 &&
            frame.cursor_column < 80 && frame.cursor_row < 25) break;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    assert(frame.valid != 0u && frame.graphics == 0u);
    {
        RECT client;
        uint32_t cursor_size = frame.cursor_size;
        int cell_height;
        int cursor_height;
        int cursor_x;
        int cursor_y;
        int display_width;
        int display_height;
        int display_left;
        int display_top;
        COLORREF initial;
        int blinked = 0;
        HDC dc;
        if (cursor_size == 0u || cursor_size > 100u) cursor_size = 100u;
        GetClientRect(window, &client);
        display_width = client.right - client.left;
        display_height = client.bottom - client.top;
        display_left = 0;
        display_top = 0;
        cell_height = display_height / 25;
        cursor_height = (int)((cell_height * cursor_size + 99u) / 100u);
        cursor_x = display_left + frame.cursor_column * display_width / 80;
        cursor_y = display_top + (frame.cursor_row + 1) * cell_height - cursor_height;
        dc = GetDC(window);
        assert(dc != NULL);
        initial = GetPixel(dc, cursor_x, cursor_y);
        deadline = GetTickCount() + 1500u;
        do {
            Sleep(25u);
            if (GetPixel(dc, cursor_x, cursor_y) != initial) {
                blinked = 1;
                break;
            }
        } while ((LONG)(GetTickCount() - deadline) < 0);
        ReleaseDC(window, dc);
        assert(blinked);
    }
    key_queued_at = GetTickCount();
    assert(PostMessageA(window, WM_KEYDOWN, 'S', 0x001f0001L));
    deadline = key_queued_at + 250u;
    do {
        assert(softpc_machine_read_physical(machine, 0x500u, &marker,
            sizeof(marker)) == SOFTPC_MACHINE_OK);
        if (marker == 0xa5u) break;
        Sleep(1u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    assert(marker == 0xa5u);
    assert((DWORD)(GetTickCount() - key_queued_at) <= 250u);
    assert(PostMessageA(window, WM_KEYUP, 'S', 0xc01f0001L));
    /* Remote Desktop and IME paths can supply a character without the
       physical scan code carried by WM_KEYDOWN.  That must still traverse
       the same normalizer, runtime queue, original 8042 and IRQ1 path; it
       must never be treated as a frontend stop request. */
    marker = 0u;
    assert(softpc_machine_write_physical(machine, 0x500u, &marker,
        sizeof(marker)) == SOFTPC_MACHINE_OK);
    key_queued_at = GetTickCount();
    assert(PostMessageA(window, WM_CHAR, 's', 0));
    deadline = key_queued_at + 250u;
    do {
        assert(softpc_machine_read_physical(machine, 0x500u, &marker,
            sizeof(marker)) == SOFTPC_MACHINE_OK);
        if (marker == 0xa5u) break;
        Sleep(1u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    assert(marker == 0xa5u);
    assert((DWORD)(GetTickCount() - key_queued_at) <= 250u);
    assert(softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_RUNNING);
    /* Esc belongs to the guest.  The standalone shell reserves exactly
       Ctrl+Alt+P (pause) and Ctrl+Alt+M (release pointer); it must not turn
       an ordinary guest key into a host machine-stop command. */
    marker = 0u;
    assert(softpc_machine_write_physical(machine, 0x500u, &marker,
        sizeof(marker)) == SOFTPC_MACHINE_OK);
    assert(PostMessageA(window, WM_KEYDOWN, VK_ESCAPE, 0x00010001L));
    /* Key-number 110 delivery is covered deterministically by the shared
       keyboard-normalizer smoke. This window-level check owns only the
       frontend policy: Esc must not request a host stop. */
    Sleep(50u);
    assert(softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_RUNNING);
    assert(PostMessageA(window, WM_KEYUP, VK_ESCAPE, 0xc0010001L));
    /* Pausing must leave the window visible but make its client surface a
       host-only view: it cannot recapture the pointer or queue a key for a
       later resume. */
    assert(softpc_runtime_pause(runtime));
    assert(softpc_window_wait_for_runtime(runtime, SOFTPC_RUNTIME_PAUSED));
    deadline = GetTickCount() + 1000u;
    do {
        char title[64];
        GetWindowTextA(window, title, sizeof(title));
        if (strcmp(title, "Insignia SoftPC (Paused)") == 0) break;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    {
        char title[64];
        GUITHREADINFO gui = { sizeof(gui) };
        GetWindowTextA(window, title, sizeof(title));
        assert(strcmp(title, "Insignia SoftPC (Paused)") == 0);
        assert(PostMessageA(window, WM_LBUTTONDOWN, MK_LBUTTON,
            MAKELPARAM(20, 20)));
        Sleep(25u);
        assert(GetGUIThreadInfo(window_thread, &gui));
        assert(gui.hwndCapture != window);
    }
    marker = 0u;
    assert(softpc_machine_write_physical(machine, 0x500u, &marker,
        sizeof(marker)) == SOFTPC_MACHINE_OK);
    assert(PostMessageA(window, WM_KEYDOWN, 'S', 0x001f0001L));
    Sleep(50u);
    assert(softpc_machine_read_physical(machine, 0x500u, &marker,
        sizeof(marker)) == SOFTPC_MACHINE_OK);
    assert(marker == 0u);
    assert(softpc_runtime_resume(runtime));
    assert(softpc_window_wait_for_runtime(runtime, SOFTPC_RUNTIME_RUNNING));
    deadline = GetTickCount() + 1000u;
    do {
        char title[64];
        GetWindowTextA(window, title, sizeof(title));
        if (strcmp(title, "Insignia SoftPC (Running)") == 0) break;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    {
        char title[64];
        GetWindowTextA(window, title, sizeof(title));
        assert(strcmp(title, "Insignia SoftPC (Running)") == 0);
    }
    assert(PostMessageA(window, WM_CLOSE, 0u, 0));
    assert(WaitForSingleObject(thread, 5000u) == WAIT_OBJECT_0);
    assert(context.result == SOFTPC_VM_FRONTEND_PAUSED);
    assert(softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_PAUSED);
    assert(softpc_runtime_resume(runtime));
    assert(softpc_window_wait_for_runtime(runtime, SOFTPC_RUNTIME_RUNNING));
    assert(softpc_runtime_stop(runtime));
    CloseHandle(thread);
    softpc_runtime_destroy(runtime);
    softpc_machine_destroy(machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
#else
int main(void)
{
    return 0;
}
#endif
