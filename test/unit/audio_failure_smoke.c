#include "insignia.h"
#include <assert.h>
#include "lib/base/sync_interface.h"

static unsigned plays, waits, resets;
static int scenario;
static int reject_destroy;
static lib_status destroy_audio(base_sync_task *task)
{
    assert(task == (base_sync_task *)1);
    return reject_destroy ? LIB_STATUS_IO_ERROR : LIB_STATUS_OK;
}
static base_sync_wait_result wait_audio(base_sync_event *const *events,
    lib_u32 count, const base_sync_task *task, lib_u32 timeout, lib_u32 *index)
{
    (void)events; (void)task;
    assert(count == 2u && ++waits <= 4u);
    *index = 1u;
    if (waits == 1u) {
        assert(timeout == LIB_UINT32_MAX);
        return BASE_SYNC_WAIT_SIGNALED;
    }
    if (scenario == 0) {
        assert(timeout == LIB_UINT32_MAX && plays == 1u);
        return BASE_SYNC_WAIT_CANCELLED; /* Failed Beep returns to blocking wait. */
    }
    assert(timeout == 0u);
    if (scenario == 1) return BASE_SYNC_WAIT_FAULT;
    if (scenario == 2) return BASE_SYNC_WAIT_CANCELLED;
    if (scenario == 3) *index = 0u; /* Stop. */
    if (scenario == 4 && waits == 2u) return BASE_SYNC_WAIT_TIMED_OUT;
    if (scenario == 4) return BASE_SYNC_WAIT_CANCELLED;
    return BASE_SYNC_WAIT_SIGNALED;
}
static lib_status reset_audio(base_sync_event *event)
{
    (void)event; ++resets;
    return (scenario == 5 && resets == 2u) || scenario == 6 ?
        LIB_STATUS_IO_ERROR : LIB_STATUS_OK;
}
static BOOL WINAPI beep(DWORD frequency, DWORD duration)
{
    assert(frequency == 440u && duration == 40u);
    assert(++plays <= 2u);
    return scenario != 0;
}
#define Beep beep
#define base_sync_wait_any wait_audio
#define base_sync_event_reset reset_audio
#define base_sync_task_destroy destroy_audio
#include "compat/audio.c"
#undef base_sync_event_reset
#undef base_sync_task_destroy
#undef base_sync_wait_any
#undef Beep

int main(void)
{
    for (scenario = 0; scenario <= 6; ++scenario) {
        plays = waits = resets = 0u;
        softpc_speaker_frequency = 440;
        softpc_speaker_worker(NULL, NULL);
        assert(plays == (scenario == 6 ? 0u : scenario == 4 ? 2u : 1u));
        assert(waits == (scenario == 6 ? 1u : scenario == 4 ? 3u : 2u));
    }
    assert(base_sync_event_create(BASE_SYNC_EVENT_MANUAL_RESET, &softpc_speaker_wake) == LIB_STATUS_OK);
    assert(base_sync_event_create(BASE_SYNC_EVENT_MANUAL_RESET, &softpc_speaker_stop) == LIB_STATUS_OK);
    softpc_speaker_task = (base_sync_task *)1;
    reject_destroy = 1;
    softpc_platform_audio_shutdown();
    assert(softpc_speaker_task == (base_sync_task *)1);
    assert(base_sync_event_signal(softpc_speaker_wake) == LIB_STATUS_OK);
    assert(base_sync_event_signal(softpc_speaker_stop) == LIB_STATUS_OK);
    reject_destroy = 0;
    softpc_platform_audio_shutdown();
    assert(!softpc_speaker_task && !softpc_speaker_wake && !softpc_speaker_stop);
    return 0;
}
