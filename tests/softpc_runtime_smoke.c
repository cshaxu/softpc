#include "runtime.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

static int softpc_runtime_wait(softpc_runtime *runtime,
    softpc_runtime_state expected)
{
    DWORD deadline = GetTickCount() + 5000u;
    do {
        if (softpc_runtime_get_state(runtime) == expected) return 1;
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
    softpc_runtime *runtime = NULL;
    softpc_runtime_frame *frame;

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
    Sleep(150u);
    frame = (softpc_runtime_frame *)calloc(1u, sizeof(*frame));
    assert(frame != NULL);
    assert(softpc_runtime_copy_frame(runtime, frame));
    assert(softpc_runtime_pause(runtime));
    assert(softpc_runtime_wait(runtime, SOFTPC_RUNTIME_PAUSED));
    assert(softpc_runtime_resume(runtime));
    assert(softpc_runtime_wait(runtime, SOFTPC_RUNTIME_RUNNING));
    assert(softpc_runtime_stop(runtime));
    assert(softpc_runtime_get_state(runtime) == SOFTPC_RUNTIME_STOPPED);
    assert(softpc_runtime_start(runtime));
    assert(softpc_runtime_stop(runtime));
    assert(softpc_runtime_set_floppy(runtime, NULL));
    free(frame);
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
