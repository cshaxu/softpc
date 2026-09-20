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

/* This test consumes the copied runtime snapshot, never the original video
 * surface.  A real command prompt is the owner-observed post-BIOS fact for
 * this installed image; CS merely leaving F000 is not an adequate proxy. */
static int frame_has_dos_prompt(const app_runtime_frame *frame)
{
    uint32_t row;

    if (frame == NULL || frame->window.valid == 0u || frame->window.graphics != 0u)
        return 0;
    for (row = 0u; row < frame->window.text.base.text_rows; ++row) {
        const uint8_t *line = &frame->window.text.base.text[row * SOFTPC_RUNTIME_TEXT_COLUMNS];
        uint32_t column;

        for (column = 0u; column + 3u < frame->window.text.base.text_columns; ++column) {
            if ((line[column] == 'C' || line[column] == 'c') &&
                line[column + 1u] == ':' && line[column + 2u] == '\\' &&
                line[column + 3u] == '>')
                return 1;
        }
    }
    return 0;
}

static void report_last_frame(app_runtime *runtime)
{
    app_runtime_frame frame;
    uint32_t row;

    if (!app_runtime_copy_frame(runtime, &frame)) return;
    fprintf(stderr, "last frame: valid=%u graphics=%u text=%ux%u sequence=%lu\n",
        (unsigned int)frame.window.valid, (unsigned int)frame.window.graphics,
        (unsigned int)frame.window.text.base.text_columns, (unsigned int)frame.window.text.base.text_rows,
        (unsigned long)frame.sequence);
    if (frame.window.graphics != 0u) return;
    for (row = 0u; row < frame.window.text.base.text_rows; ++row) {
        char line[SOFTPC_RUNTIME_TEXT_COLUMNS + 1u];
        uint32_t column;
        int nonblank = 0;

        for (column = 0u; column < frame.window.text.base.text_columns; ++column) {
            uint8_t c = frame.window.text.base.text[row * SOFTPC_RUNTIME_TEXT_COLUMNS + column];
            line[column] = c >= 0x20u && c < 0x7fu ? (char)c : ' ';
            if (line[column] != ' ') nonblank = 1;
        }
        line[frame.window.text.base.text_columns] = '\0';
        if (nonblank) fprintf(stderr, "%s\n", line);
    }
}

static int wait_for_dos_prompt(app_runtime *runtime, DWORD timeout_ms)
{
    app_runtime_frame frame;
    DWORD deadline = GetTickCount() + timeout_ms;

    do {
        if (app_runtime_copy_frame(runtime, &frame) && frame_has_dos_prompt(&frame))
            return 1;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

static int run_reaches_post_bios(app_runtime *runtime,
    runtime_frame_probe *probe, uint32_t run_generation,
    uint32_t prior_sequence)
{
    if (!wait_for_state(runtime, SOFTPC_RUNTIME_RUNNING)) goto failed;
    /* This is deliberately stronger than an executor/IP check: a new cold
     * run must commit a copied frame tagged with its own run generation.
     * Otherwise the product layer could retain the previous run's surface
     * and make a live VM look stalled after `pause -> stop -> start`. */
    if (!wait_for_frame_of_run(probe, run_generation, prior_sequence)) goto failed;
    /* No test-side wake-up: every boot must reach its normal DOS command
     * prompt under the ordinary executor clock within the product's ten
     * second expectation. */
    return wait_for_dos_prompt(runtime, 10000u);

failed:
    fprintf(stderr, "runtime restart boot check failed: run=%lu state=%d\n",
        (unsigned long)run_generation, (int)app_runtime_get_state(runtime));
    report_last_frame(runtime);
    return 0;
}

static int enter_windows(app_runtime *runtime)
{
    const uint32_t keys[] = { 'W', 'I', 'N', KVM_KEY_ENTER };
    const uint32_t scans[] = { 0x11u, 0x17u, 0x31u, 0x1cu };
    app_runtime_frame *frame = calloc(1u, sizeof(*frame));
    DWORD deadline;
    int graphics = 0, running = 1;
    if (frame == NULL) return 0;
    for (unsigned index = 0; index < 4u; ++index) {
        kvm_input_event event = { 0 };
        event.type = KVM_EVENT_KEY;
        event.data.key.key = keys[index];
        event.data.key.scan_code = scans[index];
        event.data.key.pressed = 1u;
        if (!app_runtime_enqueue_input_event(runtime, &event)) { free(frame); return 0; }
        event.data.key.pressed = 0u;
        if (!app_runtime_enqueue_input_event(runtime, &event)) { free(frame); return 0; }
    }
    /* Observe through startup, not merely its first splash frame. The fixed
       installed image remains overlay-only and the normal executor owns time. */
    deadline = GetTickCount() + 15000u;
    do {
        if (app_runtime_get_state(runtime) != SOFTPC_RUNTIME_RUNNING) {
            running = 0;
            break;
        }
        if (app_runtime_copy_frame(runtime, frame) && frame->window.graphics)
            graphics = frame->window.image.width == 640u && frame->window.image.height == 480u;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    free(frame);
    return running && graphics;
}

int main(void)
{
    softpc_machine_options options = { 0 };
    softpc_machine *machine = NULL;
    app_runtime *runtime = NULL;
    uint32_t generation;
    uint32_t sequence;
    unsigned int cycle;
    runtime_frame_probe frame_probe = { 0 };

    options.hard_disk_path = "assets/media/win31_en_installed.img";
    options.memory_bytes = 16u * 1024u * 1024u;
    options.floppy_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    options.hard_disk_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    REQUIRE(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    REQUIRE(app_runtime_create(machine, &runtime));
    app_runtime_set_frame_sink(runtime, receive_frame, &frame_probe);

    REQUIRE(app_runtime_start(runtime));
    generation = app_runtime_run_generation(runtime);
    REQUIRE(generation != 0u);
    REQUIRE(run_reaches_post_bios(runtime, &frame_probe,
        generation, 0u));
    sequence = (uint32_t)InterlockedCompareExchange(
        &frame_probe.last_sequence, 0, 0);

    /* Monitor `stop` normally arrives after pause has returned Current
       Console to the cooked monitor.  Repeat that exact public path: reset
       bugs often appear only after one or more prior controller lifetimes. */
    for (cycle = 0u; cycle < 3u; ++cycle) {
        REQUIRE(app_runtime_pause(runtime));
        REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_PAUSED));
        REQUIRE(app_runtime_stop(runtime));
        REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_STOPPED));
        REQUIRE(app_runtime_start(runtime));
        generation = app_runtime_run_generation(runtime);
        REQUIRE(generation != 0u);
        REQUIRE(run_reaches_post_bios(runtime, &frame_probe,
            generation, sequence));
        sequence = (uint32_t)InterlockedCompareExchange(
            &frame_probe.last_sequence, 0, 0);
    }

    REQUIRE(enter_windows(runtime));
    REQUIRE(app_runtime_stop(runtime));
    REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_STOPPED));
    REQUIRE(app_runtime_start(runtime));
    REQUIRE(run_reaches_post_bios(runtime, &frame_probe,
        app_runtime_run_generation(runtime), sequence));
    REQUIRE(app_runtime_stop(runtime));
    REQUIRE(wait_for_state(runtime, SOFTPC_RUNTIME_STOPPED));
    app_runtime_destroy(runtime);
    softpc_machine_destroy(machine);
    return 0;
}
#else
int main(void) { return 0; }
#endif
