#include "insignia.h"

#include <assert.h>

#include "lib/base/sync_interface.h"
#include "lib/audio/stream_interface.h"

static unsigned clears;
static unsigned enqueues;
static unsigned waits;
static unsigned writable_waits;
static unsigned initialization_step;
static unsigned stream_create_step;
static unsigned task_create_step;
static int scenario;
static lib_i16 first_sample[2];
static char event_storage[2];
static char task_storage;

static lib_status fake_stream_create(const lib_audio_stream_options *options,
    lib_audio_stream **stream)
{
    assert(options->sample_rate == 48000u && options->channel_count == 1u);
    assert(stream != NULL && *stream == NULL);
    stream_create_step = ++initialization_step;
    *stream = (lib_audio_stream *)1;
    return LIB_STATUS_OK;
}

static lib_status fake_event_create(base_sync_event_mode mode,
    base_sync_event **event)
{
    assert(mode == BASE_SYNC_EVENT_MANUAL_RESET);
    assert(event != NULL);
    *event = (base_sync_event *)&event_storage[initialization_step++];
    return LIB_STATUS_OK;
}

static lib_status fake_task_create(base_sync_task_entry function, void *context,
    base_sync_task **task)
{
    assert(function != NULL && context == NULL && task != NULL);
    task_create_step = ++initialization_step;
    *task = (base_sync_task *)&task_storage;
    return LIB_STATUS_OK;
}

static lib_status fake_enqueue(lib_audio_stream *stream, const lib_i16 *samples,
    lib_u32 frames, lib_u32 *accepted)
{
    (void)stream;
    assert(samples != NULL && frames == 512u);
    ++enqueues;
    if (enqueues <= 2u) first_sample[enqueues - 1u] = samples[0];
    if (scenario == 1) {
        *accepted = 0u;
        return LIB_STATUS_IO_ERROR;
    }
    if (scenario == 2) {
        *accepted = enqueues == 1u ? 60u : 0u;
        return enqueues == 1u ? LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
    }
    *accepted = enqueues > 4u ? 0u : frames;
    return enqueues > 4u ? LIB_STATUS_LIMIT_EXCEEDED : LIB_STATUS_OK;
}

static lib_status fake_clear(lib_audio_stream *stream)
{
    assert(stream == (lib_audio_stream *)1);
    ++clears;
    return LIB_STATUS_OK;
}

static lib_status fake_wait_writable(lib_audio_stream *stream)
{
    assert(stream == (lib_audio_stream *)1);
    ++writable_waits;
    if (scenario == 2 && writable_waits == 1u) return LIB_STATUS_OK;
    return LIB_STATUS_IO_ERROR;
}

static lib_status fake_cancel_wait(lib_audio_stream *stream)
{
    assert(stream == (lib_audio_stream *)1);
    return LIB_STATUS_OK;
}

static base_sync_wait_result fake_wait(base_sync_event *const *events,
    lib_u32 count, const base_sync_task *task, lib_u32 timeout, lib_u32 *index)
{
    (void)events;
    (void)task;
    assert(count == 2u);
    ++waits;
    if (waits == 1u) {
        assert(timeout == LIB_UINT32_MAX);
        *index = 1u;
        return BASE_SYNC_WAIT_SIGNALED;
    }
    assert(timeout == LIB_UINT32_MAX);
    return BASE_SYNC_WAIT_CANCELLED;
}

static lib_status fake_reset(base_sync_event *event)
{
    assert(event == (base_sync_event *)2);
    return LIB_STATUS_OK;
}

#define lib_audio_stream_enqueue fake_enqueue
#define lib_audio_stream_create fake_stream_create
#define lib_audio_stream_clear fake_clear
#define lib_audio_stream_wait_writable fake_wait_writable
#define lib_audio_stream_cancel_wait fake_cancel_wait
#define base_sync_event_create fake_event_create
#define base_sync_task_create fake_task_create
#define base_sync_wait_any fake_wait
#define base_sync_event_reset fake_reset
#include "core/compat/audio.c"
#undef base_sync_event_reset
#undef base_sync_wait_any
#undef base_sync_task_create
#undef base_sync_event_create
#undef lib_audio_stream_clear
#undef lib_audio_stream_wait_writable
#undef lib_audio_stream_cancel_wait
#undef lib_audio_stream_create
#undef lib_audio_stream_enqueue

int main(void)
{
    initialization_step = stream_create_step = task_create_step = 0u;
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    assert(stream_create_step != 0u && stream_create_step < task_create_step);
    softpc_speaker_task = NULL;
    softpc_speaker_stop = NULL;
    softpc_speaker_wake = NULL;
    softpc_speaker_stream = NULL;

    softpc_speaker_stop = (base_sync_event *)1;
    softpc_speaker_wake = (base_sync_event *)2;
    softpc_speaker_stream = (lib_audio_stream *)1;
    softpc_speaker_frequency = 440;

    scenario = 0;
    clears = enqueues = waits = writable_waits = 0u;
    softpc_speaker_worker(NULL, NULL);
    assert(enqueues == 5u && clears == 1u && waits == 2u);

    scenario = 1;
    clears = enqueues = waits = writable_waits = 0u;
    softpc_speaker_frequency = 440;
    softpc_speaker_worker(NULL, NULL);
    assert(enqueues == 1u && clears == 1u && waits == 2u);

    scenario = 2;
    clears = enqueues = waits = writable_waits = 0u;
    softpc_speaker_frequency = 440;
    softpc_speaker_worker(NULL, NULL);
    assert(enqueues == 2u && clears == 1u && waits == 2u &&
        first_sample[0] == 12000 && first_sample[1] == -12000);
    return 0;
}
