#include "runtime.h"
#include "win32_window.h"

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

    options.media_mode = SOFTPC_MEDIA_OVERLAY;
    sector[0] = 0xebu;
    sector[1] = 0xfeu;
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
        /* The endpoint begins with a literal 80x25 CP437 8x16 surface: no
           frontend border is permitted inside the guest client rectangle. */
        GetClientRect(window, &client);
        assert(client.right - client.left == 640);
        assert(client.bottom - client.top == 400);
    }
    assert(PostMessageA(window, WM_CLOSE, 0u, 0));
    assert(WaitForSingleObject(thread, 5000u) == WAIT_OBJECT_0);
    assert(context.result == SOFTPC_VM_FRONTEND_STOPPED);
    assert(softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_STOPPED);
    CloseHandle(thread);
    softpc_runtime_destroy(runtime);
    softpc_machine_destroy(machine);
    assert(remove(path) == 0);
    return 0;
}
#else
int main(void)
{
    return 0;
}
#endif
