#include "softpc_machine.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

static volatile LONG softpc_lifecycle_result;

static DWORD WINAPI softpc_lifecycle_run(void *opaque)
{
    softpc_machine *machine = (softpc_machine *)opaque;
    softpc_machine_executor_thread_enter(machine);
    softpc_machine_result result = softpc_machine_run(machine, 1u);
    softpc_machine_set_heartbeat(machine, 0);
    softpc_machine_executor_thread_leave(machine);
    InterlockedExchange(&softpc_lifecycle_result, (LONG)result);
    return 0u;
}

int main(void)
{
    const char *path = "softpc-lifecycle-smoke.img";
    unsigned char sector[512] = { 0 };
    FILE *file;
    HANDLE worker;
    softpc_machine_options options = { path, NULL,
        SOFTPC_PRESENTATION_CONSOLE };
    softpc_machine *machine = NULL;

    /* This probe covers executor exit, not direct-media persistence. Keep its
       fixture host-owned so its deletion has no CRT handle-timing dependency. */
    options.media_mode = SOFTPC_MEDIA_OVERLAY;

    /* jmp $: execution must leave only through the outer lifecycle request. */
    sector[0] = 0xebu;
    sector[1] = 0xfeu;
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);

    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    InterlockedExchange(&softpc_lifecycle_result, SOFTPC_MACHINE_IO_ERROR);
    softpc_machine_set_heartbeat(machine, 1);
    worker = CreateThread(NULL, 0u, softpc_lifecycle_run, machine, 0u, NULL);
    assert(worker != NULL);
    Sleep(200u);
    softpc_machine_request_stop(machine);
    assert(WaitForSingleObject(worker, 5000u) == WAIT_OBJECT_0);
    assert(InterlockedCompareExchange(&softpc_lifecycle_result, 0, 0) ==
        SOFTPC_MACHINE_OK);
    CloseHandle(worker);
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
