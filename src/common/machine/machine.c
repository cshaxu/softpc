#include "common/machine/machine_interface.h"
#include "common/machine/input_queue.h"
#include "lib/host/sync_interface.h"

#include <windows.h>
#include <stdlib.h>
#include <string.h>

struct common_machine {
    common_machine_driver driver;
    common_machine_input_queue *input_queue;
    kvm_frame *frame_buffers[2];
    CRITICAL_SECTION frame_lock;
    lib_bool frame_lock_initialized;
    int published_frame_index;
    lib_u32 published_frame_sequence;
    volatile LONG published_frame_run_generation;
    host_sync_event *command_event;
    host_sync_event *ready_event;
    host_sync_event *resume_event;
    host_sync_event *input_event;
    host_sync_event *media_event;
    host_sync_event *debug_event;
    common_machine_debug_request debug_request;
    common_machine_debug_result debug_result;
    common_machine_debug_lease debug_lease;
    lib_status debug_status;
    volatile LONG debug_requested;
    volatile LONG debug_cancel_requested;
    host_sync_task *worker;
    volatile LONG state;
    volatile LONG run_generation;
    volatile LONG debug_generation;
    volatile LONG pause_requested;
    volatile LONG stop_requested;
    volatile LONG start_requested;
    volatile LONG reset_requested;
    volatile LONG reset_active;
    volatile LONG terminate_requested;
    volatile LONG media_requested;
    common_machine_state_sink state_sink;
    void *state_context;
    common_machine_frame_sink frame_sink;
    void *frame_context;
    lib_bool media_succeeded;
    char media_path[COMMON_MACHINE_PATH_CAPACITY];
};

static void common_machine_notify_state(common_machine *machine,
    common_machine_state completed)
{
    if (machine != NULL && machine->state_sink != NULL)
        machine->state_sink(machine->state_context, completed,
            common_machine_run_generation(machine));
}

void common_machine_debug_invalidate(common_machine *machine)
{
    if (machine != NULL) (void)InterlockedIncrement(&machine->debug_generation);
}

static void common_machine_invalidate_published_frame(common_machine *machine)
{
    if (machine == NULL) return;
    EnterCriticalSection(&machine->frame_lock);
    memset(machine->frame_buffers[0], 0, sizeof(*machine->frame_buffers[0]));
    memset(machine->frame_buffers[1], 0, sizeof(*machine->frame_buffers[1]));
    machine->published_frame_index = 0;
    InterlockedExchange(&machine->published_frame_run_generation, 0);
    LeaveCriticalSection(&machine->frame_lock);
}

static lib_bool common_machine_text_frame_changed(const kvm_frame *previous,
    const kvm_frame *candidate)
{
    if (previous == NULL || previous->valid == 0u || previous->graphics != 0u)
        return LIB_TRUE;
    return previous->text_columns != candidate->text_columns ||
        previous->text_rows != candidate->text_rows ||
        previous->cursor_column != candidate->cursor_column ||
        previous->cursor_row != candidate->cursor_row ||
        previous->cursor_top != candidate->cursor_top ||
        previous->cursor_bottom != candidate->cursor_bottom ||
        previous->cursor_visible != candidate->cursor_visible ||
        previous->cursor_phase != candidate->cursor_phase ||
        previous->font_height != candidate->font_height ||
        previous->attribute_font_select != candidate->attribute_font_select ||
        memcmp(previous->text, candidate->text, sizeof(candidate->text)) != 0 ||
        memcmp(previous->attributes, candidate->attributes,
            sizeof(candidate->attributes)) != 0 ||
        memcmp(previous->text_palette, candidate->text_palette,
            sizeof(candidate->text_palette)) != 0 ||
        memcmp(previous->font, candidate->font, sizeof(candidate->font)) != 0 ||
        memcmp(previous->secondary_font, candidate->secondary_font,
            sizeof(candidate->secondary_font)) != 0;
}

static void common_machine_publish(common_machine *machine)
{
    kvm_frame *frame;
    int staging_index;
    common_machine_frame_sink sink = NULL;
    void *sink_context = NULL;
    lib_u32 sequence = 0u;
    lib_u32 generation = 0u;
    lib_bool graphics = LIB_FALSE;
    kvm_frame *published_frame = NULL;

    if (machine == NULL || machine->driver.copy_frame == NULL) return;
    EnterCriticalSection(&machine->frame_lock);
    staging_index = machine->published_frame_index == 0 ? 1 : 0;
    frame = machine->frame_buffers[staging_index];
    if (frame == NULL || !machine->driver.copy_frame(machine->driver.context, frame)) {
        LeaveCriticalSection(&machine->frame_lock);
        return;
    }
    if (frame->valid == 0u || (frame->graphics == 0u &&
        !common_machine_text_frame_changed(
            machine->frame_buffers[machine->published_frame_index], frame))) {
        LeaveCriticalSection(&machine->frame_lock);
        return;
    }
    frame->sequence = ++machine->published_frame_sequence;
    generation = common_machine_run_generation(machine);
    InterlockedExchange(&machine->published_frame_run_generation, generation);
    machine->published_frame_index = staging_index;
    sink = machine->frame_sink;
    sink_context = machine->frame_context;
    sequence = frame->sequence;
    graphics = frame->graphics != 0u;
    published_frame = frame;
    LeaveCriticalSection(&machine->frame_lock);
    if (machine->driver.frame_published != NULL)
        machine->driver.frame_published(machine->driver.context, published_frame);
    if (sink != NULL)
        sink(sink_context, sequence, graphics, generation);
}

static void common_machine_drain_input(common_machine *machine)
{
    kvm_input_event event;
    if (machine == NULL || machine->driver.deliver_input == NULL) return;
    if (common_machine_input_queue_pop(machine->input_queue, &event)) {
        machine->driver.deliver_input(machine->driver.context, &event);
        if (common_machine_input_queue_pending(machine->input_queue) &&
            machine->driver.request_wake != NULL) {
            /* The driver uses this wake edge to schedule another safe host
             * callback; it is not a lifecycle stop request. */
            machine->driver.request_wake(machine->driver.context);
        }
    }
}

static void common_machine_service_media(common_machine *machine)
{
    if (machine == NULL || InterlockedExchange(&machine->media_requested, 0) == 0)
        return;
    machine->media_succeeded = machine->driver.set_removable_media != NULL &&
        machine->driver.set_removable_media(machine->driver.context,
            machine->media_path[0] == '\0' ? NULL : machine->media_path);
    host_sync_event_signal(machine->media_event);
}

/* The control thread submits one synchronous operation at a time. Only the
 * existing executor calls the driver, including while parked in PAUSED. */
static void common_machine_service_debug(common_machine *machine)
{
    if (InterlockedExchange(&machine->debug_cancel_requested, 0) != 0 &&
        machine->driver.cancel_debug != NULL)
        machine->driver.cancel_debug(machine->driver.context);
    if (InterlockedExchange(&machine->debug_requested, 0) == 0) return;
    machine->debug_status = LIB_STATUS_INVALID_STATE;
    memset(&machine->debug_result, 0, sizeof(machine->debug_result));
    if (common_machine_state_get(machine) == COMMON_MACHINE_PAUSED &&
        InterlockedCompareExchange(&machine->pause_requested, 0, 0) != 0 &&
        machine->debug_lease.generation == (lib_u64)(lib_u32)
            InterlockedCompareExchange(&machine->debug_generation, 0, 0))
        machine->debug_status = machine->driver.execute_debug(
            machine->driver.context, &machine->debug_request, &machine->debug_result);
    host_sync_event_signal(machine->debug_event);
}

static void common_machine_executor_event(void *opaque)
{
    common_machine *machine = (common_machine *)opaque;
    lib_bool debug_stop = machine->driver.take_debug_stop != NULL &&
        machine->driver.take_debug_stop(machine->driver.context);
    if ((InterlockedExchange(&machine->debug_cancel_requested, 0) != 0 ||
         InterlockedCompareExchange(&machine->pause_requested, 0, 0) != 0) &&
        machine->driver.cancel_debug != NULL) {
        machine->driver.cancel_debug(machine->driver.context);
        debug_stop = LIB_FALSE;
    }
    if (debug_stop) InterlockedExchange(&machine->pause_requested, 1);
    common_machine_drain_input(machine);
    common_machine_publish(machine);
    if (InterlockedCompareExchange(&machine->pause_requested, 0, 0) != 0 &&
        InterlockedCompareExchange(&machine->stop_requested, 0, 0) == 0) {
        InterlockedExchange(&machine->state, COMMON_MACHINE_PAUSED);
        if (InterlockedExchange(&machine->reset_active, 0) != 0)
            common_machine_notify_state(machine, COMMON_MACHINE_RESET_COMPLETED);
        else
            common_machine_notify_state(machine, COMMON_MACHINE_PAUSED);
        while (InterlockedCompareExchange(&machine->pause_requested, 0, 0) != 0 &&
            InterlockedCompareExchange(&machine->stop_requested, 0, 0) == 0) {
            host_sync_event *events[3] = { machine->resume_event,
                machine->command_event, machine->input_event };
            lib_u32 index = UINT32_MAX;
            if (host_sync_wait_any(events, 3u, machine->worker, UINT32_MAX,
                    &index) != HOST_SYNC_WAIT_SIGNALED)
                continue;
            if (index == 0u)
                host_sync_event_reset(machine->resume_event);
            else if (index == 1u) {
                host_sync_event_reset(machine->command_event);
                common_machine_service_media(machine);
                common_machine_service_debug(machine);
            } else {
                host_sync_event_reset(machine->input_event);
                common_machine_drain_input(machine);
            }
        }
        if (InterlockedCompareExchange(&machine->stop_requested, 0, 0) == 0) {
            InterlockedExchange(&machine->state, COMMON_MACHINE_RUNNING);
            common_machine_notify_state(machine, COMMON_MACHINE_RUNNING);
        }
    }
}

static void common_machine_begin_cold_run(common_machine *machine,
    lib_bool pause_after_start)
{
    common_machine_debug_invalidate(machine);
    common_machine_input_queue_clear(machine->input_queue);
    common_machine_invalidate_published_frame(machine);
    InterlockedExchange(&machine->pause_requested, pause_after_start != 0);
    InterlockedExchange(&machine->stop_requested, 0);
    InterlockedExchange(&machine->state, COMMON_MACHINE_STARTING);
    (void)InterlockedIncrement(&machine->run_generation);
    InterlockedExchange(&machine->start_requested, 1);
}

static lib_bool common_machine_schedule_cold_run(common_machine *machine,
    lib_bool pause_after_start)
{
    if (machine == NULL || InterlockedCompareExchange(&machine->state, 0, 0) !=
        COMMON_MACHINE_STOPPED) return LIB_FALSE;
    host_sync_event_reset(machine->ready_event);
    common_machine_begin_cold_run(machine, pause_after_start);
    host_sync_event_signal(machine->command_event);
    return LIB_TRUE;
}

static void common_machine_worker(void *opaque, const host_sync_task *task)
{
    common_machine *machine = (common_machine *)opaque;
    for (;;) {
        lib_bool succeeded;
        if (host_sync_event_wait(machine->command_event, UINT32_MAX) !=
            HOST_SYNC_WAIT_SIGNALED || host_sync_task_cancelled(task)) break;
        host_sync_event_reset(machine->command_event);
        if (InterlockedCompareExchange(&machine->terminate_requested, 0, 0) != 0)
            break;
        common_machine_service_debug(machine);
        if (InterlockedCompareExchange(&machine->media_requested, 0, 0) != 0) {
            common_machine_service_media(machine);
            continue;
        }
        if (InterlockedExchange(&machine->start_requested, 0) == 0) continue;
        succeeded = machine->driver.reset(machine->driver.context);
        if (!succeeded) {
            InterlockedExchange(&machine->state, COMMON_MACHINE_ERROR);
            common_machine_notify_state(machine, COMMON_MACHINE_ERROR);
            host_sync_event_signal(machine->ready_event);
            continue;
        }
        if (InterlockedCompareExchange(&machine->stop_requested, 0, 0) != 0) {
            InterlockedExchange(&machine->state, COMMON_MACHINE_STOPPED);
            common_machine_notify_state(machine, COMMON_MACHINE_STOPPED);
            host_sync_event_signal(machine->ready_event);
            continue;
        }
        machine->driver.set_executor_callback(machine->driver.context,
            common_machine_executor_event, machine);
        machine->driver.set_heartbeat(machine->driver.context, LIB_TRUE);
        InterlockedExchange(&machine->state, COMMON_MACHINE_RUNNING);
        if (InterlockedCompareExchange(&machine->reset_active, 0, 0) == 0)
            common_machine_notify_state(machine, COMMON_MACHINE_RUNNING);
        host_sync_event_signal(machine->ready_event);
        do {
            succeeded = machine->driver.run(machine->driver.context);
        } while (succeeded &&
            InterlockedCompareExchange(&machine->stop_requested, 0, 0) == 0 &&
            InterlockedCompareExchange(&machine->terminate_requested, 0, 0) == 0);
        machine->driver.set_heartbeat(machine->driver.context, LIB_FALSE);
        machine->driver.set_executor_callback(machine->driver.context, NULL, NULL);
        if (machine->driver.cancel_debug != NULL)
            machine->driver.cancel_debug(machine->driver.context);
        common_machine_debug_invalidate(machine);
        if (succeeded && InterlockedExchange(&machine->reset_requested, 0) != 0) {
            InterlockedExchange(&machine->stop_requested, 0);
            common_machine_begin_cold_run(machine, LIB_TRUE);
            host_sync_event_signal(machine->command_event);
            continue;
        }
        InterlockedExchange(&machine->state, succeeded ? COMMON_MACHINE_STOPPED :
            COMMON_MACHINE_ERROR);
        common_machine_notify_state(machine, succeeded ? COMMON_MACHINE_STOPPED :
            COMMON_MACHINE_ERROR);
        host_sync_event_signal(machine->ready_event);
    }
}

lib_status common_machine_create(common_machine **out_machine,
    const common_machine_driver *driver)
{
    common_machine *machine;
    if (out_machine == NULL || driver == NULL || driver->reset == NULL ||
        driver->run == NULL || driver->request_stop == NULL ||
        driver->request_wake == NULL ||
        driver->set_heartbeat == NULL || driver->set_executor_callback == NULL ||
        driver->deliver_input == NULL || driver->copy_frame == NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_machine = NULL;
    machine = calloc(1u, sizeof(*machine));
    if (machine == NULL) return LIB_STATUS_NO_MEMORY;
    machine->driver = *driver;
    if (host_sync_event_create(&machine->command_event) != LIB_STATUS_OK ||
        host_sync_event_create(&machine->ready_event) != LIB_STATUS_OK ||
        host_sync_event_create(&machine->resume_event) != LIB_STATUS_OK ||
        host_sync_event_create(&machine->input_event) != LIB_STATUS_OK ||
        host_sync_event_create(&machine->media_event) != LIB_STATUS_OK ||
        host_sync_event_create(&machine->debug_event) != LIB_STATUS_OK ||
        common_machine_input_queue_create(&machine->input_queue) != LIB_STATUS_OK ||
        (machine->frame_buffers[0] = calloc(1u, sizeof(*machine->frame_buffers[0]))) == NULL ||
        (machine->frame_buffers[1] = calloc(1u, sizeof(*machine->frame_buffers[1]))) == NULL) {
        common_machine_destroy(machine);
        return LIB_STATUS_NO_MEMORY;
    }
    machine->state = COMMON_MACHINE_STOPPED;
    machine->debug_generation = 1;
    InitializeCriticalSection(&machine->frame_lock);
    machine->frame_lock_initialized = LIB_TRUE;
    if (host_sync_task_create(common_machine_worker, machine, &machine->worker) !=
        LIB_STATUS_OK) {
        common_machine_destroy(machine);
        return LIB_STATUS_IO_ERROR;
    }
    *out_machine = machine;
    return LIB_STATUS_OK;
}

void common_machine_set_state_sink(common_machine *machine,
    common_machine_state_sink sink, void *context)
{
    if (machine == NULL) return;
    machine->state_sink = sink;
    machine->state_context = context;
}

void common_machine_set_frame_sink(common_machine *machine,
    common_machine_frame_sink sink, void *context)
{
    if (machine == NULL) return;
    machine->frame_sink = sink;
    machine->frame_context = context;
}

lib_bool common_machine_start(common_machine *machine)
{
    return common_machine_schedule_cold_run(machine, LIB_FALSE);
}

void common_machine_debug_cancel(common_machine *machine)
{
    if (machine == NULL) return;
    InterlockedExchange(&machine->debug_cancel_requested, 1);
    host_sync_event_signal(machine->command_event);
    machine->driver.request_wake(machine->driver.context);
}

lib_bool common_machine_pause(common_machine *machine)
{
    if (machine == NULL || InterlockedCompareExchange(&machine->state, 0, 0) !=
        COMMON_MACHINE_RUNNING) return LIB_FALSE;
    InterlockedExchange(&machine->pause_requested, 1);
    return LIB_TRUE;
}

lib_bool common_machine_resume(common_machine *machine)
{
    if (machine == NULL || InterlockedCompareExchange(&machine->state, 0, 0) !=
        COMMON_MACHINE_PAUSED) return LIB_FALSE;
    common_machine_debug_invalidate(machine);
    InterlockedExchange(&machine->pause_requested, 0);
    host_sync_event_signal(machine->resume_event);
    return LIB_TRUE;
}

lib_bool common_machine_stop(common_machine *machine)
{
    LONG state;
    if (machine == NULL) return LIB_FALSE;
    state = InterlockedCompareExchange(&machine->state, 0, 0);
    if (state == COMMON_MACHINE_STOPPED) return LIB_TRUE;
    if (state == COMMON_MACHINE_ERROR) return LIB_FALSE;
    common_machine_debug_invalidate(machine);
    host_sync_event_reset(machine->ready_event);
    InterlockedExchange(&machine->stop_requested, 1);
    InterlockedExchange(&machine->pause_requested, 0);
    host_sync_event_signal(machine->resume_event);
    machine->driver.request_stop(machine->driver.context);
    return LIB_TRUE;
}

lib_bool common_machine_reset(common_machine *machine)
{
    LONG state;
    if (machine == NULL) return LIB_FALSE;
    state = InterlockedCompareExchange(&machine->state, 0, 0);
    if (state == COMMON_MACHINE_STOPPED) {
        InterlockedExchange(&machine->reset_active, 1);
        if (common_machine_schedule_cold_run(machine, LIB_TRUE)) return LIB_TRUE;
        InterlockedExchange(&machine->reset_active, 0);
        return LIB_FALSE;
    }
    if (state != COMMON_MACHINE_RUNNING && state != COMMON_MACHINE_PAUSED)
        return LIB_FALSE;
    common_machine_debug_invalidate(machine);
    InterlockedExchange(&machine->reset_active, 1);
    InterlockedExchange(&machine->reset_requested, 1);
    InterlockedExchange(&machine->stop_requested, 1);
    InterlockedExchange(&machine->pause_requested, 0);
    host_sync_event_signal(machine->resume_event);
    machine->driver.request_stop(machine->driver.context);
    return LIB_TRUE;
}

lib_bool common_machine_set_removable_media(common_machine *machine,
    const char *path)
{
    size_t length;
    LONG state;
    if (machine == NULL || machine->driver.set_removable_media == NULL)
        return LIB_FALSE;
    state = InterlockedCompareExchange(&machine->state, 0, 0);
    if (state != COMMON_MACHINE_STOPPED && state != COMMON_MACHINE_PAUSED)
        return LIB_FALSE;
    if (path == NULL) machine->media_path[0] = '\0';
    else {
        length = strlen(path);
        if (length >= sizeof(machine->media_path)) return LIB_FALSE;
        memcpy(machine->media_path, path, length + 1u);
    }
    host_sync_event_reset(machine->media_event);
    InterlockedExchange(&machine->media_requested, 1);
    host_sync_event_signal(machine->command_event);
    return host_sync_event_wait(machine->media_event, UINT32_MAX) ==
        HOST_SYNC_WAIT_SIGNALED && machine->media_succeeded;
}

common_machine_state common_machine_state_get(const common_machine *machine)
{
    return machine == NULL ? COMMON_MACHINE_ERROR :
        (common_machine_state)InterlockedCompareExchange(
            (volatile LONG *)&machine->state, 0, 0);
}

lib_bool common_machine_enqueue_input(common_machine *machine,
    const kvm_input_event *event)
{
    if (machine == NULL || event == NULL ||
        common_machine_state_get(machine) != COMMON_MACHINE_RUNNING ||
        !common_machine_input_queue_push(machine->input_queue, event))
        return LIB_FALSE;
    host_sync_event_signal(machine->input_event);
    machine->driver.request_wake(machine->driver.context);
    return LIB_TRUE;
}

lib_bool common_machine_copy_published_frame(common_machine *machine,
    kvm_frame *destination, lib_u32 *out_run_generation)
{
    lib_bool copied;
    if (machine == NULL || destination == NULL) return LIB_FALSE;
    EnterCriticalSection(&machine->frame_lock);
    memcpy(destination, machine->frame_buffers[machine->published_frame_index],
        sizeof(*destination));
    if (out_run_generation != NULL)
        *out_run_generation = (lib_u32)InterlockedCompareExchange(
            &machine->published_frame_run_generation, 0, 0);
    copied = destination->valid != 0u;
    LeaveCriticalSection(&machine->frame_lock);
    return copied;
}

lib_u32 common_machine_published_frame_sequence(const common_machine *machine)
{
    lib_u32 sequence;
    if (machine == NULL) return 0u;
    EnterCriticalSection((CRITICAL_SECTION *)&machine->frame_lock);
    sequence = machine->published_frame_sequence;
    LeaveCriticalSection((CRITICAL_SECTION *)&machine->frame_lock);
    return sequence;
}

lib_u32 common_machine_published_frame_run_generation(const common_machine *machine)
{
    lib_u32 generation;
    if (machine == NULL) return 0u;
    EnterCriticalSection((CRITICAL_SECTION *)&machine->frame_lock);
    generation = (lib_u32)InterlockedCompareExchange(
        (volatile LONG *)&machine->published_frame_run_generation, 0, 0);
    LeaveCriticalSection((CRITICAL_SECTION *)&machine->frame_lock);
    return generation;
}

lib_u32 common_machine_run_generation(const common_machine *machine)
{
    return machine == NULL ? 0u : (lib_u32)InterlockedCompareExchange(
        (volatile LONG *)&machine->run_generation, 0, 0);
}

lib_status common_machine_debug_acquire(common_machine *machine,
    common_machine_debug_lease *out_lease)
{
    LONG generation;

    if (machine == NULL || out_lease == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    if (common_machine_state_get(machine) != COMMON_MACHINE_PAUSED)
        return LIB_STATUS_INVALID_STATE;
    if (machine->driver.execute_debug == NULL) return LIB_STATUS_UNSUPPORTED;
    generation = InterlockedCompareExchange(&machine->debug_generation, 0, 0);
    if (generation == 0) return LIB_STATUS_INVALID_STATE;
    out_lease->generation = (lib_u64)(lib_u32)generation;
    return LIB_STATUS_OK;
}

lib_status common_machine_debug_execute_with_lease(common_machine *machine,
    const common_machine_debug_lease *lease,
    const common_machine_debug_request *request,
    common_machine_debug_result *out_result)
{
    LONG generation;

    if (machine == NULL || lease == NULL || request == NULL || out_result == NULL ||
        request->bytes > COMMON_MACHINE_DEBUG_BYTES) return LIB_STATUS_INVALID_ARGUMENT;
    if (common_machine_state_get(machine) != COMMON_MACHINE_PAUSED)
        return LIB_STATUS_INVALID_STATE;
    generation = InterlockedCompareExchange(&machine->debug_generation, 0, 0);
    if (lease->generation == 0u || lease->generation != (lib_u64)(lib_u32)generation)
        return LIB_STATUS_INVALID_STATE;
    if (machine->driver.execute_debug == NULL) return LIB_STATUS_UNSUPPORTED;
    machine->debug_request = *request;
    machine->debug_lease = *lease;
    host_sync_event_reset(machine->debug_event);
    InterlockedExchange(&machine->debug_requested, 1);
    host_sync_event_signal(machine->command_event);
    if (host_sync_event_wait(machine->debug_event, UINT32_MAX) !=
        HOST_SYNC_WAIT_SIGNALED) return LIB_STATUS_IO_ERROR;
    *out_result = machine->debug_result;
    return machine->debug_status;
}

void common_machine_destroy(common_machine *machine)
{
    if (machine == NULL) return;
    common_machine_debug_invalidate(machine);
    (void)common_machine_stop(machine);
    InterlockedExchange(&machine->terminate_requested, 1);
    if (machine->resume_event != NULL) host_sync_event_signal(machine->resume_event);
    if (machine->command_event != NULL) host_sync_event_signal(machine->command_event);
    if (machine->worker != NULL) host_sync_task_destroy(machine->worker);
    host_sync_event_destroy(machine->ready_event);
    host_sync_event_destroy(machine->resume_event);
    host_sync_event_destroy(machine->input_event);
    host_sync_event_destroy(machine->media_event);
    host_sync_event_destroy(machine->debug_event);
    common_machine_input_queue_destroy(machine->input_queue);
    if (machine->frame_lock_initialized)
        DeleteCriticalSection(&machine->frame_lock);
    free(machine->frame_buffers[0]);
    free(machine->frame_buffers[1]);
    host_sync_event_destroy(machine->command_event);
    free(machine);
}
