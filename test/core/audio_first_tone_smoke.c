#include "insignia.h"
#include "lib/base/sync_interface.h"
#include "lib/audio/stream_interface.h"

#include <assert.h>

static base_sync_event *first_submission;
static base_sync_event *startup_silence;
static base_sync_event *release_startup_silence;
static unsigned enqueue_count;
static lib_bool silence_seen;

static lib_status fake_audio_create(const lib_audio_stream_options *options,
    lib_audio_stream **out_stream)
{
    assert(options != LIB_NULL && options->sample_rate == 48000u);
    assert(options->channel_count == 1u && out_stream != LIB_NULL);
    *out_stream = (lib_audio_stream *)1;
    return LIB_STATUS_OK;
}

static lib_status fake_audio_enqueue(lib_audio_stream *stream,
    const lib_i16 *samples, lib_u32 frame_count, lib_u32 *out_accepted)
{
    lib_u32 index;
    lib_bool saw_positive = LIB_FALSE;
    lib_bool saw_negative = LIB_FALSE;

    assert(stream == (lib_audio_stream *)1 && samples != LIB_NULL);
    assert(frame_count == 512u && out_accepted != LIB_NULL);
    for (index = 0u; index < frame_count; ++index) {
        if (samples[index] > 0) saw_positive = LIB_TRUE;
        if (samples[index] < 0) saw_negative = LIB_TRUE;
    }
    if (saw_positive == LIB_FALSE && saw_negative == LIB_FALSE) {
        if (silence_seen != LIB_FALSE) {
            *out_accepted = 0u;
            return LIB_STATUS_IO_ERROR;
        }
        silence_seen = LIB_TRUE;
        *out_accepted = frame_count;
        assert(base_sync_event_signal(startup_silence) == LIB_STATUS_OK);
        assert(base_sync_event_wait(release_startup_silence, 1000u) ==
            BASE_SYNC_WAIT_SIGNALED);
        return LIB_STATUS_OK;
    }
    assert(saw_positive != LIB_FALSE && saw_negative != LIB_FALSE);
    ++enqueue_count;
    if (enqueue_count == 1u) {
        *out_accepted = frame_count;
        assert(base_sync_event_signal(first_submission) == LIB_STATUS_OK);
        return LIB_STATUS_OK;
    }
    *out_accepted = 0u;
    return LIB_STATUS_IO_ERROR;
}

static lib_status fake_audio_clear(lib_audio_stream *stream)
{ assert(stream == (lib_audio_stream *)1); return LIB_STATUS_OK; }

static lib_status fake_audio_flush(lib_audio_stream *stream)
{ assert(stream == (lib_audio_stream *)1); return LIB_STATUS_OK; }

static lib_status fake_audio_wait_writable(lib_audio_stream *stream)
{ assert(stream == (lib_audio_stream *)1); return LIB_STATUS_IO_ERROR; }

static lib_status fake_audio_cancel_wait(lib_audio_stream *stream)
{ assert(stream == (lib_audio_stream *)1); return LIB_STATUS_OK; }

static lib_status fake_audio_destroy(lib_audio_stream **stream);
void fake_host_ppi_state(IU8 value);

static lib_status fake_audio_destroy(lib_audio_stream **stream)
{
    assert(stream != LIB_NULL && *stream == (lib_audio_stream *)1);
    *stream = LIB_NULL;
    return LIB_STATUS_OK;
}

#define lib_audio_stream_create fake_audio_create
#define lib_audio_stream_enqueue fake_audio_enqueue
#define lib_audio_stream_clear fake_audio_clear
#define lib_audio_stream_flush fake_audio_flush
#define lib_audio_stream_wait_writable fake_audio_wait_writable
#define lib_audio_stream_cancel_wait fake_audio_cancel_wait
#define lib_audio_stream_destroy fake_audio_destroy
#define HostPpiState fake_host_ppi_state
#include "core/compat/audio.c"
#undef HostPpiState
#undef lib_audio_stream_cancel_wait
#undef lib_audio_stream_destroy
#undef lib_audio_stream_flush
#undef lib_audio_stream_clear
#undef lib_audio_stream_enqueue
#undef lib_audio_stream_create

BOOL T2State;
void PlaySound(BOOL pulsed_ppi) { (void)pulsed_ppi; }
void fake_host_ppi_state(IU8 value) { (void)value; }

int main(void)
{
    assert(base_sync_event_create(BASE_SYNC_EVENT_AUTO_RESET,
        &first_submission) == LIB_STATUS_OK);
    assert(base_sync_event_create(BASE_SYNC_EVENT_AUTO_RESET,
        &startup_silence) == LIB_STATUS_OK);
    assert(base_sync_event_create(BASE_SYNC_EVENT_AUTO_RESET,
        &release_startup_silence) == LIB_STATUS_OK);
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    assert(base_sync_event_wait(startup_silence, 1000u) ==
        BASE_SYNC_WAIT_SIGNALED);
    softpc_standalone_audio_set_tone(439u, INFINITE);
    assert(base_sync_event_signal(release_startup_silence) == LIB_STATUS_OK);
    assert(base_sync_event_wait(first_submission, 1000u) ==
        BASE_SYNC_WAIT_SIGNALED);
    assert(enqueue_count >= 1u);
    softpc_platform_audio_shutdown();
    base_sync_event_destroy(release_startup_silence);
    base_sync_event_destroy(startup_silence);
    base_sync_event_destroy(first_submission);
    return 0;
}
