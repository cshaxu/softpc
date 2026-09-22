#include "insignia.h"
#include "host_def.h"
#include "audio.h"
#include "lib/base/sync_interface.h"
#include "lib/audio/stream_interface.h"
#include <stdio.h>

/* Standalone audio is only a presentation sink.  nt_sound.c owns the
   original PPI/Timer2 state transitions and requests a frequency here. */
#ifdef _WIN32
ULONG GetPerfCounter(VOID)
{
    return (ULONG)(GetTickCount() * 10u);
}

#define SOFTPC_SPEAKER_MIN_HZ 10ul
#define SOFTPC_SPEAKER_MAX_HZ 20000ul
#define SOFTPC_SPEAKER_SAMPLE_RATE 48000u
#define SOFTPC_SPEAKER_BLOCK_FRAMES 512u
static base_sync_event *softpc_speaker_wake;
static base_sync_event *softpc_speaker_stop;
static base_sync_task *softpc_speaker_task;
static base_sync_mutex *softpc_speaker_request_lock;
static lib_audio_stream *softpc_speaker_stream;

typedef struct softpc_speaker_tone {
    ULONG frequency;
    ULONG duration;
    lib_u32 generation;
} softpc_speaker_tone;

static softpc_speaker_tone softpc_speaker_request;

static void softpc_speaker_read_request(softpc_speaker_tone *request)
{
    if (softpc_speaker_request_lock != NULL)
        base_sync_mutex_lock(softpc_speaker_request_lock);
    *request = softpc_speaker_request;
    if (softpc_speaker_request_lock != NULL)
        base_sync_mutex_unlock(softpc_speaker_request_lock);
}

static void softpc_speaker_write_request(ULONG frequency, ULONG duration)
{
    if (softpc_speaker_request_lock != NULL)
        base_sync_mutex_lock(softpc_speaker_request_lock);
    softpc_speaker_request.frequency = frequency;
    softpc_speaker_request.duration = duration;
    ++softpc_speaker_request.generation;
    if (softpc_speaker_request_lock != NULL)
        base_sync_mutex_unlock(softpc_speaker_request_lock);
}

static lib_status softpc_speaker_submit(lib_audio_stream *stream,
    ULONG frequency, lib_u32 frame_count, lib_u32 *phase,
    lib_u32 *out_accepted)
{
    lib_i16 samples[SOFTPC_SPEAKER_BLOCK_FRAMES];
    lib_u32 next_phase;
    lib_u32 index;
    lib_u32 accepted;
    lib_status status;

    next_phase = *phase;
    for (index = 0u; index < frame_count; ++index)
    {
        samples[index] = next_phase < (SOFTPC_SPEAKER_SAMPLE_RATE / 2u) ?
            12000 : -12000;
        next_phase += (lib_u32)frequency;
        if (next_phase >= SOFTPC_SPEAKER_SAMPLE_RATE)
            next_phase -= SOFTPC_SPEAKER_SAMPLE_RATE;
    }
    accepted = 0u;
    status = lib_audio_stream_enqueue(stream, samples,
        frame_count, &accepted);
    if (accepted != frame_count) {
        next_phase = *phase;
        for (index = 0u; index < accepted; ++index) {
            next_phase += (lib_u32)frequency;
            if (next_phase >= SOFTPC_SPEAKER_SAMPLE_RATE)
                next_phase -= SOFTPC_SPEAKER_SAMPLE_RATE;
        }
        *phase = next_phase;
        *out_accepted = accepted;
        return status == LIB_STATUS_OK ? LIB_STATUS_LIMIT_EXCEEDED : status;
    }
    *phase = next_phase;
    *out_accepted = accepted;
    return status;
}

static lib_status softpc_speaker_submit_finite(lib_audio_stream *stream,
    const softpc_speaker_tone *request, lib_u32 *phase)
{
    lib_u64 frames_remaining = (lib_u64)request->duration *
        SOFTPC_SPEAKER_SAMPLE_RATE / 1000u;

    while (frames_remaining != 0u) {
        softpc_speaker_tone current;
        lib_u32 frame_count = frames_remaining > SOFTPC_SPEAKER_BLOCK_FRAMES ?
            SOFTPC_SPEAKER_BLOCK_FRAMES : (lib_u32)frames_remaining;
        lib_u32 accepted;
        lib_status status;

        softpc_speaker_read_request(&current);
        if (current.generation != request->generation) return LIB_STATUS_OK;
        status = softpc_speaker_submit(stream, request->frequency, frame_count,
            phase, &accepted);
        if (status != LIB_STATUS_OK && status != LIB_STATUS_LIMIT_EXCEEDED)
            return status;
        frames_remaining -= accepted;
        if (status == LIB_STATUS_LIMIT_EXCEEDED &&
            lib_audio_stream_wait_writable(stream) != LIB_STATUS_OK)
            return LIB_STATUS_IO_ERROR;
    }
    return lib_audio_stream_flush(stream);
}

static void softpc_speaker_worker(void *unused, const base_sync_task *task)
{
    base_sync_event *waits[2];
    lib_u32 event_index;
    lib_u32 phase;
    lib_audio_stream *stream;
    UNUSED(unused);
    waits[0] = softpc_speaker_stop;
    waits[1] = softpc_speaker_wake;
    phase = 0u;
    stream = softpc_speaker_stream;
    for (;;)
    {
        if (base_sync_wait_any(waits, 2u, task, UINT32_MAX, &event_index) !=
            BASE_SYNC_WAIT_SIGNALED || event_index == 0u) break;
        if (base_sync_event_reset(softpc_speaker_wake) != LIB_STATUS_OK) break;
        for (;;) {
            softpc_speaker_tone request;
            lib_status status;
            lib_u32 accepted;

            softpc_speaker_read_request(&request);
            if (request.frequency == 0u) {
                if (stream != NULL) (void)lib_audio_stream_clear(stream);
                break;
            }
            if (request.duration != INFINITE) {
                status = softpc_speaker_submit_finite(stream, &request, &phase);
                if (status != LIB_STATUS_OK) {
                    softpc_speaker_write_request(0u, 0u);
                    (void)lib_audio_stream_clear(stream);
                }
                break;
            }
            status = softpc_speaker_submit(stream, request.frequency,
                SOFTPC_SPEAKER_BLOCK_FRAMES, &phase, &accepted);
            if (status != LIB_STATUS_OK && status != LIB_STATUS_LIMIT_EXCEEDED)
            {
                softpc_speaker_write_request(0u, 0u);
                (void)lib_audio_stream_clear(stream);
                break;
            }
            if (status == LIB_STATUS_LIMIT_EXCEEDED &&
                lib_audio_stream_wait_writable(stream) !=
                    LIB_STATUS_OK) {
                softpc_speaker_write_request(0u, 0u);
                (void)lib_audio_stream_clear(stream);
                break;
            }
        }
    }
}

lib_status softpc_platform_audio_start(void)
{
    lib_audio_stream_options options;
    lib_status status;

    if (softpc_speaker_task != NULL) return LIB_STATUS_OK;
    if (softpc_speaker_stream != NULL) return LIB_STATUS_IO_ERROR;
    options.sample_rate = SOFTPC_SPEAKER_SAMPLE_RATE;
    options.channel_count = 1u;
    status = lib_audio_stream_create(&options, &softpc_speaker_stream);
    if (status != LIB_STATUS_OK) return status;
    status = base_sync_mutex_create(&softpc_speaker_request_lock);
    if (status != LIB_STATUS_OK) goto fail_stream;
    status = base_sync_event_create(BASE_SYNC_EVENT_MANUAL_RESET, &softpc_speaker_wake);
    if (status != LIB_STATUS_OK) goto fail_stream;
    status = base_sync_event_create(BASE_SYNC_EVENT_MANUAL_RESET, &softpc_speaker_stop);
    if (status != LIB_STATUS_OK) {
        base_sync_event_destroy(softpc_speaker_wake);
        softpc_speaker_wake = NULL;
        goto fail_stream;
    }
    status = base_sync_task_create(softpc_speaker_worker, NULL,
        &softpc_speaker_task);
    if (status != LIB_STATUS_OK) {
        base_sync_event_destroy(softpc_speaker_wake);
        base_sync_event_destroy(softpc_speaker_stop);
        softpc_speaker_wake = NULL;
        softpc_speaker_stop = NULL;
        goto fail_stream;
    }
    return status;

fail_stream:
    base_sync_mutex_destroy(softpc_speaker_request_lock);
    softpc_speaker_request_lock = NULL;
    (void)lib_audio_stream_destroy(&softpc_speaker_stream);
    return status;
}

void softpc_standalone_audio_set_tone(ULONG frequency, ULONG duration)
{
    if (frequency < SOFTPC_SPEAKER_MIN_HZ ||
        frequency > SOFTPC_SPEAKER_MAX_HZ) {
        frequency = 0u;
        duration = 0u;
    }
    softpc_speaker_write_request(frequency, duration);
    if (softpc_speaker_wake != NULL) base_sync_event_signal(softpc_speaker_wake);
}

void softpc_platform_audio_shutdown(void)
{
    if (softpc_speaker_task != NULL)
    {
        /* wait_writable blocks on native completion, not the task cancel
           event; clear the producer request before joining it. */
        softpc_speaker_write_request(0u, 0u);
        lib_audio_stream *stream = softpc_speaker_stream;
        if (stream != NULL) (void)lib_audio_stream_cancel_wait(stream);
        base_sync_event_signal(softpc_speaker_stop);
        base_sync_event_signal(softpc_speaker_wake);
        if (base_sync_task_destroy(softpc_speaker_task) != LIB_STATUS_OK) {
            fputs("softpcvm: cannot join audio worker\n", stderr);
            return; /* Retain its global events until a successful join. */
        }
        softpc_speaker_task = NULL;
    }
    base_sync_event_destroy(softpc_speaker_wake);
    base_sync_event_destroy(softpc_speaker_stop);
    softpc_speaker_wake = NULL;
    softpc_speaker_stop = NULL;
    softpc_speaker_write_request(0u, 0u);
    if (softpc_speaker_stream != NULL &&
        lib_audio_stream_destroy(&softpc_speaker_stream) != LIB_STATUS_OK)
        fputs("softpcvm: cannot close audio stream\n", stderr);
    base_sync_mutex_destroy(softpc_speaker_request_lock);
    softpc_speaker_request_lock = NULL;
}
#else
void softpc_standalone_audio_set_tone(ULONG frequency, ULONG duration)
{ UNUSED(frequency); UNUSED(duration); }
lib_status softpc_platform_audio_start(void) { return LIB_STATUS_OK; }
void softpc_platform_audio_shutdown(void) {}
#endif
