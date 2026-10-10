#include "../fixture_time.h"
#include "lib/types/types_interface.h"
#include "machine_fixture.h"
#include "../fixture_cleanup.h"

#include <assert.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

/* CMake's package configurations define NDEBUG.  This smoke owns real
 * lifecycle side effects, so a standard assert would erase the test itself
 * in precisely the build that ships the executable. */
#undef assert
#define assert(condition) do { \
    if (!(condition)) { \
        fprintf(stderr, "runtime smoke check failed: %s at line %d\n", \
            #condition, __LINE__); \
        return 1; \
    } \
} while (0)

typedef struct runtime_completion_probe {
    volatile LONG state_facts;
    volatile LONG frame_facts;
} runtime_completion_probe;

static void runtime_state_probe_receive(void *opaque, emulator_machine_state state,
    lib_u32 run_generation)
{
    runtime_completion_probe *probe = (runtime_completion_probe *)opaque;
    (void)state;
    (void)run_generation;
    if (probe == NULL) return;
    (void)InterlockedIncrement(&probe->state_facts);
}

static void runtime_frame_probe_receive(void *opaque, lib_u32 sequence,
    lib_bool graphics, lib_u32 run_generation)
{
    runtime_completion_probe *probe = (runtime_completion_probe *)opaque;
    (void)sequence;
    (void)graphics;
    (void)run_generation;
    if (probe == NULL) return;
    (void)InterlockedIncrement(&probe->frame_facts);
}

static int runtime_wait(emulator_machine *runtime,
    emulator_machine_state expected)
{
    lib_u64 deadline = softpc_test_clock_milliseconds() + 5000u;
    do {
        if (emulator_machine_state_get(runtime) == expected) return 1;
        softpc_test_sleep_milliseconds(10u);
    } while (softpc_test_clock_milliseconds() < deadline);
    return 0;
}

int main(void)
{
    const char *path = "softpc-runtime-smoke.img";
    unsigned char sector[512] = { 0 };
    FILE *file;
    softpc_machine_options options = { path, NULL };
    softpc_machine *machine = NULL;
    softpc_machine_fixture fixture = { 0 };
    emulator_machine *runtime;
    emulator_machine_frame *frame;
    lib_u32 first_run;
    runtime_completion_probe completion_probe = { 0 };

    options.floppy_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    options.hard_disk_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    sector[0] = 0xebu;
    sector[1] = 0xfeu;
    sector[510] = 0x55u;
    sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);

    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_fixture_create(machine, &fixture));
    runtime = fixture.machine;
    emulator_machine_set_state_sink(runtime, runtime_state_probe_receive,
        &completion_probe);
    emulator_machine_set_frame_sink(runtime, runtime_frame_probe_receive,
        &completion_probe);
    assert(emulator_machine_start(runtime));
    first_run = emulator_machine_run_generation(runtime);
    assert(first_run != 0u);
    softpc_test_sleep_milliseconds(150u);
    frame = (emulator_machine_frame *)lib_allocate_zero(1u, sizeof(*frame));
    assert(frame != NULL);
    {
        lib_u64 deadline = softpc_test_clock_milliseconds() + 5000u;
        int cursor_seen = 0;
        do {
            /* A copied presentation frame is deliberately non-blocking.
               The executor may own its frame lock while publishing the first
               original renderer update, so retry rather than turning that
               defined snapshot miss into a timing-dependent test failure. */
            if (emulator_machine_copy_published_frame(runtime, frame,
                    emulator_machine_run_generation(runtime)) &&
                frame->window.graphics == 0u && frame->window.text.base.cursor_column >= 0 &&
                frame->window.text.base.cursor_column < KVM_TEXT_COLUMNS &&
                frame->window.text.base.cursor_row >= 0 &&
                frame->window.text.base.cursor_row < KVM_TEXT_ROWS &&
                frame->window.text.base.cursor_visible != 0u && frame->window.text.base.cursor_phase != 0u) {
                cursor_seen = 1;
                break;
            }
            softpc_test_sleep_milliseconds(10u);
        } while (softpc_test_clock_milliseconds() < deadline);
        /* Original nt_graph's Console cursor endpoint must reach the copied
           frame.  Both outer frontends consume this value without reading a
           controller register or a guest-memory pointer. */
        assert(cursor_seen);
        assert(frame->window.text.base.font_height > 0u && frame->window.text.base.font_height <= 16u);
        assert(frame->window.text.base.cursor_bottom == frame->window.text.base.font_height - 1u);
        assert(frame->window.text.base.cursor_top <= frame->window.text.base.cursor_bottom);
    }
    assert(emulator_machine_published_frame_sequence(runtime) == frame->sequence);
    assert(emulator_machine_published_frame_run_generation(runtime) == first_run);
    assert(frame->sequence != 0u);
    {
        lib_u32 stable_sequence = frame->sequence;
        LONG stable_state_facts;
        /* An unchanged text screen is not an executor heartbeat.  Repeated
           publication would flood the app control FIFO and starve Console
           raw input behind redundant frame completions. */
        softpc_test_sleep_milliseconds(150u);
        assert(emulator_machine_copy_published_frame(runtime, frame,
            emulator_machine_run_generation(runtime)));
        assert(frame->sequence == stable_sequence);
        /* Executor paint callbacks are frame facts only. They must not create
           additional lifecycle completions while the machine stays running. */
        stable_state_facts = InterlockedCompareExchange(
            &completion_probe.state_facts, 0, 0);
        softpc_test_sleep_milliseconds(150u);
        assert(InterlockedCompareExchange(&completion_probe.state_facts, 0, 0) ==
            stable_state_facts);
    }
    /* Runtime owns copied frame production only.  Component existence and
       Console/Window selection belong to Emulator Session/UI,
       not a shared KVM target router. */
    /* Lifecycle policy is interpreted by the injected product control;
       this runtime unit directly proves the executor request/completion ABI. */
    assert(emulator_machine_pause(runtime));
    assert(runtime_wait(runtime, EMULATOR_MACHINE_PAUSED));
    assert(emulator_machine_set_removable_media(runtime, NULL, LIB_STORAGE_MEDIUM_OVERLAY));
    assert(emulator_machine_resume(runtime));
    assert(runtime_wait(runtime, EMULATOR_MACHINE_RUNNING));
    assert(emulator_machine_stop(runtime));
    assert(runtime_wait(runtime, EMULATOR_MACHINE_STOPPED));
    /* A monitor `start` after `stop` is a new cold run, not merely an
       accepted request.  Waiting for RUNNING catches a restart that reaches
       BIOS setup but never re-enters the executor. */
    assert(emulator_machine_start(runtime));
    assert(emulator_machine_run_generation(runtime) != first_run);
    assert(runtime_wait(runtime, EMULATOR_MACHINE_RUNNING));
    /* Reset is now one runtime command.  It hides its stop/start sequence
       and returns only once the new run has reached its public paused state. */
    assert(emulator_machine_reset(runtime));
    assert(runtime_wait(runtime, EMULATOR_MACHINE_PAUSED));
    assert(emulator_machine_resume(runtime));
    assert(runtime_wait(runtime, EMULATOR_MACHINE_RUNNING));
    assert(emulator_machine_stop(runtime));
    assert(runtime_wait(runtime, EMULATOR_MACHINE_STOPPED));
    assert(emulator_machine_set_removable_media(runtime, NULL, LIB_STORAGE_MEDIUM_OVERLAY));
    lib_release(frame);
    softpc_machine_fixture_destroy(&fixture);
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
