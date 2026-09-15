#include "insignia.h"
#include <assert.h>
#include "lib/base/sync_interface.h"

static unsigned plays, waits, resets;
static int scenario;
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
#include "compat/audio.c"
#undef base_sync_event_reset
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
    return 0;
}
