#include "machine_fixture.h"
#include "common/session/control.h"
#include "common/machine/input_queue.h"
#include "../core/cleanup.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void runtime_state_probe_receive(void *opaque, common_machine_state state,
    uint32_t run_generation)
{
    runtime_completion_probe *probe = (runtime_completion_probe *)opaque;
    (void)state;
    (void)run_generation;
    if (probe == NULL) return;
    (void)InterlockedIncrement(&probe->state_facts);
}

static void runtime_frame_probe_receive(void *opaque, uint32_t sequence,
    int graphics, uint32_t run_generation)
{
    runtime_completion_probe *probe = (runtime_completion_probe *)opaque;
    (void)sequence;
    (void)graphics;
    (void)run_generation;
    if (probe == NULL) return;
    (void)InterlockedIncrement(&probe->frame_facts);
}

static int runtime_wait(common_machine *runtime,
    common_machine_state expected)
{
    DWORD deadline = GetTickCount() + 5000u;
    do {
        if (common_machine_state_get(runtime) == expected) return 1;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
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
    common_machine *runtime;
    common_machine_frame *frame;
    uint32_t first_run;
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
    common_machine_set_state_sink(runtime, runtime_state_probe_receive,
        &completion_probe);
    common_machine_set_frame_sink(runtime, runtime_frame_probe_receive,
        &completion_probe);
    /* The product control FIFO must not turn a short input burst into a
       silently dropped make/break sequence at its old fixed-64 boundary. */
    {
        common_session_queue storage = { 0 }, *queue = &storage;
        kvm_input_event event = { 0 };
        common_session_event copied;
        unsigned int index;
        assert(common_session_queue_initialize(queue));
        event.type = KVM_EVENT_TEXT;
        for (index = 0u; index < 96u; ++index) {
            event.data.text.scalar = index;
            assert(common_session_queue_push_kvm_for_run(queue, &event, 0u));
        }
        for (index = 0u; index < 96u; ++index) {
            assert(common_session_queue_take(queue, &copied, 0u));
            assert(copied.kind == COMMON_SESSION_EVENT_KVM_INPUT);
            assert(copied.value.kvm.data.text.scalar == index);
        }
        common_session_queue_dispose(queue);
    }
    {
        common_machine_input_queue storage = { 0 }, *queue = &storage;
        kvm_input_event event = { 0 };

        assert(common_machine_input_queue_initialize(queue) == LIB_STATUS_OK);
        event.type = KVM_EVENT_KEY;
        event.data.key.scan_code = 0x1eu;
        event.data.key.pressed = 1u;
        assert(common_machine_input_queue_push(queue, &event));
        assert(common_machine_input_queue_pending(queue));
        common_machine_input_queue_clear(queue);
        assert(!common_machine_input_queue_pending(queue));
        common_machine_input_queue_dispose(queue);
    }
    assert(common_machine_start(runtime));
    first_run = common_machine_run_generation(runtime);
    assert(first_run != 0u);
    Sleep(150u);
    frame = (common_machine_frame *)calloc(1u, sizeof(*frame));
    assert(frame != NULL);
    {
        DWORD deadline = GetTickCount() + 5000u;
        int cursor_seen = 0;
        do {
            /* A copied presentation frame is deliberately non-blocking.
               The executor may own its frame lock while publishing the first
               original renderer update, so retry rather than turning that
               defined snapshot miss into a timing-dependent test failure. */
            if (common_machine_copy_published_frame(runtime, frame,
                    common_machine_run_generation(runtime)) &&
                frame->window.graphics == 0u && frame->window.text.base.cursor_column >= 0 &&
                frame->window.text.base.cursor_column < KVM_TEXT_COLUMNS &&
                frame->window.text.base.cursor_row >= 0 &&
                frame->window.text.base.cursor_row < KVM_TEXT_ROWS &&
                frame->window.text.base.cursor_visible != 0u && frame->window.text.base.cursor_phase != 0u) {
                cursor_seen = 1;
                break;
            }
            Sleep(10u);
        } while ((LONG)(GetTickCount() - deadline) < 0);
        /* Original nt_graph's Console cursor endpoint must reach the copied
           frame.  Both outer frontends consume this value without reading a
           controller register or a guest-memory pointer. */
        assert(cursor_seen);
        assert(frame->window.text.base.font_height > 0u && frame->window.text.base.font_height <= 16u);
        assert(frame->window.text.base.cursor_bottom == frame->window.text.base.font_height - 1u);
        assert(frame->window.text.base.cursor_top <= frame->window.text.base.cursor_bottom);
    }
    assert(common_machine_published_frame_sequence(runtime) == frame->sequence);
    assert(common_machine_published_frame_run_generation(runtime) == first_run);
    assert(frame->sequence != 0u);
    {
        uint32_t stable_sequence = frame->sequence;
        LONG stable_state_facts;
        /* An unchanged text screen is not an executor heartbeat.  Repeated
           publication would flood the app control FIFO and starve Console
           raw input behind redundant frame completions. */
        Sleep(150u);
        assert(common_machine_copy_published_frame(runtime, frame,
            common_machine_run_generation(runtime)));
        assert(frame->sequence == stable_sequence);
        /* Executor paint callbacks are frame facts only. They must not create
           additional lifecycle completions while the machine stays running. */
        stable_state_facts = InterlockedCompareExchange(
            &completion_probe.state_facts, 0, 0);
        Sleep(150u);
        assert(InterlockedCompareExchange(&completion_probe.state_facts, 0, 0) ==
            stable_state_facts);
    }
    /* Runtime owns copied frame production only.  Component existence and
       Console/Window selection belong to Common Session/UI,
       not a shared KVM target router. */
    /* Lifecycle policy is interpreted by the injected product control;
       this runtime unit directly proves the executor request/completion ABI. */
    assert(common_machine_pause(runtime));
    assert(runtime_wait(runtime, COMMON_MACHINE_PAUSED));
    assert(common_machine_set_removable_media(runtime, NULL, LIB_STORAGE_MEDIUM_OVERLAY));
    assert(common_machine_resume(runtime));
    assert(runtime_wait(runtime, COMMON_MACHINE_RUNNING));
    assert(common_machine_stop(runtime));
    assert(runtime_wait(runtime, COMMON_MACHINE_STOPPED));
    /* A monitor `start` after `stop` is a new cold run, not merely an
       accepted request.  Waiting for RUNNING catches a restart that reaches
       BIOS setup but never re-enters the executor. */
    assert(common_machine_start(runtime));
    assert(common_machine_run_generation(runtime) != first_run);
    assert(runtime_wait(runtime, COMMON_MACHINE_RUNNING));
    /* Reset is now one runtime command.  It hides its stop/start sequence
       and returns only once the new run has reached its public paused state. */
    assert(common_machine_reset(runtime));
    assert(runtime_wait(runtime, COMMON_MACHINE_PAUSED));
    assert(common_machine_resume(runtime));
    assert(runtime_wait(runtime, COMMON_MACHINE_RUNNING));
    assert(common_machine_stop(runtime));
    assert(runtime_wait(runtime, COMMON_MACHINE_STOPPED));
    assert(common_machine_set_removable_media(runtime, NULL, LIB_STORAGE_MEDIUM_OVERLAY));
    free(frame);
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
