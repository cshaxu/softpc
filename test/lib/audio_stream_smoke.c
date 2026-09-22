#include "lib/audio/stream.h"

#include <assert.h>

struct audio_stream_platform {
    lib_i16 copied[LIB_AUDIO_STREAM_MAX_FRAMES_PER_SUBMISSION * 2u];
    lib_u32 queued;
    lib_u32 writable;
};

static struct audio_stream_platform fake_platform;
static lib_status create_status = LIB_STATUS_OK;
static lib_status enqueue_status = LIB_STATUS_OK;
static lib_status query_status = LIB_STATUS_OK;
static lib_status clear_status = LIB_STATUS_OK;
static lib_status destroy_status = LIB_STATUS_OK;
static lib_u32 accepted_limit = LIB_AUDIO_STREAM_MAX_FRAMES_PER_SUBMISSION;
static unsigned create_calls, enqueue_calls, clear_calls;

lib_status audio_stream_platform_create(const lib_audio_stream_options *options,
    audio_stream_platform **out_platform)
{
    ++create_calls;
    assert(options != LIB_NULL && out_platform != LIB_NULL);
    *out_platform = create_status == LIB_STATUS_OK ? &fake_platform : LIB_NULL;
    return create_status;
}

lib_status audio_stream_platform_enqueue(audio_stream_platform *platform,
    const lib_i16 *samples, lib_u32 frame_count, lib_u32 *out_accepted_frames)
{
    lib_u32 accepted = frame_count < accepted_limit ? frame_count : accepted_limit;
    ++enqueue_calls;
    assert(platform == &fake_platform);
    lib_memory_copy(platform->copied, samples,
        (lib_size)accepted * 2u * sizeof(*samples));
    *out_accepted_frames = accepted;
    return enqueue_status;
}

lib_status audio_stream_platform_query(audio_stream_platform *platform,
    lib_u32 *out_queued_frames, lib_u32 *out_writable_frames)
{
    assert(platform == &fake_platform);
    *out_queued_frames = platform->queued;
    *out_writable_frames = platform->writable;
    return query_status;
}

lib_status audio_stream_platform_clear(audio_stream_platform *platform)
{
    ++clear_calls;
    assert(platform == &fake_platform);
    platform->queued = 0u;
    return clear_status;
}

lib_status audio_stream_platform_destroy(audio_stream_platform **platform)
{
    assert(platform != LIB_NULL && *platform == &fake_platform);
    if (destroy_status == LIB_STATUS_OK) *platform = LIB_NULL;
    return destroy_status;
}

#include "lib/audio/stream.c"

int main(void)
{
    lib_audio_stream_options options = { 48000u, 2u };
    lib_audio_stream *stream = LIB_NULL;
    lib_i16 samples[6] = { -1, 1, -2, 2, -3, 3 };
    lib_u32 accepted = 99u, queued = 99u, writable = 99u;

    assert(lib_audio_stream_create(LIB_NULL, &stream) == LIB_STATUS_INVALID_ARGUMENT);
    options.sample_rate = 8000u;
    assert(lib_audio_stream_create(&options, &stream) == LIB_STATUS_UNSUPPORTED);
    options.sample_rate = 48000u;
    options.channel_count = 3u;
    assert(lib_audio_stream_create(&options, &stream) == LIB_STATUS_UNSUPPORTED);
    options.channel_count = 2u;
    create_status = LIB_STATUS_IO_ERROR;
    assert(lib_audio_stream_create(&options, &stream) == LIB_STATUS_IO_ERROR && stream == LIB_NULL);
    create_status = LIB_STATUS_OK;
    assert(lib_audio_stream_create(&options, &stream) == LIB_STATUS_OK);
    assert(create_calls == 2u);

    fake_platform.queued = 3u;
    fake_platform.writable = 17u;
    assert(lib_audio_stream_query(stream, &queued, &writable) == LIB_STATUS_OK);
    assert(queued == 3u && writable == 17u);
    assert(lib_audio_stream_enqueue(stream, LIB_NULL, 0u, &accepted) == LIB_STATUS_OK);
    assert(accepted == 0u && enqueue_calls == 0u);
    assert(lib_audio_stream_enqueue(stream, LIB_NULL, 1u, &accepted) == LIB_STATUS_INVALID_ARGUMENT);
    assert(lib_audio_stream_enqueue(stream, samples, 513u, &accepted) == LIB_STATUS_INVALID_ARGUMENT);
    accepted_limit = 2u;
    assert(lib_audio_stream_enqueue(stream, samples, 3u, &accepted) == LIB_STATUS_OK);
    assert(accepted == 2u && enqueue_calls == 1u);
    assert(fake_platform.copied[0] == -1 && fake_platform.copied[3] == 2);

    assert(lib_audio_stream_set_active(stream, LIB_FALSE) == LIB_STATUS_OK);
    assert(clear_calls == 1u);
    assert(lib_audio_stream_enqueue(stream, samples, 1u, &accepted) == LIB_STATUS_INVALID_STATE);
    assert(lib_audio_stream_set_active(stream, LIB_FALSE) == LIB_STATUS_OK);
    assert(clear_calls == 1u);
    assert(lib_audio_stream_set_active(stream, LIB_TRUE) == LIB_STATUS_OK);
    enqueue_status = LIB_STATUS_IO_ERROR;
    accepted_limit = 1u;
    assert(lib_audio_stream_enqueue(stream, samples, 2u, &accepted) == LIB_STATUS_IO_ERROR);
    assert(accepted == 1u);
    assert(lib_audio_stream_query(stream, &queued, &writable) == LIB_STATUS_IO_ERROR);
    clear_status = LIB_STATUS_OK;
    assert(lib_audio_stream_clear(stream) == LIB_STATUS_OK);
    assert(lib_audio_stream_set_active(stream, LIB_FALSE) == LIB_STATUS_IO_ERROR);

    destroy_status = LIB_STATUS_IO_ERROR;
    assert(lib_audio_stream_destroy(&stream) == LIB_STATUS_IO_ERROR && stream != LIB_NULL);
    destroy_status = LIB_STATUS_OK;
    assert(lib_audio_stream_destroy(&stream) == LIB_STATUS_OK && stream == LIB_NULL);
    assert(lib_audio_stream_destroy(&stream) == LIB_STATUS_OK);
    return 0;
}
