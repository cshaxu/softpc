#include "runtime.h"

#include <windows.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOFTPC_RUNTIME_INPUT_CAPACITY 256u

struct app_runtime {
    softpc_machine *machine;
    CRITICAL_SECTION input_lock;
    CRITICAL_SECTION frame_lock;
    uint8_t keys[SOFTPC_RUNTIME_INPUT_CAPACITY];
    uint8_t releases[SOFTPC_RUNTIME_INPUT_CAPACITY];
    unsigned int input_head;
    unsigned int input_tail;
    int32_t mouse_dx;
    int32_t mouse_dy;
    uint8_t mouse_left;
    uint8_t mouse_right;
    int mouse_pending;
    app_runtime_frame frames[2];
    unsigned int published_frame;
    uint32_t frame_sequence;
    volatile LONG published_frame_sequence;
    HANDLE command_event;
    HANDLE ready_event;
    HANDLE resume_event;
    HANDLE media_event;
    HANDLE worker;
    volatile LONG state;
    volatile LONG result;
    volatile LONG pause_requested;
    volatile LONG stop_requested;
    volatile LONG start_requested;
    volatile LONG terminate_requested;
    volatile LONG media_requested;
    softpc_machine_result media_result;
    char media_floppy_path[SOFTPC_RUNTIME_PATH_MAX];
    /* The original V7 standard painter may use the left half of a doubled
       host DIB as a backing surface.  This is presentation metadata only;
       it never changes C-VID's DIB or controller geometry. */
    uint32_t graphics_source_width;
    uint32_t graphics_source_height;
    uint32_t graphics_visible_width;
};

static void app_runtime_publish(app_runtime *runtime)
{
    app_runtime_frame *frame;
    unsigned int next = runtime->published_frame == 0u ? 1u : 0u;
    int published = 0;
    const void *surface;
    uint32_t columns;
    uint32_t rows;
    uint32_t stride;
    uint32_t cell_bytes;
    int32_t cursor_column;
    int32_t cursor_row;

    EnterCriticalSection(&runtime->frame_lock);
    frame = &runtime->frames[next];
    if (softpc_machine_presentation_is_graphics(runtime->machine)) {
        const void *bits;
        const void *info;
        uint32_t width;
        uint32_t height;
        uint32_t bytes;
        uint32_t row_stride;
        int32_t ignored_left;
        int32_t ignored_top;
        int32_t ignored_right;
        int32_t ignored_bottom;
        int dirty = softpc_machine_presentation_take_dirty(runtime->machine,
            &ignored_left, &ignored_top, &ignored_right, &ignored_bottom);

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
        if (!dirty) {
            LeaveCriticalSection(&runtime->frame_lock);
            return;
        }
        if (softpc_machine_presentation_dib(runtime->machine, &bits, &info,
                &width, &height) && bits != NULL && info != NULL &&
            width <= SOFTPC_RUNTIME_DIB_MAX_WIDTH &&
            height <= SOFTPC_RUNTIME_DIB_MAX_HEIGHT) {
            uint32_t visible_width;
            uint32_t destination_stride;

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
            destination_stride = (visible_width + 3u) & ~3u;
            bytes = destination_stride * height;
            if (bytes <= SOFTPC_RUNTIME_DIB_MAX_BYTES) {
                const uint8_t *source = (const uint8_t *)bits;
                uint32_t row;

                for (row = 0u; row < height; ++row)
                    memcpy(frame->dib_bits + row * destination_stride,
                        source + row * row_stride, visible_width);
                memcpy(frame->dib_info, info, sizeof(frame->dib_info));
                ((BITMAPINFO *)frame->dib_info)->bmiHeader.biWidth =
                    (LONG)visible_width;
                ((BITMAPINFO *)frame->dib_info)->bmiHeader.biSizeImage =
                    bytes;
                frame->dib_width = visible_width;
                frame->dib_height = height;
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
                frame->text_palette[palette_index] = (uint32_t)RGB(
                    colour->rgbRed, colour->rgbGreen, colour->rgbBlue);
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
            &cursor_column, &cursor_row, &frame->cursor_size);
        frame->cursor_column = cursor_column;
        frame->cursor_row = cursor_row;
        (void)softpc_machine_presentation_fonts(runtime->machine, frame->font,
            frame->secondary_font, &frame->font_height,
            &frame->attribute_font_select);
        frame->graphics = 0u;
        frame->valid = 1u;
        published = 1;
    }
    if (published) {
        frame->sequence = ++runtime->frame_sequence;
        runtime->published_frame = next;
        /* Publish only after the copied frame and its selected slot are
           complete.  Frontends use this as a cheap cadence gate before they
           acquire the frame lock and copy a full graphics surface. */
        (void)InterlockedExchange(&runtime->published_frame_sequence,
            (LONG)frame->sequence);
    }
    LeaveCriticalSection(&runtime->frame_lock);
}

static void app_runtime_drain_input(app_runtime *runtime)
{
    uint8_t key;
    uint8_t released;

    /* keyboard_io can enter a nested host_simulate frame for the original
       BIOS INT 15 keyboard hook.  A Windows make/break pair may already be
       queued by the time that callback runs, but injecting both recursively
       into that frame corrupts the original controller's service ordering.
       Deliver precisely one hardware scan event per executor callback; the
       restored 20 Hz host timer naturally schedules the next one. */
    EnterCriticalSection(&runtime->input_lock);
    if (runtime->input_tail != runtime->input_head) {
        key = runtime->keys[runtime->input_tail];
        released = runtime->releases[runtime->input_tail];
        runtime->input_tail = (runtime->input_tail + 1u) %
            SOFTPC_RUNTIME_INPUT_CAPACITY;
        LeaveCriticalSection(&runtime->input_lock);
        if (getenv("SOFTPC_INPUT_TRACE") != NULL)
            fprintf(stderr, "softpc input drain key=%u released=%u\n",
                (unsigned int)key, (unsigned int)released);
        (void)softpc_machine_key_number(runtime->machine, key, released);
    } else {
        LeaveCriticalSection(&runtime->input_lock);
    }
    EnterCriticalSection(&runtime->input_lock);
    if (runtime->mouse_pending) {
        int32_t delta_x = runtime->mouse_dx;
        int32_t delta_y = runtime->mouse_dy;
        uint8_t left = runtime->mouse_left;
        uint8_t right = runtime->mouse_right;
        runtime->mouse_dx = 0;
        runtime->mouse_dy = 0;
        runtime->mouse_pending = 0;
        LeaveCriticalSection(&runtime->input_lock);
        (void)softpc_machine_mouse_input(runtime->machine, delta_x, delta_y,
            left, right);
    } else {
        LeaveCriticalSection(&runtime->input_lock);
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
    SetEvent(runtime->media_event);
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
            HANDLE events[2] = { runtime->resume_event, runtime->command_event };
            DWORD wait = WaitForMultipleObjects(2u, events, FALSE, INFINITE);
            if (wait == WAIT_OBJECT_0 + 1u)
                app_runtime_service_media(runtime);
        }
        if (InterlockedCompareExchange(&runtime->stop_requested, 0, 0) == 0)
            InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_RUNNING);
    }
}

static DWORD WINAPI app_runtime_worker(void *opaque)
{
    app_runtime *runtime = (app_runtime *)opaque;
    for (;;) {
        softpc_machine_result result;

        (void)WaitForSingleObject(runtime->command_event, INFINITE);
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
            SetEvent(runtime->ready_event);
            continue;
        }
        if (InterlockedCompareExchange(&runtime->stop_requested, 0, 0) != 0) {
            InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_STOPPED);
            SetEvent(runtime->ready_event);
            continue;
        }

        softpc_machine_set_executor_callback(runtime->machine,
            app_runtime_executor_event, runtime);
        softpc_machine_set_heartbeat(runtime->machine, 1);
        InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_RUNNING);
        SetEvent(runtime->ready_event);
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
        SetEvent(runtime->ready_event);
    }
    return 0u;
}

int app_runtime_create(softpc_machine *machine, app_runtime **out)
{
    app_runtime *runtime;
    if (machine == NULL || out == NULL) return 0;
    *out = NULL;
    runtime = (app_runtime *)calloc(1u, sizeof(*runtime));
    if (runtime == NULL) return 0;
    runtime->machine = machine;
    runtime->command_event = CreateEventA(NULL, FALSE, FALSE, NULL);
    runtime->ready_event = CreateEventA(NULL, TRUE, FALSE, NULL);
    runtime->resume_event = CreateEventA(NULL, FALSE, FALSE, NULL);
    runtime->media_event = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (runtime->command_event == NULL || runtime->ready_event == NULL ||
        runtime->resume_event == NULL || runtime->media_event == NULL) {
        if (runtime->command_event != NULL) CloseHandle(runtime->command_event);
        if (runtime->ready_event != NULL) CloseHandle(runtime->ready_event);
        if (runtime->resume_event != NULL) CloseHandle(runtime->resume_event);
        if (runtime->media_event != NULL) CloseHandle(runtime->media_event);
        free(runtime);
        return 0;
    }
    InitializeCriticalSection(&runtime->input_lock);
    InitializeCriticalSection(&runtime->frame_lock);
    runtime->result = SOFTPC_MACHINE_OK;
    runtime->state = SOFTPC_RUNTIME_STOPPED;
    runtime->worker = CreateThread(NULL, 0u, app_runtime_worker, runtime,
        0u, NULL);
    if (runtime->worker == NULL) {
        CloseHandle(runtime->resume_event);
        CloseHandle(runtime->ready_event);
        CloseHandle(runtime->media_event);
        CloseHandle(runtime->command_event);
        DeleteCriticalSection(&runtime->frame_lock);
        DeleteCriticalSection(&runtime->input_lock);
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
    ResetEvent(runtime->ready_event);
    InterlockedExchange(&runtime->pause_requested, 0);
    InterlockedExchange(&runtime->stop_requested, 0);
    InterlockedExchange(&runtime->result, SOFTPC_MACHINE_IO_ERROR);
    InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_STARTING);
    InterlockedExchange(&runtime->start_requested, 1);
    SetEvent(runtime->command_event);
    (void)WaitForSingleObject(runtime->ready_event, INFINITE);
    return InterlockedCompareExchange(&runtime->state, 0, 0) ==
        SOFTPC_RUNTIME_RUNNING;
}

int app_runtime_pause(app_runtime *runtime)
{
    DWORD deadline;
    if (runtime == NULL || InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_RUNNING) return 0;
    InterlockedExchange(&runtime->pause_requested, 1);
    deadline = GetTickCount() + 5000u;
    do {
        if (InterlockedCompareExchange(&runtime->state, 0, 0) ==
            SOFTPC_RUNTIME_PAUSED) return 1;
        if (InterlockedCompareExchange(&runtime->state, 0, 0) !=
            SOFTPC_RUNTIME_RUNNING) return 0;
        Sleep(1u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    InterlockedExchange(&runtime->pause_requested, 0);
    return 0;
}

int app_runtime_resume(app_runtime *runtime)
{
    DWORD deadline;
    if (runtime == NULL) return 0;
    if (InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_PAUSED) return 0;
    InterlockedExchange(&runtime->pause_requested, 0);
    SetEvent(runtime->resume_event);
    /* A frontend is recreated immediately after resume.  Do not let it see
       the old PAUSED state and terminate itself before the executor has
       acknowledged the resume event. */
    deadline = GetTickCount() + 5000u;
    do {
        if (InterlockedCompareExchange(&runtime->state, 0, 0) ==
            SOFTPC_RUNTIME_RUNNING) return 1;
        if (InterlockedCompareExchange(&runtime->state, 0, 0) !=
            SOFTPC_RUNTIME_PAUSED) return 0;
        Sleep(1u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return 0;
}

int app_runtime_stop(app_runtime *runtime)
{
    if (runtime == NULL) return 0;
    if (InterlockedCompareExchange(&runtime->state, 0, 0) ==
        SOFTPC_RUNTIME_STOPPED) return 1;
    if (InterlockedCompareExchange(&runtime->state, 0, 0) ==
        SOFTPC_RUNTIME_ERROR) return 0;
    ResetEvent(runtime->ready_event);
    InterlockedExchange(&runtime->stop_requested, 1);
    InterlockedExchange(&runtime->pause_requested, 0);
    SetEvent(runtime->resume_event);
    softpc_machine_request_stop(runtime->machine);
    if (WaitForSingleObject(runtime->ready_event, INFINITE) != WAIT_OBJECT_0)
        return 0;
    return InterlockedCompareExchange(&runtime->state, 0, 0) ==
        SOFTPC_RUNTIME_STOPPED;
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
    ResetEvent(runtime->media_event);
    InterlockedExchange(&runtime->media_requested, 1);
    SetEvent(runtime->command_event);
    if (WaitForSingleObject(runtime->media_event, INFINITE) != WAIT_OBJECT_0)
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

int app_runtime_enqueue_key(app_runtime *runtime, uint8_t key_number,
    uint8_t released)
{
    unsigned int next;
    if (runtime == NULL || key_number == 0u ||
        InterlockedCompareExchange(&runtime->state, 0, 0) !=
            SOFTPC_RUNTIME_RUNNING) return 0;
    EnterCriticalSection(&runtime->input_lock);
    next = (runtime->input_head + 1u) % SOFTPC_RUNTIME_INPUT_CAPACITY;
    if (next == runtime->input_tail) {
        LeaveCriticalSection(&runtime->input_lock);
        return 0;
    }
    runtime->keys[runtime->input_head] = key_number;
    runtime->releases[runtime->input_head] = released != 0u;
    runtime->input_head = next;
    LeaveCriticalSection(&runtime->input_lock);
    if (getenv("SOFTPC_INPUT_TRACE") != NULL)
        fprintf(stderr, "softpc input enqueue key=%u released=%u\n",
            (unsigned int)key_number, (unsigned int)(released != 0u));
    softpc_machine_request_wake(runtime->machine);
    return 1;
}

int app_runtime_enqueue_mouse(app_runtime *runtime, int32_t delta_x,
    int32_t delta_y, uint8_t left_down, uint8_t right_down)
{
    if (runtime == NULL || InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_RUNNING) return 0;
    EnterCriticalSection(&runtime->input_lock);
    runtime->mouse_dx += delta_x;
    runtime->mouse_dy += delta_y;
    runtime->mouse_left = left_down != 0u;
    runtime->mouse_right = right_down != 0u;
    runtime->mouse_pending = 1;
    LeaveCriticalSection(&runtime->input_lock);
    softpc_machine_request_wake(runtime->machine);
    return 1;
}

int app_runtime_copy_frame(app_runtime *runtime,
    app_runtime_frame *destination)
{
    if (runtime == NULL || destination == NULL) return 0;
    /* A presentation client must never wait behind the executor while it is
       copying an original renderer surface.  It can retain its prior frame
       and try again on the next UI turn; only the executor owns machine
       state, and no command/input path depends on this snapshot succeeding. */
    if (!TryEnterCriticalSection(&runtime->frame_lock)) return 0;
    memcpy(destination, &runtime->frames[runtime->published_frame],
        sizeof(*destination));
    LeaveCriticalSection(&runtime->frame_lock);
    return destination->valid != 0u;
}

uint32_t app_runtime_published_frame_sequence(const app_runtime *runtime)
{
    if (runtime == NULL) return 0u;
    return (uint32_t)InterlockedCompareExchange(
        (volatile LONG *)&runtime->published_frame_sequence, 0, 0);
}

void app_runtime_destroy(app_runtime *runtime)
{
    if (runtime == NULL) return;
    (void)app_runtime_stop(runtime);
    InterlockedExchange(&runtime->terminate_requested, 1);
    SetEvent(runtime->resume_event);
    SetEvent(runtime->command_event);
    (void)WaitForSingleObject(runtime->worker, INFINITE);
    CloseHandle(runtime->worker);
    CloseHandle(runtime->ready_event);
    CloseHandle(runtime->resume_event);
    CloseHandle(runtime->media_event);
    CloseHandle(runtime->command_event);
    DeleteCriticalSection(&runtime->frame_lock);
    DeleteCriticalSection(&runtime->input_lock);
    free(runtime);
}
