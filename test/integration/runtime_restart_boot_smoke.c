#include "runtime.h"

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

typedef struct runtime_frame_probe {
    volatile LONG last_sequence;
    volatile LONG last_generation;
    volatile LONG frame_count;
} runtime_frame_probe;

#define REQUIRE(condition) do { \
    if (!(condition)) { \
        fprintf(stderr, "runtime restart boot check failed: %s at line %d\n", \
            #condition, __LINE__); \
        return 1; \
    } \
} while (0)

static int wait_for_state(app_runtime *runtime, app_runtime_state state)
{
    DWORD deadline = GetTickCount() + 10000u;
    do {
        if (app_runtime_get_state(runtime) == state) return 1;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

static void receive_frame(void *opaque, uint32_t sequence, int graphics,
    uint32_t run_generation)
{
    runtime_frame_probe *probe = (runtime_frame_probe *)opaque;
    (void)graphics;
    if (probe == NULL) return;
    InterlockedExchange(&probe->last_sequence, (LONG)sequence);
    InterlockedExchange(&probe->last_generation, (LONG)run_generation);
    InterlockedIncrement(&probe->frame_count);
}

static int wait_for_frame_of_run(const runtime_frame_probe *probe,
    uint32_t run_generation, uint32_t after_sequence)
{
    DWORD deadline = GetTickCount() + 10000u;
    do {
        uint32_t sequence = (uint32_t)InterlockedCompareExchange(
            (volatile LONG *)&probe->last_sequence, 0, 0);
        uint32_t generation = (uint32_t)InterlockedCompareExchange(
            (volatile LONG *)&probe->last_generation, 0, 0);
        if (generation == run_generation && sequence > after_sequence)
            return 1;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

/* Pause before sampling the public instruction pointer.  This does not give
 * a frontend access to guest state: it is an integration-only proof that a
 * configured boot has left the firmware reset segment on each cold run. */
static int run_reaches_post_bios(app_runtime *runtime, softpc_machine *machine,
    runtime_frame_probe *probe, uint32_t run_generation,
    uint32_t prior_sequence, DWORD uninterrupted_runtime_ms)
{
    uint16_t cs = 0u;
    uint32_t eip = 0u;

    REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_RUNNING));
    /* This is deliberately stronger than an executor/IP check: a new cold
     * run must commit a copied frame tagged with its own run generation.
     * Otherwise the product layer could retain the previous run's surface
     * and make a live VM look stalled after `pause -> stop -> start`. */
    REQUIRE(wait_for_frame_of_run(probe, run_generation, prior_sequence));
    /* Do not repeatedly pause/resume as a test-side wake-up.  The reported
     * failure is an otherwise idle second boot, so it must make progress
     * under its normal clock alone before this one diagnostic pause. */
    Sleep(uninterrupted_runtime_ms);
    REQUIRE(app_runtime_pause(runtime));
    REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_PAUSED));
    REQUIRE(softpc_machine_instruction_pointer(machine, &cs, &eip) ==
        SOFTPC_MACHINE_OK);
    REQUIRE(cs != 0xf000u);
    REQUIRE(app_runtime_resume(runtime));
    return wait_for_state(runtime, SOFTPC_RUNTIME_RUNNING);
}

int main(void)
{
    softpc_machine_options options = { 0 };
    softpc_machine *machine = NULL;
    app_runtime *runtime = NULL;
    uint32_t first_generation;
    uint32_t second_generation;
    uint32_t first_sequence;
    runtime_frame_probe frame_probe = { 0 };

    options.hard_disk_path = "assets/media/win31_en_installed.img";
    options.memory_bytes = 16u * 1024u * 1024u;
    options.presentation = SOFTPC_PRESENTATION_CONSOLE;
    options.media_mode = SOFTPC_MEDIA_OVERLAY;
    REQUIRE(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    REQUIRE(app_runtime_create(machine, &runtime));
    app_runtime_set_frame_sink(runtime, receive_frame, &frame_probe);

    REQUIRE(app_runtime_start(runtime));
    first_generation = app_runtime_run_generation(runtime);
    REQUIRE(first_generation != 0u);
    REQUIRE(run_reaches_post_bios(runtime, machine, &frame_probe,
        first_generation, 0u, 5000u));
    first_sequence = (uint32_t)InterlockedCompareExchange(
        &frame_probe.last_sequence, 0, 0);

    /* Monitor `stop` normally arrives after pause has returned Current
       Console to the cooked monitor.  This is a different executor exit
       path from a direct running stop and must cold-start just as cleanly. */
    REQUIRE(app_runtime_pause(runtime));
    REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_PAUSED));
    REQUIRE(app_runtime_stop(runtime));
    REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_STOPPED));

    REQUIRE(app_runtime_start(runtime));
    second_generation = app_runtime_run_generation(runtime);
    REQUIRE(second_generation != first_generation);
    REQUIRE(run_reaches_post_bios(runtime, machine, &frame_probe,
        second_generation, first_sequence, 10000u));

    REQUIRE(app_runtime_stop(runtime));
    REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_STOPPED));
    app_runtime_destroy(runtime);
    softpc_machine_destroy(machine);
    return 0;
}
#else
int main(void) { return 0; }
#endif
