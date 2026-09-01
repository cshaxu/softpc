#include "runtime.h"

#include <windows.h>

#include <stdlib.h>
#include <string.h>

#define SOFTPC_RUNTIME_INPUT_CAPACITY 256u

struct softpc_runtime {
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
    softpc_runtime_frame frames[2];
    unsigned int published_frame;
    uint32_t frame_sequence;
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
};

static void softpc_runtime_publish(softpc_runtime *runtime)
{
    softpc_runtime_frame *frame;
    unsigned int next = runtime->published_frame == 0u ? 1u : 0u;
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
        if (softpc_machine_presentation_dib(runtime->machine, &bits, &info,
                &width, &height) && bits != NULL && info != NULL &&
            width <= SOFTPC_RUNTIME_DIB_MAX_WIDTH &&
            height <= SOFTPC_RUNTIME_DIB_MAX_HEIGHT) {
            row_stride = (width + 3u) & ~3u;
            bytes = row_stride * height;
            if (bytes <= SOFTPC_RUNTIME_DIB_MAX_BYTES) {
                memcpy(frame->dib_bits, bits, bytes);
                memcpy(frame->dib_info, info, sizeof(frame->dib_info));
                frame->dib_width = width;
                frame->dib_height = height;
                frame->graphics = 1u;
                frame->valid = 1u;
            }
        }
    } else if (softpc_machine_presentation_text(runtime->machine, &surface,
            &columns, &rows, &stride, &cell_bytes) && surface != NULL &&
        cell_bytes >= 1u && stride >= columns) {
        const uint8_t *cells = (const uint8_t *)surface;
        uint32_t row;
        memset(frame->text, ' ', sizeof(frame->text));
        {
            size_t index;
            for (index = 0u; index < sizeof(frame->attributes) /
                    sizeof(frame->attributes[0]); ++index)
                frame->attributes[index] = 0x07u;
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
            &cursor_column, &cursor_row);
        frame->cursor_column = cursor_column;
        frame->cursor_row = cursor_row;
        frame->graphics = 0u;
        frame->valid = 1u;
    }
    if (frame->valid != 0u) {
        frame->sequence = ++runtime->frame_sequence;
        runtime->published_frame = next;
    }
    LeaveCriticalSection(&runtime->frame_lock);
}

static void softpc_runtime_drain_input(softpc_runtime *runtime)
{
    for (;;) {
        uint8_t key;
        uint8_t released;
        EnterCriticalSection(&runtime->input_lock);
        if (runtime->input_tail == runtime->input_head) {
            LeaveCriticalSection(&runtime->input_lock);
            break;
        }
        key = runtime->keys[runtime->input_tail];
        released = runtime->releases[runtime->input_tail];
        runtime->input_tail = (runtime->input_tail + 1u) %
            SOFTPC_RUNTIME_INPUT_CAPACITY;
        LeaveCriticalSection(&runtime->input_lock);
        (void)softpc_machine_key_number(runtime->machine, key, released);
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

static void softpc_runtime_executor_event(void *opaque)
{
    softpc_runtime *runtime = (softpc_runtime *)opaque;
    softpc_runtime_drain_input(runtime);
    softpc_runtime_publish(runtime);
    if (InterlockedCompareExchange(&runtime->pause_requested, 0, 0) != 0 &&
        InterlockedCompareExchange(&runtime->stop_requested, 0, 0) == 0) {
        InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_PAUSED);
        while (InterlockedCompareExchange(&runtime->pause_requested, 0, 0) != 0 &&
            InterlockedCompareExchange(&runtime->stop_requested, 0, 0) == 0)
            (void)WaitForSingleObject(runtime->resume_event, INFINITE);
        if (InterlockedCompareExchange(&runtime->stop_requested, 0, 0) == 0)
            InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_RUNNING);
    }
}

static DWORD WINAPI softpc_runtime_worker(void *opaque)
{
    softpc_runtime *runtime = (softpc_runtime *)opaque;
    for (;;) {
        softpc_machine_result result;

        (void)WaitForSingleObject(runtime->command_event, INFINITE);
        if (InterlockedCompareExchange(&runtime->terminate_requested, 0, 0) != 0)
            break;
        if (InterlockedExchange(&runtime->media_requested, 0) != 0) {
            runtime->media_result = softpc_machine_set_floppy(runtime->machine,
                runtime->media_floppy_path[0] == '\0' ? NULL :
                runtime->media_floppy_path);
            SetEvent(runtime->media_event);
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
            softpc_runtime_executor_event, runtime);
        softpc_machine_set_heartbeat(runtime->machine, 1);
        InterlockedExchange(&runtime->state, SOFTPC_RUNTIME_RUNNING);
        SetEvent(runtime->ready_event);
        result = softpc_machine_run(runtime->machine, 1u);
        softpc_machine_set_heartbeat(runtime->machine, 0);
        softpc_machine_set_executor_callback(runtime->machine, NULL, NULL);
        InterlockedExchange(&runtime->result, (LONG)result);
        InterlockedExchange(&runtime->state, result == SOFTPC_MACHINE_OK ?
            SOFTPC_RUNTIME_STOPPED : SOFTPC_RUNTIME_ERROR);
        SetEvent(runtime->ready_event);
    }
    return 0u;
}

int softpc_runtime_create(softpc_machine *machine, softpc_runtime **out)
{
    softpc_runtime *runtime;
    if (machine == NULL || out == NULL) return 0;
    *out = NULL;
    runtime = (softpc_runtime *)calloc(1u, sizeof(*runtime));
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
    runtime->worker = CreateThread(NULL, 0u, softpc_runtime_worker, runtime,
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

int softpc_runtime_start(softpc_runtime *runtime)
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

int softpc_runtime_pause(softpc_runtime *runtime)
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

int softpc_runtime_resume(softpc_runtime *runtime)
{
    if (runtime == NULL) return 0;
    if (InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_PAUSED) return 0;
    InterlockedExchange(&runtime->pause_requested, 0);
    SetEvent(runtime->resume_event);
    return 1;
}

int softpc_runtime_stop(softpc_runtime *runtime)
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

int softpc_runtime_set_floppy(softpc_runtime *runtime, const char *path)
{
    size_t length;
    if (runtime == NULL || InterlockedCompareExchange(&runtime->state, 0, 0) !=
        SOFTPC_RUNTIME_STOPPED) return 0;
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

softpc_runtime_state softpc_runtime_get_state(const softpc_runtime *runtime)
{
    if (runtime == NULL) return SOFTPC_RUNTIME_ERROR;
    return (softpc_runtime_state)InterlockedCompareExchange(
        (volatile LONG *)&runtime->state, 0, 0);
}

softpc_machine_result softpc_runtime_get_result(const softpc_runtime *runtime)
{
    if (runtime == NULL) return SOFTPC_MACHINE_INVALID_ARGUMENT;
    return (softpc_machine_result)InterlockedCompareExchange(
        (volatile LONG *)&runtime->result, 0, 0);
}

int softpc_runtime_enqueue_key(softpc_runtime *runtime, uint8_t key_number,
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
    softpc_machine_request_wake(runtime->machine);
    return 1;
}

int softpc_runtime_enqueue_mouse(softpc_runtime *runtime, int32_t delta_x,
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

int softpc_runtime_copy_frame(softpc_runtime *runtime,
    softpc_runtime_frame *destination)
{
    if (runtime == NULL || destination == NULL) return 0;
    EnterCriticalSection(&runtime->frame_lock);
    memcpy(destination, &runtime->frames[runtime->published_frame],
        sizeof(*destination));
    LeaveCriticalSection(&runtime->frame_lock);
    return destination->valid != 0u;
}

void softpc_runtime_destroy(softpc_runtime *runtime)
{
    if (runtime == NULL) return;
    (void)softpc_runtime_stop(runtime);
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
