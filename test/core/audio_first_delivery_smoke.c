#include "insignia.h"
#include "lib/audio/stream.h"

#include <assert.h>

struct audio_stream_platform {
    lib_bool delivered_non_silent_pcm;
};

static audio_stream_platform first_platform;
static base_sync_event *first_delivery;
static lib_u32 delivery_count;

void HostPpiState(IU8 value);

lib_status audio_stream_platform_create(const lib_audio_stream_options *options,
    audio_stream_platform **out_platform)
{
    assert(options != LIB_NULL && out_platform != LIB_NULL);
    assert(options->sample_rate == 48000u && options->channel_count == 1u);
    first_platform.delivered_non_silent_pcm = LIB_FALSE;
    *out_platform = &first_platform;
    return LIB_STATUS_OK;
}

lib_status audio_stream_platform_enqueue(audio_stream_platform *platform,
    const lib_i16 *samples, lib_u32 frame_count, lib_u32 *out_accepted_frames)
{
    lib_bool saw_positive = LIB_FALSE;
    lib_bool saw_negative = LIB_FALSE;
    lib_u32 index;

    assert(platform == &first_platform && samples != LIB_NULL);
    assert(frame_count == AUDIO_STREAM_PLAY_BATCH && out_accepted_frames != LIB_NULL);
    for (index = 0u; index < frame_count; ++index) {
        if (samples[index] > 0) saw_positive = LIB_TRUE;
        if (samples[index] < 0) saw_negative = LIB_TRUE;
    }
    assert(saw_positive != LIB_FALSE && saw_negative != LIB_FALSE);
    platform->delivered_non_silent_pcm = LIB_TRUE;
    ++delivery_count;
    *out_accepted_frames = frame_count;
    assert(base_sync_event_signal(first_delivery) == LIB_STATUS_OK);
    return LIB_STATUS_OK;
}

lib_status audio_stream_platform_wait_writable(audio_stream_platform *platform)
{
    assert(platform == &first_platform);
    return LIB_STATUS_OK;
}

lib_status audio_stream_platform_cancel_wait(audio_stream_platform *platform)
{
    assert(platform == &first_platform);
    return LIB_STATUS_OK;
}

lib_status audio_stream_platform_clear(audio_stream_platform *platform)
{
    assert(platform == &first_platform);
    return LIB_STATUS_OK;
}

lib_status audio_stream_platform_destroy(audio_stream_platform **platform)
{
    assert(platform != LIB_NULL && *platform == &first_platform);
    *platform = LIB_NULL;
    return LIB_STATUS_OK;
}

#include "lib/audio/stream.c"
#include "core/compat/audio.c"

BOOL T2State;
void PlaySound(BOOL pulsed_ppi) { (void)pulsed_ppi; }
void HostPpiState(IU8 value) { (void)value; }

int main(void)
{
    assert(base_sync_event_create(BASE_SYNC_EVENT_AUTO_RESET,
        &first_delivery) == LIB_STATUS_OK);
    assert(softpc_platform_audio_start() == LIB_STATUS_OK);
    softpc_standalone_audio_set_tone(439u, INFINITE);
    assert(base_sync_event_wait(first_delivery, 1000u) ==
        BASE_SYNC_WAIT_SIGNALED);
    assert(delivery_count != 0u &&
        first_platform.delivered_non_silent_pcm != LIB_FALSE);
    softpc_platform_audio_shutdown();
    base_sync_event_destroy(first_delivery);
    return 0;
}
