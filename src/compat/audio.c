#include "insignia.h"
#include "host_def.h"
#include "audio.h"
#include "lib/base/sync_interface.h"

/* Standalone audio is only a presentation sink.  nt_sound.c owns the
   original PPI/Timer2 state transitions and requests a frequency here. */
#ifdef _WIN32
ULONG GetPerfCounter(VOID)
{
    return (ULONG)(GetTickCount() * 10u);
}

#define SOFTPC_SPEAKER_MIN_HZ 10ul
#define SOFTPC_SPEAKER_MAX_HZ 20000ul
#define SOFTPC_SPEAKER_SLICE_MS 40u
static base_sync_event *softpc_speaker_wake;
static base_sync_event *softpc_speaker_stop;
static base_sync_task *softpc_speaker_task;
static volatile LONG softpc_speaker_frequency;

static void softpc_speaker_worker(void *unused, const base_sync_task *task)
{
    base_sync_event *waits[2];
    lib_u32 event_index;
    UNUSED(unused);
    waits[0] = softpc_speaker_stop;
    waits[1] = softpc_speaker_wake;
    for (;;)
    {
        if (base_sync_wait_any(waits, 2u, task, UINT32_MAX, &event_index) !=
            BASE_SYNC_WAIT_SIGNALED || event_index == 0u) break;
        if (base_sync_event_reset(softpc_speaker_wake) != LIB_STATUS_OK) break;
        while (InterlockedCompareExchange(&softpc_speaker_frequency, 0, 0) != 0)
        {
            base_sync_wait_result result;
            DWORD frequency = (DWORD)InterlockedCompareExchange(
                &softpc_speaker_frequency, 0, 0);
            if (!Beep(frequency, SOFTPC_SPEAKER_SLICE_MS)) break;
            result = base_sync_wait_any(waits, 2u, task, 0u, &event_index);
            if (result == BASE_SYNC_WAIT_TIMED_OUT) continue;
            if (result != BASE_SYNC_WAIT_SIGNALED || event_index == 0u) return;
            if (base_sync_event_reset(softpc_speaker_wake) != LIB_STATUS_OK) return;
        }
    }
}

lib_status softpc_platform_audio_start(void)
{
    lib_status status;
    if (softpc_speaker_task != NULL) return LIB_STATUS_OK;
    status = base_sync_event_create(BASE_SYNC_EVENT_MANUAL_RESET, &softpc_speaker_wake);
    if (status != LIB_STATUS_OK) return status;
    status = base_sync_event_create(BASE_SYNC_EVENT_MANUAL_RESET, &softpc_speaker_stop);
    if (status != LIB_STATUS_OK) {
        base_sync_event_destroy(softpc_speaker_wake);
        softpc_speaker_wake = NULL;
        return status;
    }
    status = base_sync_task_create(softpc_speaker_worker, NULL,
        &softpc_speaker_task);
    if (status != LIB_STATUS_OK) {
        base_sync_event_destroy(softpc_speaker_wake);
        base_sync_event_destroy(softpc_speaker_stop);
        softpc_speaker_wake = NULL;
        softpc_speaker_stop = NULL;
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
        base_sync_event_signal(softpc_speaker_stop);
        base_sync_event_signal(softpc_speaker_wake);
        base_sync_task_destroy(softpc_speaker_task);
        softpc_speaker_task = NULL;
    }
    base_sync_event_destroy(softpc_speaker_wake);
    base_sync_event_destroy(softpc_speaker_stop);
    softpc_speaker_wake = NULL;
    softpc_speaker_stop = NULL;
    InterlockedExchange(&softpc_speaker_frequency, 0);
}
#else
void softpc_standalone_audio_set_tone(ULONG frequency, ULONG duration)
{ UNUSED(frequency); UNUSED(duration); }
lib_status softpc_platform_audio_start(void) { return LIB_STATUS_OK; }
void softpc_platform_audio_shutdown(void) {}
#endif
