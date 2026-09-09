#include "runtime.h"
#include "control.h"
#include "input_queue.h"
#include "reconciler.h"
#include "test_cleanup.h"

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

static void runtime_state_probe_receive(void *opaque, app_runtime_state state,
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
    uint32_t first_run;
    runtime_completion_probe completion_probe = { 0 };

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
    app_runtime_set_state_sink(runtime, runtime_state_probe_receive,
        &completion_probe);
    app_runtime_set_frame_sink(runtime, runtime_frame_probe_receive,
        &completion_probe);
    /* The product control FIFO must not turn a short input burst into a
       silently dropped make/break sequence at its old fixed-64 boundary. */
    {
        app_control_queue *queue = NULL;
        ux_input_event event = { 0 };
        app_control_event copied;
        unsigned int index;
        assert(app_control_queue_create(&queue));
        event.type = UX_EVENT_TEXT;
        for (index = 0u; index < 96u; ++index) {
            event.data.text.scalar = index;
            assert(app_control_queue_push_ux(queue, &event));
        }
        for (index = 0u; index < 96u; ++index) {
            assert(app_control_queue_take(queue, &copied, 0u));
            assert(copied.kind == APP_CONTROL_UX_INPUT);
            assert(copied.value.ux.data.text.scalar == index);
        }
        app_control_queue_destroy(queue);
    }
    {
        app_input_queue *queue = NULL;
        ux_event event = { 0 };

        assert(app_input_queue_create(&queue));
        event.type = UX_EVENT_KEY;
        event.data.key.scan_code = 0x1eu;
        event.data.key.pressed = 1u;
        assert(app_input_queue_push(queue, &event));
        assert(app_input_queue_pending(queue));
        app_input_queue_clear(queue);
        assert(!app_input_queue_pending(queue));
        app_input_queue_destroy(queue);
    }
    assert(app_runtime_start(runtime));
    first_run = app_runtime_run_generation(runtime);
    assert(first_run != 0u);
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
                frame->cursor_visible != 0u && frame->cursor_phase != 0u) {
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
    assert(app_runtime_published_frame_run_generation(runtime) == first_run);
    assert(frame->sequence != 0u);
    {
        uint32_t stable_sequence = frame->sequence;
        LONG stable_state_facts;
        /* An unchanged text screen is not an executor heartbeat.  Repeated
           publication would flood the app control FIFO and starve Console
           raw input behind redundant frame completions. */
        Sleep(150u);
        assert(app_runtime_copy_frame(runtime, frame));
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
       Console/Window selection belong to the app presentation reconciler,
       not a shared UX target router. */
    /* Lifecycle intent is interpreted by the SoftPC control/reconciler;
       this runtime unit directly proves the executor request/completion ABI. */
    assert(app_runtime_pause(runtime));
    assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_PAUSED));
    assert(app_runtime_set_floppy(runtime, NULL));
    assert(app_runtime_resume(runtime));
    assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_RUNNING));
    assert(app_runtime_stop(runtime));
    assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_STOPPED));
    /* A monitor `start` after `stop` is a new cold run, not merely an
       accepted request.  Waiting for RUNNING catches a restart that reaches
       BIOS setup but never re-enters the executor. */
    assert(app_runtime_start(runtime));
    assert(app_runtime_run_generation(runtime) != first_run);
    assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_RUNNING));
    {
        app_reconciler reset;
        uint32_t reset_run;

        /* Drive the same one-way reset plan that monitor control uses:
           running -> stop -> cold start -> pause -> resume -> running. */
        app_reconciler_initialize(&reset, SOFTPC_PRESENTATION_CONSOLE, 1);
        app_reconciler_note_runtime(&reset, SOFTPC_RUNTIME_RUNNING);
        app_reconciler_note_intent(&reset, APP_RECONCILER_INTENT_RESET);
        assert(app_reconciler_take_action(&reset) ==
            APP_RECONCILER_ACTION_RUNTIME_STOP);
        assert(app_runtime_stop(runtime));
        assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_STOPPED));
        app_reconciler_note_runtime(&reset, SOFTPC_RUNTIME_STOPPED);
        assert(app_reconciler_take_action(&reset) ==
            APP_RECONCILER_ACTION_RUNTIME_START);
        assert(app_runtime_start(runtime));
        reset_run = app_runtime_run_generation(runtime);
        assert(reset_run != first_run);
        assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_RUNNING));
        app_reconciler_note_runtime(&reset, SOFTPC_RUNTIME_RUNNING);
        assert(app_reconciler_take_action(&reset) ==
            APP_RECONCILER_ACTION_RUNTIME_PAUSE);
        assert(app_runtime_pause(runtime));
        assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_PAUSED));
        app_reconciler_note_runtime(&reset, SOFTPC_RUNTIME_PAUSED);
        assert(app_reconciler_next_action(&reset) ==
            APP_RECONCILER_ACTION_NONE);
        app_reconciler_note_intent(&reset, APP_RECONCILER_INTENT_RESUME);
        assert(app_reconciler_take_action(&reset) ==
            APP_RECONCILER_ACTION_RUNTIME_RESUME);
        assert(app_runtime_resume(runtime));
        assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_RUNNING));
    }
    assert(app_runtime_stop(runtime));
    assert(app_runtime_wait(runtime, SOFTPC_RUNTIME_STOPPED));
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
