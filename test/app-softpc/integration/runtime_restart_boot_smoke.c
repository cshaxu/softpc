#include "../fixture_time.h"
#include "lib/types/types_interface.h"
#include "machine_fixture.h"

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

static int wait_for_state(emulator_machine *runtime, emulator_machine_state state)
{
    lib_u64 deadline = softpc_test_clock_milliseconds() + 10000u;
    do {
        if (emulator_machine_state_get(runtime) == state) return 1;
        softpc_test_sleep_milliseconds(10u);
    } while (softpc_test_clock_milliseconds() < deadline);
    return 0;
}

static ULONGLONG cpu_ticks(const FILETIME *kernel, const FILETIME *user)
{
    return (((ULONGLONG)kernel->dwHighDateTime << 32) | kernel->dwLowDateTime) +
        (((ULONGLONG)user->dwHighDateTime << 32) | user->dwLowDateTime);
}

static void receive_frame(void *opaque, lib_u32 sequence, lib_bool graphics,
    lib_u32 run_generation)
{
    runtime_frame_probe *probe = (runtime_frame_probe *)opaque;
    (void)graphics;
    if (probe == NULL) return;
    InterlockedExchange(&probe->last_sequence, (LONG)sequence);
    InterlockedExchange(&probe->last_generation, (LONG)run_generation);
    InterlockedIncrement(&probe->frame_count);
}

static int wait_for_frame_of_run(const runtime_frame_probe *probe,
    lib_u32 run_generation, lib_u32 after_sequence)
{
    lib_u64 deadline = softpc_test_clock_milliseconds() + 10000u;
    do {
        lib_u32 sequence = (lib_u32)InterlockedCompareExchange(
            (volatile LONG *)&probe->last_sequence, 0, 0);
        lib_u32 generation = (lib_u32)InterlockedCompareExchange(
            (volatile LONG *)&probe->last_generation, 0, 0);
        if (generation == run_generation && sequence > after_sequence)
            return 1;
        softpc_test_sleep_milliseconds(10u);
    } while (softpc_test_clock_milliseconds() < deadline);
    return 0;
}

/* This test consumes the copied runtime snapshot, never the original video
 * surface.  A real command prompt is the owner-observed post-BIOS fact for
 * this installed image; CS merely leaving F000 is not an adequate proxy. */
static int frame_has_dos_prompt(const kvm_window_frame *frame)
{
    lib_u32 row;

    if (frame == NULL || frame->text == LIB_NULL || frame->graphics != LIB_NULL)
        return 0;
    for (row = 0u; row < frame->text->base.text_rows; ++row) {
        const kvm_text_cell *line = &frame->text->base.cells[row * 80u];
        lib_u32 column;

        for (column = 0u; column + 3u < frame->text->base.text_columns; ++column) {
            if ((line[column].glyph_index == 'C' || line[column].glyph_index == 'c') &&
                line[column + 1u].glyph_index == ':' && line[column + 2u].glyph_index == '\\' &&
                line[column + 3u].glyph_index == '>')
                return 1;
        }
    }
    return 0;
}

static void report_last_frame(emulator_machine *runtime)
{
    static kvm_window_frame_storage frame;
    lib_u32 row;

    if (!emulator_machine_copy_published_frame(runtime, &frame,
            emulator_machine_run_generation(runtime))) return;
    fprintf(stderr, "last frame: graphics=%u text=%ux%u sequence=%lu\n",
        (unsigned int)(frame.frame.graphics != LIB_NULL),
        (unsigned int)frame.data.text->base.text_columns, (unsigned int)frame.data.text->base.text_rows,
        (unsigned long)frame.frame.sequence);
    if (frame.frame.graphics != LIB_NULL) {
        const kvm_window_graphics_frame *image = frame.frame.graphics;
        FILE *file = fopen("restart-failure.ppm", "wb");
        if (file != NULL) {
            fprintf(file, "P6\n%u %u\n255\n", image->width, image->height);
            for (lib_u32 y = 0; y < image->height; ++y)
                for (lib_u32 x = 0; x < image->width; ++x) {
                    lib_u32 pixel = image->pixels[y * image->stride + x];
                    const unsigned char rgb[] = {
                        (unsigned char)(pixel >> 16), (unsigned char)(pixel >> 8),
                        (unsigned char)pixel };
                    (void)fwrite(rgb, 1, sizeof(rgb), file);
                }
            (void)fclose(file);
        }
        return;
    }
    for (row = 0u; row < frame.data.text->base.text_rows; ++row) {
        char line[80u + 1u];
        lib_u32 column;
        int nonblank = 0;

        for (column = 0u; column < frame.data.text->base.text_columns; ++column) {
            lib_u8 c = frame.data.text->base.cells[row * 80u + column].glyph_index;
            line[column] = c >= 0x20u && c < 0x7fu ? (char)c : ' ';
            if (line[column] != ' ') nonblank = 1;
        }
        line[frame.data.text->base.text_columns] = '\0';
        if (nonblank) fprintf(stderr, "%s\n", line);
    }
}

static int wait_for_dos_prompt(emulator_machine *runtime, DWORD timeout_ms)
{
    static kvm_window_frame_storage frame;
    lib_u64 deadline = softpc_test_clock_milliseconds() + timeout_ms;

    do {
        if (emulator_machine_copy_published_frame(runtime, &frame,
                emulator_machine_run_generation(runtime)) && frame_has_dos_prompt(&frame.frame))
            return 1;
        softpc_test_sleep_milliseconds(10u);
    } while (softpc_test_clock_milliseconds() < deadline);
    return 0;
}

static int run_reaches_post_bios(emulator_machine *runtime,
    runtime_frame_probe *probe, lib_u32 run_generation,
    lib_u32 prior_sequence)
{
    if (!wait_for_state(runtime, EMULATOR_MACHINE_RUNNING)) goto failed;
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
        (unsigned long)run_generation, (int)emulator_machine_state_get(runtime));
    report_last_frame(runtime);
    return 0;
}

static int enter_windows(emulator_machine *runtime)
{
    const lib_u32 keys[] = { 'W', 'I', 'N', BASE_KEY_ENTER };
    const lib_u32 scans[] = { 0x11u, 0x17u, 0x31u, 0x1cu };
    kvm_window_frame_storage *frame = lib_allocate_zero(1u, sizeof(*frame));
    lib_u64 deadline;
    int graphics = 0, running = 1;
    if (frame == NULL) return 0;
    for (unsigned index = 0; index < 4u; ++index) {
        kvm_input_event event = { 0 };
        event.type = KVM_EVENT_KEY;
        event.data.key.key = keys[index];
        event.data.key.scan_code = scans[index];
        event.data.key.pressed = 1u;
        if (emulator_machine_enqueue_input(runtime, &event) != LIB_STATUS_OK) { kvm_window_frame_release(frame); lib_release(frame); return 0; }
        event.data.key.pressed = 0u;
        if (emulator_machine_enqueue_input(runtime, &event) != LIB_STATUS_OK) { kvm_window_frame_release(frame); lib_release(frame); return 0; }
    }
    /* Observe through startup, not merely its first splash frame. The fixed
       installed image remains overlay-only and the normal executor owns time. */
    deadline = softpc_test_clock_milliseconds() + 15000u;
    do {
        if (emulator_machine_state_get(runtime) != EMULATOR_MACHINE_RUNNING) {
            running = 0;
            break;
        }
        if (emulator_machine_copy_published_frame(runtime, frame,
                emulator_machine_run_generation(runtime)) && frame->frame.graphics)
            graphics = frame->frame.graphics->width == 640u && frame->frame.graphics->height == 480u;
        softpc_test_sleep_milliseconds(10u);
    } while (softpc_test_clock_milliseconds() < deadline);
    kvm_window_frame_release(frame); lib_release(frame);
    return running && graphics;
}

static int send_key(emulator_machine *runtime, lib_u32 key, lib_u32 scan, int down)
{
    kvm_input_event event = { 0 };
    event.type = KVM_EVENT_KEY;
    event.data.key.key = key;
    event.data.key.scan_code = scan;
    event.data.key.pressed = down != 0;
    return emulator_machine_enqueue_input(runtime, &event) == LIB_STATUS_OK;
}

static int tap_key(emulator_machine *runtime, lib_u32 key, lib_u32 scan)
{
    if (!send_key(runtime, key, scan, 1) || !send_key(runtime, key, scan, 0))
        return 0;
    softpc_test_sleep_milliseconds(50u);
    return 1;
}

static int wait_for_mode(emulator_machine *runtime, int graphics, lib_u32 after)
{
    kvm_window_frame_storage *frame = lib_allocate_zero(1u, sizeof(*frame));
    lib_u64 deadline = softpc_test_clock_milliseconds() + 10000u;
    int matched = 0;
    if (frame == NULL) return 0;
    do {
        if (emulator_machine_state_get(runtime) != EMULATOR_MACHINE_RUNNING) break;
        if (emulator_machine_copy_published_frame(runtime, frame,
                emulator_machine_run_generation(runtime)) && frame->frame.sequence > after &&
            (frame->frame.text != LIB_NULL || frame->frame.graphics != LIB_NULL) && (frame->frame.graphics != LIB_NULL) == graphics) {
            matched = 1;
            break;
        }
        softpc_test_sleep_milliseconds(10u);
    } while (softpc_test_clock_milliseconds() < deadline);
    kvm_window_frame_release(frame); lib_release(frame);
    return matched;
}

static int type_command(emulator_machine *runtime, const char *text)
{
    static const lib_u32 scans[] = {
        0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17,
        0x24, 0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13,
        0x1f, 0x14, 0x16, 0x2f, 0x11, 0x2d, 0x15, 0x2c
    };
    for (; *text; ++text) {
        lib_u32 key = (lib_u32)*text;
        if (*text != '.' && (*text < 'A' || *text > 'Z')) return 0;
        lib_u32 scan = *text == '.' ? 0x34u : scans[*text - 'A'];
        if (!tap_key(runtime, key, scan)) return 0;
    }
    return tap_key(runtime, BASE_KEY_ENTER, 0x1c);
}

static int prompt_roundtrip(emulator_machine *runtime, int windowed)
{
    kvm_window_frame_storage *frame = lib_allocate_zero(1u, sizeof(*frame));
    int succeeded = 0;
    unsigned stage = 0u;
    if (frame == NULL) return 0;
    /* Program Manager File/Run, through the ordinary machine input queue. */
    if (!send_key(runtime, BASE_KEY_ALT, 0x38, 1) ||
        !tap_key(runtime, 'F', 0x21) ||
        !send_key(runtime, BASE_KEY_ALT, 0x38, 0) ||
        !tap_key(runtime, 'R', 0x13)) goto done;
    softpc_test_sleep_milliseconds(300u);
    if (!emulator_machine_copy_published_frame(runtime, frame,
            emulator_machine_run_generation(runtime)) ||
        !type_command(runtime, windowed ? "DOSPMPTW.PIF" : "DOSPRMPT.PIF")) goto done;
    softpc_test_sleep_milliseconds(2000u);
    stage = 1u;
    if (!wait_for_mode(runtime, windowed, frame->frame.sequence)) goto done;
    for (unsigned i = 0; i < 6u; ++i) {
        stage = 2u + i;
        if (!emulator_machine_copy_published_frame(runtime, frame,
                emulator_machine_run_generation(runtime))) goto done;
        lib_u32 prior = frame->frame.sequence;
        if (!send_key(runtime, BASE_KEY_ALT, 0x38, 1) ||
            !tap_key(runtime, BASE_KEY_ENTER, 0x1c) ||
            !send_key(runtime, BASE_KEY_ALT, 0x38, 0) ||
            !wait_for_mode(runtime, windowed ^ ((i & 1u) == 0u), prior)) goto done;
        /* Let rendering/input continue after the first mode notification. */
        softpc_test_sleep_milliseconds(1000u);
        if (emulator_machine_state_get(runtime) != EMULATOR_MACHINE_RUNNING) goto done;
        if (!type_command(runtime, "CLS")) goto done;
    }
    if (!emulator_machine_copy_published_frame(runtime, frame,
            emulator_machine_run_generation(runtime)) ||
        !type_command(runtime, "EXIT")) goto done;
    softpc_test_sleep_milliseconds(2000u);
    stage = 8u;
    if (!wait_for_mode(runtime, 1, frame->frame.sequence)) goto done;
    succeeded = 1;
done:
    if (!succeeded) {
        fprintf(stderr, "prompt roundtrip windowed=%d stage=%u\n", windowed, stage);
        report_last_frame(runtime);
    }
    kvm_window_frame_release(frame); lib_release(frame);
    return succeeded;
}

int main(void)
{
    softpc_machine_options options = { 0 };
    softpc_machine *machine = NULL;
    softpc_machine_fixture fixture = { 0 };
    emulator_machine *runtime;
    lib_u32 generation;
    lib_u32 sequence;
    unsigned int cycle;
    runtime_frame_probe frame_probe = { 0 };

    options.hard_disk_path = SOFTPC_TEST_RESTART_BOOT_IMAGE;
    options.memory_bytes = 16u * 1024u * 1024u;
    options.floppy_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    options.hard_disk_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    REQUIRE(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    REQUIRE(softpc_machine_fixture_create(machine, &fixture));
    runtime = fixture.machine;
    emulator_machine_set_frame_sink(runtime, receive_frame, &frame_probe);

    REQUIRE(emulator_machine_start(runtime));
    generation = emulator_machine_run_generation(runtime);
    REQUIRE(generation != 0u);
    REQUIRE(run_reaches_post_bios(runtime, &frame_probe,
        generation, 0u));
    {
        FILETIME created, exited, kernel, user;
        ULONGLONG before;
        /* A copied prompt alone cannot prove idle: a busy CCPU can draw it.
           Allow the original heuristic to settle, then bound CPU time, not
           host speed. 25 percent leaves generous CI margin above idle. */
        softpc_test_sleep_milliseconds(1000u);
        REQUIRE(GetProcessTimes(GetCurrentProcess(), &created, &exited, &kernel, &user));
        before = cpu_ticks(&kernel, &user);
        softpc_test_sleep_milliseconds(2000u);
        REQUIRE(GetProcessTimes(GetCurrentProcess(), &created, &exited, &kernel, &user));
        fprintf(stderr, "DOS idle CPU: %.2f ms over 2000 ms\n",
            (double)(cpu_ticks(&kernel, &user) - before) / 10000.0);
        REQUIRE(cpu_ticks(&kernel, &user) - before < 5000000u);
        REQUIRE(emulator_machine_state_get(runtime) == EMULATOR_MACHINE_RUNNING);
    }
    sequence = (lib_u32)InterlockedCompareExchange(
        &frame_probe.last_sequence, 0, 0);

    /* Monitor `stop` normally arrives after pause has returned Current
       Console to the cooked monitor.  Repeat that exact public path: reset
       bugs often appear only after one or more prior controller lifetimes. */
    for (cycle = 0u; cycle < 3u; ++cycle) {
        REQUIRE(emulator_machine_pause(runtime));
        REQUIRE(wait_for_state(runtime, EMULATOR_MACHINE_PAUSED));
        REQUIRE(emulator_machine_stop(runtime));
        REQUIRE(wait_for_state(runtime, EMULATOR_MACHINE_STOPPED));
        REQUIRE(emulator_machine_start(runtime));
        generation = emulator_machine_run_generation(runtime);
        REQUIRE(generation != 0u);
        REQUIRE(run_reaches_post_bios(runtime, &frame_probe,
            generation, sequence));
        sequence = (lib_u32)InterlockedCompareExchange(
            &frame_probe.last_sequence, 0, 0);
    }

    REQUIRE(enter_windows(runtime));
    REQUIRE(prompt_roundtrip(runtime, 0));
    REQUIRE(prompt_roundtrip(runtime, 1));
    REQUIRE(emulator_machine_stop(runtime));
    REQUIRE(wait_for_state(runtime, EMULATOR_MACHINE_STOPPED));
    REQUIRE(emulator_machine_start(runtime));
    REQUIRE(run_reaches_post_bios(runtime, &frame_probe,
        emulator_machine_run_generation(runtime), sequence));
    REQUIRE(emulator_machine_stop(runtime));
    REQUIRE(wait_for_state(runtime, EMULATOR_MACHINE_STOPPED));
    softpc_machine_fixture_destroy(&fixture);
    softpc_machine_destroy(machine);
    return 0;
}
#else
int main(void) { return 0; }
#endif
