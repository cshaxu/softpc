#include "runtime.h"
#include "win32_window.h"
#include "softpc_test_cleanup.h"

#include <assert.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

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

static HWND softpc_window_smoke_find(void)
{
    HWND window = FindWindowA("SoftPCStandaloneWindow", "SoftPC VM");
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
        /* A standalone fixed machine has no resizable white sizing frame. */
        assert((style & WS_THICKFRAME) == 0);
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
        COLORREF initial;
        int blinked = 0;
        HDC dc;
        if (cursor_size == 0u || cursor_size > 100u) cursor_size = 100u;
        GetClientRect(window, &client);
        cell_height = (client.bottom - client.top) / 25;
        cursor_height = (int)((cell_height * cursor_size + 99u) / 100u);
        cursor_x = frame.cursor_column * (client.right - client.left) / 80;
        cursor_y = (frame.cursor_row + 1) * cell_height - cursor_height;
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
    assert(PostMessageA(window, WM_CLOSE, 0u, 0));
    assert(WaitForSingleObject(thread, 5000u) == WAIT_OBJECT_0);
    assert(context.result == SOFTPC_VM_FRONTEND_STOPPED);
    assert(softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_STOPPED);
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
