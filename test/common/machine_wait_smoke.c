#include "common/machine/machine_interface.h"
#include "lib/base/sync_interface.h"
#include <assert.h>

/* Run the real worker synchronously; only its scheduling waits are scripted. */
static common_machine *active;
static base_sync_wait_result wait_result;
static unsigned idle_waits, paused_waits, resets, stops, cleanups, facts;
static unsigned action;
static lib_bool heartbeat;
static common_machine_executor_callback executor;
static void *executor_context;
static common_machine_state observed[8];
static lib_bool reject_destroy;
static lib_status frame_status;
static unsigned pending_kind;
static lib_bool terminate_before_submit;
static void request_lock(base_sync_mutex *mutex);
static void register_pending(void);
static lib_status destroy_task(base_sync_task *task)
{
    if (reject_destroy) return LIB_STATUS_IO_ERROR;
    return task == (base_sync_task *)1 ? LIB_STATUS_OK : base_sync_task_destroy(task);
}
static base_sync_wait_result paused_wait(base_sync_event *const *events,
    lib_u32 count, const base_sync_task *task, lib_u32 timeout, lib_u32 *index);
static base_sync_wait_result idle_wait(base_sync_event *event, lib_u32 timeout);
static lib_status no_thread(base_sync_task_entry entry, void *context,
    base_sync_task **out)
{ (void)entry; (void)context; *out = NULL; return LIB_STATUS_OK; }
static lib_bool not_cancelled(const base_sync_task *task)
{ (void)task; return LIB_FALSE; }
#define base_sync_task_create no_thread
#define base_sync_task_destroy destroy_task
#define base_sync_task_cancelled not_cancelled
#define base_sync_event_wait idle_wait
#define base_sync_wait_any paused_wait
#define base_sync_mutex_lock request_lock
#include "common/machine/machine.c"
#undef base_sync_mutex_lock
#undef base_sync_wait_any
#undef base_sync_event_wait
#undef base_sync_task_cancelled
#undef base_sync_task_create
#undef base_sync_task_destroy

static void request_lock(base_sync_mutex *mutex)
{
    if (terminate_before_submit && mutex == active->request_lock) {
        terminate_before_submit = LIB_FALSE;
        common_machine_finish_requests(active, COMMON_MACHINE_ERROR);
    }
    base_sync_mutex_lock(mutex);
}

static void register_pending(void)
{
    switch (pending_kind) {
    case 1:
        lib_atomic_i32_store_explicit(&active->state_read_requested, 1, LIB_MEMORY_ORDER_SEQ_CST);
        lib_atomic_i32_store_explicit(&active->state_read_armed, 1, LIB_MEMORY_ORDER_SEQ_CST);
        active->state_read_continuing = LIB_TRUE;
        break;
    case 2:
        lib_atomic_i32_store_explicit(&active->state_write_waiting, 1, LIB_MEMORY_ORDER_SEQ_CST);
        break;
    case 3:
        lib_atomic_i32_store_explicit(&active->debug_requested, 1, LIB_MEMORY_ORDER_SEQ_CST);
        break;
    case 4:
        lib_atomic_i32_store_explicit(&active->media_requested, 1, LIB_MEMORY_ORDER_SEQ_CST);
        break;
    case 5:
        lib_atomic_i32_store_explicit(&active->state_write_requested, 1, LIB_MEMORY_ORDER_SEQ_CST);
        break;
    }
}

static base_sync_wait_result idle_wait(base_sync_event *event, lib_u32 timeout)
{
    assert(event == active->command_event && timeout == LIB_UINT32_MAX);
    ++idle_waits;
    assert(idle_waits <= 3u);
    if (action == 4u) {
        register_pending();
        return BASE_SYNC_WAIT_FAULT;
    }
    if (idle_waits > 1u && !(action == 2u && resets == 1u))
        lib_atomic_i32_exchange_explicit(&active->terminate_requested, 1, LIB_MEMORY_ORDER_SEQ_CST);
    return BASE_SYNC_WAIT_SIGNALED;
}

static base_sync_wait_result paused_wait(base_sync_event *const *events,
    lib_u32 count, const base_sync_task *task, lib_u32 timeout, lib_u32 *index)
{
    (void)task;
    assert(count == 3u && events[0] == active->resume_event);
    assert(timeout == LIB_UINT32_MAX);
    ++paused_waits;
    register_pending();
    assert(paused_waits <= (action == 2u ? 2u : 1u));
    if (action == 3u) assert(common_machine_reset(active));
    if (wait_result != BASE_SYNC_WAIT_SIGNALED) return wait_result;
    if (action == 0u) assert(common_machine_resume(active));
    else if (action == 2u && resets == 1u) {
        assert(common_machine_reset(active));
        lib_atomic_i32_store_explicit(&active->state_read_requested, 1, LIB_MEMORY_ORDER_SEQ_CST);
    }
    else assert(common_machine_stop(active));
    *index = 0u;
    return BASE_SYNC_WAIT_SIGNALED;
}

static lib_bool reset(void *context)
{
    (void)context;
    if (++resets == 2u) {
        assert(active->state_status == LIB_STATUS_IO_ERROR);
        assert(!lib_atomic_i32_load_explicit(&active->state_read_requested, LIB_MEMORY_ORDER_SEQ_CST));
        assert(base_sync_event_wait(active->state_event, 0u) == BASE_SYNC_WAIT_SIGNALED);
        base_sync_event_reset(active->state_event);
        active->state_status = LIB_STATUS_UNSUPPORTED;
    }
    return LIB_TRUE;
}
static void stop(void *context)
{ (void)context; ++stops; }
static void set_heartbeat(void *context, lib_bool enabled)
{ (void)context; heartbeat = enabled; }
static void callback(void *context, common_machine_executor_callback entry, void *opaque)
{
    (void)context; executor = entry; executor_context = opaque;
    if (entry == NULL) ++cleanups;
}
static lib_bool run(void *context)
{
    (void)context;
    assert(heartbeat && executor != NULL);
    if (frame_status != LIB_STATUS_OK) register_pending();
    assert(common_machine_pause(active));
    executor(executor_context);
    if (frame_status == LIB_STATUS_OK && wait_result == BASE_SYNC_WAIT_SIGNALED && action == 0u)
        assert(common_machine_stop(active));
    assert(stops != 0u);
    return LIB_TRUE; /* Unwinding the driver itself succeeds, even after fault. */
}
static void input(void *context, const kvm_input_event *event)
{ (void)context; (void)event; }
static lib_status frame(void *context, common_machine_frame *value)
{ (void)context; value->window.valid = 0u; return frame_status; }
static void state(void *context, common_machine_state value, lib_u32 generation)
{
    (void)context;
    assert(generation == (action == 4u ? 1u : resets) && facts < 8u);
    if (value == COMMON_MACHINE_STOPPED || value == COMMON_MACHINE_ERROR) {
        assert(!heartbeat && executor == NULL && cleanups == resets);
        if (pending_kind != 0u) {
            base_sync_event *event = pending_kind == 3u ? active->debug_event :
                pending_kind == 4u ? active->media_event : active->state_event;
            assert(base_sync_event_wait(event, 0u) == BASE_SYNC_WAIT_SIGNALED);
            assert(active->state_status == LIB_STATUS_IO_ERROR ||
                pending_kind == 3u || pending_kind == 4u);
            if (pending_kind == 3u) assert(active->debug_status == LIB_STATUS_IO_ERROR);
            if (pending_kind == 4u) assert(!active->media_succeeded);
            assert(!active->state_read_continuing);
            /* Consume completion; subsequent worker disposal must not send it again. */
            assert(base_sync_event_reset(event) == LIB_STATUS_OK);
        }
    }
    observed[facts++] = value;
}

static void check(base_sync_wait_result result, unsigned requested_action)
{
    common_machine_driver driver = {0};
    wait_result = result; action = requested_action;
    idle_waits = paused_waits = resets = stops = cleanups = facts = 0u;
    driver.reset = reset; driver.run = run; driver.request_stop = stop;
    driver.request_wake = stop; driver.set_heartbeat = set_heartbeat;
    driver.set_executor_callback = callback; driver.deliver_input = input;
    driver.copy_frame = frame;
    assert(common_machine_create(&active, &driver) == LIB_STATUS_OK);
    active->state_status = active->debug_status = LIB_STATUS_UNSUPPORTED;
    active->media_succeeded = LIB_TRUE;
    common_machine_set_state_sink(active, state, NULL);
    common_machine_begin_cold_run(active, LIB_FALSE);
    common_machine_worker(active, NULL);
    if (action == 4u) {
        assert(facts == 1u && observed[0] == COMMON_MACHINE_ERROR);
        assert(resets == 0u && cleanups == 0u && paused_waits == 0u);
    } else if (frame_status != LIB_STATUS_OK) {
        assert(facts == 2u && observed[1] == COMMON_MACHINE_ERROR);
        assert(paused_waits == 0u && stops == 1u && cleanups == 1u);
        assert(active->published_frame_sequence == 0u);
    } else if (result != BASE_SYNC_WAIT_SIGNALED) {
        assert(observed[1] == COMMON_MACHINE_PAUSED);
        common_machine_state terminal = result == BASE_SYNC_WAIT_CANCELLED ?
            COMMON_MACHINE_STOPPED : COMMON_MACHINE_ERROR;
        assert(paused_waits == 1u && resets == 1u && cleanups == 1u);
        assert(facts == 3u && observed[2] == terminal);
        assert(common_machine_state_get(active) == terminal);
    } else if (action == 0u) {
        assert(facts == 4u && observed[2] == COMMON_MACHINE_RUNNING);
        assert(observed[3] == COMMON_MACHINE_STOPPED);
    } else if (action == 2u) {
        assert(resets == 2u && paused_waits == 2u && facts == 4u);
        assert(observed[2] == COMMON_MACHINE_RESET_COMPLETED);
        assert(observed[3] == COMMON_MACHINE_STOPPED);
    } else assert(facts == 3u && observed[2] == COMMON_MACHINE_STOPPED);
    if (pending_kind != 0u) {
        assert(base_sync_event_wait(active->state_event, 0u) == BASE_SYNC_WAIT_TIMED_OUT);
        assert(base_sync_event_wait(active->media_event, 0u) == BASE_SYNC_WAIT_TIMED_OUT);
        assert(base_sync_event_wait(active->debug_event, 0u) == BASE_SYNC_WAIT_TIMED_OUT);
        assert(!lib_atomic_i32_load_explicit(&active->state_read_requested, LIB_MEMORY_ORDER_SEQ_CST));
        assert(!lib_atomic_i32_load_explicit(&active->state_read_armed, LIB_MEMORY_ORDER_SEQ_CST));
        assert(!lib_atomic_i32_load_explicit(&active->state_read_ready, LIB_MEMORY_ORDER_SEQ_CST));
        assert(!lib_atomic_i32_load_explicit(&active->state_write_requested, LIB_MEMORY_ORDER_SEQ_CST));
        assert(!lib_atomic_i32_load_explicit(&active->state_write_waiting, LIB_MEMORY_ORDER_SEQ_CST));
        assert(!lib_atomic_i32_load_explicit(&active->debug_requested, LIB_MEMORY_ORDER_SEQ_CST));
        assert(!lib_atomic_i32_load_explicit(&active->media_requested, LIB_MEMORY_ORDER_SEQ_CST));
    } else {
        assert(active->state_status == LIB_STATUS_UNSUPPORTED);
        assert(active->debug_status == LIB_STATUS_UNSUPPORTED && active->media_succeeded);
    }
    /* A dead worker rejects all registrations, even with an allowed state. */
    assert(!common_machine_submit_request(active, &active->state_write_requested,
        active->state_event, 1u << common_machine_state_get(active)));
    /* Deterministically finish between the public precheck and registration. */
    lib_atomic_i32_store_explicit(&active->terminate_requested, 0, LIB_MEMORY_ORDER_SEQ_CST);
    lib_atomic_i32_store_explicit(&active->state, COMMON_MACHINE_RUNNING, LIB_MEMORY_ORDER_SEQ_CST);
    terminate_before_submit = LIB_TRUE;
    assert(!common_machine_submit_request(active, &active->state_read_requested,
        active->state_event, 1u << COMMON_MACHINE_RUNNING));
    assert(!terminate_before_submit && !lib_atomic_i32_load_explicit(
        &active->state_read_requested, LIB_MEMORY_ORDER_SEQ_CST));
    /* A failed join must retain the worker and every referenced machine field. */
    active->worker = (base_sync_task *)1;
    reject_destroy = LIB_TRUE;
    assert(common_machine_shutdown(active) == LIB_STATUS_IO_ERROR);
    assert(common_machine_destroy(active) == LIB_STATUS_IO_ERROR);
    assert(active->worker == (base_sync_task *)1 && active->frame_buffers[0]);
    assert(base_sync_event_signal(active->command_event) == LIB_STATUS_OK);
    reject_destroy = LIB_FALSE;
    assert(common_machine_shutdown(active) == LIB_STATUS_OK && !active->worker);
    assert(common_machine_destroy(active) == LIB_STATUS_OK);
}

int main(void)
{
    /* Resource-only changes are publications, not just character changes. */
    static common_machine_frame before, after;
    before.window.valid = 1u;
    before.window.text.base.text_columns = 80u;
    before.window.text.base.text_rows = 25u;
    after = before;
    assert(!common_machine_text_frame_changed(&before, &after));
    after.characters.primary[65] = 0x263au;
    assert(common_machine_text_frame_changed(&before, &after));
    before = after;
    after.characters.secondary[65] = 0x2665u;
    assert(common_machine_text_frame_changed(&before, &after));
    before = after;
    after.window.text.font[65 * 16] = 0xffu;
    assert(common_machine_text_frame_changed(&before, &after));
    before = after;
    after.window.text.secondary_font[65 * 16] = 0x81u;
    assert(common_machine_text_frame_changed(&before, &after));
    check(BASE_SYNC_WAIT_FAULT, 0u);
    check(BASE_SYNC_WAIT_INVALID_ARGUMENT, 0u);
    check(BASE_SYNC_WAIT_TIMED_OUT, 0u);
    check(BASE_SYNC_WAIT_CANCELLED, 0u);
    check(BASE_SYNC_WAIT_FAULT, 3u); /* A reset arriving at the failed wait. */
    check(BASE_SYNC_WAIT_CANCELLED, 3u);
    check(BASE_SYNC_WAIT_SIGNALED, 0u); /* Resume. */
    check(BASE_SYNC_WAIT_SIGNALED, 1u); /* Stop. */
    check(BASE_SYNC_WAIT_SIGNALED, 2u); /* Reset, then stop. */
    frame_status = LIB_STATUS_UNSUPPORTED;
    check(BASE_SYNC_WAIT_SIGNALED, 0u);
    frame_status = LIB_STATUS_INVALID_ARGUMENT;
    check(BASE_SYNC_WAIT_SIGNALED, 0u);
    for (pending_kind = 1u; pending_kind <= 5u; ++pending_kind) {
        frame_status = LIB_STATUS_IO_ERROR;
        check(BASE_SYNC_WAIT_SIGNALED, 0u);
        frame_status = LIB_STATUS_OK;
        check(BASE_SYNC_WAIT_FAULT, 0u);
        check(BASE_SYNC_WAIT_CANCELLED, 0u);
        check(BASE_SYNC_WAIT_FAULT, 4u);
    }
    return 0;
}
