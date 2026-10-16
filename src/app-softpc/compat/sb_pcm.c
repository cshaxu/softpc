#include "sb_pcm.h"
#include "platform.h"

static lib_audio_pcm_stream *softpc_sb_pcm_stream;

static lib_status softpc_sb_pcm_wake(void *context)
{
    (void)context;
    softpc_platform_request_executor_wake();
    return LIB_STATUS_OK;
}

lib_status softpc_sb_pcm_open(lib_u32 rate, lib_u16 gain)
{
    lib_audio_pcm_stream_options options = { rate, 1u };
    lib_status status;
    if (softpc_sb_pcm_stream != LIB_NULL) return LIB_STATUS_INVALID_STATE;
    status = lib_audio_pcm_stream_create(&options, &softpc_sb_pcm_stream);
    if (status != LIB_STATUS_OK) return status;
    status = lib_audio_pcm_stream_set_gain(softpc_sb_pcm_stream, &gain, 1u);
    if (status == LIB_STATUS_OK)
        status = lib_audio_pcm_stream_set_notify(softpc_sb_pcm_stream,
            softpc_sb_pcm_wake, LIB_NULL);
    if (status != LIB_STATUS_OK) {
        lib_status cleanup = softpc_sb_pcm_close();
        if (cleanup != LIB_STATUS_OK) return cleanup;
    }
    return status;
}

lib_status softpc_sb_pcm_close(void)
{
    lib_status status, clear_status, destroy_status;
    if (softpc_sb_pcm_stream == LIB_NULL) return LIB_STATUS_OK;
    status = lib_audio_pcm_stream_set_notify(softpc_sb_pcm_stream, LIB_NULL, LIB_NULL);
    clear_status = lib_audio_pcm_stream_clear(softpc_sb_pcm_stream);
    destroy_status = lib_audio_pcm_stream_destroy(&softpc_sb_pcm_stream);
    if (destroy_status != LIB_STATUS_OK) return destroy_status;
    return status != LIB_STATUS_OK ? status : clear_status;
}

lib_status softpc_sb_pcm_write(const lib_u8 *samples, lib_u32 count,
    lib_u32 *accepted)
{
    lib_i16 converted[LIB_AUDIO_PCM_STREAM_MAX_FRAMES_PER_SUBMISSION];
    lib_u32 i;
    if (accepted == LIB_NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *accepted = 0u;
    if (samples == LIB_NULL && count != 0u) return LIB_STATUS_INVALID_ARGUMENT;
    if (count > LIB_AUDIO_PCM_STREAM_MAX_FRAMES_PER_SUBMISSION)
        return LIB_STATUS_LIMIT_EXCEEDED;
    if (softpc_sb_pcm_stream == LIB_NULL) return LIB_STATUS_INVALID_STATE;
    for (i = 0u; i < count; ++i)
        converted[i] = (lib_i16)(((lib_i32)samples[i] - 128) * 256);
    return lib_audio_pcm_stream_enqueue(softpc_sb_pcm_stream, converted,
        count, accepted);
}

lib_status softpc_sb_pcm_progress(lib_audio_pcm_progress *progress)
{
    if (softpc_sb_pcm_stream == LIB_NULL) return LIB_STATUS_INVALID_STATE;
    return lib_audio_pcm_stream_progress(softpc_sb_pcm_stream, progress);
}

lib_status softpc_sb_pcm_set_gain(lib_u16 gain)
{
    if (softpc_sb_pcm_stream == LIB_NULL) return LIB_STATUS_INVALID_STATE;
    return lib_audio_pcm_stream_set_gain(softpc_sb_pcm_stream, &gain, 1u);
}

lib_status softpc_sb_pcm_set_active(lib_bool active)
{
    if (softpc_sb_pcm_stream == LIB_NULL) return LIB_STATUS_INVALID_STATE;
    return lib_audio_pcm_stream_set_active(softpc_sb_pcm_stream, active);
}

lib_status softpc_sb_pcm_clear(void)
{
    if (softpc_sb_pcm_stream == LIB_NULL) return LIB_STATUS_INVALID_STATE;
    return lib_audio_pcm_stream_clear(softpc_sb_pcm_stream);
}
