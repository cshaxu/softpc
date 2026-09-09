#include "runtime.h"

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

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

/* Pause before sampling the public instruction pointer.  This does not give
 * a frontend access to guest state: it is an integration-only proof that a
 * configured boot has left the firmware reset segment on each cold run. */
static int run_reaches_post_bios(app_runtime *runtime, softpc_machine *machine)
{
    uint16_t cs = 0u;
    uint32_t eip = 0u;
    unsigned int attempt;

    REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_RUNNING));
    /* CPU width and host load change how long firmware takes.  Sample only
     * while paused, then resume if it has not yet left reset; never guess a
     * fixed boot duration. */
    for (attempt = 0u; attempt < 20u; ++attempt) {
        Sleep(500u);
        REQUIRE(app_runtime_pause(runtime));
        REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_PAUSED));
        REQUIRE(softpc_machine_instruction_pointer(machine, &cs, &eip) ==
            SOFTPC_MACHINE_OK);
        REQUIRE(app_runtime_resume(runtime));
        REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_RUNNING));
        if (cs != 0xf000u) return 1;
    }
    fprintf(stderr, "runtime restart boot check failed: firmware did not "
        "leave reset within 10 seconds\n");
    return 0;
}

int main(void)
{
    softpc_machine_options options = { 0 };
    softpc_machine *machine = NULL;
    app_runtime *runtime = NULL;
    uint32_t first_generation;
    uint32_t second_generation;

    options.hard_disk_path = "assets/media/win31_en_installed.img";
    options.memory_bytes = 16u * 1024u * 1024u;
    options.presentation = SOFTPC_PRESENTATION_CONSOLE;
    options.media_mode = SOFTPC_MEDIA_OVERLAY;
    REQUIRE(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    REQUIRE(app_runtime_create(machine, &runtime));

    REQUIRE(app_runtime_start(runtime));
    first_generation = app_runtime_run_generation(runtime);
    REQUIRE(first_generation != 0u);
    REQUIRE(run_reaches_post_bios(runtime, machine));
    /* Exercise the same long-running state as an interactive DOS session,
       rather than stopping the first scheduler turn after startup. */
    Sleep(5000u);

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
    REQUIRE(run_reaches_post_bios(runtime, machine));

    REQUIRE(app_runtime_stop(runtime));
    REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_STOPPED));
    app_runtime_destroy(runtime);
    softpc_machine_destroy(machine);
    return 0;
}
#else
int main(void) { return 0; }
#endif
