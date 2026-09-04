#include "runtime.h"
#include "test_cleanup.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

static int app_runtime_wait(app_runtime *runtime,
    app_runtime_state expected)
{
    DWORD deadline = GetTickCount() + 5000u;
    do {
        if (app_runtime_get_state(runtime) == expected) return 1;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

int main(void)
{
    const char *path = "softpc-runtime-smoke.img";
    unsigned char sector[512] = { 0 };
    FILE *file;
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;
    app_runtime *runtime = NULL;
    app_runtime_frame *frame;

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
    assert(app_runtime_create(machine, &runtime));
    assert(app_runtime_frame_event(runtime) != NULL);
    assert(app_runtime_start(runtime));
    assert(WaitForSingleObject((HANDLE)app_runtime_frame_event(runtime),
        5000u) == WAIT_OBJECT_0);
    Sleep(150u);
    frame = (app_runtime_frame *)calloc(1u, sizeof(*frame));
    assert(frame != NULL);
    {
        DWORD deadline = GetTickCount() + 5000u;
        int cursor_seen = 0;
        do {
            /* A copied presentation frame is deliberately non-blocking.
               The executor may own its frame lock while publishing the first
               original renderer update, so retry rather than turning that
               defined snapshot miss into a timing-dependent test failure. */
            if (app_runtime_copy_frame(runtime, frame) &&
                frame->graphics == 0u && frame->cursor_column >= 0 &&
                frame->cursor_column < SOFTPC_RUNTIME_TEXT_COLUMNS &&
                frame->cursor_row >= 0 &&
                frame->cursor_row < SOFTPC_RUNTIME_TEXT_ROWS &&
                frame->cursor_size > 0u && frame->cursor_size <= 100u) {
                cursor_seen = 1;
                break;
            }
            Sleep(10u);
        } while ((LONG)(GetTickCount() - deadline) < 0);
        /* Original nt_graph's Console cursor endpoint must reach the copied
           frame.  Both outer frontends consume this value without reading a
           controller register or a guest-memory pointer. */
        assert(cursor_seen);
    }
    assert(app_runtime_published_frame_sequence(runtime) == frame->sequence);
    assert(frame->sequence != 0u);
    assert(app_runtime_pause(runtime));
    assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_PAUSED));
    assert(app_runtime_set_floppy(runtime, NULL));
    assert(app_runtime_resume(runtime));
    assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_RUNNING));
    assert(app_runtime_stop(runtime));
    assert(app_runtime_get_state(runtime) == SOFTPC_RUNTIME_STOPPED);
    assert(app_runtime_start(runtime));
    assert(app_runtime_stop(runtime));
    assert(app_runtime_set_floppy(runtime, NULL));
    free(frame);
    app_runtime_destroy(runtime);
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
