#include "lib/types/test.h"
#include "compat/sb_pcm.h"

struct lib_audio_pcm_stream { lib_u32 token; };
static struct lib_audio_pcm_stream stream;
static lib_status create_status, gain_status, notify_status, write_status;
static lib_status clear_status, destroy_status, active_status, progress_status;
static lib_u32 creates, destroys, clears, writes, wakes, accept_limit = 512u;
static lib_u32 cleanup_order;
static lib_u16 gain_value;
static lib_bool active_value;
static lib_audio_pcm_notify notify;
static void *notify_context;
static lib_audio_pcm_progress counters;
static lib_i16 output[512];

void softpc_platform_request_executor_wake(void) { ++wakes; }

lib_status lib_audio_pcm_stream_create(const lib_audio_pcm_stream_options *options,
    lib_audio_pcm_stream **out)
{
    lib_test_assert(options->sample_rate == 11025u && options->channel_count == 1u);
    ++creates;
    *out = create_status == LIB_STATUS_OK ? &stream : LIB_NULL;
    return create_status;
}
lib_status lib_audio_pcm_stream_destroy(lib_audio_pcm_stream **value)
{
    lib_test_assert(*value == &stream && notify == LIB_NULL);
    lib_test_assert(cleanup_order == 2u);
    cleanup_order = 3u;
    ++destroys;
    if (destroy_status == LIB_STATUS_OK) *value = LIB_NULL;
    return destroy_status;
}
lib_status lib_audio_pcm_stream_set_gain(lib_audio_pcm_stream *value,
    const lib_u16 *gains, lib_u32 channels)
{
    lib_test_assert(value == &stream && channels == 1u);
    gain_value = gains[0];
    return gain_status;
}
lib_status lib_audio_pcm_stream_set_notify(lib_audio_pcm_stream *value,
    lib_audio_pcm_notify callback, void *context)
{
    lib_test_assert(value == &stream);
    notify = callback; notify_context = context;
    if (callback == LIB_NULL) {
        cleanup_order = 1u;
        return LIB_STATUS_OK;
    }
    /* Lib installs the sink even if the stream already has a terminal fault. */
    if (notify_status != LIB_STATUS_OK) return notify_status;
    return callback(context);
}
lib_status lib_audio_pcm_stream_clear(lib_audio_pcm_stream *value)
{
    lib_test_assert(value == &stream);
    if (notify == LIB_NULL) {
        lib_test_assert(cleanup_order == 1u);
        cleanup_order = 2u;
    }
    ++clears;
    return clear_status;
}
lib_status lib_audio_pcm_stream_set_active(lib_audio_pcm_stream *value, lib_bool active)
{
    lib_test_assert(value == &stream);
    if (active_status == LIB_STATUS_OK) active_value = active;
    return active_status;
}
lib_status lib_audio_pcm_stream_progress(lib_audio_pcm_stream *value,
    lib_audio_pcm_progress *progress)
{
    lib_test_assert(value == &stream);
    if (progress_status == LIB_STATUS_OK) *progress = counters;
    return progress_status;
}
lib_status lib_audio_pcm_stream_enqueue(lib_audio_pcm_stream *value,
    const lib_i16 *samples, lib_u32 count, lib_u32 *accepted)
{
    lib_test_assert(value == &stream && count <= 512u);
    ++writes;
    *accepted = write_status == LIB_STATUS_OK ?
        (count < accept_limit ? count : accept_limit) : 0u;
    lib_memory_copy(output, samples, *accepted * sizeof(output[0]));
    return write_status;
}

int main(void)
{
    lib_u8 samples[512];
    lib_u32 accepted = 99u, i;
    lib_audio_pcm_progress progress;
    for (i = 0; i != 512u; ++i) samples[i] = (lib_u8)i;
    lib_test_assert(softpc_sb_pcm_write(samples, 1u, &accepted) == LIB_STATUS_INVALID_STATE);
    lib_test_assert(accepted == 0u && softpc_sb_pcm_close() == LIB_STATUS_OK);
    create_status = LIB_STATUS_UNSUPPORTED;
    lib_test_assert(softpc_sb_pcm_open(11025u, 42u) == LIB_STATUS_UNSUPPORTED && destroys == 0u);
    create_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_pcm_open(11025u, 42u) == LIB_STATUS_OK && wakes == 1u && gain_value == 42u);
    lib_test_assert(softpc_sb_pcm_open(11025u, 99u) == LIB_STATUS_INVALID_STATE && creates == 2u);
    lib_test_assert(softpc_sb_pcm_write(samples, 512u, &accepted) == LIB_STATUS_OK && accepted == 512u);
    lib_test_assert(output[0] == -32768 && output[127] == -256 && output[128] == 0);
    lib_test_assert(output[129] == 256 && output[255] == 32512 && output[256] == -32768);
    accept_limit = 3u;
    lib_test_assert(softpc_sb_pcm_write(samples, 9u, &accepted) == LIB_STATUS_OK && accepted == 3u);
    lib_test_assert(writes == 2u); /* Never retries a partial prefix internally. */
    write_status = LIB_STATUS_LIMIT_EXCEEDED;
    lib_test_assert(softpc_sb_pcm_write(samples, 9u, &accepted) == LIB_STATUS_LIMIT_EXCEEDED && accepted == 0u);
    write_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_pcm_write(samples, 9u, &accepted) == LIB_STATUS_IO_ERROR && accepted == 0u);
    write_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_pcm_write(LIB_NULL, 1u, &accepted) == LIB_STATUS_INVALID_ARGUMENT);
    lib_test_assert(softpc_sb_pcm_write(samples, 513u, &accepted) == LIB_STATUS_LIMIT_EXCEEDED);
    lib_test_assert(softpc_sb_pcm_write(samples, 1u, LIB_NULL) == LIB_STATUS_INVALID_ARGUMENT);
    lib_test_assert(writes == 4u);
    lib_test_assert(softpc_sb_pcm_write(LIB_NULL, 0u, &accepted) == LIB_STATUS_OK && accepted == 0u);
    counters.epoch = 7u; counters.accepted_frames = 20u;
    counters.submitted_frames = 10u; counters.consumed_frames = 2u;
    lib_test_assert(softpc_sb_pcm_progress(&progress) == LIB_STATUS_OK);
    lib_test_assert(progress.epoch == 7u && progress.accepted_frames == 20u &&
        progress.submitted_frames == 10u && progress.consumed_frames == 2u);
    lib_test_assert(notify(notify_context) == LIB_STATUS_OK && wakes == 2u);
    lib_test_assert(softpc_sb_pcm_set_gain(0u) == LIB_STATUS_OK && gain_value == 0u);
    lib_test_assert(softpc_sb_pcm_set_active(LIB_FALSE) == LIB_STATUS_OK && !active_value);
    lib_test_assert(softpc_sb_pcm_set_active(LIB_TRUE) == LIB_STATUS_OK && active_value);
    active_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_pcm_set_active(LIB_FALSE) == LIB_STATUS_IO_ERROR && active_value);
    active_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_pcm_set_active(LIB_FALSE) == LIB_STATUS_OK && !active_value);
    active_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_pcm_set_active(LIB_TRUE) == LIB_STATUS_IO_ERROR && !active_value);
    active_status = LIB_STATUS_OK;
    progress_status = LIB_STATUS_IO_ERROR;
    progress.epoch = 99u;
    lib_test_assert(softpc_sb_pcm_progress(&progress) == LIB_STATUS_IO_ERROR);
    lib_test_assert(progress.epoch == 99u);
    progress_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_pcm_progress(&progress) == LIB_STATUS_OK && progress.epoch == 7u);
    lib_test_assert(softpc_sb_pcm_clear() == LIB_STATUS_OK && clears == 1u);
    destroy_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_pcm_close() == LIB_STATUS_IO_ERROR && notify == LIB_NULL);
    lib_test_assert(softpc_sb_pcm_open(11025u, 42u) == LIB_STATUS_INVALID_STATE);
    destroy_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_pcm_close() == LIB_STATUS_OK && destroys == 2u);
    gain_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_pcm_open(11025u, 42u) == LIB_STATUS_IO_ERROR && destroys == 3u);
    gain_status = LIB_STATUS_OK; notify_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_pcm_open(11025u, 42u) == LIB_STATUS_IO_ERROR && destroys == 4u);
    notify_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_pcm_open(11025u, 42u) == LIB_STATUS_OK);
    clear_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_pcm_close() == LIB_STATUS_IO_ERROR && destroys == 5u);
    lib_test_assert(softpc_sb_pcm_close() == LIB_STATUS_OK);
    clear_status = LIB_STATUS_OK;
    notify_status = LIB_STATUS_IO_ERROR;
    destroy_status = LIB_STATUS_IO_ERROR;
    lib_test_assert(softpc_sb_pcm_open(11025u, 42u) == LIB_STATUS_IO_ERROR);
    lib_test_assert(destroys == 6u && notify == LIB_NULL && cleanup_order == 3u);
    /* Failed startup cleanup still owns the stream: a new open must not replace it. */
    i = creates;
    lib_test_assert(softpc_sb_pcm_open(11025u, 42u) == LIB_STATUS_INVALID_STATE);
    lib_test_assert(creates == i);
    destroy_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_pcm_close() == LIB_STATUS_OK && destroys == 7u);
    notify_status = LIB_STATUS_OK;
    lib_test_assert(softpc_sb_pcm_open(11025u, 42u) == LIB_STATUS_OK);
    lib_test_assert(softpc_sb_pcm_close() == LIB_STATUS_OK && destroys == 8u);
    lib_test_assert(softpc_sb_pcm_progress(&progress) == LIB_STATUS_INVALID_STATE);
    lib_test_assert(softpc_sb_pcm_set_gain(0u) == LIB_STATUS_INVALID_STATE);
    lib_test_assert(softpc_sb_pcm_set_active(LIB_TRUE) == LIB_STATUS_INVALID_STATE);
    lib_test_assert(softpc_sb_pcm_clear() == LIB_STATUS_INVALID_STATE);
    return 0;
}
