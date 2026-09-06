#include "insignia.h"
#include "host_def.h"
#include "lib/host/sync.h"

/* Standalone audio is only a presentation sink.  nt_sound.c owns the
   original PPI/Timer2 state transitions and requests a frequency here. */
#ifdef _WIN32
#define SOFTPC_SPEAKER_MIN_HZ 10ul
#define SOFTPC_SPEAKER_MAX_HZ 20000ul
#define SOFTPC_SPEAKER_SLICE_MS 40u
static host_sync_event *softpc_speaker_wake;
static host_sync_event *softpc_speaker_stop;
static host_sync_task *softpc_speaker_task;
static volatile LONG softpc_speaker_frequency;

static void softpc_speaker_worker(void *unused, const host_sync_task *task)
{
    host_sync_event *waits[2];
    lib_u32 event_index;
    UNUSED(unused);
    waits[0] = softpc_speaker_stop;
    waits[1] = softpc_speaker_wake;
    for (;;)
    {
        if (host_sync_wait_any(waits, 2u, task, UINT32_MAX, &event_index) !=
            HOST_SYNC_WAIT_SIGNALED || event_index == 0u) break;
        host_sync_event_reset(softpc_speaker_wake);
        while (InterlockedCompareExchange(&softpc_speaker_frequency, 0, 0) != 0)
        {
            DWORD frequency = (DWORD)InterlockedCompareExchange(
                &softpc_speaker_frequency, 0, 0);
            (void)Beep(frequency, SOFTPC_SPEAKER_SLICE_MS);
            if (host_sync_event_wait(softpc_speaker_stop, 0u) ==
                HOST_SYNC_WAIT_SIGNALED) return;
            if (host_sync_event_wait(softpc_speaker_wake, 0u) ==
                HOST_SYNC_WAIT_SIGNALED)
                host_sync_event_reset(softpc_speaker_wake);
        }
    }
}

static void softpc_speaker_wake_worker(void)
{
    if (softpc_speaker_wake == NULL)
    {
        if (host_sync_event_create(&softpc_speaker_wake) != LIB_STATUS_OK ||
            host_sync_event_create(&softpc_speaker_stop) != LIB_STATUS_OK)
        {
            host_sync_event_destroy(softpc_speaker_wake);
            host_sync_event_destroy(softpc_speaker_stop);
            softpc_speaker_wake = NULL; softpc_speaker_stop = NULL;
            return;
        }
    }
    if (softpc_speaker_task == NULL)
    {
        if (host_sync_task_create(softpc_speaker_worker, NULL,
                &softpc_speaker_task) != LIB_STATUS_OK) return;
    }
    host_sync_event_signal(softpc_speaker_wake);
}

void softpc_standalone_audio_set_tone(ULONG frequency, ULONG duration)
{
    if (duration < 10u || frequency < SOFTPC_SPEAKER_MIN_HZ ||
        frequency > SOFTPC_SPEAKER_MAX_HZ) frequency = 0u;
    InterlockedExchange(&softpc_speaker_frequency, (LONG)frequency);
    if (frequency != 0u) softpc_speaker_wake_worker();
    else if (softpc_speaker_wake != NULL) host_sync_event_signal(softpc_speaker_wake);
}

void softpc_platform_audio_shutdown(void)
{
    if (softpc_speaker_task != NULL)
    {
        host_sync_event_signal(softpc_speaker_stop);
        host_sync_event_signal(softpc_speaker_wake);
        host_sync_task_destroy(softpc_speaker_task);
        softpc_speaker_task = NULL;
    }
    host_sync_event_destroy(softpc_speaker_wake);
    host_sync_event_destroy(softpc_speaker_stop);
    softpc_speaker_wake = NULL;
    softpc_speaker_stop = NULL;
    InterlockedExchange(&softpc_speaker_frequency, 0);
}
#else
void softpc_standalone_audio_set_tone(ULONG frequency, ULONG duration)
{ UNUSED(frequency); UNUSED(duration); }
void softpc_platform_audio_shutdown(void) {}
#endif
