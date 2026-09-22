#include "lib/audio/stream.h"

static lib_bool audio_stream_options_supported(const lib_audio_stream_options *options)
{
    return options->channel_count >= 1u && options->channel_count <= 2u &&
        (options->sample_rate == 22050u || options->sample_rate == 44100u ||
         options->sample_rate == 48000u);
}

static lib_status audio_stream_record_status(lib_audio_stream *stream,
    lib_status status)
{
    if (status != LIB_STATUS_OK) stream->failure = status;
    return status;
}

lib_status lib_audio_stream_create(const lib_audio_stream_options *options,
    lib_audio_stream **out_stream)
{
    lib_audio_stream *stream;
    lib_status status;

    if (out_stream == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_stream = LIB_NULL;
    if (options == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    if (audio_stream_options_supported(options) == LIB_FALSE)
        return LIB_STATUS_UNSUPPORTED;
    stream = lib_allocate_zero(1u, sizeof(*stream));
    if (stream == LIB_NULL) return LIB_STATUS_NO_MEMORY;
    status = audio_stream_platform_create(options, &stream->platform);
    if (status != LIB_STATUS_OK) {
        lib_release(stream);
        return status;
    }
    stream->active = LIB_TRUE;
    stream->failure = LIB_STATUS_OK;
    *out_stream = stream;
    return LIB_STATUS_OK;
}

lib_status lib_audio_stream_enqueue(lib_audio_stream *stream,
    const lib_i16 *samples, lib_u32 frame_count, lib_u32 *out_accepted_frames)
{
    lib_status status;

    if (out_accepted_frames == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_accepted_frames = 0u;
    if (stream == LIB_NULL || frame_count > LIB_AUDIO_STREAM_MAX_FRAMES_PER_SUBMISSION ||
        (samples == LIB_NULL && frame_count != 0u)) return LIB_STATUS_INVALID_ARGUMENT;
    if (frame_count == 0u) return LIB_STATUS_OK;
    if (stream->failure != LIB_STATUS_OK) return stream->failure;
    if (stream->active == LIB_FALSE) return LIB_STATUS_INVALID_STATE;
    status = audio_stream_platform_enqueue(stream->platform, samples, frame_count,
        out_accepted_frames);
    if (*out_accepted_frames > frame_count) {
        *out_accepted_frames = 0u;
        return audio_stream_record_status(stream, LIB_STATUS_IO_ERROR);
    }
    return audio_stream_record_status(stream, status);
}

lib_status lib_audio_stream_query(lib_audio_stream *stream,
    lib_u32 *out_queued_frames, lib_u32 *out_writable_frames)
{
    lib_status status;

    if (out_queued_frames == LIB_NULL || out_writable_frames == LIB_NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_queued_frames = 0u;
    *out_writable_frames = 0u;
    if (stream == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    if (stream->failure != LIB_STATUS_OK) return stream->failure;
    status = audio_stream_platform_query(stream->platform, out_queued_frames,
        out_writable_frames);
    return audio_stream_record_status(stream, status);
}

lib_status lib_audio_stream_clear(lib_audio_stream *stream)
{
    lib_status status;

    if (stream == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = audio_stream_platform_clear(stream->platform);
    return audio_stream_record_status(stream, status);
}

lib_status lib_audio_stream_set_active(lib_audio_stream *stream, lib_bool active)
{
    lib_status status;

    if (stream == LIB_NULL || (active != LIB_FALSE && active != LIB_TRUE))
        return LIB_STATUS_INVALID_ARGUMENT;
    if (active == stream->active) return stream->failure;
    if (stream->failure != LIB_STATUS_OK) return stream->failure;
    if (active == LIB_FALSE) {
        status = lib_audio_stream_clear(stream);
        if (status != LIB_STATUS_OK) return status;
    }
    stream->active = active;
    return LIB_STATUS_OK;
}

lib_status lib_audio_stream_destroy(lib_audio_stream **stream)
{
    lib_status status;

    if (stream == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    if (*stream == LIB_NULL) return LIB_STATUS_OK;
    status = audio_stream_platform_destroy(&(*stream)->platform);
    if (status != LIB_STATUS_OK) return status;
    lib_release(*stream);
    *stream = LIB_NULL;
    return LIB_STATUS_OK;
}
