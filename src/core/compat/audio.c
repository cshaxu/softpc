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
static base_sync_mutex *softpc_speaker_stream_gate;
static lib_audio_stream *softpc_speaker_stream;
static volatile LONG softpc_speaker_frequency;

static lib_audio_stream *softpc_speaker_current_stream(void)
{
    lib_audio_stream *stream;

    if (softpc_speaker_stream_gate == NULL) return softpc_speaker_stream;
    base_sync_mutex_lock(softpc_speaker_stream_gate);
    stream = softpc_speaker_stream;
    base_sync_mutex_unlock(softpc_speaker_stream_gate);
    return stream;
}

static void softpc_speaker_set_stream(lib_audio_stream *stream)
{
    if (softpc_speaker_stream_gate != NULL)
        base_sync_mutex_lock(softpc_speaker_stream_gate);
    softpc_speaker_stream = stream;
    if (softpc_speaker_stream_gate != NULL)
        base_sync_mutex_unlock(softpc_speaker_stream_gate);
}

static lib_status softpc_speaker_create_stream(lib_audio_stream **out_stream)
{
    lib_audio_stream_options options;

    options.sample_rate = SOFTPC_SPEAKER_SAMPLE_RATE;
    options.channel_count = 1u;
    return lib_audio_stream_create(&options, out_stream);
}

static lib_status softpc_speaker_submit(lib_audio_stream *stream,
    ULONG frequency, lib_u32 *phase)
{
    lib_i16 samples[SOFTPC_SPEAKER_BLOCK_FRAMES];
    lib_u32 next_phase;
    lib_u32 index;
    lib_u32 accepted;
    lib_status status;

    next_phase = *phase;
    for (index = 0u; index < SOFTPC_SPEAKER_BLOCK_FRAMES; ++index)
    {
        samples[index] = next_phase < (SOFTPC_SPEAKER_SAMPLE_RATE / 2u) ?
            12000 : -12000;
        next_phase += (lib_u32)frequency;
        if (next_phase >= SOFTPC_SPEAKER_SAMPLE_RATE)
            next_phase -= SOFTPC_SPEAKER_SAMPLE_RATE;
    }
    accepted = 0u;
    status = lib_audio_stream_enqueue(stream, samples,
        SOFTPC_SPEAKER_BLOCK_FRAMES, &accepted);
    if (accepted != SOFTPC_SPEAKER_BLOCK_FRAMES) {
        next_phase = *phase;
        for (index = 0u; index < accepted; ++index) {
            next_phase += (lib_u32)frequency;
            if (next_phase >= SOFTPC_SPEAKER_SAMPLE_RATE)
                next_phase -= SOFTPC_SPEAKER_SAMPLE_RATE;
        }
        *phase = next_phase;
        return status == LIB_STATUS_OK ? LIB_STATUS_LIMIT_EXCEEDED : status;
    }
    *phase = next_phase;
    return status;
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
    stream = softpc_speaker_current_stream();
    for (;;)
    {
        if (base_sync_wait_any(waits, 2u, task, UINT32_MAX, &event_index) !=
            BASE_SYNC_WAIT_SIGNALED || event_index == 0u) break;
        if (base_sync_event_reset(softpc_speaker_wake) != LIB_STATUS_OK) break;
        while (InterlockedCompareExchange(&softpc_speaker_frequency, 0, 0) != 0)
        {
            ULONG frequency = (ULONG)InterlockedCompareExchange(
                &softpc_speaker_frequency, 0, 0);
            lib_status status;
            if (stream == NULL) {
                status = softpc_speaker_create_stream(&stream);
                if (status != LIB_STATUS_OK) {
                    InterlockedExchange(&softpc_speaker_frequency, 0);
                    break;
                }
                softpc_speaker_set_stream(stream);
            }
            status = softpc_speaker_submit(stream, frequency, &phase);
            if (status != LIB_STATUS_OK && status != LIB_STATUS_LIMIT_EXCEEDED)
            {
                InterlockedExchange(&softpc_speaker_frequency, 0);
                break;
            }
            if (status == LIB_STATUS_LIMIT_EXCEEDED &&
                lib_audio_stream_wait_writable(stream) !=
                    LIB_STATUS_OK) {
                InterlockedExchange(&softpc_speaker_frequency, 0);
                break;
            }
        }
        if (stream != NULL) (void)lib_audio_stream_clear(stream);
    }
}

lib_status softpc_platform_audio_start(void)
{
    lib_status status;
    if (softpc_speaker_task != NULL) return LIB_STATUS_OK;
    if (softpc_speaker_stream != NULL) return LIB_STATUS_IO_ERROR;
    status = base_sync_event_create(BASE_SYNC_EVENT_MANUAL_RESET, &softpc_speaker_wake);
    if (status != LIB_STATUS_OK) return status;
    status = base_sync_event_create(BASE_SYNC_EVENT_MANUAL_RESET, &softpc_speaker_stop);
    if (status != LIB_STATUS_OK) {
        base_sync_event_destroy(softpc_speaker_wake);
        softpc_speaker_wake = NULL;
        return status;
    }
    status = base_sync_mutex_create(&softpc_speaker_stream_gate);
    if (status != LIB_STATUS_OK) {
        base_sync_event_destroy(softpc_speaker_wake);
        base_sync_event_destroy(softpc_speaker_stop);
        softpc_speaker_wake = NULL;
        softpc_speaker_stop = NULL;
        return status;
    }
    status = base_sync_task_create(softpc_speaker_worker, NULL,
        &softpc_speaker_task);
    if (status != LIB_STATUS_OK) {
        base_sync_event_destroy(softpc_speaker_wake);
        base_sync_event_destroy(softpc_speaker_stop);
        base_sync_mutex_destroy(softpc_speaker_stream_gate);
        softpc_speaker_wake = NULL;
        softpc_speaker_stop = NULL;
        softpc_speaker_stream_gate = NULL;
        return status;
    }
    return status;
}

void softpc_standalone_audio_set_tone(ULONG frequency, ULONG duration)
{
    if (duration < 10u || frequency < SOFTPC_SPEAKER_MIN_HZ ||
        frequency > SOFTPC_SPEAKER_MAX_HZ) frequency = 0u;
    InterlockedExchange(&softpc_speaker_frequency, (LONG)frequency);
    if (softpc_speaker_wake != NULL) base_sync_event_signal(softpc_speaker_wake);
}

void softpc_platform_audio_shutdown(void)
{
    if (softpc_speaker_task != NULL)
    {
        /* wait_writable blocks on native completion, not the task cancel
           event; clear the producer request before joining it. */
        InterlockedExchange(&softpc_speaker_frequency, 0);
        lib_audio_stream *stream = softpc_speaker_current_stream();
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
    InterlockedExchange(&softpc_speaker_frequency, 0);
    {
        lib_audio_stream *stream = softpc_speaker_current_stream();
        softpc_speaker_set_stream(NULL);
        if (stream != NULL && lib_audio_stream_destroy(&stream) != LIB_STATUS_OK) {
            softpc_speaker_set_stream(stream);
            fputs("softpcvm: cannot close audio stream\n", stderr);
        }
    }
    base_sync_mutex_destroy(softpc_speaker_stream_gate);
    softpc_speaker_stream_gate = NULL;
}
#else
void softpc_standalone_audio_set_tone(ULONG frequency, ULONG duration)
{ UNUSED(frequency); UNUSED(duration); }
lib_status softpc_platform_audio_start(void) { return LIB_STATUS_OK; }
void softpc_platform_audio_shutdown(void) {}
#endif
