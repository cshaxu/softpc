#include "lib/audio/pcm/stream_interface.h"
#include "lib/types/test.h"

/* Non-audio tests: immediate, ordered memory sink. This is not a model of
 * WASAPI timing; audio contract tests keep their own progress/failure scripts. */
struct lib_audio_pcm_stream {
    lib_audio_pcm_progress progress;
    lib_audio_pcm_notify notify;
    void *context;
    lib_u32 channels;
    lib_bool active;
};
lib_status lib_audio_pcm_stream_create(const lib_audio_pcm_stream_options *options,
    lib_audio_pcm_stream **out)
{
    lib_test_assert(options && options->sample_rate && options->channel_count);
    *out = lib_allocate_zero(1, sizeof(**out));
    lib_test_assert(*out);
    (*out)->channels = options->channel_count;
    (*out)->active = LIB_TRUE;
    return LIB_STATUS_OK;
}
lib_status lib_audio_pcm_stream_destroy(lib_audio_pcm_stream **out)
{ lib_release(*out); *out = NULL; return LIB_STATUS_OK; }
lib_status lib_audio_pcm_stream_enqueue(lib_audio_pcm_stream *out,
    const lib_i16 *samples, lib_u32 count, lib_u32 *accepted)
{
    lib_test_assert(out && (samples || !count));
    *accepted = 0;
    if (!out->active && count) return LIB_STATUS_INVALID_STATE;
    *accepted = count;
    out->progress.accepted_frames += count;
    out->progress.submitted_frames += count;
    out->progress.consumed_frames += count;
    return out->notify ? out->notify(out->context) : LIB_STATUS_OK;
}
lib_status lib_audio_pcm_stream_query(lib_audio_pcm_stream *out, lib_u32 *queued, lib_u32 *writable)
{ lib_test_assert(out); *queued = 0; *writable = 512; return LIB_STATUS_OK; }
lib_status lib_audio_pcm_stream_progress(lib_audio_pcm_stream *out, lib_audio_pcm_progress *progress)
{ lib_test_assert(out); *progress = out->progress; return LIB_STATUS_OK; }
lib_status lib_audio_pcm_stream_set_notify(lib_audio_pcm_stream *out,
    lib_audio_pcm_notify notify, void *context)
{
    lib_test_assert(out);
    out->notify = notify; out->context = context;
    return notify ? notify(context) : LIB_STATUS_OK;
}
lib_status lib_audio_pcm_stream_wait_writable(lib_audio_pcm_stream *out)
{ lib_test_assert(out); return LIB_STATUS_OK; }
lib_status lib_audio_pcm_stream_cancel_wait(lib_audio_pcm_stream *out)
{ lib_test_assert(out); return LIB_STATUS_OK; }
lib_status lib_audio_pcm_stream_flush(lib_audio_pcm_stream *out)
{ lib_test_assert(out); return LIB_STATUS_OK; }
lib_status lib_audio_pcm_stream_clear(lib_audio_pcm_stream *out)
{
    lib_test_assert(out);
    ++out->progress.epoch;
    out->progress.accepted_frames = out->progress.submitted_frames = out->progress.consumed_frames = 0;
    return LIB_STATUS_OK;
}
lib_status lib_audio_pcm_stream_set_active(lib_audio_pcm_stream *out, lib_bool active)
{
    lib_test_assert(out);
    out->active = active;
    return active ? LIB_STATUS_OK : lib_audio_pcm_stream_clear(out);
}
lib_status lib_audio_pcm_stream_set_gain(lib_audio_pcm_stream *out,
    const lib_u16 *gains, lib_u32 count)
{ lib_test_assert(out && gains && count == out->channels); return LIB_STATUS_OK; }
