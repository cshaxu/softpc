#ifndef LIB_AUDIO_STREAM_H
#define LIB_AUDIO_STREAM_H

#include "lib/audio/stream_interface.h"

typedef struct audio_stream_platform audio_stream_platform;

struct lib_audio_stream {
    audio_stream_platform *platform;
    lib_bool active;
    lib_status failure;
};

lib_status audio_stream_platform_create(const lib_audio_stream_options *options,
    audio_stream_platform **out_platform);
lib_status audio_stream_platform_enqueue(audio_stream_platform *platform,
    const lib_i16 *samples, lib_u32 frame_count, lib_u32 *out_accepted_frames);
lib_status audio_stream_platform_query(audio_stream_platform *platform,
    lib_u32 *out_queued_frames, lib_u32 *out_writable_frames);
lib_status audio_stream_platform_clear(audio_stream_platform *platform);
lib_status audio_stream_platform_destroy(audio_stream_platform **platform);

#endif
