#include "runtime.h"
#include "keyboard.h"
#include "input_queue.h"
#include "prompt_trace.h"
#include "lib/host/clock.h"
#include "lib/host/sync.h"

#include <windows.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void app_runtime_prompt_trace(uint32_t sequence, uint32_t mode_type,
    uint32_t screen_state, uint32_t graphics, uint32_t columns,
    uint32_t rows, uint32_t stride, uint32_t width, uint32_t height,
    int dirty, int32_t left, int32_t top, int32_t right, int32_t bottom)
{
    static uint32_t prior_mode = UINT32_MAX;
    static uint32_t prior_screen = UINT32_MAX;
    static uint32_t prior_graphics = UINT32_MAX;
    static uint32_t prior_columns = UINT32_MAX;
    static uint32_t prior_rows = UINT32_MAX;
    static uint32_t prior_width = UINT32_MAX;
    static uint32_t prior_height = UINT32_MAX;
    if (!app_prompt_trace_enabled()) return;
    if (prior_mode == mode_type && prior_screen == screen_state &&
        prior_graphics == graphics && prior_columns == columns &&
        prior_rows == rows && prior_width == width && prior_height == height)
        return;
    app_prompt_trace("softpc prompt frame=%lu mode=%lu state=%lu graphics=%lu text=%lux%lu stride=%lu dib=%lux%lu dirty=%d,%ld,%ld,%ld,%ld",
        (unsigned long)sequence, (unsigned long)mode_type,
        (unsigned long)screen_state, (unsigned long)graphics,
        (unsigned long)columns, (unsigned long)rows, (unsigned long)stride,
        (unsigned long)width, (unsigned long)height, dirty, (long)left,
        (long)top, (long)right, (long)bottom);
    prior_mode = mode_type; prior_screen = screen_state;
    prior_graphics = graphics; prior_columns = columns; prior_rows = rows;
    prior_width = width; prior_height = height;
}

struct app_runtime {
    softpc_machine *machine;
    app_input_queue *input_queue;
    ux_frame *frame_buffers[2];
    CRITICAL_SECTION frame_lock;
    int published_frame_index;
    uint32_t published_frame_sequence;
    volatile LONG published_frame_run_generation;
    host_sync_event *command_event;
    host_sync_event *ready_event;
    host_sync_event *resume_event;
    host_sync_event *input_event;
    host_sync_event *media_event;
    host_sync_task *worker;
    volatile LONG state;
    volatile LONG run_generation;
    volatile LONG result;
    volatile LONG pause_requested;
    volatile LONG stop_requested;
    volatile LONG start_requested;
    volatile LONG terminate_requested;
    volatile LONG media_requested;
    volatile LONG window_close_requested;
    volatile LONG window_mouse_release_requested;
    app_runtime_completion_sink completion_sink;
    void *completion_context;
    softpc_machine_result media_result;
    char media_floppy_path[SOFTPC_RUNTIME_PATH_MAX];
    /* The original V7 standard painter may use the left half of a doubled
       host DIB as a backing surface.  This is presentation metadata only;
       it never changes C-VID's DIB or controller geometry. */
    uint32_t graphics_source_width;
    uint32_t graphics_source_height;
    uint32_t graphics_visible_width;
};

static void app_runtime_notify_state(app_runtime *runtime)
{
    if (runtime != NULL && runtime->completion_sink != NULL)
        runtime->completion_sink(runtime->completion_context,
            app_runtime_get_state(runtime), 0u, 0,
            app_runtime_run_generation(runtime));
}

static void app_runtime_publish(app_runtime *runtime)
{
    app_runtime_frame *frame;
    int staging_index;
    int published = 0;
    const void *surface;
    uint32_t columns = 0u;
    uint32_t rows = 0u;
    uint32_t stride = 0u;
    uint32_t cell_bytes;
    uint32_t cursor_size = 0u;
    int32_t cursor_column;
    int32_t cursor_row;
    uint32_t mode_type = 0u, screen_state = 0u;
    int32_t trace_left = -1, trace_top = -1, trace_right = -1, trace_bottom = -1;
    int trace_dirty = 0;
    app_runtime_completion_sink completion_sink = NULL;
    void *completion_context = NULL;
    uint32_t completion_sequence = 0u;
    int completion_graphics = 0;
    uint32_t completion_run = 0u;

    if (runtime == NULL) return;
    EnterCriticalSection(&runtime->frame_lock);
    staging_index = runtime->published_frame_index == 0 ? 1 : 0;
    frame = runtime->frame_buffers[staging_index];
    if (frame == NULL) {
        LeaveCriticalSection(&runtime->frame_lock);
        return;
    }
    memset(frame, 0, sizeof(*frame));
    if (softpc_machine_presentation_is_graphics(runtime->machine)) {
        const void *bits;
        const void *info;
        uint32_t width;
        uint32_t height;
        uint32_t row_stride;
        int32_t ignored_left;
        int32_t ignored_top;
        int32_t ignored_right;
        int32_t ignored_bottom;
        int dirty = softpc_machine_presentation_take_dirty(runtime->machine,
            &ignored_left, &ignored_top, &ignored_right, &ignored_bottom);
        trace_dirty = dirty;
        trace_left = ignored_left; trace_top = ignored_top;
        trace_right = ignored_right; trace_bottom = ignored_bottom;

        /* nt_graph/nt_ega/nt_vga already report their changed DIB rectangle.
           Consume that original presentation signal here, before copying the
           complete host snapshot.  Recopying an unchanged 800 KiB DIB at
           every executor callback is outer-shell work and can starve the
           guest without making its display more current. */
        /* A reset-time DIB allocation is only a host backing store; C-VID has
           not necessarily selected a guest display mode yet. Publish a
           graphics frame only after the original renderer reports a dirty
           rectangle, otherwise a frontend can resize to that maximum scratch
           allocation before the BIOS reaches its real text mode. */
        if (!dirty) goto done;
        if (softpc_machine_presentation_dib(runtime->machine, &bits, &info,
                &width, &height) && bits != NULL && info != NULL &&
            width <= SOFTPC_RUNTIME_DIB_MAX_WIDTH &&
            height <= SOFTPC_RUNTIME_DIB_MAX_HEIGHT) {
            uint32_t visible_width;
            const BITMAPINFO *dib = (const BITMAPINFO *)info;

            row_stride = (width + 3u) & ~3u;
            if (runtime->graphics_source_width != width ||
                runtime->graphics_source_height != height) {
                runtime->graphics_source_width = width;
                runtime->graphics_source_height = height;
                runtime->graphics_visible_width = width;
            }
            /* nt_v7vga_hi_graph_std reports a physical 640-pixel update
               while nt_graph's historical doubled host DIB is 1280 pixels
               wide.  Keep the original painter and its DIB untouched, but
               publish the painted physical image rather than its unused
               black backing half. */
            if (ignored_left == 0 && ignored_top == 0 &&
                ignored_bottom >= (int32_t)height - 1 &&
                ignored_right >= 0 &&
                (uint32_t)(ignored_right + 1) * 2u == width)
                runtime->graphics_visible_width =
                    (uint32_t)(ignored_right + 1);
            visible_width = runtime->graphics_visible_width;
            if (visible_width == 0u || visible_width > width)
                visible_width = width;
            if (visible_width * height <= SOFTPC_RUNTIME_DIB_MAX_BYTES) {
                const uint8_t *source = (const uint8_t *)bits;
                uint32_t row;
                uint32_t palette_index;
                for (row = 0u; row < height; ++row)
                    memcpy(frame->graphics_pixels + row * visible_width,
                        source + row * row_stride, visible_width);
                for (palette_index = 0u;
                        palette_index < UX_GRAPHICS_PALETTE_ENTRIES;
                        ++palette_index) {
                    const RGBQUAD *colour = &dib->bmiColors[palette_index];
                    frame->graphics_palette[palette_index] =
                        ((uint32_t)colour->rgbRed << 16u) |
                        ((uint32_t)colour->rgbGreen << 8u) |
                        (uint32_t)colour->rgbBlue;
                }
                frame->graphics_width = visible_width;
                frame->graphics_height = height;
                frame->graphics_stride = visible_width;
                frame->dirty_left = ignored_left < 0 ? 0 : ignored_left;
                frame->dirty_top = ignored_top < 0 ? 0 : ignored_top;
                frame->dirty_right = ignored_right >= (int32_t)visible_width ?
                    (int32_t)visible_width - 1 : ignored_right;
                frame->dirty_bottom = ignored_bottom >= (int32_t)height ?
                    (int32_t)height - 1 : ignored_bottom;
                frame->graphics = 1u;
                frame->valid = 1u;
                published = 1;
            }
        }
    } else if (softpc_machine_presentation_text(runtime->machine, &surface,
            &columns, &rows, &stride, &cell_bytes) && surface != NULL &&
        cell_bytes >= 1u && stride >= columns) {
        const uint8_t *cells = (const uint8_t *)surface;
        const void *dib_bits;
        const void *dib_info;
        uint32_t dib_width;
        uint32_t dib_height;
        uint32_t row;
        memset(frame->text, ' ', sizeof(frame->text));
        {
            size_t index;
            for (index = 0u; index < sizeof(frame->attributes) /
                    sizeof(frame->attributes[0]); ++index)
                frame->attributes[index] = 0x07u;
        }
        /* nt_graph owns the guest DAC/VLT translation.  Copy its current
           text palette with the text cells so frontends do not substitute a
           fixed EGA table for a guest-programmed VGA palette. */
        if (softpc_machine_presentation_dib(runtime->machine, &dib_bits,
                &dib_info, &dib_width, &dib_height) && dib_info != NULL) {
            const BITMAPINFO *dib = (const BITMAPINFO *)dib_info;
            size_t palette_index;

            (void)dib_bits;
            (void)dib_width;
            (void)dib_height;
            for (palette_index = 0u; palette_index < 16u; ++palette_index) {
                const RGBQUAD *colour = &dib->bmiColors[palette_index];
                frame->text_palette[palette_index] =
                    ((uint32_t)colour->rgbRed << 16u) |
                    ((uint32_t)colour->rgbGreen << 8u) |
                    (uint32_t)colour->rgbBlue;
            }
        }
        if (columns > SOFTPC_RUNTIME_TEXT_COLUMNS)
            columns = SOFTPC_RUNTIME_TEXT_COLUMNS;
        if (rows > SOFTPC_RUNTIME_TEXT_ROWS)
            rows = SOFTPC_RUNTIME_TEXT_ROWS;
        for (row = 0u; row < rows; ++row) {
            uint32_t column;
            for (column = 0u; column < columns; ++column) {
                size_t source = ((size_t)row * stride + column) * cell_bytes;
                size_t destination = (size_t)row * SOFTPC_RUNTIME_TEXT_COLUMNS +
                    column;
                frame->text[destination] = cells[source];
                if (cell_bytes >= 2u)
                    frame->attributes[destination] = cells[source + 1u];
            }
        }
        cursor_column = -1;
        cursor_row = -1;
        (void)softpc_machine_presentation_cursor(runtime->machine,
            &cursor_column, &cursor_row, &cursor_size);
        frame->cursor_column = cursor_column;
        frame->cursor_row = cursor_row;
        (void)softpc_machine_presentation_fonts(runtime->machine, frame->font,
            frame->secondary_font, &frame->font_height,
            &frame->attribute_font_select);
        frame->graphics = 0u;
        frame->text_columns = (uint16_t)columns;
        frame->text_rows = (uint16_t)rows;
        frame->cursor_top = 0u;
        frame->cursor_bottom = cursor_size == 0u ? 15u :
            (uint8_t)((cursor_size * 16u + 99u) / 100u - 1u);
        frame->cursor_visible = cursor_column >= 0 && cursor_row >= 0;
        frame->cursor_phase = 1u;
        frame->dirty_left = 0;
        frame->dirty_top = 0;
        frame->dirty_right = -1;
        frame->dirty_bottom = -1;
        frame->valid = 1u;
        published = 1;
    }
    if (published) {
        frame->sequence = ++runtime->published_frame_sequence;
        InterlockedExchange(&runtime->published_frame_run_generation,
            InterlockedCompareExchange(&runtime->run_generation, 0, 0));
        (void)softpc_machine_presentation_state(runtime->machine, &mode_type,
            &screen_state);
        app_runtime_prompt_trace(frame->sequence, mode_type, screen_state,
            frame->graphics, columns, rows, stride, frame->graphics_width,
            frame->graphics_height, trace_dirty, trace_left, trace_top,
            trace_right, trace_bottom);
        runtime->published_frame_index = staging_index;
        completion_sink = runtime->completion_sink;
        completion_context = runtime->completion_context;
        completion_sequence = frame->sequence;
        completion_graphics = frame->graphics != 0u;
        completion_run = (uint32_t)InterlockedCompareExchange(
            &runtime->run_generation, 0, 0);
    }
done:
    LeaveCriticalSection(&runtime->frame_lock);
    if (completion_sink != NULL)
        completion_sink(completion_context, app_runtime_get_state(runtime),
            completion_sequence, completion_graphics, completion_run);
}

static void app_runtime_drain_input(app_runtime *runtime)
{
    ux_event event;

    /* keyboard_io can enter a nested host_simulate frame for the original
       BIOS INT 15 keyboard hook.  A Windows make/break pair may already be
       queued by the time that callback runs, but injecting both recursively
       into that frame corrupts the original controller's service ordering.
       Deliver precisely one hardware scan event per executor callback; the
       restored 20 Hz host timer naturally schedules the next one. */
    if (app_input_queue_pop(runtime->input_queue, &event)) {
        if (event.type == UX_EVENT_KEY) {
            if (getenv("SOFTPC_INPUT_TRACE") != NULL)
                fprintf(stderr, "softpc input drain scan=%u released=%u\n",
                    (unsigned int)event.data.key.scan_code,
                    (unsigned int)!event.data.key.pressed);
            (void)app_keyboard_inject_machine_event(runtime->machine, &event);
        } else if (event.type == UX_EVENT_MOUSE) {
            (void)softpc_machine_mouse_input(runtime->machine,
                event.data.mouse.delta_x, event.data.mouse.delta_y,
                (event.data.mouse.buttons & UX_MOUSE_BUTTON_LEFT) != 0u,
                (event.data.mouse.buttons & UX_MOUSE_BUTTON_RIGHT) != 0u);
        }
        /* The original keyboard path can re-enter the CCPU while servicing
           one transition.  It remains deliberately one transition per
           executor callback.  If the standalone queue already has another
           transition, arrange a new CCPU-safe callback rather than waiting
           for the unrelated 20 Hz device clock. */
        if (app_input_queue_pending(runtime->input_queue))
            softpc_machine_request_wake(runtime->machine);
    }
}

/* Media replacement is a host command, but the original FDC remains owned
 * by the executor.  A paused executor is deliberately waiting at an
 * evidenced CCPU callback; service the request there instead of letting a
 * monitor thread touch the controller or claiming that paused insertion is
 * supported when it cannot complete. */
static void app_runtime_service_media(app_runtime *runtime)
{
    if (InterlockedExchange(&runtime->media_requested, 0) == 0) return;
    runtime->media_result = softpc_machine_set_floppy(runtime->machine,
        runtime->media_floppy_path[0] == '\0' ? NULL :
        runtime->media_floppy_path);
    host_sync_event_signal(runtime->media_event);
}

static void app_runtime_executor_event(void *opaque)
{
    app_runtime *runtime = (app_runtime *)opaque;
    app_runtime_drain_input(runtime);
    app_runtime_publish(runtime);
    if (InterlockedCompareExchange(&runtime->pause_requested, 0, 0) != 0 &&
        InterlockedCompareExchange(&runtime->stop_requested, 0, 0) == 0) {
        InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_PAUSED);
        while (InterlockedCompareExchange(&runtime->pause_requested, 0, 0) != 0 &&
            InterlockedCompareExchange(&runtime->stop_requested, 0, 0) == 0) {
            host_sync_event *events[3] = { runtime->resume_event,
                runtime->command_event, runtime->input_event };
            lib_u32 event_index = UINT32_MAX;
            if (host_sync_wait_any(events, 3u, runtime->worker, UINT32_MAX,
                    &event_index) != HOST_SYNC_WAIT_SIGNALED)
                continue;
            if (event_index == 0u)
                host_sync_event_reset(runtime->resume_event);
            else if (event_index == 1u) {
                host_sync_event_reset(runtime->command_event);
                app_runtime_service_media(runtime);
            } else { host_sync_event_reset(runtime->input_event);
                app_runtime_drain_input(runtime); }
        }
        if (InterlockedCompareExchange(&runtime->stop_requested, 0, 0) == 0)
            InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_RUNNING);
    }
}

static void app_runtime_worker(void *opaque, const host_sync_task *task)
{
    app_runtime *runtime = (app_runtime *)opaque;
    for (;;) {
        softpc_machine_result result;

        if (host_sync_event_wait(runtime->command_event, UINT32_MAX) !=
            HOST_SYNC_WAIT_SIGNALED || host_sync_task_cancelled(task)) break;
        host_sync_event_reset(runtime->command_event);
        if (InterlockedCompareExchange(&runtime->terminate_requested, 0, 0) != 0)
            break;
        if (InterlockedCompareExchange(&runtime->media_requested, 0, 0) != 0) {
            app_runtime_service_media(runtime);
            continue;
        }
        if (InterlockedExchange(&runtime->start_requested, 0) == 0)
            continue;

        result = softpc_machine_reset(runtime->machine);
        InterlockedExchange(&runtime->result, (LONG)result);
        if (result != SOFTPC_MACHINE_OK) {
            InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_ERROR);
            host_sync_event_signal(runtime->ready_event);
            continue;
        }
        if (InterlockedCompareExchange(&runtime->stop_requested, 0, 0) != 0) {
            InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_STOPPED);
            host_sync_event_signal(runtime->ready_event);
            continue;
        }

        softpc_machine_set_executor_callback(runtime->machine,
            app_runtime_executor_event, runtime);
        softpc_machine_set_heartbeat(runtime->machine, 1);
        InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_RUNNING);
        host_sync_event_signal(runtime->ready_event);
        /* The original CPU uses BOP FE to unwind the *current* host_simulate
           frame after a firmware/device turn.  That is not a guest stop: an
           NTVDM host immediately entered the next frame.  The standalone
           runtime is now that outer host, so keep entering CCPU until an
           explicit stop or an actual machine error.  Do not reset between
           entries; the CCPU/device state remains entirely original SoftPC
           state. */
        do {
            result = softpc_machine_run(runtime->machine, (uint64_t)-1);
        } while (result == SOFTPC_MACHINE_OK &&
            InterlockedCompareExchange(&runtime->stop_requested, 0, 0) == 0 &&
            InterlockedCompareExchange(&runtime->terminate_requested, 0, 0) == 0);
        softpc_machine_set_heartbeat(runtime->machine, 0);
        softpc_machine_set_executor_callback(runtime->machine, NULL, NULL);
        InterlockedExchange(&runtime->result, (LONG)result);
        InterlockedExchange(&runtime->state, result == SOFTPC_MACHINE_OK ?
            SOFTPC_RUNTIME_STOPPED : SOFTPC_RUNTIME_ERROR);
        host_sync_event_signal(runtime->ready_event);
    }
}

int app_runtime_create(softpc_machine *machine, app_runtime **out)
{
    app_runtime *runtime;
    if (machine == NULL || out == NULL) return 0;
    *out = NULL;
    runtime = (app_runtime *)calloc(1u, sizeof(*runtime));
    if (runtime == NULL) return 0;
    runtime->machine = machine;
    if (host_sync_event_create(&runtime->command_event) != LIB_STATUS_OK ||
        host_sync_event_create(&runtime->ready_event) != LIB_STATUS_OK ||
        host_sync_event_create(&runtime->resume_event) != LIB_STATUS_OK ||
        host_sync_event_create(&runtime->input_event) != LIB_STATUS_OK ||
        host_sync_event_create(&runtime->media_event) != LIB_STATUS_OK ||
        !app_input_queue_create(&runtime->input_queue) ||
        (runtime->frame_buffers[0] = calloc(1u,
            sizeof(*runtime->frame_buffers[0]))) == NULL ||
        (runtime->frame_buffers[1] = calloc(1u,
            sizeof(*runtime->frame_buffers[1]))) == NULL) {
        host_sync_event_destroy(runtime->command_event);
        host_sync_event_destroy(runtime->ready_event);
        host_sync_event_destroy(runtime->resume_event);
        host_sync_event_destroy(runtime->input_event);
        host_sync_event_destroy(runtime->media_event);
        app_input_queue_destroy(runtime->input_queue);
        free(runtime->frame_buffers[0]);
        free(runtime->frame_buffers[1]);
        free(runtime);
        return 0;
    }
    runtime->result = SOFTPC_MACHINE_OK;
    runtime->state = SOFTPC_RUNTIME_STOPPED;
    InitializeCriticalSection(&runtime->frame_lock);
    if (host_sync_task_create(app_runtime_worker, runtime, &runtime->worker) !=
            LIB_STATUS_OK) {
        host_sync_event_destroy(runtime->resume_event);
        host_sync_event_destroy(runtime->input_event);
        host_sync_event_destroy(runtime->ready_event);
        host_sync_event_destroy(runtime->media_event);
        app_input_queue_destroy(runtime->input_queue);
        DeleteCriticalSection(&runtime->frame_lock);
        free(runtime->frame_buffers[0]);
        free(runtime->frame_buffers[1]);
        host_sync_event_destroy(runtime->command_event);
        free(runtime);
        return 0;
    }
    *out = runtime;
    return 1;
}

int app_runtime_start(app_runtime *runtime)
{
    if (runtime == NULL) return 0;
    if (InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_STOPPED) return 0;
    host_sync_event_reset(runtime->ready_event);
    InterlockedExchange(&runtime->pause_requested, 0);
    InterlockedExchange(&runtime->stop_requested, 0);
    InterlockedExchange(&runtime->result, SOFTPC_MACHINE_IO_ERROR);
    InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_STARTING);
    (void)InterlockedIncrement(&runtime->run_generation);
    InterlockedExchange(&runtime->start_requested, 1);
    host_sync_event_signal(runtime->command_event);
    (void)host_sync_event_wait(runtime->ready_event, UINT32_MAX);
    if (InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_RUNNING) return 0;
    app_runtime_notify_state(runtime);
    return 1;
}

int app_runtime_pause(app_runtime *runtime)
{
    lib_u64 deadline;
    lib_u64 now;
    if (runtime == NULL || InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_RUNNING) return 0;
    InterlockedExchange(&runtime->pause_requested, 1);
    if (host_clock_milliseconds(&now) != LIB_STATUS_OK) return 0;
    deadline = now + 5000u;
    do {
        if (InterlockedCompareExchange(&runtime->state, 0, 0) ==
            SOFTPC_RUNTIME_PAUSED) {
            app_runtime_notify_state(runtime);
            return 1;
        }
        if (InterlockedCompareExchange(&runtime->state, 0, 0) !=
            SOFTPC_RUNTIME_RUNNING) return 0;
        host_sync_sleep_milliseconds(1u);
        if (host_clock_milliseconds(&now) != LIB_STATUS_OK) break;
    } while (now < deadline);
    InterlockedExchange(&runtime->pause_requested, 0);
    return 0;
}

int app_runtime_resume(app_runtime *runtime)
{
    lib_u64 deadline;
    lib_u64 now;
    if (runtime == NULL) return 0;
    if (InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_PAUSED) return 0;
    InterlockedExchange(&runtime->pause_requested, 0);
    host_sync_event_signal(runtime->resume_event);
    /* A frontend is recreated immediately after resume.  Do not let it see
       the old PAUSED state and terminate itself before the executor has
       acknowledged the resume event. */
    if (host_clock_milliseconds(&now) != LIB_STATUS_OK) return 0;
    deadline = now + 5000u;
    do {
        if (InterlockedCompareExchange(&runtime->state, 0, 0) ==
            SOFTPC_RUNTIME_RUNNING) {
            app_runtime_notify_state(runtime);
            return 1;
        }
        if (InterlockedCompareExchange(&runtime->state, 0, 0) !=
            SOFTPC_RUNTIME_PAUSED) return 0;
        host_sync_sleep_milliseconds(1u);
        if (host_clock_milliseconds(&now) != LIB_STATUS_OK) break;
    } while (now < deadline);
    return 0;
}

int app_runtime_stop(app_runtime *runtime)
{
    if (runtime == NULL) return 0;
    if (InterlockedCompareExchange(&runtime->state, 0, 0) ==
        SOFTPC_RUNTIME_STOPPED) return 1;
    if (InterlockedCompareExchange(&runtime->state, 0, 0) ==
        SOFTPC_RUNTIME_ERROR) return 0;
    host_sync_event_reset(runtime->ready_event);
    InterlockedExchange(&runtime->stop_requested, 1);
    InterlockedExchange(&runtime->pause_requested, 0);
    host_sync_event_signal(runtime->resume_event);
    softpc_machine_request_stop(runtime->machine);
    if (host_sync_event_wait(runtime->ready_event, UINT32_MAX) !=
        HOST_SYNC_WAIT_SIGNALED)
        return 0;
    if (InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_STOPPED) return 0;
    app_runtime_notify_state(runtime);
    return 1;
}

int app_runtime_set_floppy(app_runtime *runtime, const char *path)
{
    size_t length;
    LONG state;
    if (runtime == NULL) return 0;
    state = InterlockedCompareExchange(&runtime->state, 0, 0);
    if (state != SOFTPC_RUNTIME_STOPPED && state != SOFTPC_RUNTIME_PAUSED)
        return 0;
    if (path == NULL) {
        runtime->media_floppy_path[0] = '\0';
    } else {
        length = strlen(path);
        if (length >= sizeof(runtime->media_floppy_path)) return 0;
        memcpy(runtime->media_floppy_path, path, length + 1u);
    }
    host_sync_event_reset(runtime->media_event);
    InterlockedExchange(&runtime->media_requested, 1);
    host_sync_event_signal(runtime->command_event);
    if (host_sync_event_wait(runtime->media_event, UINT32_MAX) !=
        HOST_SYNC_WAIT_SIGNALED)
        return 0;
    return runtime->media_result == SOFTPC_MACHINE_OK;
}

app_runtime_state app_runtime_get_state(const app_runtime *runtime)
{
    if (runtime == NULL) return SOFTPC_RUNTIME_ERROR;
    return (app_runtime_state)InterlockedCompareExchange(
        (volatile LONG *)&runtime->state, 0, 0);
}

softpc_machine_result app_runtime_get_result(const app_runtime *runtime)
{
    if (runtime == NULL) return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return (softpc_machine_result)InterlockedCompareExchange(
        (volatile LONG *)&runtime->result, 0, 0);
}

int app_runtime_enqueue_input_event(app_runtime *runtime,
    const ux_event *event)
{
    LONG state;
    if (runtime == NULL || event == NULL) return 0;
    state = InterlockedCompareExchange(&runtime->state, 0, 0);
    if (state != SOFTPC_RUNTIME_RUNNING &&
        !(state == SOFTPC_RUNTIME_PAUSED &&
          (event->type == UX_EVENT_HOTKEY || event->type == UX_EVENT_WINDOW_CLOSE ||
           (event->type == UX_EVENT_KEY && event->data.key.pressed == 0u))))
        return 0;
    if (!app_input_queue_push(runtime->input_queue, event))
        return 0;
    softpc_machine_request_wake(runtime->machine);
    host_sync_event_signal(runtime->input_event);
    return 1;
}

int app_runtime_copy_frame(app_runtime *runtime,
    app_runtime_frame *destination)
{
    return app_runtime_copy_published_frame(runtime, destination, NULL);
}

void app_runtime_set_completion_sink(app_runtime *runtime,
    app_runtime_completion_sink sink, void *context)
{
    if (runtime == NULL) return;
    runtime->completion_sink = sink;
    runtime->completion_context = context;
}

int app_runtime_copy_published_frame(app_runtime *runtime,
    app_runtime_frame *destination, uint32_t *out_run_generation)
{
    int copied;
    if (runtime == NULL || destination == NULL) return 0;
    EnterCriticalSection(&runtime->frame_lock);
    memcpy(destination, runtime->frame_buffers[runtime->published_frame_index],
        sizeof(*destination));
    if (out_run_generation != NULL)
        *out_run_generation = (uint32_t)InterlockedCompareExchange(
            &runtime->published_frame_run_generation, 0, 0);
    copied = destination->valid != 0u;
    LeaveCriticalSection(&runtime->frame_lock);
    return copied;
}

uint32_t app_runtime_published_frame_sequence(const app_runtime *runtime)
{
    uint32_t sequence;
    if (runtime == NULL) return 0u;
    EnterCriticalSection((CRITICAL_SECTION *)&runtime->frame_lock);
    sequence = runtime->published_frame_sequence;
    LeaveCriticalSection((CRITICAL_SECTION *)&runtime->frame_lock);
    return sequence;
}

uint32_t app_runtime_published_frame_run_generation(const app_runtime *runtime)
{
    uint32_t generation;
    if (runtime == NULL) return 0u;
    EnterCriticalSection((CRITICAL_SECTION *)&runtime->frame_lock);
    generation = (uint32_t)InterlockedCompareExchange(
        (volatile LONG *)&runtime->published_frame_run_generation, 0, 0);
    LeaveCriticalSection((CRITICAL_SECTION *)&runtime->frame_lock);
    return generation;
}

uint32_t app_runtime_run_generation(const app_runtime *runtime)
{
    return runtime == NULL ? 0u : (uint32_t)InterlockedCompareExchange(
        (volatile LONG *)&runtime->run_generation, 0, 0);
}

int app_runtime_take_window_close(app_runtime *runtime)
{ return runtime != NULL && InterlockedExchange(&runtime->window_close_requested, 0); }

int app_runtime_take_window_mouse_release(app_runtime *runtime)
{ return runtime != NULL && InterlockedExchange(&runtime->window_mouse_release_requested, 0); }

void app_runtime_destroy(app_runtime *runtime)
{
    if (runtime == NULL) return;
    (void)app_runtime_stop(runtime);
    InterlockedExchange(&runtime->terminate_requested, 1);
    host_sync_event_signal(runtime->resume_event);
    host_sync_event_signal(runtime->command_event);
    host_sync_task_destroy(runtime->worker);
    host_sync_event_destroy(runtime->ready_event);
    host_sync_event_destroy(runtime->resume_event);
    host_sync_event_destroy(runtime->input_event);
    host_sync_event_destroy(runtime->media_event);
    app_input_queue_destroy(runtime->input_queue);
    DeleteCriticalSection(&runtime->frame_lock);
    free(runtime->frame_buffers[0]);
    free(runtime->frame_buffers[1]);
    host_sync_event_destroy(runtime->command_event);
    free(runtime);
}

int app_runtime_request_window_close(app_runtime *runtime)
{
    if (runtime == NULL || InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_RUNNING) return 0;
    InterlockedExchange(&runtime->window_close_requested, 1);
    return app_runtime_pause(runtime);
}

void app_runtime_request_window_mouse_release(app_runtime *runtime)
{
    if (runtime != NULL)
        InterlockedExchange(&runtime->window_mouse_release_requested, 1);
}
