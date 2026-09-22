#ifndef LIB_AUDIO_STREAM_INTERFACE_H
#define LIB_AUDIO_STREAM_INTERFACE_H

#include "lib/types/types_interface.h"

typedef struct lib_audio_stream lib_audio_stream;

typedef struct lib_audio_stream_options {
    lib_u32 sample_rate;
    lib_u32 channel_count;
} lib_audio_stream_options;

enum { LIB_AUDIO_STREAM_MAX_FRAMES_PER_SUBMISSION = 512u };

/* PCM frames are interleaved signed 16-bit samples. A stream accepts mono or
 * stereo 22050, 44100 or 48000 Hz PCM without conversion. Windows supplies
 * four fixed native slots; other platforms may report UNSUPPORTED at create.
 * The caller owns sample storage and serializes operations on a stream. */
lib_status lib_audio_stream_create(const lib_audio_stream_options *options,
    lib_audio_stream **out_stream);
/* Copies an ordered prefix without waiting. accepted_frames is authoritative:
 * OK may report a partial nonzero prefix; LIMIT_EXCEEDED reports zero. A zero
 * count is a successful no-op and permits a null samples pointer. */
lib_status lib_audio_stream_enqueue(lib_audio_stream *stream,
    const lib_i16 *samples, lib_u32 frame_count, lib_u32 *out_accepted_frames);
lib_status lib_audio_stream_query(lib_audio_stream *stream,
    lib_u32 *out_queued_frames, lib_u32 *out_writable_frames);
/* Inactive streams reject nonempty enqueue. Disabling clears unsounded native
 * PCM; clear preserves active state. Neither operation retracts heard audio. */
lib_status lib_audio_stream_set_active(lib_audio_stream *stream, lib_bool active);
lib_status lib_audio_stream_clear(lib_audio_stream *stream);
/* Null is idempotent. A destroy failure retains the stream for another cleanup
 * attempt and never releases storage that the platform may still own. */
lib_status lib_audio_stream_destroy(lib_audio_stream **stream);

#endif
