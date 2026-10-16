#ifndef SOFTPC_COMPAT_SB_PCM_H
#define SOFTPC_COMPAT_SB_PCM_H

#include "lib/audio/pcm/stream_interface.h"

/* Executor-owned host output, separate from the PC speaker stream. No DMA,
 * guest-memory pointers, interrupt state or device timing crosses this seam.
 * A failed close may retain host resources and must be resolved at teardown. */
lib_status softpc_sb_pcm_open(lib_u32 rate, lib_u16 gain);
lib_status softpc_sb_pcm_close(void);
lib_status softpc_sb_pcm_write(const lib_u8 *samples, lib_u32 count,
    lib_u32 *accepted);
lib_status softpc_sb_pcm_progress(lib_audio_pcm_progress *progress);
lib_status softpc_sb_pcm_set_gain(lib_u16 gain);
lib_status softpc_sb_pcm_set_active(lib_bool active);
lib_status softpc_sb_pcm_clear(void);

#endif
