#include "insignia.h"

#include <assert.h>

#include "lib/base/sync_interface.h"
#include "lib/audio/stream_interface.h"

static unsigned clears;
static unsigned enqueues;
static unsigned waits;
static int scenario;

static lib_status fake_enqueue(lib_audio_stream *stream, const lib_i16 *samples,
    lib_u32 frames, lib_u32 *accepted)
{
    (void)stream;
    assert(samples != NULL && frames == 512u);
    ++enqueues;
    *accepted = scenario == 1 ? 0u : frames;
    return scenario == 1 ? LIB_STATUS_IO_ERROR : LIB_STATUS_OK;
}

static lib_status fake_clear(lib_audio_stream *stream)
{
    assert(stream == (lib_audio_stream *)1);
    ++clears;
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
    if (scenario == 0) {
        assert(timeout == 5u);
        *index = 0u;
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
#define lib_audio_stream_clear fake_clear
#define base_sync_wait_any fake_wait
#define base_sync_event_reset fake_reset
#include "core/compat/audio.c"
#undef base_sync_event_reset
#undef base_sync_wait_any
#undef lib_audio_stream_clear
#undef lib_audio_stream_enqueue

int main(void)
{
    softpc_speaker_stop = (base_sync_event *)1;
    softpc_speaker_wake = (base_sync_event *)2;
    softpc_speaker_stream = (lib_audio_stream *)1;
    softpc_speaker_frequency = 440;

    scenario = 0;
    clears = enqueues = waits = 0u;
    softpc_speaker_worker(NULL, NULL);
    assert(enqueues == 1u && clears == 0u && waits == 2u);

    scenario = 1;
    clears = enqueues = waits = 0u;
    softpc_speaker_frequency = 440;
    softpc_speaker_worker(NULL, NULL);
    assert(enqueues == 1u && clears == 1u && waits == 2u);
    return 0;
}
