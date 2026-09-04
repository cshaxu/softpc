#include "insignia.h"
#include "host_def.h"

/* Standalone audio is only a presentation sink.  nt_sound.c owns the
   original PPI/Timer2 state transitions and requests a frequency here. */
#ifdef _WIN32
#define SOFTPC_SPEAKER_MIN_HZ 10ul
#define SOFTPC_SPEAKER_MAX_HZ 20000ul
#define SOFTPC_SPEAKER_SLICE_MS 40u
static HANDLE softpc_speaker_wake;
static HANDLE softpc_speaker_stop;
static HANDLE softpc_speaker_thread;
static volatile LONG softpc_speaker_frequency;

static DWORD WINAPI softpc_speaker_worker(void *unused)
{
    HANDLE waits[2];
    UNUSED(unused);
    waits[0] = softpc_speaker_stop;
    waits[1] = softpc_speaker_wake;
    for (;;)
    {
        DWORD result = WaitForMultipleObjects(2u, waits, FALSE, INFINITE);
        if (result == WAIT_OBJECT_0) break;
        if (result != WAIT_OBJECT_0 + 1u) continue;
        ResetEvent(softpc_speaker_wake);
        while (InterlockedCompareExchange(&softpc_speaker_frequency, 0, 0) != 0)
        {
            DWORD frequency = (DWORD)InterlockedCompareExchange(
                &softpc_speaker_frequency, 0, 0);
            (void)Beep(frequency, SOFTPC_SPEAKER_SLICE_MS);
            if (WaitForSingleObject(softpc_speaker_stop, 0u) == WAIT_OBJECT_0)
                return 0u;
            if (WaitForSingleObject(softpc_speaker_wake, 0u) == WAIT_OBJECT_0)
                ResetEvent(softpc_speaker_wake);
        }
    }
    return 0u;
}

static void softpc_speaker_wake_worker(void)
{
    if (softpc_speaker_wake == NULL)
    {
        softpc_speaker_wake = CreateEventA(NULL, TRUE, FALSE, NULL);
        softpc_speaker_stop = CreateEventA(NULL, TRUE, FALSE, NULL);
        if (softpc_speaker_wake == NULL || softpc_speaker_stop == NULL)
        {
            if (softpc_speaker_wake != NULL) CloseHandle(softpc_speaker_wake);
            if (softpc_speaker_stop != NULL) CloseHandle(softpc_speaker_stop);
            softpc_speaker_wake = NULL;
            softpc_speaker_stop = NULL;
            return;
        }
    }
    if (softpc_speaker_thread == NULL)
    {
        softpc_speaker_thread = CreateThread(NULL, 0u, softpc_speaker_worker,
                                             NULL, 0u, NULL);
        if (softpc_speaker_thread == NULL) return;
    }
    SetEvent(softpc_speaker_wake);
}

void softpc_standalone_audio_set_tone(ULONG frequency, ULONG duration)
{
    if (duration < 10u || frequency < SOFTPC_SPEAKER_MIN_HZ ||
        frequency > SOFTPC_SPEAKER_MAX_HZ) frequency = 0u;
    InterlockedExchange(&softpc_speaker_frequency, (LONG)frequency);
    if (frequency != 0u) softpc_speaker_wake_worker();
    else if (softpc_speaker_wake != NULL) SetEvent(softpc_speaker_wake);
}

void softpc_platform_audio_shutdown(void)
{
    if (softpc_speaker_thread != NULL)
    {
        SetEvent(softpc_speaker_stop);
        SetEvent(softpc_speaker_wake);
        (void)WaitForSingleObject(softpc_speaker_thread, INFINITE);
        CloseHandle(softpc_speaker_thread);
        softpc_speaker_thread = NULL;
    }
    if (softpc_speaker_wake != NULL) CloseHandle(softpc_speaker_wake);
    if (softpc_speaker_stop != NULL) CloseHandle(softpc_speaker_stop);
    softpc_speaker_wake = NULL;
    softpc_speaker_stop = NULL;
    InterlockedExchange(&softpc_speaker_frequency, 0);
}
#else
void softpc_standalone_audio_set_tone(ULONG frequency, ULONG duration)
{ UNUSED(frequency); UNUSED(duration); }
void softpc_platform_audio_shutdown(void) {}
#endif
